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

void *recThread(void *arg){
  struct TCPTransfer* tcpTransfer =   ((struct TCPTransfer*)arg);
  size_t start = tcpTransfer->ID * tcpTransfer-> step;
  size_t end = min(start + tcpTransfer-> step,  tcpTransfer->size);
  recData(tcpTransfer->socket,  tcpTransfer->data + start,  end);
  return NULL;
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





void monitor(int portno, int size, int nThreads){
    int sockets1[MAXTHREADS];
    int sockets2[MAXTHREADS];
    int thread_cr_res = 0, thread_join_res;
    pthread_t *threads =(pthread_t *) malloc(MAXTHREADS * sizeof(pthread_t));

    for (int i =0; i < nThreads; i++)
      sockets2[i] = intitializeSocket(portno + i, sockets1[i]);
    float * data = (float *) malloc( size * sizeof(float));
    size_t step = size * sizeof(float) / nThreads;

  for(int i = 0; i < nThreads; i++){
    struct TCPTransfer tp;
    tp.data = static_cast<char *>(static_cast<void *>(data));
    tp.size = size * sizeof(float);
    tp.step = step;
    tp.socket = sockets2[i];
    tp.ID = i;
    thread_cr_res = pthread_create(&threads[i], NULL, recThread, (void*)(&tp));
    if(thread_cr_res != 0){
      fprintf(stderr,"THREAD CREATE ERROR");
      return;
    }
  }
  /* Later edit, joining the threads */
  for (int i = 0; i < nThreads; i++){
    thread_join_res = pthread_join(threads[i], NULL);
    if(thread_join_res != 0){
      fprintf(stderr, "JOIN ERROR");
      return;
    }       
  }
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
    for (int i =0; i < nThreads; i++){
      close(sockets1[i]);
      close(sockets2[i]);
    }

  free(threads);
} 
int main(int argc, char *argv[])
{
/*    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }*/
    int portno = 51717;//atoi(argv[1]);
    int size   = 32768000;//atoi(argv[2]);
    int nThreads = 4;



    monitor(portno, size, nThreads);
    return 0; 
}
