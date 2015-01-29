#include "cloudTransfer.h"
char command[1000];
void print(std::string message){
  for (unsigned int i = 0; i < message.size(); i++){
    printf("%d ", message[i]);	
  }
    printf("\ns\n");
}

cloudError_t sendMessage(int socketID, std::string message){
  int messageSize = message.size();
  int n = write(socketID, &messageSize, 4);
  if (n < 0) return CloudErrorWrite;
  n = write(socketID, message.c_str(), message.size());
  if (n < 0) return CloudErrorWrite;
  return CloudSuccess;
}

cloudError_t recMessage(int socketID, std::string &message){
  int messageSize = 0;
  int n = read(socketID, &messageSize, 4);
  if (n <0) return CloudErrorRead;
  n = read(socketID, command, messageSize);
  if (n < 0) return CloudErrorRead;
  message = std::string(command, messageSize);
  return CloudSuccess;
}

cloudError_t sendData(int socketID, void * data, size_t size){
  register size_t sent = 0;
  register size_t n = 0;
  while (sent < size){
    n = write(socketID, static_cast<void *>(static_cast<char *>(data) + sent), size - sent);
    if (n < 0) return CloudErrorWrite;
    sent += n;
  }
  return CloudSuccess;
}

cloudError_t recData(int socketID, void * data, size_t size){
  register size_t sent = 0;
  register size_t n = 0;
  while (sent < size){
    //static casts are added to remove the warning
    n = read(socketID, static_cast<void *>(static_cast<char *>(data) + sent), size - sent);
    sent += n;
    if (n < 0) return CloudErrorRead;
  }
  return CloudSuccess;
}



