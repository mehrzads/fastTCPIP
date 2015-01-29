/*
 * $Id$
 */
#ifndef CLOUDTRANSFER_H
#  define CLOUDTRANSFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>
#include "common.h"

void print(std::string message);
cloudError_t sendMessage(int socketID, std::string message);
cloudError_t recMessage(int socketID, std::string &message);
cloudError_t sendData(int socketID, void * data, size_t size);
cloudError_t recData(int socketID, void * data, size_t size);

#endif /* ifndef CLOUDTRANSFER_H */

