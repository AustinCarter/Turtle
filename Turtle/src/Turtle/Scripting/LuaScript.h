#pragma once

#include <lua.hpp>

#include <entt.hpp>


//#include "LuaAreaAllocator.h"

namespace Turtle {
	//not thrilled that this has to be forward declared here for CallScriptFunction to work
	int ToLua(lua_State* L, entt::meta_any& result);

	class LuaScript
	{

	public:
		LuaScript();
		~LuaScript() 
		{ 
			CloseScript(m_State); 
			//free(m_Memory);
		}

		//static lua_State* CreateScript();
		int LoadScript(const char* script);
		int LoadScriptFromFile(const std::string& filepath);
		int ExecuteScript();
		void LogError();

		const std::string& LuaScript::GetFilepath() { return m_Filepath; }

		int PutOnLuaStack() { return 0; }

		template<typename T>
		inline int PutOnLuaStack(T& toPutOnStack)
		{
			if (entt::resolve<T>().is_class()/*|| entt::resolve<T>().is_pointer()*/)
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
					TURT_CORE_ERROR("Unable to call script funciton: \n LUA ERROR: {0}", lua_tostring(m_State, -1));
				}
			}
			else
			{
				TURT_CORE_ERROR("Unknown script function {0}", funcName);
			}
		}
	private:
		lua_State* m_State = nullptr;
		void CloseScript(lua_State* L);
		std::string m_Filepath;
	};
}