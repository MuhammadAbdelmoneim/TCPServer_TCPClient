// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h> /* for read , write , close functions */
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h> /* for hostent structure (hold information about host) */
// #include <string.h>

// #include <iostream>

// int main(int argc, char *argv[])
// {
//     /* Data types definitions */
//     int sockFd;
//     int portNo;
//     int receivedBytes = 0;
//     int sentBytes = 0;
//     struct sockaddr_in servAdd;
//     struct hostent *server;
//     char request[256];
//     char response[256];
//     int retVal = 0;
//     if (argc < 3)
//     {
//         std::cout << " must enter host name and port number  " << argv[0] << std::endl;
//         retVal = 1;

//     }
//     else
//     {
//         /* Extract port number form cmd */
//         portNo = atoi(argv[2]);
//         /* Create socket */
//         sockFd = socket(AF_INET, SOCK_STREAM, 0);
//         if (sockFd < 0)
//         {
//             std::cout << " Can't create socket" << std::endl;
//             retVal = 1;
//         }
//         else
//         {
//             /* get host information */
//             server = gethostbyname(argv[1]);
//             if (server == NULL)
//             {
//                 std::cout << "no such host ,  or the server is off " << std::endl;
//                 retVal = 1;
//             }
//             else
//             {
//                 bzero((char *)&servAdd, sizeof(servAdd));
//                 servAdd.sin_family = AF_INET;
//                 /* Copy host address */
//                 bcopy((char *)server->h_addr, (char *)servAdd.sin_addr.s_addr, server->h_length);
//                 /* set the port */
//                 servAdd.sin_port = htons(portNo);
//                 /* Start connect to the server */
//                 if (connect(sockFd, reinterpret_cast<struct sockaddr *>(&servAdd), sizeof(servAdd)) < 0)
//                 {
//                     std::cout << " Connection Error " << std::endl;
//                     retVal = 1;
//                 }
//                 else
//                 {
//                     /* Do communication */
//                     while (1)
//                     {
//                         bzero(request, sizeof(request));
//                         /* get the request from the user */
//                         fgets(request, sizeof(request), stdin);
//                         /* Send the request to the server */
//                         sentBytes = write(sockFd, request, strlen(request));
//                         if (sentBytes < 0)
//                         {
//                             std::cout << " can't send data " << std::endl;
//                         }
//                         else
//                         {
//                             if (strcmp("done", request) == 0)
//                             {
//                                 close(sockFd);
//                                 break;
//                             }
//                             else
//                             {
//                                 bzero(response , sizeof(response));
//                                 receivedBytes = read(sockFd, response, sizeof(response));
//                                 if (receivedBytes < 0)
//                                 {
//                                     std::cout << " can't receive data " << std::endl;
//                                 }
//                                 else
//                                 {
//                                     std::cout << "server : " << response << std::endl;
//                                 }

//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Client: ");
    while(1)
    {
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
             error("ERROR writing to socket");
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0)
             error("ERROR reading from socket");
        printf("Server : %s\n",buffer);
        int i = strncmp("Bye" , buffer , 3);
        if(i == 0)
               break;
    }
    close(sockfd);

   
    return 0;
}
