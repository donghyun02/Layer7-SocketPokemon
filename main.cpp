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
	char namesPokemon[20][20] = { { "ġ�ڸ�Ÿ" }, { "���α�" }, { "��ī��" }, { "�׾�ŷ" }, { "�뵵��" }, { "���̸�" }, { "�嵿��" }, { "������" }, { "�赿��" }, { "���ؿ�" }, { "��ǻ��" } };
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

int socketError() {
	printf("���� ���� ���� !!\n���α׷��� �����մϴ� �Ф�\n");
	return 0;
}

int main() {
	char userID[20], userPW[20], fileUserID[20], fileUserPW[20];
	char readBuffer[30],sendBuffer[30]="���ؿ�";
	int port, sizeClientAddr;;
	char LoginOrJoin, MakeOrJoin;
	int checkFlag = 0, checkUserDuplicate = 0, checkRead = 0, line;
	int i;

	line = countLine();

	while (checkFlag == 0) {
	startPart:

		printf("[1] �α���\n");
		printf("[2] ȸ������\n");
		printf("=> ");
		fflush(stdin);
		scanf("%c", &LoginOrJoin);
		if (!(LoginOrJoin == '1' || LoginOrJoin == '2'))
			goto startPart;
		printf("Input Type -> ID PW\nex)junoim a12345\n");
		printf("=> ");
		scanf("%s %s", &userID, &userPW);

		if (LoginOrJoin == '1' && isLoginSuccess(userID,userPW,line)) {
			printf("�α��� ���� !! \n");
			checkFlag = 1;
			//Sleep(1000);
			system("cls");
			printf("�� �¸� : %d �� �й� : %d\n", countWin, countLose);
		}
		else if (LoginOrJoin == '2' && isNotDuplicateID(userID, line)) {
			file = fopen("user.txt", "a");
			fprintf(file, "\n%s %s 0 0", userID, userPW);
			fclose(file);
			++line;
			printf("ȸ�� ���� ���� !!\n");
			printf("�����Ͻ� ���̵�� �н����� : %s %s\n", userID, userPW);
		}
		else {
			printf("���� !! \n");
		}
	}

	orderPokemon* Dawn = (orderPokemon*)malloc(sizeof(orderPokemon));

	printf("****** ���ϸ� ����Ʈ �ʱ�ȭ�� ... ******\n");

	initPokemon(Dawn);
	//Sleep(1000);

	makePokemon(Dawn);

	printf("****** ���ϸ� ������ ... ******\n");
	//Sleep(1500);
	printf("****** ���ϸ� �����Ϸ� !!! ******\n");
	printf("****** ����� 6���� ���ϸ��Դϴ�. ******\n");

	printAllPokemon(Dawn);

	printf("****** ��ų �������Դϴ�.. ******\n");
	printf("****** ��ų �������� !!! ******\n");
	printf("[Notice] : ����� �÷��̽ð��� 1��ð� �̸��̹Ƿ�\n��ų�� �����ġ�� �ۿ� ������ �����ʽ��ϴ�.\n");
	printf("�����̾����� ���׷��̵�� ��밡���մϴ�. ���� ��� \\9,912,323,200��\n");
	printf("352 0273 9941 13 ���� ���ؿ�\n");

	printf("\n[1] ������ϱ�\n");
	printf("[2] �������ϱ�\n");
	printf("=> ");
	fflush(stdin);
	scanf("%c", &MakeOrJoin);

	SOCKET pokemonClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKET pokemonServer = socket(AF_INET, SOCK_STREAM, 0);
	WSADATA wsa;
	SOCKADDR_IN serverAddr;
	SOCKADDR_IN clientAddr;

	if (MakeOrJoin == '1') {
		printf("Input Port Number ! => ");
		scanf("%d", &port);
		WSADATA wsa; // init Winsock

		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
			socketError();
		}

		if (pokemonServer == INVALID_SOCKET || pokemonClient == INVALID_SOCKET)
			socketError();

		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(port);

		int status;

		status = bind(pokemonServer, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr_in));
		if (status == SOCKET_ERROR)
		{
			printf("Bind Error\n");
			return 0;
		}
		if (SOCKET_ERROR == listen(pokemonServer, 5))
		{
			printf("listen Error\n");
			return 0;
		}
		printf("���� ����� .. ��Ʈ : %d\n", port);

		while (1) {
			sizeClientAddr = sizeof(clientAddr);
			pokemonClient = accept(pokemonServer, (SOCKADDR*)&clientAddr, &sizeClientAddr);

			if (pokemonClient == INVALID_SOCKET) {
				closesocket(pokemonClient);
				closesocket(pokemonServer);
				socketError();
			}

			printf("Ŭ����Ʈ�� ����� !!\n");
			checkRead = recv(pokemonClient, readBuffer, 30, 0);
			readBuffer[checkRead-1] = NULL;
			printf("���� ~!! %s !! �ʰ���Դϴ� ~\n", readBuffer);
			if (checkRead > 0) {
				strcpy(sendBuffer, userID);
				send(pokemonClient, sendBuffer, sizeof(sendBuffer), 0); // �ٽ� �г��� ������
			}
			else {
				printf("Error !!!!!!!!!!!\n");
				getch();
				socketError();
			}
			checkRead = recv(pokemonClient, readBuffer, 30, 0);
			readBuffer[checkRead] = NULL;
			printf("%s", readBuffer);
		}
		closesocket(pokemonClient);
		closesocket(pokemonServer);
	}
	else if (MakeOrJoin == '2') { // Ŭ���̾�Ʈ �κ�
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

		send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);

		while (1)
		{
			printf("> ");
			fgets(sendBuffer, 29, stdin);
			if (strncmp(sendBuffer, "quit\n", 5) == 0)
			{
				break;
			}
			send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
			memset(readBuffer, 0x00, 30);
			recv(pokemonClient, readBuffer, 30, 0);
			printf("s -> %s\n", readBuffer);
		}

		closesocket(pokemonClient);
	}

	//�������� �ް� ���� �޴°� �������� ���� !!
	send(pokemonClient, sendBuffer, strlen(sendBuffer), 0);
	memset(readBuffer, 0x00, 30);
	recv(pokemonClient, readBuffer, 30, 0);
	send(pokemonClient, sendBuffer, sizeof(sendBuffer), 0); // �ٽ� �г��� ������
	checkRead = recv(pokemonClient, readBuffer, 30, 0);

	WSACleanup();
	return 1;
}