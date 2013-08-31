/* all platform-specific includes and defines go in this file */
#ifndef PLATFORM_H
#define PLATFORM_H

#define LARGE_INT_POWER_OF_TEN 1000000000   /* the largest power of ten which fits in an int on this architecture */
#if defined(__hppa__) || defined(__sparc__)
#define ALIGN_TYPE double                   /* the default data type to use for alignment */
#else
#define ALIGN_TYPE void *                   /* the default data type to use for alignment */
#endif

#define GLOBAL_TABLE_SIZE           97      /* global variable table */
#define STRING_TABLE_SIZE           97      /* shared string table size */
#define STRING_LITERAL_TABLE_SIZE   97      /* string literal table size */
#define PARAMETER_MAX               16      /* maximum number of parameters to a function */
#define LINEBUFFER_MAX              256     /* maximum number of characters on a line */
#define LOCAL_TABLE_SIZE            11      /* size of local variable table (can expand) */
#define STRUCT_TABLE_SIZE           11      /* size of struct/union member table (can expand) */

#define INTERACTIVE_PROMPT_START "starting picoc " PICOC_VERSION "\n"
#define INTERACTIVE_PROMPT_STATEMENT "picoc> "
#define INTERACTIVE_PROMPT_LINE "     > "


//extern jmp_buf ExitBuf;

//#  define HEAP_SIZE (8*1024)               /* space for the heap and the stack */
/*#  define NO_HASH_INCLUDE */
#  define USE_MALLOC_STACK 
#  define USE_MALLOC_HEAP
#  define NO_FP
#  define NO_CTYPE
#  define NO_MODULUS
#  include <stdlib.h>
#  include <ctype.h>
#  include <string.h>
#  include <sys/types.h>
#  include <stdarg.h>
//#  include <setjmp.h>
/* #  include <nuttx/math.h> */
#  define assert(x)
#  define BUILTIN_MINI_STDLIB
#  undef BIG_ENDIAN
#  define USE_READLINE

extern int ExitBuf[];

#endif /* PLATFORM_H */
