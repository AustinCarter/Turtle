#include "turtpch.h"
#include "Components.h"

#include <entt.hpp>

//TEMP 
#include "Turtle/Scripting/LuaScriptBinder.h"
#include "lua.hpp"


namespace Turtle {
	
	struct GlobalMetaFunctionsComponent 
	{
		int myDataTest = 0;

		void MyFuncTest()
		{
			myDataTest++;
		}
	};

	void HelloWorld()
	{
		printf("Hello, World\n");
	}

	static void HelloWorld2()
	{
		printf("Hello, World 2\n");
	}

	static void HelloWorld3(uint32_t x, int y)
	{
		printf("Hello, World 3 (%d, %d)\n", x, y);
	}

	int Add(int a, int b)
	{
		return a + b;
	}

	struct Sprite
	{
		int x;
		int y;

		Sprite() : x(0), y(0) {}
		~Sprite() {}

		int Move(int velX, int velY)
		{
			x += velX;
			y += velY;
			return x + y;
		}

		void Draw()
		{
			printf("sprite(%p): x = %d, y = %d\n", this, x, y);
		}
	};

	std::string MetaTableName(const entt::meta_type type)
	{
		const char** cName = type.prop("Name"_hs).value().try_cast<char const*>();
		std::string metaTableName = *cName;
		metaTableName.append("__MT__");
		return metaTableName;
	}

	int CreateUserDatum(lua_State* L)
	{
		entt::meta_type type  = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));

		//int size = type.prop("size"_hs).value().cast<int>();
		//TURT_CORE_ASSERT(size, "Size of type unknown");

		void* ud = lua_newuserdata(L, sizeof(entt::meta_any));
		new (ud) entt::meta_any{ type.ctor<>().invoke() };

		luaL_getmetatable(L, MetaTableName(type).c_str());
		lua_setmetatable(L, 1);

		lua_newtable(L);
		lua_setuservalue(L, 1);

		return 1;
	}

	int CreateUserDatumFromMetaObject(lua_State* L, const entt::meta_any& object)
	{
		entt::meta_type type = object.type();

		//int size = type.prop("size"_hs).value().cast<int>();
		//TURT_CORE_ASSERT(size, "Size of type unknown");

		void* ud = lua_newuserdata(L, sizeof(entt::meta_any));
		int userDataumStackIndex = lua_gettop( L );
		new (ud) entt::meta_any{ object };


		luaL_getmetatable(L, MetaTableName(type).c_str());
		lua_setmetatable(L, userDataumStackIndex);

		lua_newtable(L);
		lua_setuservalue(L, userDataumStackIndex);

		return 1;
	}

	int ToLua(lua_State* L, entt::meta_any& result)
	{
		int numReturnValues = 0;
		if (!result)
		{
			TURT_CORE_ASSERT(false, "Could not send result to LUA, are parameters of correct type?");
		}
		else if (!result.type().is_void())
		{
			if (entt::resolve<int>() == result.type())
				lua_pushnumber(L, result.cast<int>());
			else if (entt::resolve<uint32_t>() == result.type())
				lua_pushnumber(L, result.cast<uint32_t>());
			else if (entt::resolve<float>() == result.type())
				lua_pushnumber(L, result.cast<float>());
			else if (result.type().is_class())
			{
				CreateUserDatumFromMetaObject(L, result);
			}
			else
				TURT_CORE_ASSERT(false, "Unhandled return type in lua invocation");

			numReturnValues++;
			return numReturnValues;
		}
		return numReturnValues;
	}


	int PutOnLuaStack(lua_State* L) { return 0; }

	template<typename T>
	int PutOnLuaStack(lua_State* L, T& toPutOnStack)
	{
		if (entt::resolve<T>().is_class() /*|| entt::resolve<T>().is_pointer()*/)
		{
			entt::meta_any val{ std::ref(toPutOnStack) };
			return ToLua(L, val);
		}
		else
		{
			entt::meta_any val{ toPutOnStack };
			return ToLua(L, val);
		}
	}

	template<typename T, typename... REST>
	int PutOnLuaStack(lua_State* L, T& toPutOnStack, REST&... rest)
	{
		return PutOnLuaStack(L, toPutOnStack) + PutOnLuaStack(L, rest...);
	}

	template<typename... ARGS>
	void CallScriptFunction(lua_State* L, const char* funcName, ARGS&... args)
	{
		lua_getglobal(L, funcName);
		if (lua_type(L, -1) == LUA_TFUNCTION)
		{
			int numArgs = PutOnLuaStack(L, args... );
			if (lua_pcall(L, numArgs, 0, 0))
			{
				TURT_CORE_ASSERT(false, "Unable to call script funciton");
			}
		}
		else
		{
			TURT_CORE_ASSERT(false, "Unknown script function");
		}
	}

	/*
	* Invoke func on instance with arguments from lua passed on stack and results left to stack
	*/
	int InvokeMethod(lua_State* L, entt::meta_func& func, entt::meta_any& instance)
	{
		union PassByValue
		{
			int intVal;
			float floatVal;
			uint32_t uint32Val;
		};

		uint32_t numLuaArgs = lua_gettop(L);
		uint32_t numNativeArgs = func.size();
		uint32_t paramStackOffset = 0;
		if (numLuaArgs > numNativeArgs)
		{
			paramStackOffset = numLuaArgs - numNativeArgs;
			numLuaArgs = numNativeArgs;
		}
		TURT_CORE_ASSERT(numLuaArgs == numNativeArgs, "Error calling native function, parameter number mismatch");

		std::vector<PassByValue> pbv(numNativeArgs);

		std::vector<entt::meta_any> nativeArgs(numNativeArgs);

		for (int i = 0; i < numLuaArgs; i++)
		{
			//lua index starts from 1
			int luaArgIndex = i + 1 + paramStackOffset;
			entt::meta_type nativeParameterType = func.arg(i);
			int luaType = lua_type(L, luaArgIndex);
			switch (luaType)
			{
			case LUA_TNUMBER:
				if (entt::resolve<int>() == nativeParameterType)
				{
					pbv[i].intVal = (int)lua_tonumber(L, luaArgIndex);
					nativeArgs[i] = pbv[i].intVal;
				}
				else if (entt::resolve<uint32_t>() == nativeParameterType)
				{
					pbv[i].uint32Val = (uint32_t)lua_tonumber(L, luaArgIndex);
					nativeArgs[i] = pbv[i].uint32Val;
				}
				else if (entt::resolve<float>() == nativeParameterType)
				{
					pbv[i].floatVal = (float)lua_tonumber(L, luaArgIndex);
					nativeArgs[i] = pbv[i].floatVal;
				}
				else
				{
					TURT_CORE_ASSERT(false, "Unhandled parameter type in lua invocation");
				}
				break;
			default:
				TURT_CORE_ASSERT(false, "Unhandled parameter type in lua invocation");
				break;
			}

		}

		entt::meta_any result = func.invoke(instance, (entt::meta_any*)(nativeArgs.data()), nativeArgs.size());
		return ToLua(L, result);
		
		
	}

	int DestroyUserDatum(lua_State* L)
	{
		//entt::meta_type type = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));
		entt::meta_any* ud = (entt::meta_any*)lua_touserdata(L, -1);
		ud->~meta_any();
		return 0;
	}

	int InvokeFuncOnUserDatum(lua_State* L)
	{
		entt::meta_func& func = *(entt::meta_func*)lua_touserdata(L, lua_upvalueindex(1));
		if (lua_isuserdata(L, 1) == false)
		{
			TURT_CORE_ASSERT(false, "Expected a userdatum when invoking native method");
		}
		
		entt::meta_any& ud = *(entt::meta_any*)lua_touserdata(L, 1);
		return InvokeMethod(L, func, ud);
		//entt::meta_any result =  func.invoke(ud);
		//TURT_CORE_ASSERT(result, "Failed to invoke native method");

		//return 0;
	}

	int IndexUserDatum(lua_State* L)
	{
		entt::meta_type type = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));
		if (lua_isuserdata(L, 1) == false)
		{
			TURT_CORE_ASSERT(false, "Expected a userdatum to index on stack");
		}
		if (lua_isstring(L, 2) == false)
		{
			TURT_CORE_ASSERT(false, "Expected a native property/method name on stack when indexing native type");
		}

		char const* fieldName = lua_tostring(L, -1);
		
		//get index func by name
		entt::meta_func indexFunc; 
		for (auto& func : type.func())
		{
			char const** funcName = func.prop("Name"_hs).value().try_cast<char const*>();
			if (!strcmp(*funcName, fieldName))
			{
				indexFunc = func;
				break;
			}
		}
		if (indexFunc)
		{
			void* methodUD = lua_newuserdata(L, sizeof(entt::meta_func));
			new (methodUD) entt::meta_func(indexFunc);
			lua_pushcclosure(L, InvokeFuncOnUserDatum, 1);
			return 1;
		}

		//NOTE: currently member data/variables must be refered to in lua with the name registered to entt
		//for (auto& data : type.data())
		//{

//		}	
		entt::meta_data data = type.data(entt::hashed_string{fieldName});
		if (data)
		{
			entt::meta_any& ud = *(entt::meta_any*)lua_touserdata(L, 1);
			entt::meta_any result = data.get(ud);
			if (result)
			{
				return ToLua(L, result);
			}
		}

		//return the uservalue if it exists in lua

		lua_getuservalue(L, 1);
		lua_pushvalue(L, 2);
		lua_gettable(L, -2);
		return 1;
	}

	int NewIndexUserDatum(lua_State* L)
	{
		entt::meta_type type = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));
		if (lua_isuserdata(L, 1) == false)
		{
			TURT_CORE_ASSERT(false, "Expected a userdatum to index on stack");
		}
		if (lua_isstring(L, 2) == false)
		{
			TURT_CORE_ASSERT(false, "Expected a native property/method name on stack when indexing native type");
		}

		char const* fieldName = lua_tostring(L, 2);

		entt::meta_data data = type.data(entt::hashed_string{ fieldName });
		if (data)
		{
			entt::meta_any& ud = *(entt::meta_any*)lua_touserdata(L, 1);
			int luaType = lua_type(L, 3);
			switch(luaType)
			{
			case LUA_TNUMBER:
				if (entt::resolve<int>() = type)
				{
					int val = (int)lua_tonumber(L, 3);
					TURT_CORE_ASSERT(data.set(ud , val), "Failed to set native value");
				}
				else if (entt::resolve<uint32_t>() = type)
				{
					uint32_t val = (uint32_t)lua_tonumber(L, 3);
					TURT_CORE_ASSERT(data.set(ud, val), "Failed to set native value");
				}
				else if (entt::resolve<float>() = type)
				{
					float val = (float)lua_tonumber(L, 3);
					TURT_CORE_ASSERT(data.set(ud, val), "Failed to set native value");
				}
				break;
			default:
				TURT_CORE_ASSERT(false, "Unrecognized lua Type");
				break;
			}
		}


		lua_getuservalue(L, 1);
		lua_pushvalue(L, 2);
		lua_pushvalue(L, 3);
		lua_settable(L, -3);

		return 0;
	}


	int CallGlobalFromLua(lua_State* L)
	{
		//TURT_CORE_TRACE("CALLING GLOBAL FUNCTION (LUA)");
		//entt::meta_func* f = (entt::meta_func*)lua_touserdata(L, lua_upvalueindex(1));
		//entt::meta_func& func{ *f };

		entt::meta_type type = entt::resolve_id(lua_tonumber(L, lua_upvalueindex(1)));
		entt::meta_func func = type.func(lua_tonumber(L, lua_upvalueindex(2)));
		entt::meta_any instance{};
		return InvokeMethod(L, func, instance);

	}

	lua_State* CreateScript()
	{
		lua_State* L = luaL_newstate();
		luaopen_base(L);

		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setglobal(L, "Global");

		lua_pushvalue(L, -1);
		entt::meta_range<entt::meta_type> range{ entt::internal::meta_context::local() };
		for (auto& type : range)
		{
			auto funcs = type.func();
			for (auto& func : funcs)
			{
				entt::meta_any name = func.prop("Name"_hs).value();
				char const** cName = name.try_cast<char const*>();
				TURT_CORE_ASSERT(name, "Nameless Global Meta Function while binding lua.");
				if (func.is_static())
				{
					lua_pushstring(L, *cName);
					lua_pushnumber(L, type.id());
					lua_pushnumber(L, func.id());
					lua_pushcclosure(L, CallGlobalFromLua, 2);
					lua_settable(L, -3);
				}
			}

		}
		for (auto& type : range)
		{
			entt::meta_prop nameProp = type.prop("Name"_hs);
			if (nameProp) // Wont register types without a "Name"_hs prop (such as the Global struct)
			{
				char const** cName = nameProp.value().try_cast<char const*>();

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
			}
		}

		return L;
	}

	int LoadScript(lua_State* L, const char* script)
	{
		return luaL_loadstring(L, script);
		int doResult = luaL_dostring(L, script);
		if (doResult != LUA_OK)
		{
			TURT_CORE_ERROR("Error: {0}\n", lua_tostring(L, -1));
		}
	}

	int ExecuteScript(lua_State* L)
	{
		return lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	void CloseScript(lua_State* L)
	{
		lua_close(L);
	}

	//Register types and functions to ECS (entt) meta system
	void InitComponentMeta()
	{
		//-------------------LUA TESTING--------------------------------------------------------

		entt::meta<GlobalMetaFunctionsComponent>().type("Global"_hs)
			.func<&HelloWorld>("HelloWorld"_hs).prop("Name"_hs, "HelloWorld")
			.func<&HelloWorld2>("HelloWorld2"_hs).prop("Name"_hs, "HelloWorld2")
			.func<&HelloWorld3>("HelloWorld3"_hs).prop("Name"_hs, "HelloWorld3")
			.func<&GlobalMetaFunctionsComponent::MyFuncTest>("MyFuncTest"_hs).prop("Name"_hs, "MyFuncTest")
			.func<&Add>("Add"_hs).prop("Name"_hs, "Add");


		entt::meta<Sprite>().type("Sprite"_hs)
			.prop("Name"_hs, "Sprite")
			.ctor<>()
			.func<&Sprite::Move>("Move"_hs).prop("Name"_hs, "Move")
			.func<&Sprite::Draw>("Draw"_hs).prop("Name"_hs, "Draw")
			.data<&Sprite::x>("x"_hs)
			.data<&Sprite::y>("y"_hs);
			

		constexpr char* LUA_SCRIPT = R"(
			-- this is a lua script
			Global.HelloWorld()
			Global.HelloWorld2()
			local c= Global.Add(10, 2)
			Global.HelloWorld3( c, 42)
			local spr = Sprite.new()
			spr:Draw()
			local d = spr:Move(2, 2)
			spr:Draw()
			-- move(4, 4)
			spr:Move(d, d)
			spr:Draw()
			local x = spr.x
			spr.x = 10
			-- move 12, 12
			spr:Move(x, x)
			spr:Draw()
			spr.img = 42
			local im = spr.img	
			print(im)

			function Foo(x, y)
				Global.HelloWorld3( x, y)
			end

			function Bar()
				print("Im The Bar")
			end

			function Render(sprite)
				sprite.x = sprite.x + 10
				sprite:Draw()
			end
		)";

		lua_State* L = CreateScript();
		LoadScript(L, LUA_SCRIPT);
		if (ExecuteScript(L) != LUA_OK)
		{
			TURT_CORE_ASSERT("Error: {0}\n", lua_tostring(L, -1));
		}

		int x = 1;
		int y = 2;

		CallScriptFunction(L, "Foo", x, y);
		CallScriptFunction(L, "Bar");
		Sprite sprite;
		CallScriptFunction(L, "Render", sprite);
		CallScriptFunction(L, "Render", sprite);
		sprite.Draw();
		sprite.x += 10;
		CallScriptFunction(L, "Render", sprite);


		// -----------------------------------------------------------------------------------------------


		RegisterComponent<TagComponent>("TagComponent");
		RegisterComponent<TransformComponent>("TransformComponent");
		RegisterComponent<SpriteRendererComponent>("SpriteRendererComponent");
		RegisterComponent<CameraComponent>("CameraComponent");
		RegisterComponent<NativeScriptComponent>("NativeScriptComponent");
		RegisterComponent<ParticleSpawnerComponent>("ParticleSpawnerComponent");
		RegisterComponent<TileSetComponent>("TileSetComponent");
		RegisterComponent<GridComponent>("GridComponent");
		RegisterComponent<TileMapComponent>("TileMapComponent");



	}
} 