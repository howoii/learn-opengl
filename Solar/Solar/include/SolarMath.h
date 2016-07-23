#ifndef SOLARMATH_H
#define SOLARMATH_H

#include "Common.h"

class SolarMath
{
public:

	/*
	* 用开普勒公式计算天体偏近点角，角度采用弧度制
	* M         : 平近点角
	* Eccentric : 椭圆轨道离心率
	* error     : 计算误差
	*/
	static GLfloat EccentricAnomaly(GLfloat M, GLfloat Eccentric, GLfloat error = 0.000001);

	/*
	* 根据平近点角计算行星实际偏转角和与太阳的距离
	* E			: 偏近点角
	* A			: 轨道半长轴
	* Eccentric : 椭圆轨道离心率
	* r			: 返回值距离的地址
	* theta		: 返回值实际偏转角的地址
	*/
	static void TrueAnomalyAndDistance(GLfloat E, GLfloat A, GLfloat Eccentric, GLfloat *r, GLfloat *theta);
	~SolarMath();
private:
	SolarMath();

};

#endif