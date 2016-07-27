#ifndef PLANETOBJECT_H
#define PLANETOBJECT_H

#include "Common.h"
#include "SObject.h"

#include <vector>

class PlanetObject: public SObject
{
public:
	GLfloat T; //公转周期

	//轨道根数
	GLfloat A; //半长轴
	GLfloat ε; //轨道离心率
	GLfloat I; //轨道偏角
	GLfloat ω; //近日点幅角
	GLfloat Ω; //升交点黄经
	GLfloat M; //平近点角

	//自转参数
	GLfloat R; //半径
	GLfloat D; //自转周期
	GLfloat O; //自转轴倾角(obliguity)

	GLfloat ViewSize;
	//其他参数
	GLfloat Reflect; //反射率

	GLfloat Rotation; //自转角度
	glm::vec3 ViewDirection;
	glm::vec3 InitPos;

	PlanetObject();
	PlanetObject(Mesh *mesh, Texture2D *texture, std::vector<GLfloat> parameters);
	glm::vec3 CalcPostion(GLfloat time);
	void UpdatePosition(GLfloat time);
	void UpdateRotation(GLfloat time);
	void UpdateViewDirecton(PlanetObject *earth, GLfloat longitude, GLfloat latitude);
	void UpdateViewSize(PlanetObject *earth);
	void Draw(Shader shader);
	~PlanetObject();
private:
};

#endif