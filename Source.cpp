#include<stdio.h>
#include <string>
#include <iostream>
#include<winsock2.h>
#include "Ws2tcpip.h"
#include "commands.h"
#include <time.h>

#include <map>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

//#define SERVER "10.0.0.245"  //ip address of udp server
#define SERVER "192.168.0.101"  //ip address of udp server
#define BUFLEN 2041  //Max length of buffer
#define PORT 14551   //The port on which to listen for incoming data

using std::to_string;		using std::cout;
using std::cin;				using std::endl;

int main(void)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	std::string msg;
	WSADATA wsa;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 10000;
	bool isExit = false;
	bool lastCmdSet = false;
	int lastCmd;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) < 0)
	{
		printf("set socket options failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	/*si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);*/
	InetPton(AF_INET, SERVER, &si_other.sin_addr.S_un.S_addr);

	int command = 0;

	//start communication
	while (1)
	{
		/*commented out for drone message
		printf("Enter message : ");
		std::cin >> msg;*/

		if (!isExit)
		{
			command = getCommandKeyPress(isExit);
		}
		if (isExit && !lastCmdSet)
		{
			lastCmd = command;
		}

		msg = std::to_string(command);
		if (sendto(s, msg.c_str(), msg.size(), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		/*commented out for drone specific messages
		//send the message
		if (sendto(s, msg.c_str(), msg.size(), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}*/

		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		if (isExit && (std::stoi(std::string(buf)) == lastCmd))
		{
			break;
		}

		puts(buf);
		command = 0;
		Sleep(100);
		
		
	}

	closesocket(s);
	WSACleanup();

	/*char c;
	std::cin >> c;*/
	return 0;
}