/*
 *
 * Author: Mehrzad Samadi
 * First Client Service
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h> 
#include <chrono>
#include "cloud.h"
#include "cloudTimer.h"

int main(int argc, char *argv[])
{
  int sockfd;
  
  // Reading the arguments
  if (argc < 1) {
     fprintf(stderr,"usage %s hostname port\n", argv[0]);
     exit(0);
  }
  char * hostname = argv[1];
  int portno = 51717;//atoi(argv[2]);
  int size = 32768;//atoi(argv[3]);
  int count = 1000;

  cloudInit(portno, hostname, sockfd);
  printf("%d\n",size);

  float * A = (float *) malloc( size * sizeof(float));
  float * C = (float *) malloc( size * sizeof(float));
    
  for (int i = 0; i < size; i++){
      A[i] = i;
      C[i] = 0;
  }

  CloudTimer cloudTimer;
  cloudTimer.start();
  
  for (int i = 0; i < count; i++)
    cloudSend(sockfd,  A,  size * sizeof(float));
  
  cloudTimer.end();    
  double time_in_seconds = cloudTimer.getDurationInSeconds();
  printf("Upload rate %f Gbps\n", (count * size * sizeof(float) * 8  )/(1024 * 1024 *1024 *time_in_seconds) );
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
  cloudFinish(sockfd);
  return 0;
}
