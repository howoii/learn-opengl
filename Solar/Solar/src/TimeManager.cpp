#include "TimeManager.h"

SolarTime TimeManager::Time;
SolarTime TimeManager::Epoch;
GLfloat TimeManager::Scale;

void TimeManager::InitTime()
{
	Time = SolarTime();
	Epoch = SolarTime(TIME_EPOCH);
	Scale = 1.0f;
}

double TimeManager::GetTime(){
	return Time.Difference(Epoch);
}

void TimeManager::UpdateTime(GLfloat dt){
	GLint deltaTime = GLint(dt * Scale);
	Time.AddSeconds(deltaTime);
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