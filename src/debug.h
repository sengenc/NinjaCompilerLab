//
// Created by student on 27.01.22.
//

#ifndef VERSION8MK3_DEBUG_H
#define VERSION8MK3_DEBUG_H
#include "structs.h"

#define IMMEDIATE(x) (x & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) \
| 0xFF000000 : (i))

extern unsigned int stack_pointer;
extern unsigned int opcode;
extern unsigned int instruction;
extern unsigned int program_counter;
extern unsigned int *program_memory;
extern unsigned int frame_pointer;
extern int static_pointer;
extern int immediate;
extern int num_instructions;
extern int num_var;
extern int break_point;
extern int version;
extern StackSlot *stack;
extern ObjRef *sda;
long strtol_value;


extern void debug_version(void);
extern void help(void);
extern void debug();
extern void halt();
extern int isEmpty(void);
extern int isFull(void);
extern void push_object(int x);
extern void push_number(int x);
extern int pop_num(void);
extern ObjRef pop_ref(void);
extern void pushc(int x);
extern void rdchr(void);
extern void rdint(void);
extern void wrint(void);
extern void wrchr(void);
extern void add(void);
extern void sub(void);
extern void mul(void);
extern void division(void);
extern void mod(void);
extern void pushg(int n);
extern void popg(int n);
extern void asf(int n);
extern void rsf(void);
extern void pushl(int n);
extern void popl(int n);
extern void eq(void);
extern void ne(void);
extern void lt(void);
extern void le(void);
extern void gt(void);
extern void ge(void);
extern void jmp(int n);
extern void brf(int n);
extern void brt(int n);
extern void call(int n);
extern void ret();
extern void drop(int n);
extern void pushr();
extern void popr();
extern void dup();
extern ObjRef newPrimitiveObject(unsigned int numBytes);
extern ObjRef newCompoundObject(int numObjRefs);
extern void new(int arrSize);
extern void getf(int index);
extern void putf(int index);
extern void newa(void);
extern void getfa(void);
extern void putfa(void);
extern void getsz(void);
extern void pushn(void);
extern void refeq(void);
extern void refne(void);

extern void list();

#define HALT 0
#define PUSHC 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define MOD 6
#define RDINT 7
#define WRINT 8
#define RDCHR 9
#define WRCHR 10
#define PUSHG 11
#define POPG 12
#define ASF 13
#define RSF 14
#define PUSHL 15
#define POPL 16
#define EQ 17
#define NE 18
#define LT 19
#define LE 20
#define GT 21
#define GE 22
#define JMP 23
#define BRF 24
#define BRT 25
#define CALL 26
#define RET 27
#define DROP 28
#define PUSHR 29
#define POPR 30
#define DUP 31
#define NEW 32
#define GETF 33
#define PUTF 34
#define NEWA 35
#define GETFA 36
#define PUTFA 37
#define GETSZ 38
#define PUSHN 39
#define REFEQ 40
#define REFNE 41
#endif //VERSION8MK3_DEBUG_H
