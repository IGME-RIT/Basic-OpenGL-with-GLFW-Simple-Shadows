#include "Entity.h"





Entity::Entity(std::string name)
{
	transform = new Transform();
	this->name = name;
}


Entity::~Entity()
{
	delete transform;
	delete model;

}

void Entity::SetModelMesh(Mesh* mesh)
{
	if (model == NULL)
		model = new Model(name);

	model->SetMesh(mesh);
}

void Entity::SetModelMaterial(Material * material)
{
	if (model == NULL)
		model = new Model(name);

	model->SetMaterial(material);
}

void Entity::UpdateMatrix(char * name, glm::mat4 matrix)
{
	if (model != NULL)
		model->UpdateMatrix(name, matrix);
}

void Entity::Update()
{
	transform->Update();

	if (model != NULL)
		UpdateMatrix((char*)"worldMatrix", transform->GetWorldMatrix());
	
}

void Entity::DrawModel()
{
	if (model == NULL)
	{
		std::cout << name << " model is null." << std::endl;
		return;
	}

	model->DrawMesh();
}


