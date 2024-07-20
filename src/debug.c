#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "debug.h"
#include "structs.h"
#include "bigint.h"

//filename als parameter
//codesize
//datasize


int break_point = -1;

void debug_version(void) {
    printf("Ninja Virtual Machine version 8 (compiled %s, %s)", __DATE__, __TIME__);
    exit(0);
}

void help(void) {
    printf("Usage: ./njvm [options] <code file>\n"
           "Options:\n"
           "  --debug       start virtual machine in debug mode\n"
           "  --version     show version and exit\n"
           "  --help        show this help and exit\n");
    exit(0);
}


void inspect() {
    char input[99];
    printf("DEBUG [inspect]: stack, data, object?\n");
    fgets(input, 99, stdin);
    int i;
    if (strncmp(input, "stack", 5) == 0) {
        printf("           ---Printing Stack---\n");
        if (stack_pointer == 0 && frame_pointer == 0) {
            printf("sp, fp    --->    %04d:    (xxxxxx) xxxxxx\n", stack_pointer);
        } else {
            printf("sp    --->    %04d:    (xxxxxx) xxxxxx\n", stack_pointer);
            for (i = stack_pointer - 1; i >= 0; i--) {
                if (i == frame_pointer && frame_pointer != stack_pointer) {
                    if (stack[i].isObjRef) {
                        printf("fp    --->    %04d:    (objref) %p\n", frame_pointer, (void *) stack[i].u.objRef);
                    } else {
                        printf("fp    --->    %04d:    (number) %d\n", frame_pointer, stack[i].u.number);
                    }
                } else {
                    if (stack[i].isObjRef) {
                        printf("              %04d:    (objref) %p\n", i, (void *) stack[i].u.objRef);
                    } else {
                        printf("              %04d:    (number) %d\n", i, stack[i].u.number);
                    }
                }
            }
        }
        printf("           ---Bottom of Stack---\n");
    } else if (strncmp(input, "data", 4) == 0) {
        printf("        ---Printing Data---\n");
        for (int i = 0; i < num_var; ++i) {
            printf("data[%04d]:    (objref) %p\n", i, (void *) sda[i]);
        }
        printf("        ---End of Data---\n");
    } else if (strncmp(input, "object", 6) == 0) {
        ObjRef address = NULL;
        printf("object reference?\n");
        scanf("%lx", (unsigned long *) &address);
        for (int i = 0; i < stack_pointer; ++i) {
            if (stack[i].u.objRef->data == address->data) {
                if (!IS_PRIMITIVE(stack[i].u.objRef)) {
                    printf("<compound object>\n");
                    for (int j = 0; j < GET_ELEMENT_COUNT(stack[i].u.objRef); ++j) {
                        printf("field[%04d]:  (objref) %p\n", j, (void *)GET_REFS_PTR(stack[i].u.objRef)[j]);
                    }
                } else {
                    bip.op1 = address;
                    printf("<primitive object>\n");
                    printf("value:   ");
                    bigPrint(stdout);
                    printf("\n");
                    //continue;
                }
            }
        }
        for (int j = 0; j < num_var; ++j) {
            if (sda[j]->data == address->data) {
                if (!IS_PRIMITIVE(sda[j])) {
                    printf("<compound object>\n");
                    for (int k = 0; k < GET_ELEMENT_COUNT(sda[j]); ++k) {
                        printf("field[%04d]:  (objref) %p\n", k, (void *)GET_REFS_PTR(sda[j])[k]);
                    }
                    break;
                } else {
                    bip.op1 = address;
                    printf("<primitive object>\n");
                    printf("value:   ");
                    bigPrint(stdout);
                    printf("\n");
                    break;
                }
            }
        }
    }

}


void debug_list(int position) {
    unsigned int list_instruction;
    unsigned int list_opcode;
    unsigned int list_immediate;
    printf("%04d: \t", position);
    list_instruction = program_memory[position];
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

void breakpoint() {
    char input[99];
    char *ptr;
    if (break_point == -1) {
        printf("DEBUG [breakpoint]: cleared\n");
    } else {
        printf("DEBUG [breakpoint]: set at %d\n", break_point);
    }
    printf("DEBUG [breakpoint]: address to set, -1 to clear, <ret> for no change?\n");
    fgets(input, 99, stdin);
    long input_long = strtol(input, &ptr, 10);

    if (input_long == -1) {
        break_point = -1;
    } else if (input[0] == '\n') {
        //macht nichts
    } else {
        strtol_value = strtol(input, &ptr, 10);
        break_point = strtol_value;
        printf("DEBUG [breakpoint]: now set at %d\n", break_point);
    }

}


void run_debug(unsigned int num) {
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


void debug() {
    printf("Ninja Virtual Machine started\n");
    while (opcode != HALT) {
        debug_list(program_counter);
        printf("DEBUG: inspect, list, breakpoint, step, run ,quit?\n");

        char input[99];
        fgets(input, 99, stdin);

        if ((strncmp(input, "i", 1)) == 0) {
            inspect();
        } else if (strncmp(input, "l", 1) == 0) {
            list();
            printf("        --- end of code ---\n");
        } else if (strncmp(input, "b", 1) == 0) {
            breakpoint();
        } else if (strncmp(input, "s", 1) == 0) {
            instruction = program_memory[program_counter];
            program_counter++;
            run_debug(instruction);

        } else if (strncmp(input, "r", 1) == 0) {
            while (opcode != HALT) {
                if (program_counter == break_point) {
                    break;
                }
                instruction = program_memory[program_counter];
                program_counter++;
                run_debug(instruction);
            }
        } else if (strncmp(input, "quit", 4) == 0) {
            printf("Ninja Virtual Machine stopped\n");
            opcode = HALT;
        }

    }
    exit(0);
}
