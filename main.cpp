#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<Windows.h>

/*
* structure of pokemon game
*/
FILE *file;
int countWin, countLose;

typedef struct _eachPokemon{
	char name[20];

	int health;
	struct _eachPokemon* next;
}eachPokemon, *eachPokemonPointer;

typedef struct _orderPokemon {
	int count;
	eachPokemonPointer head;
}orderPokemon;

void initPokemon(orderPokemon* op) {
	op->count = 0;
	op->head = NULL;
}

void makePokemon(orderPokemon *op) {
	srand(time(0));

	int i, j;
	int randValue;
	char namesPokemon[20][20] = { { "치코리타" }, { "꼬부기" }, { "피카츄" }, { "잉어킹" }, { "펭도리" }, { "파이리" }, { "장동건" }, { "고유빈" }, { "김동현" }, { "김준기" }, { "네퓨즈" } };
	int healthesPokemon[20] = { 500, 480, 450, 520, 420, 500, 510, 530, 520, 510, 10 };

	for (i = 0; i<6; i++) {
		eachPokemonPointer doMake = (eachPokemonPointer)malloc(sizeof(eachPokemon));
		randValue = rand() % 11;
		while (namesPokemon[randValue][0] == NULL) {
			randValue = rand() % 11;
		}
		strcpy(doMake->name, namesPokemon[randValue]);
		namesPokemon[randValue][0] = NULL;
		doMake->health = healthesPokemon[randValue];
		if (op->count == 0) {
			op->head = doMake;
		}
		else {
			eachPokemonPointer tmp;
			tmp = op->head;
			for (j = 0; j<(op->count) - 1; j++) {
				tmp = tmp->next;
			}
			tmp->next = doMake;
		}
		doMake->next = NULL;
		op->count++;
	}
}

void printAllPokemon(orderPokemon *op) {
	eachPokemonPointer tmp = op->head;
	int i;
	for (i = 0; i < (op->count); i++) {
		printf("[%d] ", i + 1);
		printf("%s HP : ", tmp->name);
		printf("%d\n", tmp->health);
		tmp = tmp->next;
	}
}

int hitPokemon(orderPokemon *op, int count, int damage) {
	int i;
	eachPokemonPointer tmp;
	tmp = op->head;

	for (i = 1; i<count; i++) {
		tmp = tmp->next;
	}

	tmp->health -= damage;

	if (tmp->health <= 0) {
		return 0;
	}
	else {
		return 1;
	}
}

int countLine() {
	int line = 1;
	char checkLine;
	file = fopen("user.txt", "r");

	while ((checkLine = fgetc(file)) != EOF)
	if (checkLine == '\n')
		++line;

	fclose(file);
	return line;
}

int isLoginSuccess(char *userID,char*userPW,int line) {
	int i;
	char fileUserID[20], fileUserPW[20];
	file = fopen("user.txt", "r");
	
	for (i = 0; i < line; i++) {
		fscanf(file, "%s %s %d %d", fileUserID, fileUserPW, &countWin, &countLose);
		if (!(strcmp(userID, fileUserID)) && !(strcmp(userPW, fileUserPW))) {
			return 1;
		}
	}

	fclose(file);
	
	return 0;
}

int isNotDuplicateID(char *userID, int line) {
	int i;
	char fileUserID[20], fileUserPW[20];
	file = fopen("user.txt", "r");

	for (i = 0; i < line; i++) {
		fscanf(file, "%s %s %d %d", fileUserID, fileUserPW, &countWin, &countLose);
		if (!(strcmp(userID, fileUserID))) {
			return 0;
		}
	}

	fclose(file);

	return 1;
}


int main() {
	char userID[20], userPW[20], fileUserID[20], fileUserPW[20];
	char LoginOrJoin;
	int checkFlag = 0, checkUserDuplicate = 0, line;
	int i;
	
	line = countLine();

	while (checkFlag == 0) {
	startPart:

		printf("[1] 로그인\n");
		printf("[2] 회원가입\n");
		printf("=> ");
		fflush(stdin);
		scanf("%c", &LoginOrJoin);
		if (!(LoginOrJoin == '1' || LoginOrJoin == '2'))
			goto startPart;
		printf("Input Type -> ID PW\nex)junoim a12345\n");
		printf("=> ");
		scanf("%s %s", &userID, &userPW);

		if (LoginOrJoin == '1' && isLoginSuccess(userID,userPW,line)) {
			printf("로그인 성공 !! \n");
			checkFlag = 1;
			Sleep(1000);
			system("cls");
			printf("총 승리 : %d 총 패배 : %d\n", countWin, countLose);
		}
		else if (LoginOrJoin == '2' && isNotDuplicateID(userID, line)) {
			file = fopen("user.txt", "a");
			fprintf(file, "\n%s %s 0 0", userID, userPW);
			fclose(file);
			++line;
			printf("회원 가입 성공 !!\n");
			printf("가입하신 아이디와 패스워드 : %s %s\n", userID, userPW);
		}
		else {
			printf("실패 !! \n");
		}

	}

	orderPokemon* Dawn = (orderPokemon*)malloc(sizeof(orderPokemon));

	printf("****** 포켓몬 리스트 초기화중 ... ******\n");
	initPokemon(Dawn);
	Sleep(1000);
	makePokemon(Dawn);
	printf("****** 포켓몬 생성중 ... ******\n");
	Sleep(1500);
	printf("****** 포켓몬 생성완료 !!! ******\n");
	printf("****** 당신의 6마리 포켓몬입니다. ******\n");
	printAllPokemon(Dawn);
	printf("****** 스킬 배정중입니다.. ******\n");

	return 1;
}