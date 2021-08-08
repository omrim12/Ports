#ifndef DATE_H_
#define DATE_H_
#include "days_in_month.h"
#include <exception>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <map>
using namespace std;
/*************************/
class date 	{

	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	std::map<int,int> days_in_month;

public:
	/**c'tors & d'tors**/
	date():month(0),day(0),hour(0),minute(0)	{}
	date(string time):month(0),day(0),hour(0),minute(0),days_in_month(std::map<int,int>{{1,JANUARY},
																						{2,FEBRUARY},
																						{3,MARCH},
																						{4,APRIL},
																						{5,MAY},
																						{6,JUNE},
																						{7,JULY},
																						{8,AUGUST},
																						{9,SEPTEMBER},
																						{10,OCTOBER},
																						{11,NOVEMBER},
																						{12,DECEMBER}})	{
		stringstream ss;
		stringstream convert;
		string buffer;
		ss.str(time);

		/** PARSING STRING **/
		//parse day
		getline(ss,buffer,'/');
		convert.str(buffer);
		convert >> day;
		if( ss.fail() || day > 31 || day < 0 )	{ throw exception(); }
		convert.clear();

		//parse month
		getline(ss,buffer,' ');
		convert.str(buffer);
		convert >> month;
		if( ss.fail() || month > 12 || month < 0 )	{ throw exception(); }
		convert.clear();

		//parse hour
		getline(ss,buffer,':');
		convert.str(buffer);
		convert >> hour;
		if( ss.fail() || hour > 23 || hour < 0 )	{ throw exception(); }
		convert.clear();

		//parse minute
		getline(ss,buffer);
		convert.str(buffer);
		convert >> minute;
		if( ss.fail() || minute > 59 || minute < 0 )	{ throw exception(); }
	}
	date(const date& other):month(other.month),day(other.day),hour(other.hour),minute(other.minute)	{}
	date(date&& other):month(other.month),day(other.day),hour(other.hour),minute(other.minute)	{}
	~date()	{}

	/** operators **/
	date& operator=(const date& other)	{
		month = other.month;
		day = other.day;
		hour = other.hour;
		minute = other.minute;

		return *this;
	}
	date& operator=(date&& other)	{
		month = other.month;
		day = other.day;
		hour = other.hour;
		minute = other.minute;

		return *this;
	}
	bool operator==(const date& other)	{
		if( month == other.month && day == other.day && hour == other.hour && minute == other.minute )	{
			return true;
		}
		return false;
	}
	bool operator==(date&& other)	{
		date d = other;
		return *this == d;
	}

	bool operator <=(const date& other)	{
		if( *this < other || *this == other )	{return true;}
		return false;
	}

	/**class functions**/
	unsigned int get_month() const	{ return month; }
	unsigned int get_day() const	{ return day; }
	unsigned int get_hour() const	{ return hour; }
	unsigned int get_minute() const	{ return minute; }
	void set_month( unsigned int new_month )	{ month = new_month; }
	void set_day( unsigned int new_day)	{ day = new_day; }
	void set_hour( unsigned int new_hour )	{ hour = new_hour; }
	void set_minute( unsigned int new_minute )	{ minute = new_minute; }
	bool operator<(const date& other) const 	{

		if( other.month > month )	{ return true; }
		else if( other.month < month )	{ return false; }

		else if( other.day > day )	{ return true; }
		else if( other.day < day )	{ return false; }

		else if( other.hour > hour )	{ return true; }
		else if( other.hour < hour )	{ return false; }

		else if( other.minute > minute )	{ return true; }

		return false;
	}
	bool operator>(const date& other) const 	{

		if( other.month < month )	{ return true; }
		else if( other.month > month )	{ return false; }

		else if( other.day < day )	{ return true; }
		else if( other.day > day )	{ return false; }

		else if( other.hour < hour )	{ return true; }
		else if( other.hour > hour )	{ return false; }

		else if( other.minute < minute )	{ return true; }

		return false;
	}
	int time_differ(date other)	{

		unsigned int differ = 0;
		differ += minute;
		differ += hour * 60;
		differ += day * 60 * 24;
		differ += month * 60 * 24 * days_in_month[month];

		differ -= other.minute;
		differ -= other.hour * 60;
		differ -= other.day * 60 * 24;
		differ -= other.month * 60 * 24 * days_in_month[other.month];

		return differ;
	}

	friend ostream& operator<<(ostream& out, const date& d)	{
		out << d.day << "/" << d.month << " " << d.hour << ":" << d.minute;
		return out;
	}
};
/*************************/
#endif
