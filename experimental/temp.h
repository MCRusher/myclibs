#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>

#ifndef NDEBUG
#include <stdint.h>
#include <inttypes.h>
#endif //NDEBUG

//maybe keep address of node stored behind address to data
//upside: generally faster reallocation and insertion
// (also removal if made a doubly linked list)
//downside: adds sizeof(*) for each allocation,
// freeing or changing the address of a node will result in
// a given pointer working, but operations on the node
// through the pointer failing 

typedef struct cgEfficientSafeAllocNode {
    struct cgEfficientSafeAllocNode *prev, *next;
    void * addr;
    size_t count;
#ifndef NDEBUG
    char const * file;
    uintmax_t line;
#endif //NDEBUG
} cgESANode;

static cgESANode * cgESANodeFirst = NULL;

void cgESA_freeall(void);
#ifndef NDEBUG
void * cgESA_alloc(char const * const file, uintmax_t line, size_t count)
#else
void * cgESA_alloc(size_t count)
#endif //NDEBUG
{
    static _Bool once_only = 0;
    if(!once_only){
        atexit(cgESA_freeall);
        once_only = 1;
    }
    cgESANode * n = malloc(sizeof(cgESANode));
    if(cgESANodeFirst!=NULL)
        cgESANodeFirst->prev = n;
    n->prev = NULL;
    n->next = cgESANodeFirst;
    alignas(alignof(void*)) cgESANode ** buf = malloc(sizeof(cgESANode*)+count);
    n->addr = buf;
    n->count = count;
#ifndef NDEBUG
    n->file = file;
    n->line = line;
#endif //NDEBUG
    *buf = n;
    ++buf;
    cgESANodeFirst = n;
    return (void*)buf;
}

#ifndef NDEBUG
void * cgESA_realloc(char const * const file, uintmax_t line, void * addr, size_t count)
#else
void * cgESA_realloc(void * addr, size_t count)
#endif //NDEBUG
{
    if(addr==NULL){
#ifndef NDEBUG
        return cgESA_alloc(file,line,count);
#else
        return cgESA_alloc(count);
#endif //NDEBUG
    }
    alignas(alignof(void*)) cgESANode ** orig = addr;
    --orig;
    cgESANode * node = *orig;
    void * tmp = realloc(orig,sizeof(cgESANode*)+count);
    //NULL CHECK
    node->addr = tmp;
    node->count = count;
    //intentional no change to node->line
    return tmp;
}

void * cgESA_free(void * addr){
    alignas(alignof(void*)) cgESANode ** orig = addr;
    --orig;
    cgESANode * node = *orig;
    free(orig);
    if(node->prev!=NULL)
        node->prev->next = node->next;
    if(node->next!=NULL)
        node->next->prev = node->prev;
    if(node==cgESANodeFirst)
        cgESANodeFirst = NULL;
    free(node);
    return NULL;
}

void cgESA_freeall(void){
    if(cgESANodeFirst != NULL){
#ifndef NDEBUG
        uintmax_t counter = 0;
        uintmax_t bytes = 0;
        _Bool ovf = 0;
        fputs("cgESA Allocation Cleanup:\n",stderr);
#endif //NDEBUG
        cgESANode * n = cgESANodeFirst;
        while(n!=NULL){
#ifndef NDEBUG
            ++counter;
            if(!ovf){
                if(bytes+n->count < bytes)
                    ovf = 1;
                else
                    bytes += n->count;
            }

            fprintf(stderr,
                " %" PRIuMAX " = [\n"
                "  File: \"%s\",\n"
                "  Line: %" PRIuMAX ",\n"
                "  Addr: %p,\n"
                "  Size: %zu bytes\n"
                " ]\n",
                counter,n->file,n->line, n->addr,n->count);
#endif //NDEBUG
            free(n->addr);
            cgESANode * tmp = n;
            n = n->next;
            free(tmp);
        }
#ifndef NDEBUG
        fprintf(stderr,
            "-----------------\n"
            " Total Cleaned:\n"
            "  -Blocks: %" PRIuMAX "\n",
            counter);
        if(!ovf)
            fprintf(stderr,
                "  -Bytes: %" PRIuMAX "\n",
                bytes);
        else
            fputs(
                "  -Bytes: Overflowed\n",stderr);
#endif //NDEBUG
    }
}

size_t cgESA_bytes(void * addr){
    alignas(alignof(void*)) cgESANode ** orig = addr;
    --orig;
    cgESANode * node = *orig;
    return node->count;
}

#ifndef NDEBUG
#define cgESA_alloc(count) cgESA_alloc(__FILE__, __LINE__, count)
#define cgESA_realloc(addr, count) cgESA_realloc(__FILE__, __LINE__, addr, count)
#endif //NDEBUG

#include <stdint.h>

int main(void){
    char * str = cgESA_alloc(2);
    cgESA_realloc(str,4);
    str[0] = 'o';
    str[1] = 'h';
    str[2] = '!';
    str[3] = '\0';

    printf("\"%s\" : %zu\n",str,cgESA_bytes(str));

    cgESA_alloc(12);

    cgESA_alloc(4096);

    cgESA_alloc(SIZE_MAX-1/2);

    //cgESA_free(str);
}