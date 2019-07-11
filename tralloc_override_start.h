//does the equivalent of below, while ensuring a copy of tralloc.h has been included
//Should be defined at the start of function files that perform allocations but are
//also designed to use tralloc.

#ifdef TRALLOC_REPLACE
#include "tralloc.h"
#define malloc(x) tr_malloc((x))
#define calloc(x,y) tr_calloc((x),(y))
#define realloc(x,size) tr_realloc((x),(size))
#define free(x) tr_free((x))
#endif // TRALLOC_REPLACE
