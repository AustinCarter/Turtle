#pragma once

#include <lua.hpp>

#include <entt.hpp>

namespace Turtle {
	int ToLua(lua_State* L, entt::meta_any& result);
	class LuaScript
	{
	public:
		LuaScript();
		~LuaScript() { CloseScript(m_State); }

		//static lua_State* CreateScript();
		int LoadScript(const char* script);
		int ExecuteScript();

		
		/*
		std::string MetaTableName(const entt::meta_type type);

		int CreateUserDatum(lua_State* L);
		int CreateUserDatumFromMetaObject(lua_State* L, const entt::meta_any& object);
		int ToLua(lua_State* L, entt::meta_any& result);
		int InvokeMethod(lua_State* L, entt::meta_func& func, entt::meta_any& instance);
		int DestroyUserDatum(lua_State* L);
		static int InvokeFuncOnUserDatum(lua_State* L);
		int IndexUserDatum(lua_State* L);
		int NewIndexUserDatum(lua_State* L);
		int CallGlobalFromLua(lua_State* L);
		*/

		int PutOnLuaStack() { return 0; }

		template<typename T>
		inline int PutOnLuaStack(T& toPutOnStack)
		{
			if (entt::resolve<T>().is_class() /*|| entt::resolve<T>().is_pointer()*/)
			{
				entt::meta_any val{ std::ref(toPutOnStack) };
				return ToLua(m_State, val);
			}
			else
			{
				entt::meta_any val{ toPutOnStack };
				return ToLua(m_State, val);
			}
		}

		template<typename T, typename... REST>
		inline int PutOnLuaStack(T& toPutOnStack, REST&... rest)
		{
			return PutOnLuaStack( toPutOnStack) + PutOnLuaStack(rest...);
		}

		template<typename... ARGS>
		inline void CallScriptFunction(const char* funcName, ARGS&... args)
		{
			lua_getglobal(m_State, funcName);
			if (lua_type(m_State, -1) == LUA_TFUNCTION)
			{
				int numArgs = PutOnLuaStack(args... );
				if (lua_pcall(m_State, numArgs, 0, 0))
				{
					TURT_CORE_ASSERT(false, "Unable to call script funciton");
				}
			}
			else
			{
				TURT_CORE_ASSERT(false, "Unknown script function");
			}
		}
	private:
		lua_State* m_State = nullptr;
		void CloseScript(lua_State* L);
		std::string m_Path;
	};
}