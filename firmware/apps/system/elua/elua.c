
/* platform-dependent code for running programs is in this file */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./lua/lua.h"

int elua_main(int argc, char **argv)
{
    char* lua_argv[] = { (char *)"lua", NULL };
    lua_main(1, lua_argv);

    return 0;
}


