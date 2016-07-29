#include "TimeManager.h"

SolarTime TimeManager::Time;
SolarTime TimeManager::Epoch;
GLfloat TimeManager::Scale;
GLboolean TimeManager::Stopped;
GLfloat TimeManager::deltaTime;

void TimeManager::InitTime()
{
	Time = SolarTime();
	Epoch = SolarTime(TIME_EPOCH);
	Scale = 1.0f;
	Stopped = false;
	deltaTime = 0.0f;
}

double TimeManager::GetTime(){
	return Time.Difference(Epoch);
}

std::string TimeManager::TimeStringUtc(){
	return Time.Time2StringUTC();
}

std::string TimeManager::TimeStringLocal(GLfloat longitude){
	return Time.Time2StringLocal(longitude);
}

void TimeManager::UpdateTime(GLfloat dt){
	if (!Stopped)
	{
		GLfloat increaseTime = dt * Scale + deltaTime;
		GLint updateTime = GLint(increaseTime);
		deltaTime = increaseTime - updateTime;
		Time.AddSeconds(updateTime);
	}
}

GLboolean TimeManager::IsStopped(){
	return Stopped;
}

void TimeManager::Stop(){
	Stopped = true;
}

void TimeManager::Continue(){
	Stopped = false;
}

void TimeManager::SetTimeScale(GLint scale)
{
	Scale = scale;
}

void TimeManager::AddHours(GLint hour)
{
	Time.AddHours(hour);
}

void TimeManager::AddDays(GLint day)
{
	Time.AddDays(day);
}

void TimeManager::AddMonths(GLint month)
{
	Time.AddMonths(month);
}

void TimeManager::AddYears(GLint year){
	Time.AddYears(year);
}