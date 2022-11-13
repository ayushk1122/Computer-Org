/*
Name: Ayush Krishnappa 
Professor Kuzmin Computer Org 
*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

//const string arrays to hold string values for all registers 
const char* const savedRegs[] = { "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"};
const char* const tempRegs[] = { "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"};

int findSymbol(char target, char *arr) {
    int i = 0;
    while(arr[i]) {
        if (target == arr[i]) return i;
        i++;
    }

    arr[i] = target;
    return i;
}

int findSymbolIndex(char* op, char symbol) { // 0 = false, 1 = true
    for (int i = 0; i < strlen(op); i++) {
        if (op[i] == symbol) {
            return i;
        }
    }
    return 0;
}

int findPowers(int x, int *powers) {
    int size = 0;
    int temp = 0;
    do {
        temp = x % 2;
        powers[size] = temp;
        x = x / 2;
        size++;
    } while (x > 0);
    if (x == 0) {
        // Return the size of array
        return size;
    } else {
        return 0;
    }
}

int findPowersOf2(int value) {
    int count = 0;
    do {
        if (value % 2 != 0){
            return 0;
        }
        value = value / 2;
        count++;
    } while (value != 1);
    return count;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid arguments\n");
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

    char * saved = (char*)malloc(9 * sizeof(char));
    char * operation = (char*)malloc(128 * sizeof(char));
    char line[128];

    int i, j = 0; 
    int opCount = 0;
    int index, startIndex = 0;
    int k=0, tempRegCount=0, labelnum=0;
    int temp1, temp2 = 0;
    int assign = 0;
    int negRight, initialNeg, reset=0, hasTempReg;
    int saveRegIndex;
    int lNum1, lNum2, rNum1, rNum2, numPowers;

    

    while (fgets(line, sizeof(line), file)) {
        i = j = opCount = temp1 = temp2 = assign = index = startIndex = hasTempReg = 0;
        printf("# %s", line); //prints original mips operation 
        
        while (line[i] != '\0') { //removes spaces from operation
            if (line[i] != ' ') 
                operation[j++] = line[i++];
            else
                i++;
        }
        
        saveRegIndex = findSymbol(operation[0], saved);
        
        i=2, j=3, k=4; //different starting indexes to account for numbers operation sign and end 

        while (operation[j] != ';' || (i == 2)) { // || operation[j] == '\0'
            lNum1 = lNum2 = rNum1 = rNum2 = negRight = initialNeg = -1;
            //operation[i] is substraction flips initial negation sign 
            if (operation[i] == '-') {
                initialNeg = 1;
                i++;
            }

            // If operation[i] is a digit
            if (isdigit(operation[i])) {
                lNum1 = 0;
                lNum2 = operation[i] - '0'; //converts string ASCII representation of number into int
                while (isdigit(operation[i])) {
                    lNum1 = lNum1*10 + lNum2; //for nums larger then 1 digit 
                    i++;
                    lNum2 = operation[i] - '0';
                }
                j = i;
                k = i + 1;
            }
            
            // If operation[k] is a minus sign
            if (operation[k] == '-') {
                negRight = 1;
                k++;
            }

            // If operation[k] is a digit
            if (isdigit(operation[k])) { // checks for a second number in operation 
                rNum1 = 0;
                rNum2 = operation[k] - '0';
                while (isdigit(operation[k])) { 
                    rNum1 = rNum1*10 + rNum2;
                    k++;
                    rNum2 = operation[k] - '0';
                }
                k--;
            }

            // If operation[i] is ';', represents end of individual operation 

            if (operation[j] == ';' && startIndex == 0) {
                //prints li operations for single num operations without arithmitic opertations 
                if (initialNeg == 1) {
                    printf("li %s,-%d\n", savedRegs[saveRegIndex], lNum1);
                } 
                else {
                    printf("li %s,%d\n", savedRegs[saveRegIndex], lNum1);
                }
                break;
            } 
            
            //addition or subtraction operations 
            if (operation[j] == '-' || operation[j] == '+') {
                if ((operation[j] == '-') && (rNum1 == -1) && (lNum1 == -1)){
                    printf("sub ");
                } else if ((operation[j] == '+') && (rNum1 == -1) && (lNum1 == -1)) {
                    printf("add ");
                } else if ((operation[j] == '-') || (operation[j] == '+')) {
                    printf("addi ");
                }
                
                // If it comes to the end of the operation
                if (operation[k+1] == ';') {
                    printf("%s,", savedRegs[saveRegIndex]);

                    if (startIndex == 0) { //prints saved registers 
                        printf("%s,", savedRegs[findSymbol(operation[i], saved)]);
                        startIndex++;
                    } else { //prints temp registers 
                        printf("%s,", tempRegs[tempRegCount-1]);
                        if (tempRegCount == 10)
                            tempRegCount = 0;
                    }

                    if (rNum1 >= 0) { //if there is two numbers in operation
                        if (operation[j] == '-') {
                           printf("-%d\n", rNum1);
                        } else {
                           printf("%d\n", rNum1);
                        }
                        
                    } else { //otherwise prints next saved register 
                        printf("%s\n", savedRegs[findSymbol(operation[k], saved)]);
                    }
                } 
                else { 
                    if (tempRegCount == 10) {
                        printf("$t0,$t9,");
                        tempRegCount = 1;
                    }
                    else if (startIndex == 0) { //prints first temp and second saved registers for start index of 0
                        printf("%s,", tempRegs[tempRegCount]);
                        tempRegCount++;
                        startIndex++;
                        printf("%s,", savedRegs[findSymbol(operation[i], saved)]);
                    } else { //two saved register case
                        printf("%s,%s,", tempRegs[tempRegCount], tempRegs[tempRegCount-1]);
                        tempRegCount++;
                    }
                    if (rNum1 >= 0) { //checks if there are two numbers 
                        if (negRight == 1) {
                           printf("-%d\n", rNum1);
                        } else {
                           printf("%d\n", rNum1);
                        }
                    } else { //otherwise uses second saved register 
                        printf("%s\n", savedRegs[findSymbol(operation[k], saved)]);
                    }
                }
            }


            if (operation[j] == '/') { //division operation 
                if (isalpha(operation[i]) && isalpha(operation[k])) { //both operations alphabetic 
                    printf("div ");
                    if (startIndex == 0) {
                        printf("%s,", savedRegs[findSymbol(operation[i], saved)]);
                    } else {
                        printf("%s,", tempRegs[tempRegCount]);
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        }
                    }
                    printf("%s\n", savedRegs[findSymbol(operation[k], saved)]); //prints saved register if start index not 0

                    // If it comes to the end of the operation
                    if (operation[k+1] == ';') {
                        printf("mflo %s\n", savedRegs[findSymbol(operation[0], saved)]); //use mflo for simplification rule 
                    } else {
                        if (reset == 1) reset = 0;
                        printf("mflo %s\n", tempRegs[tempRegCount]); //checks for reset condition 
                    }
                }

                if (isalpha(operation[i]) && isdigit(operation[k])) { //left alphabetical and right digit number 
                    hasTempReg++;
                    int ispower;

                    // If the right operand is 0, then print an error message
                    if (rNum1 == 0) {
                        fprintf(stderr, "Division by zero undefined\n");
                    } 
                    else if(rNum1 == 1) { //default right number 
                        if (operation[k+1] == ';') {
                            if (negRight == 1) { //negative case 
                                printf("sub %s,$zero,", savedRegs[findSymbol(operation[0], saved)]);
                                if (startIndex == 0) {
                                    printf("%s\n", savedRegs[findSymbol(operation[i], saved)]);
                                } else {
                                    printf("%s\n", tempRegs[tempRegCount]);
                                    tempRegCount++;
                                }
                            }
                            else { //move branch with saved register or temp register 
                                printf("move %s,", savedRegs[findSymbol(operation[0], saved)]);
                                if (startIndex == 0) {
                                    printf("%s\n", savedRegs[findSymbol(operation[i], saved)]);
                                } else {
                                    printf("%s\n", tempRegs[tempRegCount]);
                                    tempRegCount++;
                                }
                            }
                        }
                        else {
                            if(negRight == 1) { //for negative right, does substraction 
                                printf("sub %s,$zero,%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[i], saved)]);
                            }
                            else { //move branch otherwise
                                printf("move %s,", tempRegs[tempRegCount]);
                                if (startIndex == 0) {
                                    printf("%s\n", savedRegs[findSymbol(operation[i], saved)]);
                                } else {
                                    printf("%s\n", tempRegs[tempRegCount]);
                                    if (tempRegCount == 9) {
                                        tempRegCount = 0;
                                        reset = 1;
                                    } else {
                                        tempRegCount++;
                                    }
                                }
                            }
                        }
                    } else { //finds powers of 2's for divide by 2 case 
                        ispower = findPowersOf2(rNum1);
                    }

                    if (ispower > 0 && rNum1 > 1) { //checks if num is power of 2 and there is a right number 
                        hasTempReg++;
                        if (labelnum > 0) labelnum++;
                        if (startIndex == 0) { //blitz branch with L register 
                            printf("bltz %s,L%d\n", savedRegs[findSymbol(operation[i], saved)], labelnum);
                        }
                        else { //reset case 
                            if (reset == 1) reset = 0;
                            printf("bltz %s,L%d\n", tempRegs[tempRegCount], labelnum);
                        }
                        
                        printf("srl %s,", savedRegs[findSymbol(operation[0], saved)]); //shifts saved register right 

                        if (startIndex == 0) { 
                            printf("%s,", savedRegs[findSymbol(operation[i], saved)]);
                        } else {
                            printf("%s,", tempRegs[tempRegCount]);
                        }
                        
                        printf("%d\n", ispower); //prints power of 2's

                        if (negRight == 1) { //if right number is negative 
                            printf("sub %s,$zero,%s\n", savedRegs[findSymbol(operation[0], saved)], savedRegs[findSymbol(operation[0], saved)]);
                        }
                        printf("j L%d\n", labelnum+1);
                        printf("L%d:\n", labelnum);
                        if (negRight == 1) {
                            if (startIndex == 0) {
                                if (reset == 1) reset = 0;
                                printf("li %s,-%d\n", tempRegs[tempRegCount], rNum1);
                            } else {
                                
                                if (tempRegCount == 9) {
                                    tempRegCount = 0;
                                    reset = 1;
                                } else {
                                    tempRegCount++;
                                }
                                printf("li %s,-%d\n", tempRegs[tempRegCount], rNum1);
                            }
                        } else { 
                            if (startIndex == 0) {
                                if (reset == 1) { //li operations for single num commands 
                                    printf("li $t9,%d\n", rNum1);
                                } else {
                                    printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                                }
                            } else {
                                
                                if (tempRegCount == 9) {
                                    tempRegCount = 0;
                                    reset = 1;
                                } else {
                                    tempRegCount++;
                                }
                                printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                            }
                        }
                        
                        if (startIndex == 0) {  //prints div branch for two saved registers 
                            printf("div %s,%s\n", savedRegs[findSymbol(operation[i], saved)], tempRegs[tempRegCount]);
                        } else {
                            if (reset == 1) { //for reset case prints with temp registers 
                                printf("div $t0,$t9\n");
                                reset = 0;
                            } else {
                                printf("div %s,%s\n", tempRegs[tempRegCount-1], tempRegs[tempRegCount]);
                            }
                        }
                        
                        //prints mflo branch and L registers 
                        printf("mflo %s\n", savedRegs[findSymbol(operation[0], saved)]);
                        printf("L%d:\n", labelnum+1);
                        labelnum++;
                        tempRegCount++;
                    } 

                    else if (ispower == 0 && rNum1 > 0) { //not power of 2 and a right number case 

                        if (negRight == 1) {
                            printf("li %s,-%d\n", tempRegs[tempRegCount], rNum1);
                        } else {
                            printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                        }

                        if (startIndex == 0 && hasTempReg > 0) {
                            printf("div %s,%s\n", savedRegs[findSymbol(operation[i], saved)], tempRegs[tempRegCount]);
                        } else {
                            if (reset == 1) {
                                printf("div $t0,$t9\n");
                                reset = 0;
                            } else {
                                printf("div %s,%s\n", tempRegs[tempRegCount-1], tempRegs[tempRegCount]);
                            }
                        }

                        // If it comes to the end of the operation
                        if (operation[k+1] == ';') {
                            printf("mflo %s\n", savedRegs[findSymbol(operation[0], saved)]);
                        } else {
                            if (tempRegCount == 9) {
                                printf("mflo $t0\n");
                            } else {
                                printf("mflo %s\n", tempRegs[tempRegCount+1]);
                            }
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                            } else {
                                tempRegCount++;
                            }
                        }
                        
                    }
                }

                else if (lNum1 >= 0 && isalpha(operation[k])){ //left number and right alphabetical case 
                    hasTempReg++;
                    if (startIndex > 0) { //div and mflo branch 
                        printf("div %s,%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[k], saved)]);
                        if (operation[k+1] == ';') {
                            printf("mflo %s\n", savedRegs[saveRegIndex]);
                        } else {
                            
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                            printf("mflo %s\n", tempRegs[tempRegCount]);
                        }
                    }
                }

                else if (lNum1 >= 0 && rNum1 >= 0) { //left and right number case 
                    hasTempReg++;
                    if (rNum1 == 0) {
                        fprintf(stderr, "Division by zero undefined\n");
                    }
                    else if (rNum1 == 1) {
                        if (startIndex == 0) {
                            printf("li %s,%d\n", tempRegs[tempRegCount], lNum1);
                        }

                        if (operation[k+1] == ';') {
                            if (negRight == 1) {
                                printf("sub %s,$zero,", savedRegs[findSymbol(operation[0], saved)]);
                                printf("%s\n", tempRegs[tempRegCount]);
                            }
                            else {
                                printf("move %s,", savedRegs[findSymbol(operation[0], saved)]);
                                printf("%s\n", tempRegs[tempRegCount]);
                            }

                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                        }
                        else {
                            if(negRight == 1) {
                                if (tempRegCount + 1 == 9) {
                                    tempRegCount = 0;
                                    reset = 1;
                                } else {
                                    tempRegCount++;
                                }

                                if (reset == 1) {
                                    printf("sub $t0,$zero,$t9\n");
                                    reset = 0;
                                } else {
                                    printf("sub %s,$zero,%s\n", tempRegs[tempRegCount], tempRegs[tempRegCount-1]);
                                }
                            }
                            else {
                                if (tempRegCount + 1 == 9) {
                                    tempRegCount = 0;
                                    reset = 1;
                                } else {
                                    tempRegCount++;
                                }

                                if (reset == 1) {
                                    printf("move $t0,$t9\n");
                                    reset = 0;
                                } else {
                                    printf("move %s,", tempRegs[tempRegCount]);
                                    printf("%s\n", tempRegs[tempRegCount-1]);
                                }
                            }
                        }
                    }
                    else { //non special case 
                        int ispower = findPowersOf2(rNum1);
                        if (startIndex == 0) {
                            printf("li %s,%d\n", tempRegs[tempRegCount], lNum1);
                        }

                        if (ispower > 0 && rNum1 > 1) { //if is power of 2 and right number exists 
                            if (labelnum > 0) labelnum++;
                            printf("bltz %s,L%d\n", tempRegs[tempRegCount], labelnum);
                            printf("srl %s,%s,", savedRegs[findSymbol(operation[0], saved)], tempRegs[tempRegCount]);
                        
                            printf("%d\n", ispower);

                            if (negRight == 1) { //negative right number 
                                printf("sub %s,$zero,%s\n", savedRegs[findSymbol(operation[0], saved)], savedRegs[findSymbol(operation[0], saved)]);
                            }
                            printf("j L%d\n", labelnum+1);
                            printf("L%d:\n", labelnum);
                            
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }

                            if (negRight == 1) {
                                printf("li %s,-%d\n", tempRegs[tempRegCount], rNum1);
                            } else {
                                printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                            }

                            if (reset == 1) {
                                printf("div $t9,$t0\n");
                            } else {
                                printf("div %s,%s\n", tempRegs[tempRegCount-1], tempRegs[tempRegCount]);
                            }
                            
                            printf("mflo %s\n", savedRegs[findSymbol(operation[0], saved)]);
                            printf("L%d:\n", labelnum+1);
                            labelnum++;
                            tempRegCount++;
                        } 
                        else if (ispower == 0 && rNum1 > 0) { //not power of two and no right number 
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }

                            if (negRight == 1) { //negative right number adds negative sign 
                                printf("li %s,-%d\n", tempRegs[tempRegCount], rNum1);
                            } else {
                                printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                            }

                            if (reset == 1) { //reset case uses last and first temp reg
                                printf("div $t9,$t0\n");
                            } else {
                                printf("div %s,%s\n", tempRegs[tempRegCount-1], tempRegs[tempRegCount]);
                            }
                            
                            // If it comes to the end of the operation
                            if (operation[k+1] == ';') {
                                printf("mflo %s\n", savedRegs[findSymbol(operation[0], saved)]);
                                if (tempRegCount == 9) tempRegCount = 0;
                                else tempRegCount++;
                            } else {
                                if (tempRegCount == 9) tempRegCount = 0;
                                else tempRegCount++;
                                printf("mflo %s\n", tempRegs[tempRegCount]);
                            }
                        }
                    }
                }
                startIndex++;
            }


            if (operation[j] == '*') { //multiplication operation
                if (isalpha(operation[i]) && isalpha(operation[k])) { //two alphabetic operations 
                    printf("mult ");
                    if (startIndex == 0) {
                        printf("%s,", savedRegs[findSymbol(operation[i], saved)]);
                    } else {
                        if (tempRegCount == 0 && reset == 1) {
                            printf("$t0,");
                            reset = 0;
                        } else {
                            printf("%s,", tempRegs[tempRegCount]);
                        }
                        if (tempRegCount == 9)  {
                            tempRegCount = 0;
                            reset = 1;
                        }
                        else tempRegCount++;
                    }
                    printf("%s\n", savedRegs[findSymbol(operation[k], saved)]);
                    
                    // If it comes to the end of the operation
                    if (operation[k+1] == ';') {
                        printf("mflo %s\n", savedRegs[findSymbol(operation[0], saved)]);
                    } else {
                        printf("mflo %s\n", tempRegs[tempRegCount]);
                    }
                } 
                // If the left operand is an alphabet and the right one is a digit
                else if (isalpha(operation[i]) && isdigit(operation[k])  ) {
                    hasTempReg++;
                    numPowers = 0;
                    int * powers = (int*)malloc(50 * sizeof(int));

                    // If the rNum1 is 1, no need to check for powers of 2
                    if (rNum1 == 1) {
                        printf("move %s,%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[i], saved)]);
                        // If it is negative 
                        if (negRight == 1) {
                            if (operation[k+1] == ';') {
                                printf("sub %s,$zero,%s\n", savedRegs[saveRegIndex], tempRegs[tempRegCount]);
                            } else {
                                printf("sub %s,$zero,%s\n", tempRegs[tempRegCount], savedRegs[tempRegCount]);
                            }

                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                        }
                        // If it is not negative
                         else if (negRight != 1) {
                            if (operation[k+1] == ';') {
                                printf("move %s,%s\n", savedRegs[findSymbol(operation[0], saved)], tempRegs[tempRegCount]); //*************
                            }
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                        }
                    }
                    // If multiply a zero 
                    else if (rNum1 == 0) {
                        printf("mult %s,%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[i], saved)]);
                        printf("mflo %s\n", tempRegs[tempRegCount+1]);
                        printf("li %s,0\n", savedRegs[findSymbol(operation[0], saved)]);
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        }
                        break;
                    }
                    else {  
                        // Check if we can replace the right operand with powers of 2
                        numPowers = findPowers(rNum1, powers);
                    }
                    
                    if (numPowers > 0) { //can replace right num with powers of 2
                        int firstShift = 0;
                        for (int a = numPowers-1; a >= 0; a--) { //shifts right 
                            if (firstShift == 0 && powers[a] == 1) { //uses sll and move to multiply by powers 2
                                printf("sll %s,%s,%d\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[i], saved)], a);
                                printf("move %s,%s\n", tempRegs[tempRegCount+1], tempRegs[tempRegCount]);
                                firstShift++;
                            }
                            else if (a == 0 && powers[a] == 1) {
                                printf("add %s,%s,%s\n", tempRegs[tempRegCount+1], tempRegs[tempRegCount+1], savedRegs[findSymbol(operation[i], saved)]);
                            } else if (powers[a] == 1) {
                                printf("sll %s,%s,%d\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[i], saved)], a);
                                printf("add %s,%s,%s\n", tempRegs[tempRegCount+1], tempRegs[tempRegCount+1], tempRegs[tempRegCount]);
                            }
                        }
                        if (operation[k+1] == ';') { //end of operation 
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                            } else {
                                tempRegCount++;
                            }
                            if (negRight == 1) {
                                printf("sub %s,$zero,%s\n", savedRegs[findSymbol(operation[0], saved)], tempRegs[tempRegCount]);
                            } else {
                                printf("move %s,%s\n", savedRegs[findSymbol(operation[0], saved)], tempRegs[tempRegCount]);
                            }

                            if (tempRegCount == 8) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount += 2;
                            }
                        } else {
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount += 2;;
                            }
                        }
                    }
                    // If it cannot be converted to powers of 2
                    else if (rNum1 != 1 && numPowers == 0 && rNum1 != 0) {
                        // Print li
                        printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                        printf("mult %s,%s\n", savedRegs[findSymbol(operation[i], saved)], tempRegs[tempRegCount]);

                        // end of the operation
                        if (operation[k+1] == ';') {
                            // If rNum1 is negative
                            if (negRight == 1) {
                                printf("mflo %s\n", savedRegs[findSymbol(operation[0], saved)]);
                                printf("sub %s,$zero,%s\n", savedRegs[findSymbol(operation[0], saved)], savedRegs[findSymbol(operation[0], saved)]);
                            } else {
                                printf("mflo %s\n", savedRegs[findSymbol(operation[0], saved)]);
                            }
                            tempRegCount += 2;
                        } else { //positive rNum1
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                            } else {
                                tempRegCount++;
                            }
                            printf("mflo %s\n", tempRegs[tempRegCount]);
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                        }
                    }
                }

                else if (lNum1 >= 0 && isalpha(operation[k]) && operation[k+2] != '0') { //left num and right alphabetic operation 
                    hasTempReg++;
                    if (startIndex > 0) { // for starting point farther then start 
                        printf("mult %s,%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[k], saved)]);
                        if (operation[k+1] == ';') {
                            printf("mflo %s\n", savedRegs[saveRegIndex]);
                        } else {
                            printf("mflo %s\n", tempRegs[tempRegCount]);
                            
                        }
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        }
                    }
                    else if (startIndex == 0 && lNum1 != 0) { //starting from beginning and left number exists 
                        printf("li %s,%d\n", tempRegs[tempRegCount], lNum1);
                        printf("mult %s,%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[k], saved)]);
                        if (operation[k+1] == ';') {
                            printf("mflo %s\n", savedRegs[saveRegIndex]);
                        } else {
                            printf("mflo %s\n", tempRegs[tempRegCount]);
                            
                        }
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        }
                    }
                }

                else if (lNum1 >= 0 && rNum1 >= 0) { //both right and left numbers exist 
                    hasTempReg++;
                    if ((lNum1 == 0 && startIndex == 0) || rNum1 == 0) { // if left or right num is 0 case
                        printf("li %s,0\n", savedRegs[saveRegIndex]); //prints li command 
                        if (startIndex > 0) {
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                        }
                        break;
                    }

                    if (startIndex == 0) { //starting at beginning of operation 
                        printf("li %s,%d\n", tempRegs[tempRegCount], lNum1);
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        }
                    } 
                      
                    if (rNum1 == 1) { // right number is 1 case, special case 
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        }
                        if (negRight == 1) { //negative case 
                            if (reset == 1) {
                                printf("sub $t0,$zero,$t9\n");
                                reset = 0;
                            } else {
                                printf("sub %s,$zero,%s\n", tempRegs[tempRegCount], tempRegs[tempRegCount-1]);
                            } 
                        }
                        else { //positive 1 
                            if (reset == 1) { 
                                printf("move $t0,$t9\n");
                                reset = 0;
                            } else {
                                printf("move %s,%s\n", tempRegs[tempRegCount], tempRegs[tempRegCount-1]);
                            }
                        }
                    } else { //right number other then 1 and 0 
                        if (negRight == 1) {
                            printf("li %s,-%d\n", tempRegs[tempRegCount], rNum1);
                        } else {
                            printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                        } 

                        if (reset == 1) { //normal mult using temp registers 
                            printf("mult $t9,$t0\n");
                            reset = 0;
                        } else {
                            printf("mult %s,%s\n", tempRegs[tempRegCount-1], tempRegs[tempRegCount]);
                        }
                    
                        printf("mflo %s\n", tempRegs[tempRegCount+1]); 
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        } 
                    }
                }
                startIndex++;
            } 


            if (operation[j] == '%') { //mod operation 
                if (isalpha(operation[i]) && isalpha(operation[k])) { //left alphabetic operation and right alphabetic operation 
                    printf("div ");
                    if (startIndex == 0) { //prints saved registers 
                        printf("%s,%s\n", savedRegs[findSymbol(operation[i], saved)], savedRegs[findSymbol(operation[k], saved)]);
                    } else {
                        printf("%s,%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[k], saved)]);
                    }
                    if (operation[k+1] == ';') {
                        printf("mfhi %s\n", savedRegs[findSymbol(operation[0], saved)]);
                        if (startIndex > 0) {
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                        }
                    } else {
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                        } else {
                            tempRegCount++;
                        }
                        printf("mfhi %s\n", tempRegs[tempRegCount]); //mfhi branch
                    }
                    
                }

                else if (isalpha(operation[i]) && isdigit(operation[k])) { //left alphabetic operation and right numerical operation 
                    hasTempReg++;
                    if (rNum1 == 1) {
                        if (operation[k+1] == ';') {
                            if (negRight == 1) { //negative right number 
                                printf("sub %s,$zero,", savedRegs[findSymbol(operation[0], saved)]);
                                if (startIndex == 0) {
                                    printf("%s\n", savedRegs[findSymbol(operation[i], saved)]);
                                } else {
                                    printf("%s\n", tempRegs[tempRegCount]);
                                    if (tempRegCount == 9) {
                                        tempRegCount = 0;
                                        reset = 1;
                                    } else {
                                        tempRegCount++;
                                    }
                                }
                            }
                            else { //positive right number 
                                printf("move %s,", savedRegs[findSymbol(operation[0], saved)]);
                                if (startIndex == 0) {
                                    printf("%s\n", savedRegs[findSymbol(operation[i], saved)]);
                                } else {
                                    printf("%s\n", tempRegs[tempRegCount]);
                                    if (tempRegCount == 9) {
                                        tempRegCount = 0;
                                    } else {
                                        tempRegCount++;
                                    }
                                }
                            }
                        }
                        else {
                            if(negRight == 1) { //negative right number 
                                printf("sub %s,$zero,%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[i], saved)]);
                            }
                            else { //positive negative number 
                                printf("move $t%d,", tempRegCount);
                                if (startIndex == 0) {
                                    printf("%s\n", savedRegs[findSymbol(operation[i], saved)]);
                                } else {
                                    printf("%s\n", tempRegs[tempRegCount]);
                                    if (tempRegCount == 9) {
                                        tempRegCount = 0;
                                    } else {
                                        tempRegCount++;
                                    }
                                }
                            }
                        }
                    } else if (rNum1 == 0) { //can't divide by 0 
                        fprintf(stderr, "Division by zero error\n");
                    } else { //right number greater then 1
                        if (negRight == 1) {
                            printf("li %s,-%d\n", tempRegs[tempRegCount], rNum1);
                        } else {
                            printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                        }
                        printf("div ");

                        if (startIndex == 0) { //div branch with save and temp register 
                            printf("%s,", savedRegs[findSymbol(operation[i], saved)]);
                        } else {
                            if (reset == 1) {
                                printf("$t9,");
                                reset = 0;
                            } else {
                                printf("%s,", tempRegs[tempRegCount-1]);
                            }
                        
                        }
                        printf("$t%d\n", tempRegCount);

                        if (operation[k+1] == ';') { //mfhi branch 
                            printf("mfhi %s\n", savedRegs[saveRegIndex]);
                        } else {
                            printf("mfhi %s\n", tempRegs[tempRegCount+1]);
                        }
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                        } else {
                            tempRegCount++;
                        }
                    }
                    
                }

                else if (lNum1 >= 0 && isalpha(operation[k])) { //left number exists and alphabetic right operation 
                    hasTempReg++;
                    if (startIndex > 0) { //div and mfhi branch 
                        printf("div %s,$%s\n", tempRegs[tempRegCount], savedRegs[findSymbol(operation[k], saved)]);
                        if (operation[k+1] == ';') {
                            printf("mfhi %s\n", savedRegs[saveRegIndex]);
                        } else {
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                            printf("mfhi %s\n", tempRegs[tempRegCount]);
                        }
                        
                    }
                }

                else if (lNum1 >= 0 && rNum1 >= 0) { //left and right numbers exist 
                    hasTempReg++;
                    if (startIndex > 0) {
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        }

                        if (negRight == 1) { //li commands for negative or positive 
                            printf("li %s,-%d\n", tempRegs[tempRegCount], rNum1);
                        } else {
                            printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                        }

                        printf("div ");

                        if (reset == 1) {
                            printf("$t9,");
                            reset = 0;
                        } else {
                            printf("%s,", tempRegs[tempRegCount-1]);
                        }
                        printf("%s\n", tempRegs[tempRegCount]);

                        if (operation[k+1] == ';') {
                            printf("mfhi %s\n", savedRegs[saveRegIndex]);                            
                        } else {
                            if (tempRegCount == 9) {
                                tempRegCount = 0;
                                reset = 1;
                            } else {
                                tempRegCount++;
                            }
                            printf("mfhi %s\n", tempRegs[tempRegCount]);                            
                        }

                        

                    } else { //start index 0
                        printf("li %s,%d", tempRegs[tempRegCount], lNum1);
                        if (tempRegCount == 9) {
                            tempRegCount = 0;
                            reset = 1;
                        } else {
                            tempRegCount++;
                        }

                        if (negRight == 1) {
                            printf("li %s-%d\n", tempRegs[tempRegCount], rNum1);
                        } else {
                            printf("li %s,%d\n", tempRegs[tempRegCount], rNum1);
                        }

                        printf("div ");

                        if (reset == 1) {
                            printf("$t9,");
                            reset = 0;
                        } else {
                            printf("%s,", tempRegs[tempRegCount - 1]);
                        }
                        printf("%s\n", tempRegs[tempRegCount]);

                        if (operation[k+1] == ';') { //mfhi branch 
                            printf("mfhi %s\n", savedRegs[saveRegIndex]);
                            tempRegCount++;
                        } else {
                            printf("mfhi %s\n", tempRegs[tempRegCount+1]);
                            tempRegCount++;
                        }
                    }
                }
                startIndex++;
            }
            
            //updates all indexes for position in operation 
            i = k;
            j = i + 1;
            k = j + 1;
            startIndex++;
            
        } 
        
    }

    //free's allocated memory and closes read file 
    fclose(file);

    free(saved);
    free(operation);

    return EXIT_SUCCESS;
}