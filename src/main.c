#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"
#include "structs.h"
#include "bigint.h"

#define IMMEDIATE(x) (x & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) \
| 0xFF000000 : (i))

#define TRUE    1
#define FALSE   0
#define MSB (1 << (8 * sizeof(unsigned int) - 1))
#define IS_PRIMITIVE(objRef) (((objRef)->size & MSB) == 0)
#define GET_ELEMENT_COUNT(objRef) ((objRef)->size & ~MSB)
#define GET_REFS_PTR(objRef) ((ObjRef *) (objRef)->data)
#define HAS_BROKENHEART(objRef) (((objRef)->size & BROKENHEART) != 0)
#define BROKENHEART (1 << (8 * sizeof(unsigned int) - 2))

StackSlot *stack;
ObjRef *sda;
ObjRef retValReg;
bool isStackInit = false;
bool isHeapInit = false;
bool isGCPurge = false;
long stack_cap;
long total_heap_cap;
long maxHalbspeicher;
long benutzteBytes = 0;
char *freizeiger;
char *zielspeicher;
char *quellspeicher;
unsigned int stack_pointer = 0;
unsigned int program_counter = 0;
unsigned int frame_pointer = 0;
unsigned int *program_memory;
unsigned int instruction;
unsigned int opcode = 1;
int static_pointer = 0;
int num_instructions;
int immediate;
int num_var;
int version;

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


void halt(void) {
    printf("Ninja Virtual Machine stopped\n");
    exit(0);
}

int isEmpty(void) {
    if (stack_pointer == -1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int isFull(void) {
    if (stack_pointer == stack_cap * 1024) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void push(StackSlot val) {
    if (!isFull()) {
        stack[stack_pointer] = val;
        stack_pointer = static_pointer + 1;
    } else {
        printf("Stack is full!");
        exit(1);
    }
}

void push_object(int x) {
    if (!isFull()) {
        stack[stack_pointer].isObjRef = true;
        stack[stack_pointer].u.objRef = NULL;
        bigFromInt(SIGN_EXTEND(x));
        stack[stack_pointer].u.objRef = bip.res;
        stack_pointer = stack_pointer + 1;
    } else {
        printf("Stack is full!");
        exit(1);
    }
}


void push_ref(ObjRef x) {
    if (!isFull()) {
        stack[stack_pointer].isObjRef = true;
        stack[stack_pointer].u.objRef = x;
        stack_pointer = stack_pointer + 1;
    } else {
        printf("Stack is full!");
        exit(1);
    }
}

void push_number(int x) {
    if (!isFull()) {
        stack[stack_pointer].isObjRef = false;
        stack[stack_pointer].u.number = x;
        stack_pointer = stack_pointer + 1;
    } else {
        printf("Stack is full!");
        exit(1);
    }
}

int pop_num(void) {
    int data;
    if (!isEmpty()) {
        data = stack[stack_pointer - 1].u.number;
        stack[stack_pointer - 1].isObjRef = false;
        stack[stack_pointer - 1].u.number = 0;
        stack_pointer = stack_pointer - 1;
        return data;
    } else {
        printf("Stack is empty!");
        exit(1);
    }
}

ObjRef pop_ref(void) {
    if (!isEmpty()) {
        if (stack[stack_pointer - 1].isObjRef) {
            ObjRef intObject = NULL;
            intObject = stack[stack_pointer - 1].u.objRef;
            stack[stack_pointer - 1].u.objRef = NULL;
            stack[stack_pointer - 1].isObjRef = true;
            stack_pointer = stack_pointer - 1;
            return intObject;
        } else {
            printf("In der Position steht kein ObjRef\n");
            exit(1);
        }
    } else {
        printf("Stack is empty!");
        exit(1);
    }
}

StackSlot pop(void) {
    return stack[--stack_pointer];
}


void pushc(int x) {
    push_object(x);

}

void rdchr(void) {
    ObjRef obj = NULL;
    char char_input;
    scanf("%c", &char_input);
    bigFromInt(char_input);
    obj = bip.res;
    push_ref(obj);
}


void rdint(void) {
    ObjRef obj = NULL;
    bigRead(stdin);
    obj = bip.res;
    push_ref(obj);
}


void wrint(void) {
    bip.op1 = pop_ref();
    bigPrint(stdout);
}

void wrchr(void) {
    bip.op1 = pop_ref();
    printf("%c", bigToInt());
}

void add(void) {
    bip.op2 = pop_ref(); //n2
    bip.op1 = pop_ref(); //n1
    bigAdd();
    push_ref(bip.res);
}

void sub(void) {
    bip.op2 = pop_ref();
    bip.op1 = pop_ref();
    bigSub();
    push_ref(bip.res);
}

void mul(void) {
    bip.op2 = pop_ref();
    bip.op1 = pop_ref();
    bigMul();
    push_ref(bip.res);
}

void division(void) {
    bip.op2 = pop_ref();
    bip.op1 = pop_ref();
    bigDiv();
    push_ref(bip.res);
}

void mod(void) {
    bip.op2 = pop_ref();
    bip.op1 = pop_ref();
    bigDiv();
    push_ref(bip.rem);
}

void pushg(int n) {
    push_ref(sda[n]);
}

void popg(int n) {
    sda[n] = pop_ref();
}

void asf(int n) {
    int counter;
    push_number(SIGN_EXTEND(frame_pointer));
    frame_pointer = stack_pointer;
    stack_pointer = stack_pointer + n;

    counter = SIGN_EXTEND(frame_pointer);
    while (counter < stack_pointer) {
        stack[counter].isObjRef = true;
        stack[counter].u.objRef = NULL;
        counter++;
    }
}


void rsf(void) {
    stack_pointer = frame_pointer;

    if (stack[stack_pointer - 1].isObjRef) {
        printf("ERROR rsf: couldn`n find old fp\n");
        exit(1);
    } else {
        frame_pointer = pop_num();
    }
}

void pushl(int n) {
    ObjRef ref = NULL;

    ref = stack[frame_pointer + n].u.objRef;

    push_ref(ref);
}


void popl(int n) {
    stack[frame_pointer + n].isObjRef = true;
    stack[frame_pointer + n].u.objRef = pop_ref();
}


void eq(void) {
    bip.op2 = pop_ref(); //n2
    bip.op1 = pop_ref(); //n1

    if (bigCmp() == 0) {
        bigFromInt(1);
        push_ref(bip.res);
    } else {
        bigFromInt(0);
        push_ref(bip.res);
    }

}

void ne(void) {
    bip.op2 = pop_ref(); //n2
    bip.op1 = pop_ref(); //n1
    if (bigCmp() != 0) {
        bigFromInt(1);
        push_ref(bip.res);
    } else {
        bigFromInt(0);
        push_ref(bip.res);
    }

}

void lt(void) {
    bip.op2 = pop_ref(); //n2
    bip.op1 = pop_ref(); //n1
    if (bigCmp() < 0) {
        bigFromInt(1);
        push_ref(bip.res);
    } else {
        bigFromInt(0);
        push_ref(bip.res);
    }
    //   push(b<a);
}

void le(void) {
    bip.op2 = pop_ref(); //n2
    bip.op1 = pop_ref(); //n1
    if (bigCmp() <= 0) {
        bigFromInt(1);
        push_ref(bip.res);
    } else {
        bigFromInt(0);
        push_ref(bip.res);
    }
    //  push(b<=a);
}

void gt(void) {
    bip.op2 = pop_ref(); //n2
    bip.op1 = pop_ref(); //n1

    if (bigCmp() > 0) {
        bigFromInt(1);
        push_ref(bip.res);
    } else {
        bigFromInt(0);
        push_ref(bip.res);
    }
    //   push(b>a);
}

void ge(void) {
    bip.op2 = pop_ref(); //n2
    bip.op1 = pop_ref(); //n1

    if (bigCmp() >= 0) {
        bigFromInt(1);
        push_ref(bip.res);
    } else {
        bigFromInt(0);
        push_ref(bip.res);
    }
    //   push(b>=a);
}

void jmp(int n) {
    program_counter = n;
}

void brf(int n) {
    bip.op1 = pop_ref();
    if (bigToInt() == FALSE) {
        jmp(n);
    }
}

void brt(int n) {
    bip.op1 = pop_ref();
    if (bigToInt() == TRUE) {
        jmp(n);
    }
}

void call(int n) {
    push_number(SIGN_EXTEND(program_counter));
    jmp(n);
}

void ret(void) {
    if (stack[stack_pointer].isObjRef) {
        printf("Error by ret_value, didnt find any number\n");
        exit(1);
    } else {
        int ret_value = pop_num();
        program_counter = ret_value;
    }

}

void drop(int n) {
    for (int i = 0; i < n; ++i) {
        pop();
    }
}

void pushr(void) {
    push_ref(retValReg);
}

void popr(void) {
    retValReg = pop_ref();
}

void dup(void) {
    ObjRef refs = pop_ref();
    push_ref(refs);
    push_ref(refs);
}


ObjRef newPrimObject(int dataSize) {
    ObjRef objRef = NULL;

    objRef = alloc_memory(sizeOfPrim(dataSize));
    if (objRef == NULL) {
        fatalError("newPrimObject() got no memory");
    }

    objRef->size = dataSize;

    return objRef;
}

ObjRef newCompoundObject(int numObjRefs) {
    ObjRef compObj;

    compObj = alloc_memory(sizeOfComp(numObjRefs));
    if (compObj == NULL) {
        perror("NewCompObject malloc error.");
        exit(1);
    }
    unsigned int arraySize = 0x80000000 | numObjRefs;

    compObj->size = arraySize;

    for (int i = 0; i < numObjRefs; ++i) {
        GET_REFS_PTR(compObj)[i] = NULL;
    }
    return compObj;
}

void new(int arrSize) {
    ObjRef objRef = NULL;
    objRef = newCompoundObject(arrSize);
    push_ref(objRef);
}

void getf(int index) {
    ObjRef objRef;
    ObjRef valRef;
    objRef = pop_ref();

    if (!IS_PRIMITIVE(objRef)) {
        valRef = GET_REFS_PTR(objRef)[index];
        push_ref(valRef);
    } else {
        perror("GETF Error.\n");
        exit(1);
    }

}

void putf(int index) {
    ObjRef objRef;
    objRef = pop_ref();
    GET_REFS_PTR(pop_ref())[index] = objRef;
}

void newa(void) {
    ObjRef objRef = NULL;
    bip.op1 = pop_ref();
    objRef = newCompoundObject(bigToInt());
    push_ref(objRef);
}

void getfa(void) {
    bip.op1 = pop_ref();
    int index = bigToInt();
    ObjRef valRef;
    ObjRef arrRef;
    arrRef = pop_ref();
    if (!IS_PRIMITIVE(arrRef)) {
        if (GET_REFS_PTR(arrRef)[index] != NULL) {
            valRef = GET_REFS_PTR(arrRef)[index];
            push_ref(valRef);
        } else {
            perror("GETFA Error.\n");
            exit(1);
        }
    } else {
        perror("GETFA IS_PRIMITIVE Error.\n");
        exit(1);
    }

}

void putfa(void) {
    //ob array nicht null ist, und index auf die gueltige stelle zeigt
    ObjRef valRef;
    valRef = pop_ref();
    bip.op1 = pop_ref();
    int index = bigToInt();
    ObjRef arrRef;
    arrRef = pop_ref();

    if (!IS_PRIMITIVE(arrRef)) {
        if (index != -1) {
            GET_REFS_PTR(arrRef)[index] = valRef;
        } else {
            perror("PUTFA Index Error.\n");
            exit(1);
        }
    } else {
        perror("PUTFA IS_PRIMITIVE Error.\n");
        exit(1);
    }
}

void getsz(void) {
    ObjRef objRef;
    objRef = pop_ref();
    int temp;

    if (!IS_PRIMITIVE(objRef)) {
        temp = GET_ELEMENT_COUNT(objRef);
        bigFromInt(temp);
        push_ref(bip.res);
    } else {
        bigFromInt(-1);
        push_ref(bip.res);
    }
}

void pushn(void) {
    ObjRef objRef;
    objRef = NULL;
    push_ref(objRef);
}

void refeq(void) {
    ObjRef obj1;
    ObjRef obj2;

    obj2 = pop_ref();
    obj1 = pop_ref();

    if (obj1 == obj2) {
        bigFromInt(1);
        push_ref(bip.res);
    } else {
        bigFromInt(0);
        push_ref(bip.res);
    }
}

void refne(void) {
    ObjRef obj1;
    ObjRef obj2;

    obj2 = pop_ref();
    obj1 = pop_ref();

    if (obj1 != obj2) {
        bigFromInt(1);
        push_ref(bip.res);
    } else {
        bigFromInt(0);
        push_ref(bip.res);
    }
}

void read(char path[]) {
    FILE *fp;
    size_t read_objects;

    int compare;
    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    char c[4];
    fread(c, 1, sizeof(int), fp); //ueberpruefen ob fread die Zahl 4 zurueckgibt, weil n=4
    compare = strncmp(c, "NJBF", sizeof(int));

    if (compare != 0) {
        printf("Format passt nicht.\n");
        exit(1);
    }

    fread(&version, sizeof(int), 1, fp);
    //nochmal 4 byte fuer version ueberpruefen
    //ueberpruefen ob die versionen passen, ob version=4
    if (version != 8) {
        printf("Version passt nicht.\n");
        exit(1);
    }


    fread(&num_instructions, sizeof(int), 1, fp);
    //mit malloc den Speicher holen und schreiben (fuer die Anzahl der Instr.) //malloc darf nicht NULL sein
    //

    program_memory = malloc(sizeof(int) * num_instructions);
    if (program_memory == NULL) {
        printf("Kein Speicher vorhanden.\n");
        exit(1);
    }

    fread(&num_var, sizeof(int), 1, fp);
    //wieder mit malloc genau wie bei num_instr.
    sda = malloc(sizeof(ObjRef) * num_var); //soll global sein
    //sda = malloc(sizeof(int) * num_var); //soll global sein
    if (sda == NULL) {
        printf("Kein Speicher fuer statische Daten.\n");
        exit(1);
    }

    for (int i = 0; i < num_var; ++i) {
        sda[i] = NULL;
    }


    read_objects = fread(program_memory, sizeof(int), num_instructions, fp); //ueberpruefen ob fread=num_instructions
    if (read_objects != num_instructions) {
        printf("Read error.\n");
        exit(1);
    }

    if (fclose(fp) != 0) {
        perror("ERROR - fclose\n");
        exit(1);
    }
}


void list() {
    unsigned int list_instruction;
    unsigned int list_opcode;
    unsigned int list_immediate;
    for (int i = 0; i < num_instructions; ++i) {
        printf("%03d: \t", i);
        list_instruction = program_memory[i];
        list_opcode = list_instruction >> 24;
        list_immediate = SIGN_EXTEND(IMMEDIATE(list_instruction));
        switch (list_opcode) {
            case 0:
                printf("halt\n");
                break;
            case 1:
                printf("pushc\t%d\n", list_immediate);
                break;
            case 2:
                printf("add\n");
                break;
            case 3:
                printf("sub\n");
                break;
            case 4:
                printf("mul\n");
                break;
            case 5:
                printf("div\n");
                break;
            case 6:
                printf("mod\n");
                break;
            case 7:
                printf("rdint\n");
                break;
            case 8:
                printf("wrint\n");
                break;
            case 9:
                printf("rdchr\n");
                break;
            case 10:
                printf("wrchr\n");
                break;
            case 11:
                printf("pushg\t%d\n", list_immediate);
                break;
            case 12:
                printf("popg\t%d\n", list_immediate);
                break;
            case 13:
                printf("asf\t%d\n", list_immediate);
                break;
            case 14:
                printf("rsf\n");
                break;
            case 15:
                printf("pushl\t%d\n", list_immediate);
                break;
            case 16:
                printf("popl\t%d\n", list_immediate);
                break;
            case 17:
                printf("eq\n");
                break;
            case 18:
                printf("ne\n");
                break;
            case 19:
                printf("lt\n");
                break;
            case 20:
                printf("le\n");
                break;
            case 21:
                printf("gt\n");
                break;
            case 22:
                printf("ge\n");
                break;
            case 23:
                printf("jmp\t%d\n", list_immediate);
                break;
            case 24:
                printf("brf\t%d\n", list_immediate);
                break;
            case 25:
                printf("brt\t%d\n", list_immediate);
                break;
            case 26:
                printf("call\t%d\n", list_immediate);
                break;
            case 27:
                printf("ret\n");
                break;
            case 28:
                printf("drop\t%d\n", list_immediate);
                break;
            case 29:
                printf("pushr\n");
                break;
            case 30:
                printf("popr\n");
                break;
            case 31:
                printf("dup\n");
                break;
            case 32:
                printf("new\t%d\n", list_immediate);
                break;
            case 33:
                printf("getf\t%d\n", list_immediate);
                break;
            case 34:
                printf("putf\t%d\n", list_immediate);
                break;
            case 35:
                printf("newa\n");
                break;
            case 36:
                printf("getfa\n");
                break;
            case 37:
                printf("putfa\n");
                break;
            case 38:
                printf("getsz\n");
                break;
            case 39:
                printf("pushn\n");
                break;
            case 40:
                printf("refeq\n");
                break;
            case 41:
                printf("refne\n");
                break;
            default:
                break;
        }
    }
}

void execute(unsigned int num) {
    opcode = num >> 24;
    immediate = SIGN_EXTEND(IMMEDIATE(num));
    switch (opcode) {
        case 0:
            halt();
            break;
        case 1:
            pushc(immediate);
            break;
        case 2:
            add();
            break;
        case 3:
            sub();
            break;
        case 4:
            mul();
            break;
        case 5:
            division();
            break;
        case 6:
            mod();
            break;
        case 7:
            rdint();
            break;
        case 8:
            wrint();
            break;
        case 9:
            rdchr();
            break;
        case 10:
            wrchr();
            break;
        case 11:
            pushg(immediate);
            break;
        case 12:
            popg(immediate);
            break;
        case 13:
            asf(immediate);
            break;
        case 14:
            rsf();
            break;
        case 15:
            pushl(immediate);
            break;
        case 16:
            popl(immediate);
            break;
        case 17:
            eq();
            break;
        case 18:
            ne();
            break;
        case 19:
            lt();
            break;
        case 20:
            le();
            break;
        case 21:
            gt();
            break;
        case 22:
            ge();
            break;
        case 23:
            jmp(immediate);
            break;
        case 24:
            brf(immediate);
            break;
        case 25:
            brt(immediate);
            break;
        case 26:
            call(immediate);
            break;
        case 27:
            ret();
            break;
        case 28:
            drop(immediate);
            break;
        case 29:
            pushr();
            break;
        case 30:
            popr();
            break;
        case 31:
            dup();
            break;
        case 32:
            new(immediate);
            break;
        case 33:
            getf(immediate);
            break;
        case 34:
            putf(immediate);
            break;
        case 35:
            newa();
            break;
        case 36:
            getfa();
            break;
        case 37:
            putfa();
            break;
        case 38:
            getsz();
            break;
        case 39:
            pushn();
            break;
        case 40:
            refeq();
            break;
        case 41:
            refne();
            break;
        default:
            break;
    }
}

void run_main() {
    while (opcode != HALT) {
        instruction = program_memory[program_counter];
        program_counter++;
        execute(instruction);
    }
}


int main(int argc, char *argv[]) {
    char *chr;
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--version", 9) == 0) {
            debug_version();
        } else if (strncmp(argv[i], "--help", 6) == 0) {
            help();
        } else if (strncmp(argv[i], "--debug", 7) == 0) {

            total_heap_cap = 8192;
            maxHalbspeicher = ((total_heap_cap / 2) * 1024);
            zielspeicher = malloc((total_heap_cap / 2) * 1024);
            quellspeicher = malloc((total_heap_cap / 2) * 1024);
            freizeiger = zielspeicher;
            retValReg = NULL;
            isHeapInit = true;
            stack_cap = 64;
            stack = malloc(stack_cap * 1024);
            isStackInit = true;

            read(argv[i + 1]);
            printf("DEBUG: file '%s' loaded (code size = %d, data size = %d)\n", argv[i + 1], num_instructions,
                   num_var);
            debug(program_memory);
        } else if (strncmp(argv[i], "--stack", 7) == 0) {
            if ((stack_cap = strtol(argv[i + 1], &chr, 10)) > 0) {
                stack = malloc(stack_cap * 1024);
                isStackInit = true;
                i++;
            }
            continue;
        } else if (strncmp(argv[i], "--heap", 6) == 0) {
            if ((total_heap_cap = strtol(argv[i + 1], &chr, 10)) > 0) {
                maxHalbspeicher = ((total_heap_cap / 2) * 1024);
                zielspeicher = malloc((total_heap_cap / 2) * 1024);
                quellspeicher = malloc((total_heap_cap / 2) * 1024);
                freizeiger = zielspeicher;
                retValReg = NULL;
                isHeapInit = true;
                i++;
            }
            continue;
        } else if (strncmp(argv[i], "--gcpurge", 9) == 0) {
            isGCPurge = true;
            continue;
        }
        if (!isHeapInit) {
            total_heap_cap = 8192 * 1024;
            maxHalbspeicher = ((total_heap_cap / 2));
            zielspeicher = malloc((total_heap_cap / 2));
            quellspeicher = malloc((total_heap_cap / 2));
            freizeiger = zielspeicher;
            retValReg = NULL;
            isHeapInit = true;
        }
        if (!isStackInit) {
            stack_cap = 64;
            stack = malloc(stack_cap * 1024);
            isStackInit = true;
        }
        read(argv[i]);
    }


    printf("Ninja Virtual Machine started\n");

    run_main();


    return 0;
}


ObjRef alloc_memory(size_t size) {
    ObjRef toPush;

    if (size + benutzteBytes < maxHalbspeicher) {
        toPush = (ObjRef) (freizeiger);
        benutzteBytes = benutzteBytes + size;
        freizeiger = size + freizeiger;
    } else {
        garbage_collector();
        if (size + benutzteBytes < maxHalbspeicher) {
            toPush = (ObjRef) (freizeiger);
            benutzteBytes = benutzteBytes + size;
            freizeiger = size + freizeiger;
        } else {
            printf("Alloc_memory error.\n");
            exit(1);
        }
    }

    return toPush;
}

void flip(void) {
    char *temp = quellspeicher;
    quellspeicher = zielspeicher;
    zielspeicher = temp;
    freizeiger = temp;
}

void garbage_collector(void) {
    flip();

    benutzteBytes = 0;

    for (int i = 0; i < stack_pointer; i++) {
        if (stack[i].isObjRef) {
            stack[i].u.objRef = relocate(stack[i].u.objRef);
        }
    }


    for (int i = 0; i < num_var; ++i) {
        sda[i] = relocate(sda[i]);
    }


    bip.op1 = relocate(bip.op1);
    bip.op2 = relocate(bip.op2);
    bip.res = relocate(bip.res);
    bip.rem = relocate(bip.rem);

    retValReg = relocate(retValReg);


    scan();


    if (isGCPurge) {
        memset(quellspeicher, 0, total_heap_cap / 2);
    }

}

ObjRef relocate(ObjRef orig) {
    ObjRef copy = NULL;
    if (orig == NULL) {
        copy = NULL;
    } else if (HAS_BROKENHEART(orig)) {
        copy = calculateForwardPointer(orig);
    } else {
        copy = (ObjRef) freizeiger;
        int size;
        if (IS_PRIMITIVE(orig)) {
            size = sizeOfPrim(orig->size);
        } else {
            size = sizeOfComp(GET_ELEMENT_COUNT(orig));
        }
        memcpy(copy, orig, size);
        orig->size = setBrokenheart(copy);
        freizeiger += size;
        benutzteBytes += size;
    }
    return copy;
}


void scan(void) {
    char *scan = zielspeicher;
    ObjRef scannedObj = NULL;
    unsigned int size;
    while (scan < freizeiger) {
        scannedObj = (ObjRef) (scan);
        if (!IS_PRIMITIVE(scannedObj)) {
            size = sizeOfComp(GET_ELEMENT_COUNT(scannedObj));
            for (int i = 0; i < GET_ELEMENT_COUNT(scannedObj); i++) {
                GET_REFS_PTR(scannedObj)[i] = relocate(GET_REFS_PTR(scannedObj)[i]);
            }
        } else {
            size = sizeOfPrim(scannedObj->size);
        }
        scan = scan + size;
    }
}

unsigned int sizeOfComp(int numOfVars) {
    unsigned int size = (sizeof(void *) + sizeof(unsigned int) + sizeof(bool) + (numOfVars * sizeof(void *)));
    return size;
}

int sizeOfPrim(int numBytes) {
    int size = (sizeof(void *) + sizeof(unsigned int) + sizeof(bool) + (numBytes * sizeof(unsigned char)));
    return size;
}

ObjRef calculateForwardPointer(ObjRef orig) {
    ObjRef toCopy = NULL;
    toCopy = (ObjRef) ((orig->size & ~MSB & ~BROKENHEART) + zielspeicher);
    return toCopy;
}


unsigned int setBrokenheart(ObjRef copy) {
    unsigned int temp;
    temp = benutzteBytes | BROKENHEART | (copy->size & MSB);
    return temp;
}

