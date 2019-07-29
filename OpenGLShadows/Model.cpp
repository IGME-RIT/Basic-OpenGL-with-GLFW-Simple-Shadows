#include "Model.h"



Model::Model(std::string name)
{
	m_name = name;
}


Model::Model(std::string name, Mesh* mesh, Material* material)
{
	m_name = name;
	SetMesh(mesh);
	SetMaterial(material);
}

Model::~Model()
{

}

void Model::SetMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

void Model::SetMaterial(Material * material)
{
	m_material = material;
}

void Model::UpdateMatrix(char* name, glm::mat4 matrix)
{
	m_material->SetMatrix(name, matrix);	
}

void Model::DrawMesh()
{
	if (m_mesh == NULL)
	{
		std::cout << m_name << " mesh is null." << std::endl;
		return;
	}
	if (m_material == NULL)
	{
		std::cout << m_name << " material is null." << std::endl;
		return;
	}

		m_material->Bind();
		m_mesh->Draw();
		m_material->Unbind();

}
