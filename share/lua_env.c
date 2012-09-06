/*
 * lua.c
 *
 *  Created on: Sep 6, 2012
 *      Author: luan
 */


#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdarg.h>

#include "./gdef.h"

static lua_State *L = NULL;

lua_State *
get_lua_state(){
	return L;
}

void
init_lua_state(char *file, char *path, char *cpath){
	L=luaL_newstate();
    luaopen_base(L);
    luaopen_table(L);
    luaopen_string(L);
    luaopen_math(L);
    luaL_openlibs(L);
	int err = luaL_dofile(L, file);
	if (err) {
		fprintf(stderr,"%s\n",lua_tostring(L,-1));
		lua_close(L);
		exit(1);
	}
	setenv("LUA_PATH",path,1);
	setenv("LUA_CPATH",cpath,1);
}

void
reg_function(char *name, void *func){
	lua_pushcfunction(L,func) ;
	lua_setglobal(L,name) ;
}

void
call_message_function(char *func, char *msg){
	lua_getglobal(L, func);
	lua_pushstring(L, msg);
	lua_pcall(L, 1, 0, 0);

}



static void
l_error(lua_State *L, const char *fmt, ...)
{
	char nowTimeBuff[64];
	time_t now = time(NULL);
	strftime(nowTimeBuff,sizeof(nowTimeBuff),"%d %b %H:%M:%S",localtime(&now));

	char msg[1024];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(msg, sizeof(msg), fmt, ap);
	va_end(ap);

	FILE *fp = stdout;
	fprintf(fp,"[%d] %s %s\n", getpid(), nowTimeBuff, msg);
	fflush(fp);

	if (L) lua_close(L);

	exit(EXIT_FAILURE);
}

void
call_va (const char *func, const char *sig, ...) {
	va_list vl;
	int narg, nres;
	/* number of arguments and results */
	va_start(vl, sig);
	lua_getglobal(L, func); /* get function */
	/* push arguments */
	narg = 0;
	while (*sig) {
		/* push arguments */
		switch (*sig++) {
		case 'd': /* double argument */
			lua_pushnumber(L, va_arg(vl, double));
			break;
		case 'i': /* int argument */
			lua_pushnumber(L, va_arg(vl, int));
			break;
		case 's': /* string argument */
			lua_pushstring(L, va_arg(vl, char *));
			break;
		case '>':
			goto endwhile;
		default:
			l_error(L, "invalid option (%c)", *(sig - 1));
			break;
		}
		narg++;
		luaL_checkstack(L, 1, "too many arguments");
	}
endwhile:
	/* do the call */
	nres = strlen(sig);
	/* number of expected results */
	if (lua_pcall(L, narg, nres, 0) != 0){ /* do the call */
		l_error(L, "error running function `%s': %s",
				func, lua_tostring(L, -1));
	}
	/* retrieve results */
	nres = -nres; /* stack index of first result */
	while (*sig) { /* get results */
		switch (*sig++) {
		case 'd': /* double result */
			if (!lua_isnumber(L, nres))
				l_error(L, "wrong result type");
			*va_arg(vl, double *) = lua_tonumber(L, nres);
			break;
		case 'i': /* int result */
			if (!lua_isnumber(L, nres))
				l_error(L, "wrong result type");
			*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
			break;
		case 's': /* string result */
			if (!lua_isstring(L, nres))
				l_error(L, "wrong result type");
			*va_arg(vl, const char **) = lua_tostring(L, nres);
			break;
		default:
			l_error(L, "invalid option (%c)", *(sig - 1));
			break;
		}
		nres++;
	}
	va_end(vl);
}
