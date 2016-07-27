#ifndef SOLARTIME_H
#define SOLARTIME_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <locale>

#define HOUR_SECONDS 3600
#define DAY_HOURS 24
#define YEAR_MONTHS 12

class SolarTime
{
public:
	SolarTime();

	/*
	* Construct object from a given time string
	* timeStr		: Time string
	* timeFormat	: Time Format
	*	default format example "2000-01-01 12:00:00"
	*/
	SolarTime(std::string timeStr, std::string timeFormat = "%Y-%m-%d %H:%M:%S");

	SolarTime(int year, int month, int day, int hour, int minute, int second);

	void AddSeconds(std::time_t seconds);
	void AddHours(int hours);
	void AddDays(int days);
	void AddMonths(int months);
	void AddYears(int years);
	double Difference(SolarTime &epoch);

	std::time_t GetTime();
	std::string Time2StringUTC();
	std::string Time2StringLocal(float longitude);
	~SolarTime();

private:
	std::time_t Time;
	std::time_t mkUtcTime(std::tm *calenderTime);
	void checkTimeBoundary();
};

#endif