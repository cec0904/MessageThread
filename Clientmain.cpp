#include <iostream>
#include <winsock2.h>
#include <thread>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

using namespace std;


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PACKET_SIZE 1024

SOCKET skt;

void proc_recv()
{


	char buffer[PACKET_SIZE] = {};
	string cmd;
	while (!WSAGetLastError())
	{
		time_t timer = time(NULL);
		struct tm* t;
		timer = time(NULL);
		t = localtime(&timer);

		ZeroMemory(&buffer, PACKET_SIZE);
		recv(skt, buffer, PACKET_SIZE, 0);
		cmd = buffer;
		if (cmd == "hi")
		{
			break;
		}
		cout << "상대 : " << buffer << " - " << t->tm_hour << "시 : " << t->tm_min << "분 : " << t->tm_sec << "초" << endl;
	}
}

int main()
{
	

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN sAddr = {};
	sAddr.sin_family = AF_INET;
	// sAddr.sin_addr.s_addr = inet_pton(AF_INET, "192.168.10.3", &sAddr.sin_addr);
	sAddr.sin_addr.s_addr = inet_addr("192.168.10.3");
	sAddr.sin_port = htons(7777);

	connect(skt, (SOCKADDR*)&sAddr, sizeof(sAddr));
	cout << "Server와 연결되었습니다." << '\n';


	thread proc1(proc_recv);
	char buffer[PACKET_SIZE] = { 0 };


	while (!WSAGetLastError())
	{
		//cout << "채팅을 입력해주세요 : ";
		cin >> buffer;
		send(skt, buffer, strlen(buffer), 0);
	}

	proc1.join();
	closesocket(skt);
	WSACleanup();

}
