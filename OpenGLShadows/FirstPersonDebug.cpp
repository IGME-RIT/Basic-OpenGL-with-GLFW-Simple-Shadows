#include "FirstPersonDebug.h"





FirstPersonDebug::FirstPersonDebug(GLfloat fovX, GLfloat width, GLfloat height, GLfloat near, GLfloat far)
{
	camera = new Camera(fovX, width, height, near, far);
	transform = new Transform();
}


FirstPersonDebug::~FirstPersonDebug()
{
	delete camera;
	delete transform;
}

void FirstPersonDebug::Update(GLFWwindow* window, GLfloat width, GLfloat height, glm::vec2 mousePosition, float deltaTime)
{
	// Get the distance from the center of the screen that the mouse has moved
	glm::vec2 mouseMovement = mousePosition - (glm::vec2(width,height) / 2.0f);

	// Calculate the vertical view angle
	float pitch = transform->eulerRotation.x - (int)mouseMovement.y * .001f;

	// Clamp the camera from looking up over 90 degrees.
	if (transform->eulerRotation.x < -glm::half_pi<GLfloat>())
		pitch = -glm::half_pi<GLfloat>();
	else if (transform->eulerRotation.x > glm::half_pi<GLfloat>())
		pitch = glm::half_pi<GLfloat>();

	float yaw = transform->eulerRotation.y - (int)mouseMovement.x * .001f;;


	transform->eulerRotation = glm::vec3(pitch, yaw, 0);

	// Here we get some input, and use it to move the camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		transform->MoveForward(5.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		transform->MoveRight(-5.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		transform->MoveForward(-5.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		transform->MoveRight(5.0f * deltaTime);
	}


	transform->recalculateWMatrix = true;
	transform->Update();
	// Move the cursor to the center of the screen
	glfwSetCursorPos(window, mousePosition.x - mouseMovement.x, mousePosition.y - mouseMovement.y);

	camera->UpdateView(glm::inverse(transform->GetWorldMatrix()));
}

glm::mat4 FirstPersonDebug::GetCameraMatrix()
{
	return camera->GetCameraMatrix();
}

