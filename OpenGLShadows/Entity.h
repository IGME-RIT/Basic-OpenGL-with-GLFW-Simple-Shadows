

#pragma once


#define GLEW_STATIC
#include <vector>
#include <string>
#include "Transform.h"
#include "Model.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//GLM provides new data types and math helper functions for any vector math we want to do.
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

class Entity
{
	
	Model* model;
	std::string name;

public:
	Transform* transform;
	Entity(std::string name);
	virtual ~Entity();


	void SetModelMesh(Mesh* mesh);
	void SetModelMaterial(Material* material);
	void UpdateMatrix(char* name, glm::mat4 matrix);
	void Update();
	void DrawModel();

	
};

