#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void removeSpaces(char *s){
	char *d = s;
	while (*d) {
		if (isspace((unsigned char)*d)) {
			d++;
		} else {
			*s++ = *d++;
		}
	}
	*s = '\0';
}
struct Pair {
	char mnemonic[64];
	int value;
};
struct Pair arr[20000];

int main(void){
	printf("input file name\n");
	char fileName[10];
	scanf("%s" , fileName);
	FILE *source;
	source = fopen(fileName, "r");
	if (source == NULL) {
		printf("File Not Found\n");
		return -1;
	}
	//source file opened
	//create and initialize symbol table
	strcpy(arr[0].mnemonic, "R0");      	arr[0].value = 0;
	strcpy(arr[1].mnemonic, "R1");      	arr[1].value = 1;
	strcpy(arr[2].mnemonic, "R2");      	arr[2].value = 2;
	strcpy(arr[3].mnemonic, "R3");      	arr[3].value = 3;
	strcpy(arr[4].mnemonic, "R4");      	arr[4].value = 4;
	strcpy(arr[5].mnemonic, "R5");      	arr[5].value = 5;
	strcpy(arr[6].mnemonic, "R6");      	arr[6].value = 6;
	strcpy(arr[7].mnemonic, "R7");      	arr[7].value = 7;
	strcpy(arr[8].mnemonic, "R8");      	arr[8].value = 8;
	strcpy(arr[9].mnemonic, "R9");      	arr[9].value = 9;
	strcpy(arr[10].mnemonic, "R10");    	arr[10].value = 10;
	strcpy(arr[11].mnemonic, "R11");    	arr[11].value = 11;
	strcpy(arr[12].mnemonic, "R12");    	arr[12].value = 12;
	strcpy(arr[13].mnemonic, "R13");    	arr[13].value = 13;
	strcpy(arr[14].mnemonic, "R14");    	arr[14].value = 14;
	strcpy(arr[15].mnemonic, "R15");    	arr[15].value = 15;
	strcpy(arr[16].mnemonic, "SP");     	arr[16].value = 0;
	strcpy(arr[17].mnemonic, "LCL");    	arr[17].value = 1;
	strcpy(arr[18].mnemonic, "ARG");    	arr[18].value = 2;
	strcpy(arr[19].mnemonic, "THIS");   	arr[19].value = 3;
	strcpy(arr[20].mnemonic, "THAT");   	arr[20].value = 4;
	strcpy(arr[21].mnemonic, "SCREEN"); 	arr[21].value = 16384;
	strcpy(arr[22].mnemonic, "KBD");    	arr[22].value = 24576;
	//prep for first pass
	char currentLine[512];
	char inbracket[64];
	int i = 23;
	int currentLineValue = 0;
	char containsText;
	char containsComment;
	char currentLineNoComment[64];
	bool isLabel;
	bool hasText;
	bool isComment;
	bool hasComment;
	FILE *temp = tmpfile();
	if (temp) {
//	printf("temp file made\n");
	}
	//First Pass (labels and comment removal)
	while (fgets(currentLine , sizeof(currentLine) , source) != NULL) {
		if (strchr(currentLine, '(') != NULL) {
			isLabel = true;
		} else {
			isLabel = false;
		}
		if (sscanf(currentLine , " %c" , &containsText) == 1) {
			hasText = true;
		} else {
			hasText = false;
		}
		sscanf(currentLine , " %c" , &containsComment);
		if (containsComment == '/') {
			isComment = true;
		} else {
			isComment = false;
		} if (isLabel){
			sscanf(currentLine , " %*c %[^)]" , inbracket);
			strcpy(arr[i].mnemonic, inbracket);
			arr[i].value = currentLineValue; i++;
		}
		if ((strchr(currentLine, '/') != NULL) && (!isComment)) {
			hasComment = true;
		} else {
			hasComment = false;
		}
		if ((hasText) && (!isLabel) && (hasComment)) {
			sscanf(currentLine , "%63[^/]" , currentLineNoComment);
//			printf("line read: %s \n", currentLineNoComment);
			removeSpaces(currentLineNoComment);
			fprintf(temp, "%s\n" , currentLineNoComment);
			currentLineValue++;
		}
		if ((hasText) && (!isLabel) && (!isComment) && (!hasComment)) {
			if (currentLine[0] != '\n') {
			removeSpaces(currentLine);
			fprintf(temp, "%s\n" , currentLine);
			currentLineValue++;
			}
		}
	}
	//check temp file
	//rewind(temp);
	//printf("outputting temp file\n");
	//while (fgets(currentLine, 80, temp) != NULL) {
	//	printf("%s" , currentLine);
	//}
	//
	//init out file
	FILE *out;
	out = fopen("code.hack" , "wb");
	//2nd pass prep
	int num;
	int isDigitCount;
	int existsCount;
	int varNum = 16;
	char d1 = '0';
	char d2 = '0';
	char d3 = '0';
	char j1 = '0';
	char j2 = '0';
	char j3 = '0';
	char a = '0';
	char dest[8];
	char jmp[8];
	char currentComp[10];
	char compBits[7] = "000000";
	bool isJmp;
	bool isDest;
	bool xUsed;
	bool yUsed;
	bool numUsed;
	bool isDigit;
	bool exists;
	char currentLineTemp[512];
	//using temp file
	rewind(temp);
	while (fgets(currentLine , sizeof(currentLine) , temp) != NULL) {
		removeSpaces(currentLine);
		//reset values
		memset(dest, 0, sizeof(dest));
		memset(jmp, 0, sizeof(jmp));
		memset(currentComp, 0, sizeof(currentComp));
		strcpy(compBits, "000000");
		d1 = '0';
		d2 = '0';
		d3 = '0';
		j1 = '0';
		j2 = '0';
		j3 = '0';
		a = '0';
		exists = false;
		isDigit = false;
		isDest = false;
		isJmp = false;
//		printf("CURRENT LINE IS:%s",currentLine);
		//A-instruction else C-instruction
		if (currentLine[0] == '@') {
			sscanf(currentLine,"%*c%s",currentLineTemp);
			memcpy(currentLine, currentLineTemp, 512);
			isDigitCount = 0;
			while ((isDigitCount<=80) && (currentLine[isDigitCount] != '\0')) {
				if (isdigit(currentLine[isDigitCount]) != 0) {
					isDigit = true;
				} else {
					isDigit = false;
					break;
				}
				isDigitCount++;
			}
			if (isDigit) {
//				printf("current num is: %s\n",currentLine);
				num = atoi(currentLine);
//				printf("current num is: %b \n",num);
				fprintf(out,"0%015b\n",num);
			} else {
				existsCount = 0;
				while (existsCount<=i) {
					if (strcmp(currentLine, arr[existsCount].mnemonic) == 0) {
						exists = true;
						break;
					}
					existsCount++;
				}
				if (exists) {
					num = arr[existsCount].value;
					fprintf(out,"0%015b\n",num);
				}
				else {
					strcpy(arr[i].mnemonic, currentLine);
					arr[i].value = varNum;
					fprintf(out,"0%015b\n",varNum);
					varNum++; i++;
				}
			}
		} else {
			//dest
			if (strchr(currentLine,'=') != NULL) {
				isDest = true;
				sscanf(currentLine,"%[^=]",dest);
//				printf("current dest is: %s\n",dest);
				for (int k = 0 ; k != 3 ; k++) {
					switch (dest[k]) {
						case 'A':
							d1 = '1';
							break;
						case 'D':
							d2 = '1';
							break;
						case 'M':
							d3 = '1';
							break;
					}
//					printf("dest (%d) is %c\n",k,dest[k]);
				}
			}
			//jmp
			if (strchr(currentLine,';')!=NULL) {
				isJmp = true;
				sscanf(currentLine,"%*[^J]%s",jmp);
//				printf("jump is %s\n",jmp);
				if (strncmp(jmp , "JMP" , 3) == 0) {
					j1 = '1';
					j2 = '1';
					j3 = '1';
				}
				else if (strncmp(jmp , "JLE" , 3) == 0) {
					j1 = '1';
					j2 = '1';
					j3 = '0';
				}
				else if (strncmp(jmp , "JNE" , 3) == 0) {
					j1 = '1';
					j2 = '0';
					j3 = '1';
				}
				else if (strncmp(jmp , "JLT" , 3) == 0) {
					j1 = '1';
					j2 = '0';
					j3 = '0';
				}
				else if (strncmp(jmp , "JGE" , 3) == 0) {
					j1 = '0';
					j2 = '1';
					j3 = '1';
				}
				else if (strncmp(jmp , "JEQ" , 3) == 0) {
					j1 = '0';
					j2 = '1';
					j3 = '0';
				}
				else if (strncmp(jmp , "JGT" , 3) == 0) {
					j1 = '0';
					j2 = '0';
					j3 = '1';
				}
			}
//			printf("isJmp and isDest is:%d%d\n",isJmp,isDest);
			//extrtacting comp 
			if ((isJmp) && (isDest)) {
				sscanf(currentLine , "%*[^=]=%8[^;]" , currentComp);
//				printf("isJmp and isDest Comp: %s\n",currentComp);
			}
			else if ((isJmp) && (!isDest)) {
				sscanf(currentLine , "%8[^;]" , currentComp);
//				printf("isJmp and !isDest Comp: %s\n",currentComp);
			}
			else if ((!isJmp) && (isDest)) {
				sscanf(currentLine , "%*[^=]=%9s" , currentComp);
//				printf("!isJmp and isDest Comp: %s\n",currentComp);
			}
			else if ((!isJmp) && (!isDest)) {
				sscanf(currentLine , "%9s" , currentComp);
//				printf("!isJmp and !isDest Comp: %s\n",currentComp);
			}
			if (strchr(currentComp , 'M') != NULL) {
				a = '1';
			}
//			printf("current j1 j2 j3 and a be %c %c %c and %c\n",j1,j2,j3,a);
//			printf("currentComp after 4 ifs is %s\n",currentComp);
			//check what comp contains
			if (strchr(currentComp, 'D') != NULL) {
				xUsed = true;
			} else {
				xUsed = false;
			}
			if ((strchr(currentComp, 'M') != NULL)||(strchr(currentComp, 'A')!= NULL)) {
				yUsed = true;
			} else {
				yUsed = false;
			}
			if ((strchr(currentComp, '0') != NULL)||(strchr(currentComp, '1')!= NULL)) {
				numUsed = true;
			} else {
				numUsed = false;
			}
			//a bit
			if (strchr(currentComp, 'M') != NULL) {
				a = '1';
			}
			//comp bits
			//no D is x and M/A are y depending on a bit
			if (!xUsed && !yUsed && numUsed) {
				if (strncmp(currentComp, "0", 1) == 0) {
					strcpy(compBits, "101010");
				}
				if (strncmp(currentComp, "1", 1) == 0) {
					strcpy(compBits, "111111");
				}
				if (strncmp(currentComp, "-1", 2) == 0) {
					strcpy(compBits, "111010");
				}
			}
			else if (xUsed && !yUsed && !numUsed) {
				if (strchr(currentComp, '-') != NULL) {
					strcpy(compBits, "001111");
				}
				else if (strchr(currentComp, '!') != NULL) {
					strcpy(compBits, "001101");
				} else {
					strcpy(compBits, "001100");
				}
			}
			else if (!xUsed && yUsed && !numUsed) {
				if (strchr(currentComp, '-') != NULL) {
					strcpy(compBits, "110011");
				}
				else if (strchr(currentComp, '!') != NULL) {
					strcpy(compBits, "110001");
				} else {
					strcpy(compBits, "110000");
				}
			}
			else if (xUsed && !yUsed && numUsed) {
				if (strchr(currentComp, '-') != NULL) {
					strcpy(compBits, "001110");
				} else {
					strcpy(compBits, "011111");
				}
			}
			else if (!xUsed && yUsed && numUsed) {
				if (strchr(currentComp, '-') != NULL) {
					strcpy(compBits, "110010");
				} else {
					strcpy(compBits, "110111");
				}
			}
			else if (xUsed && yUsed && !numUsed) {
				if (strchr(currentComp, '+') != NULL) {
					strcpy(compBits, "000010");
				}
				else if (strchr(currentComp, '&') != NULL) {
					strcpy(compBits, "000000");
				}
				else if (strchr(currentComp, '|') != NULL) {
					strcpy(compBits, "010101");
				}
				else if (strchr(currentComp, '-') != NULL) {
					if (strstr(currentComp, "D-") != NULL) {
						strcpy(compBits, "010011");
					} else {
						strcpy(compBits, "000111");
					}
				}
			}
			fprintf(out, "111%c%s%c%c%c%c%c%c\n",a,compBits,d1,d2,d3,j1,j2,j3);
		}
	}
	if (fclose(source) == 0 ) {
		printf("closed source file successfully\n");
	}
	if (fclose(out) == 0 ) {
		printf("closed out file successfully\n");
	}
	if (fclose(temp) == 0) {
		printf("temp file gone");
	}
	return 0;
}
