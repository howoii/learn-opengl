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

void SetTimeScale(GLint scale);
void AddHours(GLint hour = 1);
void AddDays(GLint day = 1);
void AddMonths(GLint month = 1);
void AddYears(GLint year = 1);