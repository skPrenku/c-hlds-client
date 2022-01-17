// udpClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib,"ws2_32.lib")

using namespace std;
typedef unsigned char BYTE;
int main(int argc,char *argv[])
{
	string serverIP;
	string password;
	cout << "==============================================" << endl;
	cout << "HL Server IP:";
	cin >> serverIP;


	WSADATA ws;

	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
	{
		cout << "Error initializing winsock..";
		WSACleanup();
		exit(1);
	}

	//create hint for server
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(27015);

	char csip[sizeof(serverIP) / sizeof(serverIP[0])];
	strcpy(csip, serverIP.c_str());
	inet_pton(AF_INET, csip, &server.sin_addr);

	//socket 
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

// since the hlds accepts 1byte[8bits] we have to convert our text to 1byte, Line 44 is the same as line 46 & 47
// but u see alot of complicated examples of converting simple text to byte[] 
	char send[256] = { 255,255,255,255, ' ','c','h','a','l','l','e','n','g',' e',' ','r','c','o','n'};
  
	//char send[256]
	//sprintf(send, "\xFF\xFF\xFF\xFF challenge rcon\n");

// from down here u'll see using sprintf() wich seems much simpler and cleaner . I used the char Array just to have a better understanding of the data.
	int srvLength = sizeof(server);

	//write out thru socket
	int sendOK = sendto(out, send, sizeof(send), 0, (sockaddr*)&server, sizeof(server));
	if (sendOK == SOCKET_ERROR) {
		cout << "Sending didnt work";
		exit(1);
	}

	char buf[1024];
	//char challenge[20];


	ZeroMemory(buf, 1024);
	int recive = recv(out, buf, 1024, 0);
	cout << buf << " "<< endl;
	cout << "----------------" << endl;
	cout << argv[0] << endl;

	string s = ""; // string to store challenge code

	for (int i = 19; i < 29;i++)
	{
		cout << buf[i] << endl;
			s.push_back(buf[i]);


	}

	int n = s.length();
	char dt[sizeof(s)/sizeof(s[0])];
	strcpy(dt, s.c_str());

	
	ZeroMemory(send, 256);
	sprintf(send, "\xFF\xFF\xFF\xFF rcon \"%s\" yourrconpassword sv_restart" ,dt);
	cout << send << endl;
	cout << "-----------------" << endl;
	sendOK = sendto(out, send, sizeof(send), 0, (sockaddr*)&server, sizeof(server));
	recive = recv(out, buf, 1024, 0);
	cout << buf << " " << endl;


	closesocket(out);
	WSACleanup();

	return 0;
}


