#pragma once

#include <string>
#include "Mesh.h"
#include "Material.h"

class Model
{
	Mesh* m_mesh;
	Material* m_material;
	std::string m_name;
	
public:
	Model(std::string name);
	Model(std::string name, Mesh* mesh, Material* material);
	~Model();

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	void UpdateMatrix(char* name, glm::mat4 matrix);
	void DrawMesh();
	
	
};

