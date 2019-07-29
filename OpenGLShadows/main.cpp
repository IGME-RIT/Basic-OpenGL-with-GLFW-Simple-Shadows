
#include <iostream>
#include <vector>

// We are using the glew32s.lib
// Thus we have a define statement
// If we do not want to use the static library, we can include glew32.lib in the project properties
// If we do use the non static glew32.lib, we need to include the glew32.dll in the solution folder
// The glew32.dll can be found here $(SolutionDir)\..\External Libraries\GLEW\bin\Release\Win32
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "Entity.h"
#include "FirstPersonDebug.h"


// Variables for the Height and width of the window
const GLint WIDTH = 800, HEIGHT = 600;

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

glm::vec2 mousePosition = glm::vec2();

void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
	mousePosition = glm::vec2(mouseX, mouseY);
}

void UpdateVec3(char * name, glm::vec3 data, ShaderProgram* program)
{
	GLint uniform = glGetUniformLocation(program->GetGLShaderProgram(), name);
	if (uniform == -1)
	{
		std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
		return;
	}


	glUniform3fv(uniform, 1, &data[0]);
}


int main()
{
	#pragma region GL setup
	//Initializes the glfw
	glfwInit();

	// Setting the required options for GLFW

	// Setting the OpenGL version, in this case 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 99);
	// Setting the Profile for the OpenGL.

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// Setting the forward compatibility of the application to true
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// We don't want the window to resize as of now.
	// Therefore we will set the resizeable window hint to false.
	// To make is resizeable change the value to GL_TRUE.
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create the window object
	// The first and second parameters passed in are WIDTH and HEIGHT of the window we want to create
	// The third parameter is the title of the window we want to create
	// NOTE: Fourth paramter is called monitor of type GLFWmonitor, used for the fullscreen mode.
	//		 Fifth paramter is called share of type GLFWwindow, here we can use the context of another window to create this window
	// Since we won't be using any of these two features for the current tutorial we will pass nullptr in those fields
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shaders Tutorial", nullptr, nullptr);

	// We call the function glfwGetFramebufferSize to query the actual size of the window and store it in the variables.
	// This is useful for the high density screens and getting the window size when the window has resized.
	// Therefore we will be using these variables when creating the viewport for the window
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	// Check if the window creation was successful by checking if the window object is a null pointer or not
	if (window == nullptr)
	{
		// If the window returns a null pointer, meaning the window creation was not successful
		// we print out the messsage and terminate the glfw using glfwTerminate()
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		// Since the application was not able to create a window we exit the program returning EXIT_FAILURE
		return EXIT_FAILURE;
	}

	// Creating a window does not make it the current context in the windows.
	// As a results if the window is not made the current context we wouldn't be able the perform the operations we want on it
	// So we make the created window to current context using the function glfwMakeContextCurrent() and passing in the Window object
	glfwMakeContextCurrent(window);

	// Enable GLEW, setting glewExperimental to true.
	// This allows GLEW take the modern approach to retrive function pointers and extensions
	glewExperimental = GL_TRUE;

	glfwSetCursorPosCallback(window, mouseMoveCallback);

	// Initialize GLEW to setup OpenGL function pointers
	if (GLEW_OK != glewInit())
	{
		// If the initalization is not successful, print out the message and exit the program with return value EXIT_FAILURE
		std::cout << "Failed to initialize GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	// Setting up the viewport
	// First the parameters are used to set the top left coordinates
	// The next two parameters specify the height and the width of the viewport.
	// We use the variables screenWidth and screenHeight, in which we stored the value of width and height of the window,
	glViewport(0, 0, screenWidth, screenHeight);
	#pragma endregion


	//Loading our model mesh
	Mesh* mainMesh = new Mesh("Assets/building.obj", true);
	//Creating our only entity
	Entity* mainEntity = new Entity("main");
	mainEntity->SetModelMesh(mainMesh);
	mainEntity->transform->position.z = -2.f;

	//Our FP Cam
	FirstPersonDebug* camera = new FirstPersonDebug(90, WIDTH, HEIGHT, 0.1f, 100.f);

	//This entire section is needed for shadow mapping
	#pragma region Shadow_Asset_loading
	//Enable depth testing, which fills up our depth buffer
	glEnable(GL_DEPTH_TEST);
	
	//Create a framebuffer object for our depth map
	GLuint FBO;
	//Bind our framebuffer object
	glGenFramebuffers(1, &FBO);

	//Create the actual depth map using a 2D texture
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//Anisotropic filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Attach the depth texture to the framebuffers depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

	//Set both draw and read buffers to none in order to not render color data
	//Color is not necessary for our grayscale depthmap
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Shader and material creation for our depth shader
	Shader* depthvShader = new Shader("shaders/depthvShader.glsl", GL_VERTEX_SHADER);
	Shader* depthfShader = new Shader("shaders/depthfShader.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram* depthprogram = new ShaderProgram();
	depthprogram->AttachShader(depthvShader);
	depthprogram->AttachShader(depthfShader);

	//Material used to render depth frame onto texture in our first pass
	Material* depthMat= new Material(depthprogram);

	//Shader and material creation for our shadow shader
	Shader* shadowvShader = new Shader("shaders/shadowvShader.glsl", GL_VERTEX_SHADER);
	Shader* shadowfShader = new Shader("shaders/shadowfShader.glsl", GL_FRAGMENT_SHADER);
	ShaderProgram* shadowprogram = new ShaderProgram();
	shadowprogram->AttachShader(shadowvShader);
	shadowprogram->AttachShader(shadowfShader);

	//Material used in our final render pass
	Material* shadowMat = new Material(shadowprogram);
	shadowMat->SetTexture((char*)"tex", new Texture((char*)"Assets/BrickColor.png"));

	#pragma endregion

	//MSAA
	glEnable(GL_MULTISAMPLE);
	float totalTime = 0;
	// This is the game loop, the game logic and render part goes in here.
	// It checks if the created window is still open, and keeps performing the specified operations until the window is closed
	while (!glfwWindowShouldClose(window))
	{
		
		// Calculate delta time.
		float dt = glfwGetTime();
		totalTime += dt/4.0f;
		// Reset the timer.
		glfwSetTime(0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		// Specifies the RGBA values which will be used by glClear to clear the color buffer
		glClearColor(0.f, 0.f, 0.f, 1.0f);
	
	#pragma region Updates
		//Valued for our circling light
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.f);
		glm::vec3 lightPosition = glm::vec3(-3.5f * glm::cos(totalTime),2.0f, -3.5f* glm::sin(totalTime));
		glm::mat4 lightView = glm::lookAt(lightPosition,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		camera->Update(window, WIDTH, HEIGHT, mousePosition, dt);
	#pragma endregion
		//First pass
	#pragma region Depth_Pass
		//Set our viewport to the resoultion of our shadow texture
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		//Bind our shadow frame buffer object onto the current framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		//Clear the depth buffer so we can populate it with new values
		glClear(GL_DEPTH_BUFFER_BIT);

		//Set material with depth shader attached
		mainEntity->SetModelMaterial(depthMat);
		//Upload light matrix 
		mainEntity->UpdateMatrix((char*)"lightSpaceMatrix", lightSpaceMatrix);
		//Upload world matrix
		mainEntity->Update();

		//Draw for our first render pass
		mainEntity->DrawModel();
		//unbind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	#pragma endregion
		//Second Pass
	#pragma region Shadow_Pass
		glViewport(0, 0, WIDTH,HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Set material with shadow shader attached
		mainEntity->SetModelMaterial(shadowMat);
		//Upload camera matrix 
		mainEntity->UpdateMatrix((char*)"cameraMatrix", camera->GetCameraMatrix());
		//Upload light matrix 
		mainEntity->UpdateMatrix((char*)"lightSpaceMatrix", lightSpaceMatrix);
		//Upload world matrix
		mainEntity->Update();
		//Upload light position
		UpdateVec3((char*)"lightPos", lightPosition, shadowprogram);

		GLint uniform = glGetUniformLocation(shadowprogram->GetGLShaderProgram(), (char*)"shadowMap");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(uniform, 1);

		//Final render
		mainEntity->DrawModel();
	#pragma endregion

		// Swaps the front and back buffers of the specified window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete camera;
	delete mainEntity;
	delete mainMesh;
	delete depthMat;
	delete shadowMat;
	// Terminate all the stuff related to GLFW and exit the program using the return value EXIT_SUCCESS
	glfwTerminate();

	return EXIT_SUCCESS;
}


