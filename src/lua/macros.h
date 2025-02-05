#ifndef LUA_MACROS_H
#define LUA_MACROS_H

#include "lua.h"

// Stack navigation
#define LUA_GET_GLOBAL_TABLE(name) \
    lua_getglobal(L, name); \
    if (lua_isnil(L, -1)) { \
        lua_pop(L, 1); \
        lua_newtable(L); \
        lua_setglobal(L, name); \
        lua_getglobal(L, name); \
    }

#define LUA_GET_FIELD(name) \
    lua_getfield(L, -1, name); \
    if (lua_isnil(L, -1)) { \
        lua_pop(L, 1); \
        lua_newtable(L); \
        lua_setfield(L, -2, name); \
        lua_getfield(L, -1, name); \
    }

#define LUA_CHECK_TABLE(name) \
    if (!lua_istable(L, -1)) { \
        TraceLog(LOG_ERROR, "Table %s not found or not a table!", name); \
        return; \
    }

// Value setters
#define LUA_SET_TABLE(name, ...) \
    lua_newtable(L); \
    __VA_ARGS__ \
    lua_setfield(L, -2, name);

#define LUA_SET_INT(num, str) \
    lua_pushinteger(L, num); \
    lua_setfield(L, -2, str);

#define LUA_SET_NUMBER(num, str) \
    lua_pushnumber(L, num); \
    lua_setfield(L, -2, str);

#define LUA_SET_BOOL(b, str) \
    lua_pushboolean(L, b); \
    lua_setfield(L, -2, str);

#define LUA_SET_STRING(s, str) \
    lua_pushstring(L, s); \
    lua_setfield(L, -2, str);

#define LUA_SET_FUNCTION(func, str) \
    lua_pushcfunction(L, func); \
    lua_setfield(L, -2, str);

// Value getters with defaults
#define LUA_GET_STRING_FIELD(dest, name, default_val) \
    lua_getfield(L, -1, name); \
    dest = lua_isstring(L, -1) ? lua_tostring(L, -1) : default_val; \
    lua_pop(L, 1); \

#define LUA_GET_INT_FIELD(dest, name, default_val) \
    lua_getfield(L, -1, name); \
    dest = lua_isnumber(L, -1) ? lua_tointeger(L, -1) : default_val; \
    lua_pop(L, 1); \

#define LUA_GET_NUMBER_FIELD(dest, name, default_val) \
    lua_getfield(L, -1, name); \
    dest = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : default_val; \
    lua_pop(L, 1); \

#define LUA_GET_BOOL_FIELD(dest, name, default_val) \
    lua_getfield(L, -1, name); \
    dest = lua_isboolean(L, -1) ? lua_toboolean(L, -1) : default_val; \
    lua_pop(L, 1); \

// Stack cleanup
#define LUA_POP(n) lua_pop(L, n)

#endif // LUA_MACROS_H
