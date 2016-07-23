#ifndef PLANETOBJECT_H
#define PLANETOBJECT_H

#include "Common.h"
#include "SObject.h"

class PlanetObject: public SObject
{
public:
	//轨道根数
	GLfloat T; //公转周期
	GLfloat A; //半长轴
	GLfloat ε; //轨道离心率
	GLfloat I; //轨道偏角
	GLfloat ω; //近日点幅角
	GLfloat Ω; //升交点黄经
	GLfloat M; //平近点角

	PlanetObject(glm::vec3 initPos, Mesh *mesh, Texture2D *texture);
	void UpdatePosition(GLfloat time);
	~PlanetObject();

private:
};

#endif