/*
 * $Id$
 */
#ifndef CLOUD_H
#  define CLOUD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include "common.h"

cloudError_t cloudInit(int portno, char * hostname, int &socketID);
cloudError_t cloudSend(int socketID,  void *  data,  size_t  count);
cloudError_t cloudRec(int socketID,  void *  data,  size_t  count);
cloudError_t cloudFinish( int socketID);

#endif /* ifndef CLOUD_H */

