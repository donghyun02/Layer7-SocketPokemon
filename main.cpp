#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<string.h>
#pragma comment(lib,"Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include<Windows.h>

/*
* structure of pokemon game
*/
int countWin, countLose;
const int CHANGE_POKEMON = 1, HIT_POKEMON = 2, GAME_END = 3;

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
	char namesPokemon[20][20] = { { "치코리타" }, { "꼬부기" }, { "피카츄" }, { "잉어킹" }, { "펭도리" }, { "파이리" }, { "장동건" }, { "고유빈" }, { "김동현" }, { "임준오" }, { "네퓨즈boss" } };
	int healthesPokemon[20] = { 500, 480, 450, 520, 420, 500, 510, 530, 520, 510, 10000 };

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

int damagePokemon(orderPokemon *op, int damage) {
	int i;
	eachPokemonPointer tmp;
	tmp = op->head;
	tmp->health -= damage;

	if (tmp->health <= 0) {
		op->head = tmp->next;
		return 0;
	}
	else {
		return 1;
	}
}

int countLine() {
	FILE *file;
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
	FILE *file;
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
	FILE *file;
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

int socketError() {
	printf("윈속 생성 오류 !!\n프로그램을 종료합니다 ㅠㅠ\n");
	return 0;
}

int doingWhat(char *string) {
	if (strstr(string, "changePokemon")){
		return CHANGE_POKEMON;
	}
	else if (strstr(string, "hitPokemon")){
		return HIT_POKEMON;
	}
	else if(strstr(string, "gameEnd")){
		return GAME_END;
	}
}

int main() {
	FILE *file2;
	char userID[20], userPW[20], fileUserID[20], fileUserPW[20], remoteUserID[20], nowPokemon[20];
	int nowHealth=0, dieCount = 0;
	char userInput;
	char uWin[] = "You WIN !!!", uLose[] = "You Lose .....", uu[] = "WinAPI zz";
	char changePokemon[30] = "changePokemon ", gameEnd[30] = "gameEnd ", hitPokemon[30] = "hitPokemon ";
	char stringDie[] = "die",stringEnd[] = "end";
	char tempBuffer[30]="";
	char readBuffer[30], sendBuffer[30];
	int port, sizeClientAddr;
	char LoginOrJoin, MakeOrJoin;
	int checkFlag = 0, checkUserDuplicate = 0, checkRead = 0,tempINT,line;
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
			//printf("총 승리 : %d 총 패배 : %d\n", countWin, countLose);
		}
		else if (LoginOrJoin == '2' && isNotDuplicateID(userID, line)) {
			file2 = fopen("user.txt", "a");
			fprintf(file2, "\n%s %s 0 0", userID, userPW);
			fclose(file2);
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

	printf("\n[1] 방생성하기\n");
	printf("[2] 방참여하기\n");
	printf("=> ");
	fflush(stdin);
	scanf("%c", &MakeOrJoin);
	
	if (MakeOrJoin == '1') { // server
		WSADATA wsa;
		SOCKET pokemonClient;
		SOCKET pokemonServer;
		struct sockaddr_in serverAddr, clientAddr;
		printf("Input Port Number ! => ");
		scanf("%d", &port);

		WSAStartup(MAKEWORD(2, 2), &wsa);
		pokemonServer = socket(AF_INET, SOCK_STREAM, 0);
		pokemonClient = socket(AF_INET, SOCK_STREAM, 0);
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(port);
		bind(pokemonServer, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr_in));
		listen(pokemonServer, 5);

		printf("연결 대기중 .. 포트 : %d\n", port);

		sizeClientAddr = sizeof(clientAddr);
		pokemonClient = accept(pokemonServer, (SOCKADDR*)&clientAddr, &sizeClientAddr);

		printf("클리언트와 연결됨 !!\n");
		memset(readBuffer, 0, 30);
		memset(remoteUserID, 0, 30);
		memset(sendBuffer, 0, 30);

		recv(pokemonClient, readBuffer, 30, 0);
		strcpy(remoteUserID, readBuffer);

		printf("System : Enemy is %s.\n", remoteUserID);
		strcpy(sendBuffer, userID);
		send(pokemonClient, sendBuffer, sizeof(sendBuffer), 0); // send own nickname

		Sleep(600);
		system("cls");

		nowHealth = Dawn->head->health;
		strcpy(nowPokemon, Dawn->head->name);
		
		recv(pokemonClient, readBuffer, 30, 0);
		readBuffer[strlen(readBuffer)] = '\0';

		printf("%s : 가랏 !!! %s몬\n", remoteUserID, readBuffer);
		
		printf("%s : 가랏 !!! %s몬\n", userID, Dawn->head->name);

		memset(sendBuffer, 0, 30);
		strcpy(sendBuffer, Dawn->head->name);
		send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);

		while (1) {
			memset(readBuffer, 0, 30);
			memset(sendBuffer, 0, 30);
			memset(tempBuffer, 0, 30);
			//My turn;
			printf("\nSystem : This is your turn.\n");
			HitTurn:
			printf("[1] 랜덤 공격 (random damage 1~150)\n");
			printf("[2] 몸통박치기 (fixed damage 80)\n");
			printf("[3] 대지진 (random damage 30~50)\n");
			printf("[4] 로또 공격 (fixed damage 100 or 0)\n");
			fflush(stdin);
			scanf("%c", &userInput);
			memset(sendBuffer, 0, 30);

			if (userInput == '1')
				itoa((rand() % 150) + 1, tempBuffer, 10);
			else if (userInput == '2')
				itoa(80, tempBuffer, 10);
			else if (userInput == '3')
				itoa((rand() % 20) + 30, tempBuffer, 10);
			else if (userInput == '4')
				if (rand() % 2 == 0)
					itoa(100, tempBuffer, 10);
				else
					itoa(0, tempBuffer, 10);
			else
				   goto HitTurn;//

			strcat(hitPokemon, tempBuffer);
			strcpy(sendBuffer, tempBuffer);
			send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);

			printf("System : %s shown %s damage\n", Dawn->head->name , tempBuffer);

			//Enemy turn;
			printf("\nSystem : This is enemy's turn.\n");
			memset(readBuffer, 0, 30);
			recv(pokemonClient, readBuffer, 30, 0);
			//printf("####### %s ##########\n", readBuffer);//

			if (!strcmp(readBuffer,stringDie)) {
				printf("System : enemy is dead !! \n");
				memset(readBuffer, 0, 30);
				recv(pokemonClient, readBuffer, 30, 0);
				readBuffer[strlen(readBuffer)] = '\0';
				printf("%s : 가랏 !!! %s몬\n", remoteUserID, readBuffer);
				memset(readBuffer, 0, 30);
				recv(pokemonClient, readBuffer, 30, 0);
				readBuffer[strlen(readBuffer)] = '\0';
			}
			else if (!strcmp(readBuffer, stringEnd)) {
				printf("The game is End...\n");
				printf("The winner is you !!\n");
				closesocket(pokemonClient);
				closesocket(pokemonServer);
				MessageBox(NULL, TEXT("니가 이김!! ~~"), TEXT("WinApi kkkk"), MB_OK);
				getch();
				exit(0);
			}
			readBuffer[strlen(readBuffer)] = '\0';
			if (damagePokemon(Dawn, atoi(readBuffer))) {
				printf("System : your pokemon's remain health : %d\n", Dawn->head->health);
			}
			else {
				if (dieCount == 5) {
					memset(sendBuffer, 0, 30);
					strcpy(sendBuffer, stringEnd);
					send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
					printf("The game is End...\n");
					printf("The winner is enemy !!\n");
					closesocket(pokemonClient);
					closesocket(pokemonServer);
					MessageBox(NULL, TEXT("니가 짐!! ~~"), TEXT("WinApi kkkk"), MB_OK);
					getch();
					exit(0);
				}
				dieCount++;
				// 1x 2x 3x 4x 5x 6
				printf("System : Your pokemon is dead .....\n");
				printf("%s : 가랏 !!! %s몬\n", userID, Dawn->head->name);
				memset(sendBuffer, 0, 30);
				strcpy(sendBuffer, stringDie);
				send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
				//printf("~~~~~~%s~~~~~\n", sendBuffer);
				Sleep(1000);
				memset(sendBuffer, 0, 30);
				strcpy(sendBuffer,  Dawn->head->name);
				send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
				//printf("~~~~~~%s~~~~~\n", sendBuffer);
			}
		}
	}
	else if (MakeOrJoin == '2') { // 클라이언트 부분
		WSADATA wsa;
		SOCKET pokemonClient = socket(AF_INET, SOCK_STREAM, 0);
		SOCKADDR_IN serverAddr;
		SOCKADDR_IN clientAddr;
		char serverIP[30];
		int serverPort;

		printf("Input => IP PORT ");
		scanf("%s %d", serverIP, &serverPort);

		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
			socketError();
		}
		if ((pokemonClient = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			socketError();
		}

		memset((void *)&serverAddr, 0x00, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr(serverIP);
		serverAddr.sin_port = htons(serverPort);

		if (connect(pokemonClient, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			socketError();
		}

		printf("서버와 연결됨 !!\n");
		strcpy(sendBuffer, userID);
		send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
		memset(readBuffer, 0, 30);
		checkRead = recv(pokemonClient, readBuffer, 30, 0);
		memset(remoteUserID, 0, 30);
		strcpy(remoteUserID, readBuffer);

		if (checkRead <= 0)
			socketError();

		printf("상대는 ~ !! %s 입니다!!\n", readBuffer);
		
		//Sleep(600);
		system("cls");

		//First Send, Second Recv


		nowHealth = Dawn->head->health;
		strcpy(nowPokemon, Dawn->head->name);

		memset(sendBuffer, 0, 30);
		strcpy(sendBuffer, Dawn->head->name);
		send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
		printf("%s : 가랏 !!! %s몬\n", userID, Dawn->head->name);
		recv(pokemonClient, readBuffer, 30, 0);
		readBuffer[strlen(readBuffer)] = '\0';
		printf("%s : 가랏 !!! %s몬\n", remoteUserID, readBuffer);


		while (1) {
			memset(readBuffer, 0, 30);
			memset(sendBuffer, 0, 30);
			memset(tempBuffer, 0, 30);
			//Enemy turn;
			printf("\nSystem : This is enemy's turn.\n");
			memset(readBuffer, 0, 30);
			recv(pokemonClient, readBuffer, 30, 0);
			
			//printf("####### %s ##########\n", readBuffer);
			if (!strcmp(readBuffer, stringDie)) {
				//printf("DIEDIEDIEDIEIDEIDIEIEI\n");
				printf("System : enemy is dead !! \n");
				memset(readBuffer, 0, 30);
				recv(pokemonClient, readBuffer, 30, 0);
				readBuffer[strlen(readBuffer)] = '\0';
				printf("%s : 가랏 !!! %s몬\n", remoteUserID, readBuffer);
				recv(pokemonClient, readBuffer, 30, 0);
				readBuffer[strlen(readBuffer)] = '\0';
			}
			else if (!strcmp(readBuffer, stringEnd)) {
				printf("The game is End...\n");
				printf("The winner is you !!\n");
				closesocket(pokemonClient);
				MessageBox(NULL, TEXT("니가 이김!! ~~"), TEXT("WinApi kkkk"), MB_OK);
				getch();
				exit(0);
			}
			readBuffer[strlen(readBuffer)] = '\0';
			if (damagePokemon(Dawn, atoi(readBuffer))) {
				printf("System : your pokemon's remain health : %d\n", Dawn->head->health);
			}
			else {
				if (dieCount == 5) {
					memset(sendBuffer, 0, 30);
					strcpy(sendBuffer, stringEnd);
					send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
					printf("The game is End...\n");
					printf("The winner is enemy !!\n");
					closesocket(pokemonClient);
					getch();
					MessageBox(NULL, TEXT("니가 짐!! ~~"), TEXT("WinApi kkkk"), MB_OK);
					exit(0);
				}
				dieCount++;
				// 1x 2x 3x 4x 5x 6
				printf("System : Your pokemon is dead .....\n");
				printf("%s : 가랏 !!! %s몬\n", userID, Dawn->head->name);
				memset(sendBuffer, 0, 30);
				strcpy(sendBuffer, stringDie);
				send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
				//printf("~~~~~~%s~~~~~\n", sendBuffer);
				Sleep(1000);
				memset(sendBuffer, 0, 30);
				strcpy(sendBuffer, Dawn->head->name);
				send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
				//printf("~~~~~~%s~~~~~\n", sendBuffer);
			}
			//My turn;
			printf("\nSystem : This is your turn.\n");
		HitTurn2:
			printf("[1] 랜덤 공격 (random damage 1~150)\n");
			printf("[2] 몸통박치기 (fixed damage 80)\n");
			printf("[3] 대지진 (random damage 30~50)\n");
			printf("[4] 로또 공격 (fixed damage 100 or 0)\n");
			fflush(stdin);
			scanf("%c", &userInput);
			memset(sendBuffer, 0, 30);

			if (userInput == '1')
				itoa((rand() % 150) + 1, tempBuffer, 10);
			else if (userInput == '2')
				itoa(80, tempBuffer, 10);
			else if (userInput == '3')
				itoa((rand() % 20) + 30, tempBuffer, 10);
			else if (userInput == '4')
			if (rand() % 2 == 0)
				itoa(100, tempBuffer, 10);
			else
				itoa(0, tempBuffer, 10);
			else
				goto HitTurn2;

			strcat(hitPokemon, tempBuffer);
			strcpy(sendBuffer, tempBuffer);
			send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);

			printf("System : %s shown %s damage\n", Dawn->head->name, tempBuffer);


		}
	}

	fclose(file2);
	getch();
	WSACleanup();
	return 1;
}