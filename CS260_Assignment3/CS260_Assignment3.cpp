// Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment( lib, "WS2_32.lib" )

#include <iostream>
#include <chrono>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>

std::string parse_host(std::string in_buff)
{
    
    size_t front = in_buff.find_first_of("\r\nHost: ");
    if (front != 0)
    {
        size_t end = in_buff.find_first_of("\r\n", front);
        return in_buff.substr(front, end - front);
    }
    else return nullptr;
}



int main(int argc, char* argv[])
{
    u_long iMode = 1;

    //INIT WINSOCK/////////////////////////////////
    WSADATA wsa_data;
    int res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (res != 0)
    {
        std::cerr << "Error in WSAStartup: " << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////






    //CREATE ADDRESS///////////////////////////////
    //52.12.14.56
    //8888
    sockaddr_in myAddr = sockaddr_in();
    memset(&myAddr.sin_zero, 0, 8);
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    myAddr.sin_port = htons(atoi(argv[1])); //port 8888


    /*if (res != 1)
    {
        std::cerr << "inet_pton failed " << WSAGetLastError() << std::endl;
        return 1;
    }*/
    ///////////////////////////////////////////////

    //CREATE SOCKET.///////////////////////////////
    SOCKET tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == INVALID_SOCKET)
    {
        std::cerr << "failed to create the socket" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

    //CONNECT TO SERVER////////////////////////////
    /*res = connect(tcp_socket, (const sockaddr*)&myAddr, (int)sizeof(myAddr));
    if (res != 0)
    {
        std::cerr << "failed to connect to the server" << WSAGetLastError() << std::endl;
        return 1;
    }*/

    ///////////////////////////////////////////////


    //SET SOCKET TO NON-BLOCKING///////////////////
    res = ioctlsocket(tcp_socket, FIONBIO, &iMode);
    if (res != 0)
    {
        std::cerr << "failed to set to non-blocking mode" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

     //BIND THE SOCKET///////////////////
    res = bind(tcp_socket, (const sockaddr*)&myAddr, sizeof(myAddr));
    if (res != 0)
    {
        std::cerr << "failed to bind the socket" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

	//LISTEN ON THE SOCKET///////////////////
    res = listen(tcp_socket, SOMAXCONN);
    if (res != 0)
    {
        std::cerr << "failed to listen on socket" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

    //SEND DATA////////////////////////////////////
    /*int bytes_sent = sendto(tcp_socket, argv[0], (int)strlen(argv[0]), 0, (const sockaddr*)&myAddr, (int)sizeof(myAddr));
    if (res != 0)
    {
        std::cerr << "failed to send data to the server" << WSAGetLastError() << std::endl;
        return 1;
    }*/
    ///////////////////////////////////////////////

    //DONE SENDING/////////////////////////////////
    /*res = shutdown(tcp_socket, SD_SEND);
    if (res != 0)
    {
        std::cerr << "failed to send shutdown signal" << WSAGetLastError() << std::endl;
        return 1;
    }*/

    //RECEIVE//////////////////////////////////////
    int bufferSize = 1500;
    std::string output;
    char* recv_buff = new char[bufferSize];
    memset(recv_buff, 0, bufferSize);


	char* client_addr_buff;
    int client_addr_size = 0;
    sockaddr client_addr;

    while (true)
    {
        SOCKET client = accept(tcp_socket, &client_addr, &client_addr_size);

        if(client)
        {
            /*res = recvfrom(client, recv_buff, bufferSize, 0, NULL, NULL);*/
            while (true)
            {
                res = recvfrom(tcp_socket, recv_buff, bufferSize, 0, NULL, NULL);
                output += recv_buff;
                //clear out buffer for next recv
                memset(recv_buff, 0, bufferSize);
                //check for end
                if (res == 0) break;

                //catch possible errors
                if (res == SOCKET_ERROR)
                {
                    //check for non fatal errors
                    if (WSAGetLastError() != 10035)
                    {
                        std::cerr << "recvfrom failed " << WSAGetLastError() << std::endl;
                        return 1;
                    }
                }
                /*//wait for next recv
                std::this_thread::sleep_for(std::chrono::milliseconds(100));*/
            }
            shutdown(client, SD_RECEIVE);
            const char* host = parse_host(std::string(recv_buff)).c_str();

            //TODO make this thread take in args and output value
        	std::thread resolver = std::thread(inet_pton(AF_INET, host, &client_addr));
            resolver.detach();

            //TODO fallback if pton doesn't work
        	if(res != 0)
            {
	            //getaddrinfo(host, myAddr.sin_port, )
            }

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    std::cout << output << std::endl;
    delete[] recv_buff;

    ///////////////////////////////////////////////


    //CLOSE SOCKET/////////////////////////////////

    res = closesocket(tcp_socket);
    if (res != 0)
    {
        std::cerr << "closesocket failed " << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

    //CLEANUP /////////////////////////////////////

    res = WSACleanup();
    if (res != 0)
    {
        std::cerr << "WSACleanup failed" << WSAGetLastError() << std::endl;
    }

    return 0;
    ///////////////////////////////////////////////

}
