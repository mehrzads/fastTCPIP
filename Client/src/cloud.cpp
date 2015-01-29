#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <utility>
#include "cloud.h" 
#include "cloudTransfer.h" 
using namespace std;


void error(const char *msg)
{
    perror(msg);
    exit(0);
}


// Initializing the connection
cloudError_t  cloudInit(int portno, char * hostname, int &socketID){
    struct sockaddr_in serv_addr;
    struct hostent *server;
    socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID < 0) return CloudErrorOpen;
    server = gethostbyname(hostname);
    if (server == NULL) return CloudErrorNoHost;
    bzero(reinterpret_cast<char *>(&serv_addr), sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(static_cast<char *>(server->h_addr), 
         reinterpret_cast<char *>(&serv_addr.sin_addr.s_addr),
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(socketID,reinterpret_cast<struct sockaddr *>( &serv_addr),sizeof(serv_addr)) < 0) 
        return CloudErrorConnection;
    return CloudSuccess;
}


// cloudMemcpy is transferring data between client and the server
cloudError_t cloudSend(int socketID,  void *  data,  size_t  count){
  sendData(socketID, data, count);
  return CloudSuccess;
} 

// cloudMemcpy is transferring data between client and the server
cloudError_t cloudRec(int socketID,  void *  data,  size_t  count){
  recData(socketID, data, count);
  return CloudSuccess;
} 


// Finishing the connection
cloudError_t cloudFinish( int socketID){
    close(socketID);
    return CloudSuccess; 
}


