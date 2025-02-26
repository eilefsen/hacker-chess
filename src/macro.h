#pragma once
#include <stdio.h>

#ifdef DEBUG_PRINT
#define DEBUG(...) fprintf (stderr, __VA_ARGS__)
#else
#define DEBUG // no op
#endif

#ifdef DEBUG_PRINT_CHECK
#define DEBUG_CHECK(...) fprintf (stderr, __VA_ARGS__)
#else
#define DEBUG_CHECK // no op
#endif

#ifdef DEBUG_PRINT_VALIDATE
#define DEBUG_VALIDATE(...) fprintf (stderr, __VA_ARGS__)
#else
#define DEBUG_VALIDATE // no op
#endif
