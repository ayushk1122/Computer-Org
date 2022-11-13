#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <iostream>

const char* const savedRegs[] = { "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"};
const char* const tempRegs[] = { "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"};

int findSymbol(char* op, char symbol) { // 0 = false, 1 = true
    for (int i = 0; i < strlen(op); i++) {
        if (op[i] == symbol) {
            return i;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid usage\n");
        return EXIT_FAILURE; 
    }

    // Open input file
    char const* const fileName = argv[1];
    FILE * file;
    file = fopen(fileName, "r");

    // check if the file can be opened
    if (!file) {
        fprintf(stderr, "Error: Cannot open file!\n");
        return EXIT_FAILURE;
    } 

    char line[128];
    char* operation = (char*)malloc(128 * sizeof(char));
    char* operationC = (char*)malloc(128 * sizeof(char));
    char* assignOp = (char*)malloc(128 * sizeof(char));
    char* newVars = (char*)malloc(9 * sizeof(char));
    int newVarCount = 0;
    int i = 0;
    int j = 0;
    int numOps = 0;
    int opLen = 0;
    int assignOps = 0;

    while (fgets(line, sizeof(line), file)) { //line stores original mips code 
        printf("# %s\n", line); //prints commented mips code for assigment operation
        i = 0;
        j = 0;
        opLen = 0;
        

        while (line[i] != '\0') { //remove spaces from operation lines 
            if (!isspace(line[i])) {
                operation[j++] = line[i++]; //operation stores mips code without spaces 
                opLen++;
            } else {
                i++;
            }
        }

        if ((findSymbol(operation, '+') == 0) && (findSymbol(operation, '-') == 0)  //converts assignment operations 
        && (findSymbol(operation, '*') == 0) && (findSymbol(operation, '/') == 0)) {
            char* assignNum = strtok(operation, "="); //splits operation by = delimiter twice to get assignment value
            newVars[newVarCount] = (char*)assignNum;
            assignNum = strtok(NULL, "=");
            
            printf("li %s, %s\n", savedRegs[newVarCount], assignNum);
            newVarCount++;
        }

        while (line[i] != '\0') {
            if (findSymbol(operation, '+') || findSymbol(operation, '-')) { //converts addition operation 
                char* assignVar = strtok(operation, "=");
                int numArgs = 0;
                if (findSymbol(newVars, assignVar[0]) == 0) {
                    newVarCount++;
                    newVars[newVarCount] = (char*)assignVar;
                }

                char* rightHandSide = strtok(NULL, "=");

                for (int i = 0; i < strlen(rightHandSide); i++) {
                    if ((rightHandSide[i] == '+') || (rightHandSide[i] == '-')) {
                        numArgs += (3);
                    }
                }

                if ((findSymbol(rightHandSide, '+') != 0) && (findSymbol(rightHandSide, '-') != 0)) {
                    if (findSymbol(rightHandSide, '+') < findSymbol(rightHandSide, '-')) 
                        printf("add ");
                    else 
                        printf("sub ");
                } else if (findSymbol(rightHandSide, '+') != 0) {
                    printf("add ");
                } else {
                    printf("sub ");
                }           

                for (int i = 0; i < strlen(rightHandSide); i++) {
                    if ((rightHandSide[i] != '+') || (rightHandSide[i] != '-')) {
                       if (!isdigit(rightHandSide[i])) {
                          
                       } 
                    }
                }
            }   
        }


        numOps++;
        //printf("%s\n", operation);
    }

    


    return 0;
}