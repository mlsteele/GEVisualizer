#pragma once

#include "ofMain.h"

class TimeEvent{
public:
    TimeEvent(unsigned int year=0,unsigned int month=0,unsigned int day=0,unsigned int hour=0,unsigned int minute=0,unsigned int second=0,unsigned int millisecond=0){
        this->year = year;
        this->month = month;
        this->day = day;
        this->hour = hour;
        this->minute = minute;
        this->second = second;
        this->millisecond = millisecond;
    }
    
    TimeEvent(const TimeEvent &rhs){
        this->year = rhs.year;
        this->month = rhs.month;
        this->day = rhs.day;
        this->hour = rhs.hour;
        this->minute = rhs.minute;
        this->second = rhs.second;
        this->millisecond = rhs.millisecond;
    }
    
    ~TimeEvent(){
        
    }
    
    TimeEvent& operator=(const TimeEvent &rhs){
        if( this != &rhs ){
            this->year = rhs.year;
            this->month = rhs.month;
            this->day = rhs.day;
            this->hour = rhs.hour;
            this->minute = rhs.minute;
            this->second = rhs.second;
            this->millisecond = rhs.millisecond;
        }
        return *this;
    }
    
    bool operator>(TimeEvent &rhs){
        if( this->getTimeInMilliseconds() > rhs.getTimeInMilliseconds() ){
            return true;
        }
        return false;
    }
    
    unsigned int getTimeInMilliseconds(){
        return (((hour*24*60*60) + (minute*60) + second)*1000) + millisecond;
    }
    
    string getTimeEventAsString(){
        string t = "";
        t += ofToString(day) + "/" + ofToString(month) + "/" + ofToString(year);
        t += " " + ofToString(hour) + ":" + ofToString(minute) + ":" + ofToString(second) + "." + ofToString(millisecond);
        return t;
    }
    
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int millisecond;
};

