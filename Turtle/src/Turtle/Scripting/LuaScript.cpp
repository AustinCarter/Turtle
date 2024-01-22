/*
Script binding based off of tutorial series by David Poo
https://www.youtube.com/playlist?list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk
 */
#include "LuaScript.h"
#include "turtpch.h"

#include <glm/glm.hpp>

namespace Turtle {
struct LuaMem {
  static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)ud;
    (void)osize; /* not used */
    if (nsize == 0) {
      free(ptr);
      return NULL;
    } else {
      // TURT_CORE_TRACE("Reallocating from {0} to {1}: {2}", osize, nsize, ptr
      // );
      // NOTE: allocating nsize * 2 seems to have prevented heap corruption
      // errors that were occuring, however I probably shouldn't need to do this
      // TODO: find source of heap corruption, presumably sizing something
      // incorrectly somewhere in LuaScript -> Seems to often occur during file
      // load which causes a stack reallocation
      return realloc(ptr, nsize * 2);
    }
  }
};

int LuaScript::LoadScript(const char *script) {
  return luaL_loadstring(m_State, script);
}

int LuaScript::LoadScriptFromFile(const std::string &filepath) {
  m_Filepath = filepath;
  return luaL_loadfile(m_State, filepath.c_str());
}

int LuaScript::ExecuteScript() { return lua_pcall(m_State, 0, LUA_MULTRET, 0); }

void LuaScript::CloseScript(lua_State *L) { lua_close(L); }

void LuaScript::LogError() {
  TURT_CORE_ERROR("LUA ERROR: {0}", lua_tostring(m_State, -1));
}

std::string MetaTableName(const entt::meta_type &type) {
  const char **cName = type.prop("Name"_hs).value().try_cast<char const *>();
  std::string metaTableName = *cName;
  metaTableName.append("__MT__");
  return metaTableName;
}

int CreateUserDatum(lua_State *L) {
  entt::meta_type type = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));

  void *ud = lua_newuserdata(L, sizeof(entt::meta_any));
  new (ud) entt::meta_any{type.ctor<>().invoke()};

  luaL_getmetatable(L, MetaTableName(type).c_str());
  lua_setmetatable(L, 1);

  lua_newtable(L);
  lua_setuservalue(L, 1);

  return 1;
}

int CreateUserDatumFromMetaObject(lua_State *L, const entt::meta_any &object) {
  entt::meta_type type = object.type();

  void *ud = lua_newuserdata(L, sizeof(entt::meta_any));
  int userDataumStackIndex = lua_gettop(L);
  new (ud) entt::meta_any{object};

  luaL_getmetatable(L, MetaTableName(type).c_str());
  lua_setmetatable(L, userDataumStackIndex);

  lua_newtable(L);
  lua_setuservalue(L, userDataumStackIndex);

  return 1;
}

int ToLua(lua_State *L, const entt::meta_any &result) {
  int numReturnValues = 0;
  if (!result) {
    TURT_CORE_ERROR(
        "Could not send result to LUA, are parameters of correct type?");
  } else if (!result.type().is_void()) {
    auto type = result.type();
    if (entt::resolve<int>() == result.type())
      lua_pushnumber(L, result.cast<int>());
    else if (entt::resolve<uint32_t>() == result.type())
      lua_pushnumber(L, result.cast<uint32_t>());
    else if (entt::resolve<float>() == result.type())
      lua_pushnumber(L, result.cast<float>());
    else if (entt::resolve<bool>() == result.type())
      lua_pushnumber(L, result.cast<bool>());
    else if (result.type().is_class()) {
      CreateUserDatumFromMetaObject(L, std::ref(result));
    } else
      TURT_CORE_ERROR("Unhandled return type in lua invocation");

    numReturnValues++;
    return numReturnValues;
  }
  return numReturnValues;
}

/*
 * Invoke func on instance with arguments from lua passed on stack and results
 * left to stack
 */
int InvokeMethod(lua_State *L, entt::meta_func &func,
                 entt::meta_any &instance) {
  union PassByValue {
    int intVal;
    float floatVal;
    uint32_t uint32Val;
  };

  uint32_t numLuaArgs = lua_gettop(L);
  uint32_t numNativeArgs = func.size();
  uint32_t paramStackOffset = 0;
  if (numLuaArgs > numNativeArgs) {
    paramStackOffset = numLuaArgs - numNativeArgs;
    numLuaArgs = numNativeArgs;
  }
  // TURT_CORE_ASSERT(numLuaArgs == numNativeArgs, "Error calling native
  // function, parameter number mismatch");

  std::vector<PassByValue> pbv(numNativeArgs);

  std::vector<entt::meta_any> nativeArgs(numNativeArgs);

  for (int i = 0; i < numLuaArgs; i++) {
    // lua index starts from 1
    int luaArgIndex = i + 1 + paramStackOffset;
    entt::meta_type nativeParameterType = func.arg(i);
    int luaType = lua_type(L, luaArgIndex);
    switch (luaType) {
    case LUA_TNUMBER:
      if (entt::resolve<int32_t>() == nativeParameterType) {
        pbv[i].intVal = (int32_t)lua_tonumber(L, luaArgIndex);
        nativeArgs[i] = pbv[i].intVal;
      } else if (entt::resolve<uint32_t>() == nativeParameterType) {
        pbv[i].uint32Val = (uint32_t)lua_tonumber(L, luaArgIndex);
        nativeArgs[i] = pbv[i].uint32Val;
      } else if (entt::resolve<float>() == nativeParameterType) {
        pbv[i].floatVal = (float)lua_tonumber(L, luaArgIndex);
        nativeArgs[i] = pbv[i].floatVal;
      } else {
        TURT_CORE_ERROR("Unhandled parameter type in lua invocation");
      }
      break;
    default:
      TURT_CORE_ERROR("Unhandled parameter type in lua invocation");
      break;
    }
  }

  const entt::meta_any &result = func.invoke(
      instance, (entt::meta_any *)(nativeArgs.data()), nativeArgs.size());
  return Turtle::ToLua(L, result);
}

int DestroyUserDatum(lua_State *L) {
  // entt::meta_type type = entt::resolve_id(lua_tonumber(L,
  // lua_upvalueindex(1)));
  entt::meta_any *ud = (entt::meta_any *)lua_touserdata(L, -1);
  ud->~meta_any();
  return 0;
}

int InvokeFuncOnUserDatum(lua_State *L) {
  entt::meta_func &func =
      *(entt::meta_func *)lua_touserdata(L, lua_upvalueindex(1));
  if (lua_isuserdata(L, 1) == false) {
    TURT_CORE_ASSERT(false, "Expected a userdatum when invoking native method");
  }

  entt::meta_any &ud = *(entt::meta_any *)lua_touserdata(L, 1);
  return InvokeMethod(L, func, ud);
  // entt::meta_any result =  func.invoke(ud);
  // TURT_CORE_ASSERT(result, "Failed to invoke native method");

  // return 0;
}

int IndexUserDatum(lua_State *L) {
  entt::meta_type type = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));
  if (lua_isuserdata(L, 1) == false) {
    TURT_CORE_ASSERT(false, "Expected a userdatum to index on stack");
  }
  if (lua_isstring(L, 2) == false) {
    TURT_CORE_ASSERT(false, "Expected a native property/method name on stack "
                            "when indexing native type");
  }

  char const *fieldName = lua_tostring(L, -1);

  // get index func by name
  entt::meta_func indexFunc;
  for (const auto &func : type.func()) {
    entt::meta_prop nameProp = func.prop("Name"_hs);
    if (nameProp) {
      char const **funcName = nameProp.value().try_cast<char const *>();
      TURT_CORE_ASSERT(funcName,
                       "Function name could not be resolved to a string");
      if (!strcmp(*funcName, fieldName)) {
        indexFunc = func;
        break;
      }
    }
  }
  if (indexFunc) {
    void *methodUD = lua_newuserdata(L, sizeof(entt::meta_func));
    new (methodUD) entt::meta_func(indexFunc);
    lua_pushcclosure(L, InvokeFuncOnUserDatum, 1);
    return 1;
  }

  entt::meta_data data = type.data(entt::hashed_string{fieldName});
  if (data) {
    entt::meta_any &ud = *(entt::meta_any *)lua_touserdata(L, 1);
    const entt::meta_any &result = data.get(ud);
    if (result) {
      return Turtle::ToLua(L, result);
    }
  }

  // return the uservalue if it exists in lua

  lua_getuservalue(L, 1);
  lua_pushvalue(L, 2);
  lua_gettable(L, -2);
  return 1;
}

int NewIndexUserDatum(lua_State *L) {
  entt::meta_type type = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));
  if (lua_isuserdata(L, 1) == false) {
    TURT_CORE_ERROR("Expected a userdatum to index on stack");
  }
  if (lua_isstring(L, 2) == false) {
    TURT_CORE_ERROR("Expected a native property/method name on stack when "
                    "indexing native type");
  }

  char const *fieldName = lua_tostring(L, 2);

  entt::meta_data data = type.data(entt::hashed_string{fieldName});
  if (data) {
    entt::meta_any &ud = *(entt::meta_any *)lua_touserdata(L, 1);
    int luaType = lua_type(L, 3);
    switch (luaType) {
    case LUA_TNUMBER:
      if (entt::resolve<uint32_t>() == data.type()) {
        uint32_t val = (uint32_t)lua_tonumber(L, 3);
        bool set = data.set(ud, val);
        if (!set)
          TURT_CORE_ERROR("Failed to set native uint32 value: {0}", fieldName);
      } else if (entt::resolve<int>() == data.type()) {
        int val = (int)lua_tonumber(L, 3);
        bool set = data.set(ud, val);
        if (!set)
          TURT_CORE_ERROR("Failed to set native int value: {0}", fieldName);
      } else if (entt::resolve<float>() == data.type()) {
        float val = (float)lua_tonumber(L, 3);
        bool set = data.set(ud, val);
        if (!set)
          TURT_CORE_ERROR("Failed to set native float value: {0}", fieldName);
      } else {
        TURT_CORE_ERROR("Could not convert type of {0} to native numeric arg",
                        fieldName);
      }

      break;
    default:
      TURT_CORE_ASSERT(false, "Unrecognized lua type");
      break;
    }
    return 0;
  }

  lua_getuservalue(L, 1);
  lua_pushvalue(L, 2);
  lua_pushvalue(L, 3);
  lua_settable(L, -3);

  return 0;
}

int CallGlobalFromLua(lua_State *L) {
  //
  entt::meta_type type = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));
  entt::meta_func func = type.func(lua_tonumber(L, lua_upvalueindex(2)));
  entt::meta_any instance{};
  return InvokeMethod(L, func, instance);
}

LuaScript::LuaScript() {

  lua_State *L = lua_newstate(LuaMem::l_alloc, m_Memory);

  lua_newtable(L);
  lua_pushvalue(L, -1);
  lua_setglobal(L, "Global");

  luaL_openlibs(L);
  // Allows to include libs such as keycodes, might want to have it point to dir
  // in turtle or somewhere else that will work for client app
  luaL_dostring(L, "package.path = package.path .. ';./assets/scripts/?.lua'");

  lua_pushvalue(L, -1);
  entt::meta_range<entt::meta_type> range{
      entt::internal::meta_context::local()};
  // for (auto& type : range)
  // {
  // 	auto funcs = type.func();
  // 	for (auto& func : funcs)
  // 	{

  // 		//TURT_CORE_ASSERT(name, "Nameless Global Meta Function while
  // binding lua."); 		if (func.is_static())
  // 		{
  // 			entt::meta_prop nameProp = func.prop("Name"_hs);
  // 			if(nameProp)
  // 			{
  // 				entt::meta_any name = nameProp.value();
  // 				char const** cName = name.try_cast<char
  // const*>(); 				lua_pushstring(L, *cName);
  // lua_pushnumber(L, type.id()); lua_pushnumber(L, func.id());
  // 				lua_pushcclosure(L, CallGlobalFromLua, 2);
  // 				lua_settable(L, -3);
  // 			}
  // 		}
  // 	}

  // }

  int stackSize = lua_gettop(L);
  lua_pop(L, stackSize);

  for (const auto &type : range) {
    entt::meta_prop nameProp = type.prop("Name"_hs);
    if (nameProp) // Wont register types without a "Name"_hs prop (such as the
                  // Global struct)
    {
      char const **cName = nameProp.value().try_cast<char const *>();

      lua_newtable(L);
      lua_pushvalue(L, -1);
      lua_setglobal(L, *cName);

      lua_pushvalue(L, -1);
      lua_pushnumber(L, type.id());
      lua_pushcclosure(L, CreateUserDatum, 1);
      lua_setfield(L, -2, "new");

      luaL_newmetatable(L, MetaTableName(type).c_str());
      lua_pushstring(L, "__gc");
      lua_pushcfunction(L, DestroyUserDatum);
      lua_settable(L, -3);

      lua_pushstring(L, "__index");
      lua_pushnumber(L, type.id());
      lua_pushcclosure(L, IndexUserDatum, 1);
      lua_settable(L, -3);

      lua_pushstring(L, "__newindex");
      lua_pushnumber(L, type.id());
      lua_pushcclosure(L, NewIndexUserDatum, 1);
      lua_settable(L, -3);

      auto funcs = type.func();

      for (const auto &func : funcs) {

        // TURT_CORE_ASSERT(name, "Nameless Global Meta Function while binding
        // lua.");
        if (func.is_static()) {
          entt::meta_prop funcNameProp = func.prop("Name"_hs);
          if (funcNameProp) {
            entt::meta_any funcName = funcNameProp.value();
            char const **funcCName = funcName.try_cast<char const *>();
            lua_pushstring(L, *funcCName);
            lua_pushnumber(L, type.id());
            lua_pushnumber(L, func.id());
            lua_pushcclosure(L, CallGlobalFromLua, 2);
            lua_settable(L, -4);
          }
        }
      }
    }
  }

  stackSize = lua_gettop(L);
  lua_pop(L, stackSize);
  m_State = L;
  void *m_Memory = nullptr;
}
} // namespace Turtle
