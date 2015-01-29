/*
 *
 * Author: Mehrzad Samadi
 * First Client Service
 *
 */

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
#include <limits.h>
#include <chrono>
#include <math.h> 
#include "cloud.h"
#include "cloudTimer.h"
using namespace std;

void *sendThread(void *arg){
  struct TCPTransfer* tcpTransfer =   ((struct TCPTransfer*)arg);
  size_t start = tcpTransfer->ID * tcpTransfer-> step;
  size_t end = min(start + tcpTransfer-> step, tcpTransfer->size);
  printf("%d\t%d\t%d\n" , tcpTransfer->ID, start,end);
  cloudSend(tcpTransfer->socket,  tcpTransfer->data + start,  end);
  return NULL;
}


int main(int argc, char *argv[])
{
  int sockets[MAXTHREADS];
  int thread_cr_res = 0, thread_join_res;
  pthread_t *threads =(pthread_t *) malloc(MAXTHREADS * sizeof(pthread_t));
  struct TCPTransfer * tp = (struct TCPTransfer *) malloc(MAXTHREADS * sizeof(struct TCPTransfer));

  
  // Reading the arguments
  if (argc < 1) {
     fprintf(stderr,"usage %s hostname port\n", argv[0]);
     exit(0);
  }
  char * hostname = argv[1];
  int portno = 51717;//atoi(argv[2]);
  int size = 32768000;//atoi(argv[3]);
  int nThreads = 4;

  for (int i = 0; i < nThreads; i++)
    cloudInit(portno + i, hostname, sockets[i]);

  printf("%d\n",size);

  float * A = (float *) malloc( size * sizeof(float));
  float * C = (float *) malloc( size * sizeof(float));
    
  for (int i = 0; i < size; i++){
      A[i] = i;
      C[i] = 0;
  }
  size_t step = (size * sizeof(float)) / nThreads;

  CloudTimer cloudTimer;
  cloudTimer.start();
  
  for(int i = 0; i < nThreads; i++){
    tp[i].data = static_cast<char *>(static_cast<void *>(A));
    tp[i].size = size * sizeof(float);
    tp[i].step = step;
    tp[i].socket = sockets[i];
    tp[i].ID = i;
    thread_cr_res = pthread_create(&threads[i], NULL, sendThread, (void*)(&tp[i]));
    if(thread_cr_res != 0){
      fprintf(stderr,"THREAD CREATE ERROR");
      return (-1);
    }
  }
  /* Later edit, joining the threads */
  for (int i = 0; i < nThreads; i++){
    thread_join_res = pthread_join(threads[i], NULL);
    if(thread_join_res != 0){
      fprintf(stderr, "JOIN ERROR");
      return (-1);
    }       
  }
  
  cloudTimer.end();    
  double time_in_seconds = cloudTimer.getDurationInSeconds();
  printf("Upload rate %f Gbps\n", (size * sizeof(float) * 8  )/(1024 * 1024 *1024 *time_in_seconds) );
#if 0  
  cloudTimer.start();

//  cloudRec(sockfd,  C,  size * sizeof(float));
  
  cloudTimer.end();    
  time_in_seconds = cloudTimer.getDurationInSeconds();
  printf("Download rate %f Gbps\n", (size * sizeof(float) * 8  )/(1024 * 1024 *1024 *time_in_seconds) );
            
  int ferror = 0;
  for (int i = 0; i < size ; i++)
     if (C[i] != A[i])
       ferror =1;
  if (ferror ==0)
     printf("Passed\n");
  else
     printf("Failed\n");       
#endif
  free(A);
  free(C);
  free(threads);
  free(tp);
  for (int i = 0; i < nThreads; i++)
    cloudFinish(sockets[i]);
  return 0;
}
