#include "Camera.h"



void Camera::CalcCameraMatrix()
{
	cameraMatrix = proj * view;
}

Camera::Camera()
{
}

Camera::Camera(GLfloat fovX, GLfloat width, GLfloat height, GLfloat near, GLfloat far)
{
	UpdateProj(fovX, width, height, near, far);
}

void Camera::UpdateProj(GLfloat fovX, GLfloat width, GLfloat height, GLfloat near, GLfloat far )
{
	//https://en.wikipedia.org/wiki/Field_of_view_in_video_games
	fovY = 2 * glm::atan(tan(glm::radians(fovX) / 2) * height / width);
	zNear = near;
	zFar = far;

	proj = glm::perspective(fovY, width / height, zNear, zFar);

	if (view != glm::mat4())
		CalcCameraMatrix();
}

void Camera::UpdateView(glm::mat4 viewMat)
{
	view = viewMat;

	if (proj != glm::mat4())
		CalcCameraMatrix();
}

glm::mat4 Camera::GetProj()
{
	return proj;
}


glm::mat4 Camera::GetView()
{
	return view;
}

glm::mat4 Camera::GetCameraMatrix()
{
	return cameraMatrix;
}

Camera::~Camera()
{
}
