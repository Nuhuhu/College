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
		//0,3,4,5중 선택 -> 오류시 계속 무한 루프
		if (!mode(&modeNum, 0)) { errorMsg(1); continue; }
		else { break; }
	}
	/*
	1. 0 선택시 TA 모드
	2. 답을 직접 작성하고 볼 수 있음
	*/
	if (modeNum == 0) {
		ta = 1;
		while (true) {
			if (!mode(&modeNum, 1)) { errorMsg(2); continue; }
			else { break; }
		}
		createSelect(&answer, modeNum);
	}
	// 0이 아닐시 숫자에 맞는 자릿수의 자연수 임의 생성
	else { createRandom(&answer, modeNum); }
	
	//게임 스타트
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
	//숫자 입력
	fgets(modeBuffer, sizeof(modeBuffer) - 1, stdin);
	modeBuffer[strlen(modeBuffer) - 1] = '\0';
	//0,3,4,5 인지 검토
	if (checkModeNum(modeBuffer)) {
		*modeNum = atoi(modeBuffer);
		return true;
	}
	else { return false; }
}

/*
  TA 모드 일때 숫자 생성
  3,4,5 자리 숫자중 하나 생성
  같은 수가 다른 자리에 있거나(ex 112) 자릿수 안맞는 숫자들 검토
*/
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

//임의 숫자 생성
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

/*
  TA모드일때 답을 보여주고 아닐 시 직접 맞춤
  숫자 입력하고 각 자릿수 및 숫자 검토
*/
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

/*
  bulls / cows 갯수 확인
*/
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

/*
  숫자 검토 함수
  0,3,4,5 외에는 false
*/
bool checkModeNum(char* modeBuffer) {
	int length = strlen(modeBuffer);
	
	if (length == 1) {
		if (strcmp(modeBuffer, "0") == 0) { return true; }
		if (atoi(modeBuffer) > 2 && atoi(modeBuffer) < 6) { return true; }
		return false;
	}
	else { return false; }
}

/*
  직접 입력한 숫자 검토
  3자리 자연수에 두자리 자연수가 오면 맨 앞에 0토글
  다른 자리에 같은 숫자 검토
*/
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

/*
 임의로 만든 숫자 각 자리의 숫자 검토
*/
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