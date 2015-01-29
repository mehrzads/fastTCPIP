#include "cloudTimer.h"

CloudTimer::CloudTimer(): 
      start_time (std::chrono::steady_clock::now()),
      end_time (std::chrono::steady_clock::now()),
      isStartSet(false),
      isEndSet(false)
{
}             
void CloudTimer::start() {
  isStartSet = true;
  start_time =  std::chrono::steady_clock::now(); 
}

void CloudTimer::end() {
  isEndSet = true;
  end_time =  std::chrono::steady_clock::now(); 
}

double CloudTimer::getDurationInSeconds() {
	    if (!isStartSet)
	      printf("Start time is not set\n");
	    if (!isEndSet)
	      printf("End time is not set\n");
	    return std::chrono::duration_cast<std::chrono::milliseconds>
            (end_time - start_time).count() / 1000.0;
}
