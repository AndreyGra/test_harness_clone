//
// Created by andre on 7/12/2017.
//

#include "TimeKeeper.h"

TimeKeeper::TimeKeeper()
{
    _time_duration = duration<double>::zero();
    _start = high_resolution_clock::time_point();
    _end = high_resolution_clock::time_point();

}

TimeKeeper::~TimeKeeper() {}

void TimeKeeper::start() {

    _start = high_resolution_clock::now();

}

void TimeKeeper::endTimer() {

    _end = high_resolution_clock::now();

}

double TimeKeeper::getTime() {

    calculate_duration();
    return _time_duration.count();

}

void TimeKeeper::calculate_duration() {

    _time_duration = duration_cast< duration <double> >(_end - _start);

}

void TimeKeeper::resetTimer() {

    _start = high_resolution_clock::time_point();
    _end = high_resolution_clock::time_point();

}

