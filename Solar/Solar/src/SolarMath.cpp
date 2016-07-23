#include "SolarMath.h"
#include "math.h"

SolarMath::SolarMath()
{
}

SolarMath::~SolarMath()
{
}

/*
* 用开普勒公式计算天体偏近点角，角度采用弧度制
* M         : 平近点角
* Eccentric : 椭圆轨道离心率
* error     : 计算误差
*/
GLfloat SolarMath::EccentricAnomaly(GLfloat M, GLfloat Eccentric, GLfloat error){
	GLfloat E0, E;
	E0 = M;
	E = E0 + (M + Eccentric * glm::sin(E0) - E0) / (1 - Eccentric * glm::cos(E0));
	GLint turns = 0;
	while (abs(E - E0) > error)
	{
		E0 = E;
		E = E0 + (M + Eccentric * glm::sin(E0) - E0) / (1 - Eccentric * glm::cos(E0));
		if (turns++ > 100) break;
	}
	return E;
}

/*
* 根据平近点角计算行星实际偏转角和与太阳的距离
* E			: 偏近点角
* A			: 轨道半长轴
* Eccentric : 椭圆轨道离心率
* r			: 返回值距离的地址
* theta		: 返回值实际偏转角的地址
*/
void SolarMath::TrueAnomalyAndDistance(GLfloat E, GLfloat A, GLfloat Eccentric, GLfloat *r, GLfloat *theta){
	*r = A * (1 - Eccentric * glm::cos(E));
	GLfloat tanSemiTheta = sqrt((1 + Eccentric) / (1 - Eccentric)) * glm::tan(E / 2);
	*theta = 2 * glm::atan(tanSemiTheta);
}
