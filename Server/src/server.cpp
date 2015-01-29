/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <chrono>
#include <utility>
#include <math.h>              /* Basic Linear Algebra I/O */
#include "common.h"
#include "cloudTransfer.h"
#include "server.h"
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


// Initializing the connection
int intitializeSocket(int portno, int &sockfd){

    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, reinterpret_cast<struct sockaddr *>(&serv_addr),
             sizeof(serv_addr)) < 0) 
             error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, 
                reinterpret_cast<struct sockaddr *>(&cli_addr), 
                &clilen);
    if (newsockfd < 0) error("ERROR on accept");
    return newsockfd;
    
}





void monitor(int portno, int size, int count){
    int sockfd, newsockfd;
    newsockfd = intitializeSocket(portno, sockfd);
    float * data = (float *) malloc( size * sizeof(float));

    for (int it =0; it < count; it++)
	    recData(newsockfd, data, size * sizeof(float));
//    sendData(newsockfd, data, size);
#if 0
    int count = 0;
    int perror = 0;
    for (int i = 0; i < size; i++){
      if (data[i] != i){
	perror = 1;
	if (count < 10)
	{
	  count++;
	  printf("%d\t%f\n",i , data[i]);
	}
      }

    }
      if (perror == 0)
	printf("Passed\n");
      else
	printf("Failed\n");

#endif
    free(data);
    close(newsockfd);
    close(sockfd);

} 
int main(int argc, char *argv[])
{
/*    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }*/
    int portno = 51717;//atoi(argv[1]);
    int size   = 32768000;//atoi(argv[2]);
    int count = 3;



    monitor(portno, size, count);
    return 0; 
}
