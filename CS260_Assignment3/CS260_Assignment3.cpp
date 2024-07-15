// Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment( lib, "WS2_32.lib" )

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <winsock2.h>
#include <WS2tcpip.h>


/**
 * \brief Parses a host address from the received HTTP Request
 * \param in_buff string containing the HTTP Request
 * \return Host address as a string
 */
std::string parse_host(std::string in_buff)
{
    
    size_t front = in_buff.find("\r\nHost: ") + sizeof("\r\nHost:");
    if (front != 0)
    {
        size_t end = in_buff.find_first_of("\r\n", front);
        std::string temp = in_buff.substr(front, end - front);
        return temp;
    }

    return std::string();
}

/**
 * \brief Resolves an address with inet_pton given a host but falls back to DNS
 * \param host string representation of the address
 * \param destination sockaddr struct to fill with resolved addr
 * \return result of the function
 */
int resolve_ip(const char* host, sockaddr_in* destination)
{
    int res = inet_pton(AF_INET, host, &((*destination).sin_addr));

    //fallback to getaddrinfo (DNS Resquest)
    if (res == 0 )
    {
        PADDRINFOA dest = PADDRINFOA();
        struct addrinfo hints = addrinfo();
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        res = getaddrinfo(host, "80", &hints, &dest);
        if (res != 0)
        {
            std::cerr << "failed to getaddrinfo" << WSAGetLastError() << std::endl;
            return 0;
        }
        //copy result into output
        *destination = *(sockaddr_in*)dest->ai_addr;
        return 1;
    }

    return res;
}

/**
 * \brief Sends request to webserver and repeats response immediately to the client
 * \param host addr for the function to resolve
 * \param client Client socket to relay to
 * \param data_string Request to send to the webserver
 * \return result of function
 */
int repeater(std::string host, SOCKET client, const std::string data_string)
{
    int res;
    const char* data = data_string.c_str();

    sockaddr_in destination = sockaddr_in();
    memset(&destination.sin_zero, 0, 8);
    destination.sin_family = AF_INET;
    destination.sin_port = htons(80); //port 80


    res = resolve_ip(host.c_str(), &destination);
	if(res != 1)
	{
        std::cerr << "failed to resolve the host" << WSAGetLastError() << std::endl;
        return 1;
	}

    //CREATE SOCKET.///////////////////////////////
    SOCKET webserver_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (webserver_socket == INVALID_SOCKET)
    {
        std::cerr << "failed to create the relay socket" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

    //CONNECT TO WEBSERVER////////////////////////////
    res = connect(webserver_socket, (const sockaddr*)&destination, (int)sizeof(destination));
    if (res != 0)
    {
        std::cerr << "failed to connect to the server" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////
    
	//SEND DATA////////////////////////////////////
    int bytes_sent = sendto(webserver_socket, data, (int)strlen(data), 0, NULL, NULL);
    if (res != 0)
    {
        std::cerr << "failed to send data to the server" << WSAGetLastError() << std::endl;
        return 1;
    }

    ///receive once from webserver
    int bufferSize = 1500;
    std::string output;
    char* recv_buff = new char[bufferSize];

    while (true)
    {
        memset(recv_buff, 0, bufferSize);
        res = recvfrom(webserver_socket, recv_buff, bufferSize, 0, NULL, NULL);
        bytes_sent = sendto(client, recv_buff, (int)strlen(recv_buff), 0, NULL, NULL);
        if (bytes_sent != strlen(recv_buff))
        {
            std::cerr << "failed to send correct amount of data to the server" << std::endl;
        }

        //check for end
        if (res == 0)
        {

            shutdown(webserver_socket, SD_BOTH);
            shutdown(client, SD_SEND);
            closesocket(webserver_socket);
            closesocket(client);
            break;
        }
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
    }

    return 0;
}

/**
 * \brief handle http request received from the client
 * \param client socket the request is received from
 * \return result of function
 */
int handle_request(SOCKET client)
{
    int res;
    int bufferSize = 1500;
    std::string output;
    char* recv_buff = new char[bufferSize];
    memset(recv_buff, 0, bufferSize);

    while(true)
    {
        memset(recv_buff, 0, bufferSize);
        res = recvfrom(client , recv_buff, bufferSize, 0, NULL, NULL);
        output += recv_buff;

        //check for end
        if (res == 0)
        {
            break;
        }
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
    }

    shutdown(client, SD_RECEIVE);
	std::string host = parse_host(output) + "\0" ;
    repeater(host, client, output);

    return 0;
}


int main(int argc, char* argv[])
{
    u_long iMode = 1;
    //BOOT CHECK/////////////////////////////////
    if (atoi(argv[1]) < 1 || atoi(argv[1]) > 65535)
    {
        std::cout << "INPUT INVALID: please run the program again with a valid port greater than 1 and less than 65535" << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

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
    sockaddr_in listen_addr = sockaddr_in();
    memset(&listen_addr.sin_zero, 0, 8);
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    listen_addr.sin_port = htons(atoi(argv[1])); 
    ///////////////////////////////////////////////

    //CREATE SOCKET.///////////////////////////////
    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == INVALID_SOCKET)
    {
        std::cerr << "failed to create the socket" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

    //SET SOCKET TO NON-BLOCKING///////////////////
    res = ioctlsocket(listen_socket, FIONBIO, &iMode);
    if (res != 0)
    {
        std::cerr << "failed to set to non-blocking mode" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

     //BIND THE SOCKET///////////////////
    res = bind(listen_socket, (sockaddr*)&listen_addr, sizeof(listen_addr));
    if (res != 0)
    {
        std::cerr << "failed to bind the socket" << WSAGetLastError() << std::endl;
        return 1;
    }
    ///////////////////////////////////////////////

	//LISTEN ON THE SOCKET///////////////////
    res = listen(listen_socket, SOMAXCONN);
    if (res != 0)
    {
        std::cerr << "failed to listen on socket" << WSAGetLastError() << std::endl;
        return 1;
    }
    std::cout << "Listening on port: " << argv[1] << std::endl;
    ///////////////////////////////////////////////

    //RECEIVE//////////////////////////////////////
    while (true)
    {

        SOCKET client = accept(listen_socket, NULL, NULL);

        if(client == INVALID_SOCKET)
        {
            res = WSAGetLastError();
            if (res != WSAEWOULDBLOCK)
            {
                std::cerr << "accept failed" << res << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;

        }
        //make sure still not blocking
        ioctlsocket(client, FIONBIO, &iMode);
        //handle new request on another thread
        std::thread handler = std::thread(handle_request, client);
        handler.detach();
        
    }
}