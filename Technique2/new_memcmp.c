#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
    printf("memcmp(%s, %s, %u)\n", s1, s2, (int)n);
    
    return 0;
}