//
// Created by student on 27.01.22.
//

#ifndef VERSION8MK3_STRUCTS_H
#define VERSION8MK3_STRUCTS_H

#include <stdbool.h>
#include "support.h"

#define MSB (1 << (8 * sizeof(unsigned int) - 1))
#define IS_PRIMITIVE(objRef) (((objRef)->size & MSB) == 0)
#define GET_ELEMENT_COUNT(objRef) ((objRef)->size & ~MSB)
#define GET_REFS_PTR(objRef) ((ObjRef *) (objRef)->data)

void garbage_collector(void);
void scan(void);
void flip(void);
ObjRef relocate(ObjRef orig);
ObjRef alloc_memory(size_t memory_size);
ObjRef calculateForwardPointer(ObjRef orig);
unsigned int sizeOfComp(int numOfVars);
unsigned int setBrokenheart(ObjRef copy);
int sizeOfPrim(int numBytes);

typedef struct {
    unsigned int size;
    unsigned char data[1];
} Object;

typedef struct {
    bool isObjRef;
    union {
        ObjRef objRef; // isObjRef = TRUE
        int number; // isObjRef = FALSE
    } u;
} StackSlot;

#endif //VERSION8MK3_STRUCTS_H
