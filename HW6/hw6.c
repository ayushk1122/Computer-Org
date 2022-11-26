#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    int lineCount = 0, i, lwNum=-1, regsCount, index, k;
    //initial states for arrays
    int hasNop[5] = {-1, -1, -1, -1, -1};
    int nop1[5] = {-1, -1, -1, -1, -1};
    int nop2[5] = {-1, -1, -1, -1, -1};
    int orig[5] = {-1, -1, -1, -1, -1};
    char regPrev[5][2], regCurr1[5][2], regCurr2[5][2];
    char mip[5][128];

    // Parse pipelines
    while (fgets(mip[lineCount], sizeof(mip[lineCount]), file)) {
        i = 0;
        regsCount = 0;

        while (mip[lineCount][i] != '\n') {
            if (mip[lineCount][i] == '$') {
                // If the first register of this line
                if (regsCount == 0) {
                    regPrev[lineCount][0] = mip[lineCount][i+1];
                    regPrev[lineCount][1] = mip[lineCount][i+2];
                    if (mip[lineCount][0] == 's' && mip[lineCount][1] == 'w') { //sw command
                        for (index = 0; index < lineCount; index++) { //compares current register to previous 
                            if ((regPrev[lineCount][0] == regCurr2[index][0]) && (regPrev[lineCount][1] == regCurr2[index][1])) {
                                lwNum = index;
                                break;
                            }
                            else if ((regPrev[lineCount][0] == regCurr1[index][0]) && (regPrev[lineCount][1] == regCurr1[index][1])) {
                                lwNum = index;
                                break;
                            }
                        } 
                    }
                } 
                // If the second register of this line
                else if (regsCount == 1) {
                    regCurr1[lineCount][0] = mip[lineCount][i+1];
                    regCurr1[lineCount][1] = mip[lineCount][i+2];
                    // Find if there is any dependencies 
                    if (lineCount > 0) {
                        for (index = 0; index < lineCount; index++) {
                            if ((regPrev[index][0] == regCurr1[lineCount][0]) && (regPrev[index][1] == regCurr1[lineCount][1])) {
                                nop1[lineCount] = index;
                                break;
                            }
                        }
                    }
                }
                // If the third register of this line
                else if (regsCount == 2) {
                    regCurr2[lineCount][0] = mip[lineCount][i+1];
                    regCurr2[lineCount][1] = mip[lineCount][i+2];
                    // Find if there is any dependencies 
                    if (lineCount > 0) {
                        for (index = 0; index < lineCount; index++) {
                            if ((regPrev[index][0] == regCurr2[lineCount][0]) && (regPrev[index][1] == regCurr2[lineCount][1])) {
                                nop2[lineCount] = index;
                                break;
                            }
                        }
                    }
                }
                i += 2;
                regsCount++;
            }
            i++;
        }
        mip[lineCount][i] = '\t'; //represents tab
        lineCount++;
    }

    printf("START OF SIMULATION\n\n");

    for (int cycCount = 1; cycCount < 10; cycCount++) { //runs 9 cycles regardless 
        if (cycCount == lineCount+5) break;
        printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
        
        for (int i = 0; i < lineCount; i++) { 
            // If cycCount is 1
            if (cycCount == 1) {
                printf("%s", mip[i]);
                // If first line
                if (i == 0) {
                    printf("IF\t.\t.\t.\t.\t.\t.\t.\t.\n");
                } 
                else {
                    printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
            }

            // If cycCount is 2
            else if (cycCount == 2) {
                printf("%s", mip[i]);
                // If first line
                if (i == 0) {
                    printf("IF\tID\t.\t.\t.\t.\t.\t.\t.\n");
                }
                else if (i == 1) {
                    printf(".\tIF\t.\t.\t.\t.\t.\t.\t.\n");
                } 
                else {
                    printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
            }

            // If cycCount is 3
            else if (cycCount == 3) {
                printf("%s", mip[i]);
                // If first line
                if (i == 0) {
                    printf("IF\tID\tEX\t.\t.\t.\t.\t.\t.\n");
                } 
                else if (i == 1) {
                    printf(".\tIF\tID\t.\t.\t.\t.\t.\t.\n");
                }
                else if (i == 2) {
                    printf(".\t.\tIF\t.\t.\t.\t.\t.\t.\n");
                }
                else {
                    printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                }
            }

            // If cycCount is 4
            else if (cycCount == 4) {
                // If first line
                if (i == 0) {
                    printf("%s", mip[i]);
                    printf("IF\tID\tEX\tMEM\t.\t.\t.\t.\t.\n");
                }
                else if (i == 1) {
                    // If it has dependencies 
                    if ((nop1[i] == 0) || (nop2[i] == 0)) {
                        for (k = lineCount; k > i; k--) {
                            strncpy(mip[k], mip[k-1], sizeof(mip[k]));
                            orig[k] = k-1;
                        }
                        strncpy(mip[i], "nop\t\t", sizeof(mip[i]));
                        lineCount++;

                        printf("%s", mip[i]);
                        printf(".\tIF\tID\t*\t.\t.\t.\t.\t.\n");
                        i++;

                        if ((nop1[i] == 0) || (nop2[i] == 0)) {
                            for (k = lineCount; k > i; k--) {
                                strncpy(mip[k], mip[k-1], sizeof(mip[k]));
                                orig[k] = orig[k-1];
                            }
                            strncpy(mip[i], "nop\t\t", sizeof(mip[i]));
                            lineCount++;

                            printf("%s", mip[i]);
                            printf(".\tIF\tID\t*\t.\t.\t.\t.\t.\n");
                            i++;

                            printf("%s", mip[i]);
                            printf(".\tIF\tID\tID\t.\t.\t.\t.\t.\n");
                            i++;

                            printf("%s", mip[i]);
                            printf(".\t.\tIF\tIF\t.\t.\t.\t.\t.\n");
                            hasNop[i-1] = hasNop[i] = 6;
                            
                        } else {
                            printf("%s", mip[i]);
                            printf(".\tIF\tID\tID\t.\t.\t.\t.\t.\n");
                            hasNop[i] = 6;
                            i++;
                            if (mip[i][0] != '\0') {
                                printf("%s", mip[i]);
                                printf(".\t.\tIF\tID\t.\t.\t.\t.\t.\n");
                            }
                        }
                    } 
                    else {
                        printf("%s", mip[i]);
                        printf(".\tIF\tID\tEX\t.\t.\t.\t.\t.\n");
                        i++;
                        printf("%s", mip[i]);
                        printf(".\t.\tIF\tID\t.\t.\t.\t.\t.\n");
                    }
                }
                else if (i == 3) {
                    printf("%s", mip[i]);
                    printf(".\t.\t.\tIF\t.\t.\t.\t.\t.\n");
                }
                else {
                    printf("%s", mip[i]);
                    if (orig[i] == 3) {
                        printf(".\t.\t.\tIF\t.\t.\t.\t.\t.\n");
                    }
                    else {
                        printf(".\t.\t.\t.\t.\t.\t.\t.\t.\n");
                    }
                }
            }

            // If cycCount is 5
            else if (cycCount == 5) {
                if (i == 0) {
                    printf("%s", mip[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                }
                else if (i == 1) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") == 0) {
                        printf(".\tIF\tID\t*\t*\t.\t.\t.\t.\n");
                    } 
                    else {
                        printf(".\tIF\tID\tEX\tMEM\t.\t.\t.\t.\n");
                    }
                }
                else if (i == 2) {
                    if (strcmp(mip[i], "nop\t\t") == 0) {
                        printf("%s", mip[i]);
                        printf(".\tIF\tID\t*\t*\t.\t.\t.\t.\n");
                    }
                    else if (orig[i] == -1) {
                        if (nop1[i] >= 0 || nop2[i] >= 0) {
                            
                            // Dependencies are the same
                            if (nop1[i] == nop2[i]) {
                                for (k = lineCount; k > i; k--) {
                                    strncpy(mip[k], mip[k-1], sizeof(mip[k]));
                                    if (orig[k-1] > 0) orig[k] = orig[k-1];
                                    else orig[k] = k-1;
                                }
                                strncpy(mip[i], "nop\t\t", sizeof(mip[i]));
                                lineCount++;
                                
                                if (nop1[i] == 0) {
                                    hasNop[i+1] = 6; 
                                } 
                                else if (nop1[i] == 1) {
                                    hasNop[i+1] = 7;
                                }
                                if (lwNum == nop1[i]) lwNum = -1;
                            }
                            else if (nop2[i] == -1) {
                                for (k = lineCount; k > i; k--) {
                                    strncpy(mip[k], mip[k-1], sizeof(mip[k]));
                                    if (orig[k-1] > 0) orig[k] = orig[k-1];
                                    else orig[k] = k-1;
                                }
                                strncpy(mip[i], "nop\t\t", sizeof(mip[i]));
                                lineCount++;
                                
                                if (nop1[i] == 0) {
                                    hasNop[i+1] = 6; 
                                } 
                                else if (nop1[i] == 1) {
                                    hasNop[i+1] = 7;
                                }
                                if (lwNum == nop1[i]) lwNum = -1;
                            }
                            else if (nop1[i] == -1) {
                                for (k = lineCount; k > i; k--) {
                                    strncpy(mip[k], mip[k-1], sizeof(mip[k]));
                                    if (orig[k-1] > 0) orig[k] = orig[k-1];
                                    else orig[k] = k-1;
                                }
                                strncpy(mip[i], "nop\t\t", sizeof(mip[i]));
                                lineCount++;
                                
                                if (nop2[i] == 0) {
                                    hasNop[i+1] = 6; 
                                } 
                                else if (nop2[i] == 1) {
                                    hasNop[i+1] = 7;
                                }
                                if (lwNum == nop2[i]) lwNum = -1;
                            }
                            printf("%s", mip[i]);
                            printf(".\t.\tIF\tID\t*\t.\t.\t.\t.\n");
                            i++;
                            printf("%s", mip[i]);
                            printf(".\t.\tIF\tID\tID\t.\t.\t.\t.\n");

                        }
                    }
                    else {
                        printf("%s", mip[i]);
                        if (hasNop[i] != 6) {
                            printf(".\t.\tIF\tID\tEX\t.\t.\t.\t.\n");
                        }
                        else {
                            printf(".\tIF\tID\tID\tID\t.\t.\t.\t.\n");
                        }
                    }
                }
                else if (i == 3) {
                    printf("%s", mip[i]);
                    if (hasNop[i] != 6) {
                        if (orig[i] != 2) {
                            printf(".\t.\t.\tIF\tID\t.\t.\t.\t.\n");
                        } else {
                            printf(".\t.\tIF\tID\tEX\t.\t.\t.\t.\n");
                        }
                    }
                    else {
                        if (strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\tIF\tID\tID\tID\t.\t.\t.\t.\n");
                        }   
                        else {
                            printf(".\t.\tIF\tID\tID\tEX\t.\t.\t.\n");
                        }
                    }
                }
                else if (i == 4) {
                    printf("%s", mip[i]);
                    if (orig[i] == 2 && strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                        printf(".\t.\tIF\tIF\tIF\t.\t.\t.\t.\n");
                    }
                    else if (orig[i] != 3) {
                        printf(".\t.\t.\t.\tIF\t.\t.\t.\t.\n");
                    }
                    else {
                        printf(".\t.\t.\tIF\tID\t.\t.\t.\t.\n");
                    }
                    
                }
            }

            else if (cycCount == 6) {
                if (i == 0) {
                    printf("%s", mip[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                }

                else if (i == 1) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") != 0) {
                        printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                    } 
                    else {
                        printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                    }
                }

                else if (i == 2) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") != 0) {
                        if (hasNop[i] != 6) { 
                            printf(".\t.\tIF\tID\tEX\tMEM\t.\t.\t.\n");
                        }
                        else {
                            printf(".\tIF\tID\tID\tID\tEX\t.\t.\t.\n");
                        }
                    }
                    else {
                        if (strcmp(mip[i-1], "nop\t\t") != 0) {
                            printf(".\t.\tIF\tID\t*\t*\t.\t.\t.\n");
                        }
                        else {
                            printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                        }
                    }
                }

                else if (i == 3) {
                    if (strcmp(mip[i], "nop\t\t") == 0) {
                        printf("%s", mip[i]);
                        printf(".\t.\tIF\tID\t*\t*\t.\t.\t.\n");
                    }
                    else if (hasNop[i] == 6) {
                        printf("%s", mip[i]);
                        if (strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\tIF\tID\tID\tID\tEX\t.\t.\t.\n");
                        }
                        else {
                            printf(".\t.\tIF\tID\tID\tEX\t.\t.\t.\n");
                        }
                    }
                    else if (hasNop[i] == 7) {
                        printf("%s", mip[i]);
                        printf(".\t.\tIF\tID\tID\tID\t.\t.\t.\n");
                    }
                    else {
                        // if (nop1[i] == 1 || nop2[i] == 1) {
                        //     // if (lineCount == 4) {
                        //     //     strncpy(mip[lineCount], mip[i], sizeof(mip[lineCount]));
                        //     //     strncpy(mip[i], "nop\t\t", sizeof(mip[i]));
                        //     //     orig[lineCount] = i;
                        //     //     lineCount++;

                        //     //     printf("%s", mip[i]);
                        //     //     printf(".\t.\t.\tIF\tID\t*\t.\t.\t.\n");
                        //     //     i++;
                        //     //     hasNop[i] = 7;
                        //     //     printf("%s", mip[i]);
                        //     //     printf(".\t.\t.\tIF\tID\tID\t.\t.\t.\n");
                        //     // }

                        // }
                        if (orig[i] != 2) {
                            printf("%s", mip[i]);
                            printf(".\t.\t.\tIF\tID\tEX\t.\t.\t.\n");
                        }
                        else {
                            printf("%s", mip[i]);
                            printf(".\t.\tIF\tID\tEX\tMEM\t.\t.\t.\n");
                        }
                    }
                }

                else if (i == 4) {
                    printf("%s", mip[i]);
                    if (orig[i] == 2) {
                        if (hasNop[i] == 6 && strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\t.\tIF\tIF\tIF\tID\t.\t.\t.\n");
                        }
                        else if (hasNop[i] == 7 && strcmp(mip[i-2], "nop\t\t") == 0) {
                            printf(".\t.\tIF\tID\tID\tID\t.\t.\t.\n");
                        }
                    }
                    else if (orig[i] != 3) {
                        printf(".\t.\t.\t.\tIF\tID\t.\t.\t.\n");
                    }
                    else {
                       printf(".\t.\t.\tIF\tID\tEX\t.\t.\t.\n"); 
                    }
                }
            }

            else if (cycCount == 7) {
                if (i == 0) {
                    printf("%s", mip[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                }

                else if (i == 1) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") != 0) {
                        printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                    } 
                    else {
                         printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");                       
                    }
                }

                else if (i == 2) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") != 0) {
                        if (hasNop[i] != 6) {
                            printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        }
                        else {
                            printf(".\tIF\tID\tID\tID\tEX\tMEM\t.\t.\n");                           
                        }
                    } 
                    else {
                        if (strcmp(mip[i-1], "nop\t\t") != 0) {
                            printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                        }
                        else {
                            printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                        }
                    }
                }

                else if (i == 3) {
                    printf("%s", mip[i]);
                    if (hasNop[i] == 6) {
                        if (strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\tIF\tID\tID\tID\tEX\tMEM\t.\t.\n");
                        }
                        else {
                            printf(".\t.\tIF\tID\tID\tEX\tMEM\t.\t.\n");
                        }
                    }
                    else if (hasNop[i] == 7) {
                        printf(".\t.\tIF\tID\tID\tID\tEX\t.\t.\n");
                    }
                    else if (strcmp(mip[i], "nop\t\t") != 0) {
                        if (hasNop[2] >= 0) {
                            printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        } else {
                            printf(".\t.\t.\tIF\tID\tEX\tMEM\t.\t.\n");
                        }
                    }
                    else {
                        if (strcmp(mip[i-1], "nop\t\t") != 0) {
                            printf(".\t.\t.\tIF\tID\t*\t*\t.\t.\n");
                        } else {
                            printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                        }
                    }
                }

                else if (i == 4) {
                    printf("%s", mip[i]);
                    if (orig[i] == 2) {
                        if (hasNop[i] == 7) {
                            if (strcmp(mip[i-2], "nop\t\t") != 0) {
                                printf(".\t.\t.\tIF\tID\tID\tEX\t.\t.\n");
                            }
                            else {
                                printf(".\t.\tIF\tID\tID\tID\tEX\t.\t.\n");
                            }
                        }
                        else if (hasNop[i] == 6 && strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\t.\tIF\tIF\tIF\tID\tEX\t.\t.\n");
                        }
                    }
                    else if (orig[i] == 3) {
                        if (hasNop[i] != 7) {
                            printf(".\t.\t.\tIF\tID\tEX\tMEM\t.\t.\n");
                        }
                        else {
                            printf(".\t.\t.\tIF\tID\tID\tEX\t.\t.\n");
                        }
                        
                    }
                    else {
                        printf(".\t.\t.\t.\tIF\tID\tEX\t.\t.\n");
                    }
                }

            }

            else if (cycCount == 8) {
                if (i == 0) {
                    printf("%s", mip[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                }

                else if (i == 1) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") != 0) {
                        printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                    } 
                    else {
                        printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                    }
                } 

                else if (i == 2) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") != 0) {
                        if (hasNop[i] == 6) {
                            printf(".\tIF\tID\tID\tID\tEX\tMEM\tWB\t.\n");
                        }
                        else {
                            printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        }
                    } 
                    else {
                        if (strcmp(mip[i-1], "nop\t\t") != 0) {
                            printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                        }
                        else {
                            printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                        }
                    }
                }

                else if (i == 3) {
                    printf("%s", mip[i]);
                    if (hasNop[i] == 6) {
                        if (strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\tIF\tID\tID\tID\tEX\tMEM\tWB\t.\n");
                        }
                        else {
                            printf(".\t.\tIF\tID\tID\tEX\tMEM\tWB\t.\n");
                        }
                    }
                    else if (hasNop[i] == 7) {
                        printf(".\t.\tIF\tID\tID\tID\tEX\tMEM\t.\n");
                    }
                    else if (strcmp(mip[i], "nop\t\t") != 0) {
                        if (hasNop[2] >= 0) {
                            printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        } else {
                            printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                        }
                    }
                    else {
                        if (strcmp(mip[i-1], "nop\t\t") != 0) {
                            printf(".\t.\t.\tIF\tID\t*\t*\t*\t.\n");
                        } else {
                            printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                        }
                    }
                }

                else if (i == 4) {
                    printf("%s", mip[i]);
                    if (orig[i] == 2) {
                        if (hasNop[i] == 7) {
                            if (strcmp(mip[i-2], "nop\t\t") != 0) {
                                printf(".\t.\t.\tIF\tID\tID\tEX\tMEM\t.\n");
                            }
                            else {
                                printf(".\t.\tIF\tID\tID\tID\tEX\tMEM\t.\n");  
                            }
                        }
                        else if (hasNop[i] == 6 && strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\t.\tIF\tIF\tIF\tID\tEX\tMEM\t.\n");
                        }
                    }
                    else if (orig[i] == 3) {
                        if (hasNop[i] != 7) {
                            printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                        }
                        else {
                            printf(".\t.\t.\tIF\tID\tID\tEX\tMEM\t.\n");
                        }
                    }
                    else {
                        printf(".\t.\t.\t.\tIF\tID\tEX\tMEM\t.\n");
                    }
                }
            }

            else if (cycCount == 9) {
                if (i == 0) {
                    printf("%s", mip[i]);
                    printf("IF\tID\tEX\tMEM\tWB\t.\t.\t.\t.\n");
                }

                else if (i == 1) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") != 0) {
                        printf(".\tIF\tID\tEX\tMEM\tWB\t.\t.\t.\n");
                    } 
                    else {
                        printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                    }
                } 

                else if (i == 2) {
                    printf("%s", mip[i]);
                    if (strcmp(mip[i], "nop\t\t") != 0) {
                        if (hasNop[i] == 6) {
                            printf(".\t.\tIF\tID\tID\tEX\tMEM\tWB\t.\n");
                        }
                        else {
                            printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        }
                    } 
                    else {
                        if (strcmp(mip[i-1], "nop\t\t") != 0) {
                            printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                        }
                        else {
                            printf(".\tIF\tID\t*\t*\t*\t.\t.\t.\n");
                        }
                    }
                }

                else if (i == 3) {
                    printf("%s", mip[i]);
                    if (hasNop[i] == 6) {
                        if (strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\tIF\tID\tID\tID\tEX\tMEM\tWB\t.\n");
                        }
                        else {
                            printf(".\t.\tIF\tID\tID\tEX\tMEM\tWB\t.\n");
                        }
                    }
                    else if (hasNop[i] == 7) {
                        printf(".\t.\tIF\tID\tID\tID\tEX\tMEM\tWB\n");
                    }
                    else if (strcmp(mip[i], "nop\t\t") != 0) {
                        if (hasNop[2] >= 0) {
                            printf(".\t.\tIF\tID\tEX\tMEM\tWB\t.\t.\n");
                        } else {
                            printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n");
                        }
                    }
                    else {
                        if (strcmp(mip[i-1], "nop\t\t") != 0) {
                            printf(".\t.\t.\tIF\tID\t*\t*\t*\t.\n");
                        } else {
                            printf(".\t.\tIF\tID\t*\t*\t*\t.\t.\n");
                        }
                    }
                }

                else if (i == 4) {
                    printf("%s", mip[i]);
                    if (orig[i] == 2) {
                        if (hasNop[i] == 7) {
                            if (strcmp(mip[i-2], "nop\t\t") != 0) {
                                printf(".\t.\t.\tIF\tID\tID\tEX\tMEM\tWB\n");
                            }
                            else {
                                printf(".\t.\tIF\tID\tID\tID\tEX\tMEM\tWB\n");
                            }
                        }
                        else if (hasNop[i] == 6 && strcmp(mip[1], "nop\t\t") == 0 && strcmp(mip[2], "nop\t\t") == 0) {
                            printf(".\t.\tIF\tIF\tIF\tID\tEX\tMEM\tWB\n");
                        }
                    }
                    else if (orig[i] == 3) {
                        if (hasNop[i] != 7) {
                            printf(".\t.\t.\tIF\tID\tEX\tMEM\tWB\t.\n"); 
                        }
                        else {
                            printf(".\t.\t.\tIF\tID\tID\tEX\tMEM\t.\n");
                        }
                    }
                    else {
                        printf(".\t.\t.\t.\tIF\tID\tEX\tMEM\tWB\n");
                    }
                }
            }
        }
        printf("\n");
    }

    // End simulation
    printf("END OF SIMULATION\n");

    fclose(file);

    return EXIT_SUCCESS;
}