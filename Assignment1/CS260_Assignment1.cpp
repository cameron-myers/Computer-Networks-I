#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

int main(int argc, char* argv[])
{
    WSADATA wsa_data;
    int res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if(res != 0)
    {
        std::cerr << "Error in WSAStartup: " << WSAGetLastError() << std::endl;
        return 1;
    }

    SOCKET udp_socket = socket(AF_INET, SOCK_DGRAM, 0 );
    if(udp_socket == INVALID_SOCKET)
    {
        std::cerr << "failed to create the socket" << WSAGetLastError() << std::endl;
        return 1;
    }
    //52.12.14.56
    //8888
    sockaddr_in myAddr;
    memset(&myAddr.sin_zero, 0, 8);
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(8888); //port 8888

    res = inet_pton(AF_INET, "52.12.14.56", &myAddr.sin_addr);
    if(res != 1)
    {
        std::cerr << "inet_pton failed " << WSAGetLastError() << std::endl;
        return 1;
    }

    int bytes_sent = sendto(udp_socket, argv[0], (int)strlen(argv[0]), 0, (const sockaddr*)&myAddr, (int)sizeof(myAddr));

	//receive

    int bufferSize = 1500;
    char* recv_buff = new char[bufferSize];
    memset(recv_buff, 0, bufferSize);


    res = recvfrom(udp_socket, recv_buff, bufferSize - 1, 0,NULL,NULL);
    if (res == SOCKET_ERROR)
    {
        std::cerr << "recvfrom failed " << WSAGetLastError() << std::endl;
        return 1;
    }

    std::cout << recv_buff << std::endl;

    delete[] recv_buff;

    res = closesocket(udp_socket);
    if(res != 0)
    {
        std::cerr << "closesocket failed " << WSAGetLastError() << std::endl;
        return 1;
    }

    res = WSACleanup();
    if(res != 0)
    {
        std::cerr << "WSACleanup failed" << WSAGetLastError() << std::endl;
    }

    return 0;
}

