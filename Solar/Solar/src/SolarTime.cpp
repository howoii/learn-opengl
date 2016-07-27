#include "SolarTime.h"
#include <stdio.h>

SolarTime::SolarTime()
{
	this->Time = std::time(nullptr);
}

SolarTime::~SolarTime()
{
}

SolarTime::SolarTime(std::string timeStr, std::string timeFormat)
{
	std::tm t = {};
	std::istringstream ss(timeStr);
	ss >> std::get_time(&t, timeFormat.c_str());
	if (ss.fail())
	{
		std::cout << "ERROR::SOLARTIME::CONSTRUCTOR Invalid Time." << std::endl;
		this->Time = std::time(nullptr);
	}
	else
	{
		this->Time = this->mkUtcTime(&t);
		this->checkTimeBoundary();
	}
}

SolarTime::SolarTime(int year, int month, int day, int hour, int minute, int second){
	std::string timeFormat = "%Y-%m-%d %H:%M:%S";
	char timeStr[128];
	sprintf(timeStr, "%d-%02d-%d %02d:%02d:%02d", year, month, day, hour, minute, second);
	new(this) SolarTime(std::string(timeStr), timeFormat);
}

std::time_t SolarTime::mkUtcTime(std::tm *calendarTime){
	std::time_t tNow = std::time(nullptr);
	std::time_t tDiff = tNow - std::mktime(std::gmtime(&tNow));
	return std::mktime(calendarTime) + tDiff;
}

std::time_t SolarTime::GetTime()
{
	return this->Time;
}

std::string SolarTime::Time2StringUTC(){
	std::tm *calendarTime = std::gmtime(&this->Time);
	return std::string(std::asctime(calendarTime));
}

std::string SolarTime::Time2StringLocal(float longitude){
	if (longitude > 180.0 || longitude < -180.0)
	{
		std::cout << "ERROR::SOLARTIME Invalid longitude." << std::endl;
		return std::string("Invalid Longitude");
	}
	int timeZone;
	timeZone = int(longitude + 7.5) / 15;
	time_t localTime = this->Time + timeZone * 3600;
	std::tm *calendarTime = std::gmtime(&localTime);
	return std::string(std::asctime(calendarTime));
}

void SolarTime::AddSeconds(std::time_t seconds){
	this->Time += seconds;
	this->checkTimeBoundary();
}

void SolarTime::AddHours(int hours){
	std::time_t delta = hours * HOUR_SECONDS;
	this->Time += delta;
	this->checkTimeBoundary();
}

void SolarTime::AddDays(int days){
	std::time_t delta = days * DAY_HOURS * HOUR_SECONDS;
	this->Time += delta;
	this->checkTimeBoundary();
}

void SolarTime::AddMonths(int months){
	std::tm *calenderTime = std::gmtime(&this->Time);
	int monthNow = calenderTime->tm_mon;
	int yearNow = calenderTime->tm_year;

	int monthNext = (monthNow + months) % YEAR_MONTHS;
	int yearNext = yearNow + (monthNow + months) / YEAR_MONTHS;

	std::tm newCalender = {};
	newCalender.tm_year = yearNext;
	newCalender.tm_mon = monthNext;
	newCalender.tm_mday = calenderTime->tm_mday;
	newCalender.tm_hour = calenderTime->tm_hour;
	newCalender.tm_min = calenderTime->tm_min;
	newCalender.tm_sec = calenderTime->tm_sec;
	newCalender.tm_isdst = calenderTime->tm_isdst;

	this->Time = this->mkUtcTime(&newCalender);
	this->checkTimeBoundary();
}

void SolarTime::AddYears(int years){
	std::tm *calenderTime = std::gmtime(&this->Time);
	calenderTime->tm_year += years;
	
	std::tm newCalender = {};
	newCalender.tm_year = calenderTime->tm_year;
	newCalender.tm_mon = calenderTime->tm_mon;
	newCalender.tm_mday = calenderTime->tm_mday;
	newCalender.tm_hour = calenderTime->tm_hour;
	newCalender.tm_min = calenderTime->tm_min;
	newCalender.tm_sec = calenderTime->tm_sec;
	newCalender.tm_isdst = calenderTime->tm_isdst;

	this->Time = this->mkUtcTime(&newCalender);
	this->checkTimeBoundary();
}

double SolarTime::Difference(SolarTime &epoch){
	return std::difftime(this->Time, epoch.GetTime());
}

void SolarTime::checkTimeBoundary(){
	std::tm *calenderTime = std::gmtime(&this->Time);
	if (calenderTime->tm_year == 70)
	{
		this->Time = std::time(nullptr);
		std::cout << "Time out of boundary, reset to current time" << std::endl;
	}
}