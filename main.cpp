#include <iostream>
#include <windows.h>
#include <winsock.h>
#include <thread>
#include <ctime>
#include <cstdio>

#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PACKET_SIZE 1024
using namespace std;

SOCKET serverSKT;
SOCKET clientSKT;


void proc_recvs()
{
	char buffer[PACKET_SIZE] = { 0 };
	while (!WSAGetLastError())
	{
		time_t timer = time(NULL);
		struct tm* t;
		timer = time(NULL);
		t = localtime(&timer);

		ZeroMemory(&buffer, PACKET_SIZE);
		recv(clientSKT, buffer, PACKET_SIZE, 0);
		cout << "상대 : " << buffer << " - " << t->tm_hour << "시 : " << t->tm_min << "분 : " << t->tm_sec << "초" << endl;
	}

}

int main()
{
	cout << "[Server]" << '\n';
	// WSADATA winsock.h 에 있는 자료형
	WSADATA wsa;

	// WSADATA 초기화
	WSAStartup(MAKEWORD(2, 2), &wsa);


	// socket(통신, 통신유형, 프로토콜)
	// IPv4 를 이용
	// 연결 지향적인 통신 유형
	// TCP 프로토콜을 사용
	serverSKT = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN sAddr = {};								// 소켓 정보담기
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = htonl(INADDR_ANY);		// 소켓의 ip를 입력 (예 : 111.111.111.111, 여기서는 내 pc라서 저렇게)
	sAddr.sin_port = htons(7777);						// 소켓의 port 번호를 입력

	// sAddr에 저장해 놓은 IP, port 번호를 소켓에 묶는 역할
	bind(serverSKT, (SOCKADDR*)&sAddr, sizeof(sAddr));
	// 서버 소켓을 수동모드로 설정해서 클라이언트가 연결하기를 기다리기, SOMAXCONN은 대기열의 최대 길이
	listen(serverSKT, SOMAXCONN);


	SOCKADDR_IN cAddr = {};
	int cSize = sizeof(cAddr);
	ZeroMemory(&cAddr, cSize);
	clientSKT = accept(serverSKT, (SOCKADDR*)&cAddr, &cSize);
	cout << "클라이언트가 입장하였습니다." << '\n';


	char buffer[PACKET_SIZE] = { 0 };
	thread proc2(proc_recvs);

	while (!WSAGetLastError())
	{
		//cout << "채팅을 입력해주세요 : ";
		cin >> buffer;
		send(clientSKT, buffer, strlen(buffer), 0);
		//cout << "나 : " << buffer << "-" << t->tm_hour << " : " << t->tm_min << " : " << t->tm_sec << endl;
	}


	proc2.join();


	closesocket(clientSKT);
	closesocket(serverSKT);
	WSACleanup();
}