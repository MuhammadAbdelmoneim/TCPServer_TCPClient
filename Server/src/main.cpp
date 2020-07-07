#include <stdio.h>
#include <sys/types.h>  // Define Data Types used in System Calls
#include <sys/socket.h> // Define Number of definitions of structures needed in Socket programming
#include <netinet/in.h> // Contains Constants and Structures needed for internet domain address
#include <stdlib.h>     // Defines four data types & several function
#include <iostream>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Port Number not provided" << std::endl;
        return 0;
    }
    // Data types definitions
    int sockFd;
    int newSockFd;
    int portNo;
    char request[256U];
    char response[256U];
    int numberOfReceivedBytes = 0U;
    int numberOfSentBytes = 0U;

    /* Structure describing an Internet socket address.  */
    struct sockaddr_in servAdd, clientAdd;
    socklen_t client; /* clientAdd var size */

    // Create Socket
    //               IPV4   ,   TCP       , TCP default
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0)
    {
        //failed
        std::cout << "Can't create socket" << std::endl;
        return 0;
    }

    //get the port number
    portNo = atoi(argv[1]);
    //Set server IP family addresses to IPV4
    servAdd.sin_family = AF_INET;

    servAdd.sin_addr.s_addr = INADDR_ANY; /* The socket accepts connections to all the IPs of the machine */
                                          /* you don't need to bind a socket to a specific IP */

    servAdd.sin_port = htons(portNo); /* Host to Network Short */

    //Bind the server
    if (bind(sockFd, reinterpret_cast<struct sockaddr *>(&servAdd), sizeof(servAdd)) < 0)
    {
        //binding failed
        std::cout << "Binding failed" << std::endl;
    }

    // Listen for clients
    listen(sockFd, 5);
    client = sizeof(clientAdd);

    // accept a connection
    newSockFd = accept(sockFd, reinterpret_cast<struct sockaddr *>(&servAdd), &client);

    if (newSockFd < 0)
    {
        //Failed to accept
        std::cout << "Failed to accept the client request" << std::endl;
        return 0;
    }

    while (1)
    {
        std::cout << "Server Connected \n";
        numberOfReceivedBytes = read(newSockFd, request, sizeof(request));
        if (numberOfReceivedBytes < 0)
        {
            //Failed to read any thing
            std::cout << "Failed to read" << std::endl;
            return 0;
        }
        else
        {
            if (strcmp(request, "done") == 0)
            {
                //Terminate the session
                close(newSockFd);
                close(sockFd);
            }
            std::cout << "Client : " << request << std::endl;
            fgets(response, sizeof(response), stdin);
            numberOfSentBytes = write(newSockFd, response, strlen(response));
            if (numberOfSentBytes < 0)
            {
                //Failed to sent bytes
                std::cout << "Failed to send response" << std::endl;
            }
        }
    }

    return 0;
}