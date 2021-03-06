#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 40

int counterline(FILE *Ptr) {							//計算過濾檔行數
	char buffer[SIZE];
	int counter = 0;

	while (fgets(buffer, SIZE, Ptr) != NULL) {			//一次一行，有讀到字則counter+1
		counter++;
	}

	return counter;
}

void filter(FILE *Ptr, char **input, int times) {		//讀取過濾檔

	for (int i = 1; i <= times; i++) {					//times為行數

		char buffer[SIZE];
		fgets(buffer, SIZE, Ptr);
		int length = strlen(buffer);					//讀取字串長

		for (int j = 1; j <= length; j++) {
			while (!((buffer[j - 1] >= 48 && buffer[j - 1] <= 57) || (buffer[j - 1] >= 65 && buffer[j - 1] <= 90) || (buffer[j - 1] >= 97 && buffer[j - 1] <= 122)||buffer[j-1]=='\0')) {			//處理buffer中不為英數的字
				for (int k = j; k <= length; k++) {
					buffer[k - 1] = buffer[k];
				}
				length--;
			}
		}

		input[i - 1] = (char*)malloc(sizeof(char)*(length + 1));
		strcpy(input[i - 1], buffer);					//buffer轉存input
	}
}

FILE* open(char fileName[], char type[]) {
	FILE *fPtr = fopen(fileName, type);
	if (fPtr == NULL) {
		printf("The file opened error.\n");
		exit(1);
	}
	return fPtr;
}

void fill(FILE *Ptr, char ** filter, int times) {
	char ch;
	char buffer[SIZE];
	int counter = 0, c = 0;											//counter是記錄指標位移值，c是字長以及非英數符號

	while ((ch = fgetc(Ptr)) != EOF) {
		if ((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)) {   //如果是英數就讀到buffer
			buffer[c] = ch;
			c++;
		}

		else {
			buffer[c] = '\0';
			for (int i = 1; i <= times; i++) {
				if (strcmp(buffer, filter[i - 1]) == 0) {
					fseek(Ptr, counter*sizeof(char), SEEK_SET);			//回到需覆蓋的位置
					for (int i = 1; i <= c; i++) {						//覆蓋
						fputc(' ', Ptr);
					}
					fputc(ch, Ptr);
				}
			}
			c++;
			counter += c;			

			c = 0;
			buffer[c] = '\0';

			fseek(Ptr, counter * sizeof(char), SEEK_SET);				//回到下一個讀取位置
		}
	}
}