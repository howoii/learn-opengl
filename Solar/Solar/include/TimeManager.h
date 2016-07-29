#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "Common.h"
#include "SolarTime.h"

#define TIME_EPOCH "2000-01-01 12:00:00"

class TimeManager
{
public:
	static void InitTime();
	static double GetTime();
	static std::string TimeStringUtc();
	static std::string TimeStringLocal(GLfloat longitude);
	static void UpdateTime(GLfloat dt);
	static void Stop();
	static void Continue();
	static GLboolean IsStopped();
	static void SetTimeScale(GLint scale);
	static void AddHours(GLint hour = 1);
	static void AddDays(GLint day = 1);
	static void AddMonths(GLint month = 1);
	static void AddYears(GLint year = 1);
	~TimeManager(){}

private:
	static SolarTime Time;
	static SolarTime Epoch;
	static GLfloat Scale;
	static GLfloat deltaTime;
	static GLboolean Stopped;
	TimeManager(){}
};

#endif // !TIMEMANAGER_H
