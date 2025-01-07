#include "Core.h"

#include <iostream>
#include <lua.hpp> 

namespace Core
{
    void InitializeLua()
    {
        lua_State* L = luaL_newstate(); // Create a new Lua state
        if (!L) {
            printf("Failed to create Lua state.\n");
            return;
        }

        luaL_openlibs(L); // Open Lua standard libraries

        // Load and run a Lua script
        if (luaL_dofile(L, "script.lua") != LUA_OK) {
            printf("Lua Error: %s\n", lua_tostring(L, -1));
        }

        lua_close(L); // Close the Lua state

    }

	void PrintHelloWorld()
	{
		std::cout << "Hello World!\n";

	}



}


