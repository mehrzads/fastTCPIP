/*
 * $Id$
 */
#ifndef SERVER_H
#  define SERVER_H
typedef struct MPIInfo{
  int nProcs;
  int rank;
  int nCols;
  int nRows;
  int myRow;
  int myCol;
  int grid;
};


#endif /* ifndef SERVER_H */

