#include<stdio.h>
#include <stdlib.h>
#include <String.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

bool mode(int*, int);
void createSelect(char**, int);
void createRandom(char**, int);
void start(char*, int, int);
int checkBullsCows(char*, char*, int);
bool checkModeNum(char*);
bool checkSelect(char*, int);
bool checkRandom(char*, int, int);
void errorMsg(int);

int main() {
	int modeNum = 0, ta = 0;
	char* answer = NULL;

	while (true) {
		if (!mode(&modeNum, 0)) { errorMsg(1); continue; }
		else { break; }
	}
	if (modeNum == 0) {
		ta = 1;
		while (true) {
			if (!mode(&modeNum, 1)) { errorMsg(2); continue; }
			else { break; }
		}
		createSelect(&answer, modeNum);
	}
	else { createRandom(&answer, modeNum); }
	
	start(answer, modeNum, ta);
	free(answer);
	return 0;
}

bool mode(int* modeNum, int option) {
	char modeBuffer[101] = { '\0' };

	if (option == 1) { 
		printf("----------------TA MODE----------------\n");
		printf("Enter number of digits in code(3,4,5): ");
	}
	else { printf("Enter number of digits in code(0 or 3,4,5): "); }
	fgets(modeBuffer, sizeof(modeBuffer) - 1, stdin);
	modeBuffer[strlen(modeBuffer) - 1] = '\0';
	if (checkModeNum(modeBuffer)) {
		*modeNum = atoi(modeBuffer);
		return true;
	}
	else { return false; }
}

void createSelect(char** answer, int digit) {
	char answerBuffer[11] = { '\0' };
	int length = 0;

	*answer = (char**)calloc(digit + 1, sizeof(char));
	while (true) {
		printf("Enter Number: ");
		fgets(answerBuffer, sizeof(answerBuffer) - 1, stdin);
		answerBuffer[strlen(answerBuffer) - 1] = '\0';
		length = strlen(answerBuffer);

		if (length > digit) { errorMsg(3); continue; }
		else if (length <= digit - 2) { errorMsg(4); continue; }
		else if (!checkSelect(answerBuffer, digit)) { errorMsg(4); continue; }
		else { strcpy(*answer, answerBuffer); break; }
	}
}

void createRandom(char** answer, int digit) {
	int num = 0;

	*answer = (char**)calloc((digit + 1), sizeof(char));
	while (true) {
		srand(time(NULL));
		num = rand() % (int)(pow(10, digit) - pow(10, (double)digit - 2)) + pow(10, (double)digit - 2);
		if (!checkRandom(*answer, num, digit)) { continue; }
		else { break; }
	}
}

void start(char* answer, int size, int ta) {
	int i = 0, j = 0, bulls = 0, length = 0;
	char guess[101];

	while (bulls != size) {
		if (ta == 1) {
			printf("Nuber to guess: ");
			for (i = 0; i < size; i++) {
				if (i == size - 1) { printf("%c\n", answer[i]); }
				else { printf("%c-", answer[i]); }
			}
		}
		printf("Enter guess: ");
		fgets(guess, sizeof(guess) - 1, stdin);
		guess[strlen(guess) - 1] = '\0';
		length = strlen(guess);

		if (length > size) { errorMsg(3); continue; }
		else if (length <= size - 2) { errorMsg(4); continue; }
		else if (!checkSelect(guess, size)) { errorMsg(4); continue; }
		else { bulls = checkBullsCows(answer, guess, size); }
	}
}

int checkBullsCows(char* answer, char* guess, int size) {
	int i = 0, j = 0, bulls = 0, cows = 0;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (answer[i] == guess[j] && i == j) { ++bulls; }
			if (answer[i] == guess[j] && i != j) { ++cows; }
		}
	}
	if (bulls != size) {
		printf("%d BULLS / %d COWS\n", bulls, cows);
	}
	else { printf("%d BULLS!\n", bulls); }
	
	return bulls;
}

bool checkModeNum(char* modeBuffer) {
	int length = strlen(modeBuffer);
	
	if (length == 1) {
		if (strcmp(modeBuffer, "0") == 0) { return true; }
		if (atoi(modeBuffer) > 2 && atoi(modeBuffer) < 6) { return true; }
		return false;
	}
	else { return false; }
}

bool checkSelect(char* answerBuffer, int size) {
	int i = 0, j = 0;
	char buf[10] = { '\0' };

	if (strlen(answerBuffer) < size) { sprintf(buf, "0%s", answerBuffer); }
	else { sprintf(buf, "%s", answerBuffer); }
	for (i = 0; i < size; i++) {
		for (j = 0; j < i; j++) {
			if (buf[i] == buf[j]) { return false; }
		}
	}
	strcpy(answerBuffer, buf);
	return true;
}

bool checkRandom(char* answer, int num, int size) {
	int i = 0, j = 0;

	if (num < pow(10, (double)size - 1)) { sprintf(answer, "0%d", num); }
	else { sprintf(answer, "%d", num); }

	for (i = 0; i < size; i++) {
		for (j = 0; j < i; j++) {
			if (answer[i] == answer[j]) { return false; }
		}
	}
	return true;
}

void errorMsg(int option) {
	switch (option) {
	case 1:
		printf("Select 0 or 3,4,5\n");
		break;
	case 2:
		printf("Select 3,4,5\n");
		break;
	case 3:
		printf("Too many digits\n");
		break;
	case 4:
		printf("Each number must be differen\n");
		break;
	}
}