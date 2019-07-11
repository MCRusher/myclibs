//Should be at the end of the

#ifdef TRALLOC_REPLACE
#undef malloc
#undef calloc
#undef realloc
#undef free
#undef TRALLOC_REPLACE
#endif // TRALLOC_REPLACE
