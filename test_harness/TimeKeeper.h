//
// Created by andre on 7/12/2017.
//

#ifndef DIRENT_TIMEKEEPER_H
#define DIRENT_TIMEKEEPER_H

#include <iostream>
#include <ctime>
#include <chrono>
using namespace std::chrono;
//Nearly stole this from some C++ website

class TimeKeeper {

private:

   high_resolution_clock::time_point _start;
   high_resolution_clock::time_point _end;
   duration<double> _time_duration;

    void calculate_duration();

public:
    TimeKeeper();
    ~TimeKeeper();

    void start();

    void endTimer();

    double getTime();

    void resetTimer();
};


#endif //DIRENT_TIMEKEEPER_H
