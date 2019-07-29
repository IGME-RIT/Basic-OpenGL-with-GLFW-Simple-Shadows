#pragma once
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

class Camera
{

	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 cameraMatrix;

	GLfloat fovY;
	GLfloat zNear;
	GLfloat zFar;

	void CalcCameraMatrix();
	
public:
	Camera();
	Camera(GLfloat fovX, GLfloat width, GLfloat height, GLfloat near, GLfloat far);
	void UpdateProj(GLfloat fovX, GLfloat width, GLfloat height, GLfloat near, GLfloat far);
	void UpdateView(glm::mat4 viewMat);
	glm::mat4 GetProj();
	glm::mat4 GetView();
	glm::mat4 GetCameraMatrix();
	~Camera();

};

