/*
 * $Id$
 */
#ifndef CLOUDTIMER_H
#  define CLOUDTIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>

class CloudTimer {
  private:
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    bool isStartSet;
    bool isEndSet;
  public:
    CloudTimer();
    double getDurationInSeconds();
    void start();
    void end();
};

#endif /* ifndef CLOUDTIMER_H */

