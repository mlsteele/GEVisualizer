#pragma once
#include <iostream>
#include <string>
#include "Timer.h"

using namespace std;

class TimeStamp{
public:
    TimeStamp(unsigned int year=0,unsigned int month=0,unsigned int day=0,unsigned int hour=0,unsigned int minute=0,unsigned int second=0,unsigned int millisecond=0){
        this->year = year;
        this->month = month;
        this->day = day;
        this->hour = hour;
        this->minute = minute;
        this->second = second;
        this->millisecond = millisecond;
    }

    TimeStamp(const TimeStamp &rhs){
        this->year = rhs.year;
        this->month = rhs.month;
        this->day = rhs.day;
        this->hour = rhs.hour;
        this->minute = rhs.minute;
        this->second = rhs.second;
        this->millisecond = rhs.millisecond;
    }
    
    TimeStamp(const string &timeStampAsString){
        this->year = 0;
        this->month = 0;
        this->day = 0;
        this->hour = 0;
        this->minute = 0;
        this->second = 0;
        this->millisecond = 0;
	setTimeStampFromString( timeStampAsString );
    }

    ~TimeStamp(){

    }

    TimeStamp& operator=(const TimeStamp &rhs){
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

    bool operator>(TimeStamp &rhs){
        if( this->getTimeInMilliseconds() > rhs.getTimeInMilliseconds() ){
            return true;
        }
        return false;
    }

    unsigned int getTimeInMilliseconds(){
        return (((hour*24*60*60) + (minute*60) + second)*1000) + millisecond;
    }

    bool setTimeStampAsNow(){
#if defined(__GE_OSX_BUILD__) || defined(__GE_LINUX_BUILD__)

        //Get the date and time
        time_t tim = time(NULL);
        tm *now = localtime( &tim );

        if( time == NULL ) return false;

        //Get the millisecon time
        struct timeval nowTimeval;
        gettimeofday( &nowTimeval, NULL );

        year = (unsigned int)now->tm_year + 1900;
        month = (unsigned int)now->tm_mon + 1;
        day = (unsigned int)now->tm_mday;
        hour = (unsigned int)now->tm_hour;
        minute = (unsigned int)now->tm_min;
        second = (unsigned int)now->tm_sec;
        millisecond = (unsigned int)nowTimeval.tv_usec/1000;

        return true;
#endif
#ifdef __GE_WINDOWS_BUILD__
        return false;
#endif
        printf("WARNING: Failed to get time stamp value! Unknown OS!\n");
        return false;
    }
    
    bool setTimeStampFromString(const string &timeString){
	    
	    //Find all the _
	    vector<string> s;
	    string tempString;
	    for(unsigned int i=0; i<timeString.length(); i++ ){
		if( timeString[i] == '_' || timeString[i] == '\n' || timeString[i] == '\r'  ){
			s.push_back( tempString );
			tempString = "";
		}else tempString += timeString[i];
	    }
	    
	    if( tempString.size() > 0 ) s.push_back( tempString );
	    
	    if( s.size() != 7 ){
		    cout << "WARNING: Failed to set timestamp from string. Incorrect size! Size: " << s.size() << endl;
		    return false;
	    }
	    
	    year = stringToInt( s[0] );
	    month = stringToInt( s[1]  );
	    day = stringToInt( s[2] );
	    hour = stringToInt( s[3]  );
	    minute = stringToInt( s[4] );
	    second = stringToInt( s[5]  );
	    millisecond = stringToInt( s[6]  );
	    
	    return true;
    }

    string getTimeStampAsString(){
        string timeString = "";
        timeString += intToString(year) + "_" + intToString(month) + "_" + intToString(day);
        timeString += "_" + intToString(hour) + "_" + intToString(minute) + "_" + intToString(second) + "_" + intToString(millisecond);
        return timeString;
    }

    std::string intToString( unsigned int i ){
        std::stringstream s;
        s << i;
        return  s.str();
    }

    std::string intToString( int i ){
        std::stringstream s;
        s << i;
        return  s.str();
    }
    
    unsigned int stringToInt(const string &s){
	    unsigned int value = 0;
	    try {
		value = boost::lexical_cast< unsigned int >( s );
	    } catch( boost::bad_lexical_cast const& ) {
		printf("ERROR: TimeStamp::stringToInt(string s) - Input string in not valid. String: %s\n",s.c_str());
	    }
	     return value;
    }

    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int millisecond;
};

