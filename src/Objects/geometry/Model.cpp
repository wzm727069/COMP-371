/*
Model

Depends on: Polygon.h

Description: A container for groups of related geometry. It handles the creation of 
arbitrarily sized vertex arrays from the polgons that it contains.

origin: the point of origin for this particular polygon
vaComponentCount: the number of vertex components in the Polygons of the model
vaByteSize: the number Bytes required to contain the vertices of all the polygons in memory

*/

#pragma once

#include "Model.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Common.h"
#include <GL/glew.h> 
#include "../../utils/GL_Error.h"
#include "../../Opengl_a/Texture.h"
#include "../../Opengl_a/Material.h"
#include <glm/gtx/transform2.hpp>

//Model constructor, setting up position, texture and color components
Model::Model(bool position, bool texture, bool color, bool normal, std::string name, Shader* shader, int textureIndex)
{
	Model::textureIndex = textureIndex;
	Model::shader = shader;
	Model::name = name;
	Model::position = position;
	Model::texture = texture;
	Model::color = color;
	Model::normal = normal;
	Model::material = &g_materials[textureIndex];
	
	origin = glm::vec3(0.0f);
	rotate_vec = glm::vec3(0.0f, 0.0f, 1.0f);
	translate_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	scale_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate_angle = 0.0;
	aabb.min = glm::vec4(0.0f);
	aabb.max = glm::vec4(0.0f);
	rotate_angleX = 0.0;
	rotate_angleY = 0.0;
	rotate_angleZ = 0.0;
	shearX = glm::vec2(0.0f, 0.0f);
	shearY = glm::vec2(0.0f, 0.0f);
	shearZ = glm::vec2(0.0f, 0.0f);

	setupShearMatrix();
}

Model::Model(bool position, bool texture, bool color, bool normal, std::string name, Shader* shader, Material* material)
{
	Model::textureIndex = textureIndex;
	Model::shader = shader;
	Model::name = name;
	Model::position = position;
	Model::texture = texture;
	Model::color = color;
	Model::normal = normal;
	Model::material = material;

	origin = glm::vec3(0.0f);
	rotate_vec = glm::vec3(0.0f, 0.0f, 1.0f);
	translate_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	scale_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate_angle = 0.0;
	aabb.min = glm::vec4(0.0f);
	aabb.max = glm::vec4(0.0f);
	rotate_angleX = 0.0;
	rotate_angleY = 0.0;
	rotate_angleZ = 0.0;
	shearX = glm::vec2(0.0f, 0.0f);
	shearY = glm::vec2(0.0f, 0.0f);
	shearZ = glm::vec2(0.0f, 0.0f);

	setupShearMatrix();
}

//default constructor
Model::Model() 
{
	Model::textureIndex = 0;
	Model::shader = nullptr;
	Model::name = "default";
	Model::position = true;
	Model::texture = false;
	Model::color = false;
	Model::normal = false;
	origin = glm::vec3(0.f);
	rotate_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	scale_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate_angle = 0.0;
	rotate_angleX = 0.0;
	rotate_angleY = 0.0;
	rotate_angleZ = 0.0;

	aabb.min = glm::vec4(0.0f);
	aabb.max = glm::vec4(0.0f);

	shearX = glm::vec2(0.0f, 0.0f);
	shearY = glm::vec2(0.0f, 0.0f);
	shearZ = glm::vec2(0.0f, 0.0f);

	setupShearMatrix();
}

void Model::resetShear() 
{
	shearX = glm::vec2(0.0f, 0.0f);
	shearY = glm::vec2(0.0f, 0.0f);
	shearZ = glm::vec2(0.0f, 0.0f);
	setupShearMatrix();
}

//sets up the values of the shear matrix for each axis
void Model::setupShearMatrix()
{
	shearMatrix[0][0] = 1;
	shearMatrix[0][1] = shearZ.y;
	shearMatrix[0][2] = shearY.y;
	shearMatrix[0][3] = 0;

	shearMatrix[1][0] = shearZ.x;
	shearMatrix[1][1] = 1;
	shearMatrix[1][2] = shearX.y;
	shearMatrix[1][3] = 0;

	shearMatrix[2][0] = shearY.x;
	shearMatrix[2][1] = shearX.x;
	shearMatrix[2][2] = 1;
	shearMatrix[2][3] = 0;

	shearMatrix[3][0] = 0;
	shearMatrix[3][1] = 0;
	shearMatrix[3][2] = 0;
	shearMatrix[3][3] = 1;
}
void Model::setBoolean(bool position, bool texture, bool color, bool normal) 
{
	Model::position = position;
	Model::texture = texture;
	Model::color = color;
	Model::normal = normal;

	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		(**it).setVertexController(position, texture, color, normal);
	}

}

//Method that updates the values of the x-y-z components of the rotation vector used to calculate the model transformation matrix
void Model::addRotation(float degrees, glm::vec3 axis) 
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		if (dynamic_cast<Model*>(*it) != NULL)
			dynamic_cast<Model*>(*it)->addRotation(degrees, axis);
	}

	rotate_vec.x += axis.x;
	rotate_vec.y += axis.y;
	rotate_vec.z += axis.z;
	rotate_angle += degrees;

	// Update AABB (Collision)
	setAABB();
}

void Model::setRotation(float degrees, glm::vec3 axis)
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		if (dynamic_cast<Model*>(*it) != NULL)
			dynamic_cast<Model*>(*it)->setRotation(degrees, axis);
	}

	rotate_vec = axis;
	rotate_angle = degrees;
}

void Model::addRotationX(float degrees)
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		if (dynamic_cast<Model*>(*it) != NULL)
			dynamic_cast<Model*>(*it)->addRotationX(degrees);
	}

	rotate_angleX += degrees;
}

void Model::addRotationY(float degrees)
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		if (dynamic_cast<Model*>(*it) != NULL)
			dynamic_cast<Model*>(*it)->addRotationY(degrees);
	}

	rotate_angleY += degrees;
}

void Model::addRotationZ(float degrees)
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		if (dynamic_cast<Model*>(*it) != NULL)
			dynamic_cast<Model*>(*it)->addRotationZ(degrees);
	}

	rotate_angleZ += degrees;
}

//Method that updates the values of the x-y-z components of the scale vector used to calculate the model transformation matrix
void Model::addScale(glm::vec3 scale)
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		if (dynamic_cast<Model*>(*it) != NULL)
			dynamic_cast<Model*>(*it)->addScale(scale);
	}

	if (scale_vec.x >= 0.0 && scale_vec.y >= 0.0 && scale_vec.z >= 0.0) {
		scale_vec.x += scale.x;
		scale_vec.y += scale.y;
		scale_vec.z += scale.z;
	}

	// Update AABB (Collision)
	setAABB();
}

//Method that updates the values of the x-y-z components of the translation vector used to calculate the model transformation matrix
void Model::addTranslation(glm::vec3 translate)
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		if (dynamic_cast<Model*>(*it) != NULL)
			dynamic_cast<Model*>(*it)->addTranslation(translate);
	}



	Model::translate_vec.x += translate.x;
	Model::translate_vec.y += translate.y;
	Model::translate_vec.z += translate.z;

	// Update AABB (Collision)
	setAABB();
}

//adds shears to model according to the axis passed, which will determine around which axis it will shear
void Model::addShearMatrix(glm::vec2 shear, char axis)
{
	if (axis == 'x') {
		for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
		{
			if (dynamic_cast<Model*>(*it) != NULL)
				dynamic_cast<Model*>(*it)->addShearMatrix(shear, axis);
		}
		shearX.x += shear.x;
		shearX.y += shear.y;
	}
	else if (axis == 'y')
	{
		for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
		{
			if (dynamic_cast<Model*>(*it) != NULL)
				dynamic_cast<Model*>(*it)->addShearMatrix(shear, axis);
		}
		shearY.x += shear.x;
		shearY.y += shear.y;
	}
	else if (axis == 'z')
	{
		for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
		{
			if (dynamic_cast<Model*>(*it) != NULL)
				dynamic_cast<Model*>(*it)->addShearMatrix(shear, axis);
		}
		shearZ.x += shear.x;
		shearZ.y += shear.y;
	}
}

//Method that updates the position of the model
void Model::Reposition(glm::vec3 position)
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		if (dynamic_cast<Model*>(*it) != NULL)
			dynamic_cast<Model*>(*it)->Reposition(position);
	}

	Model::translate_vec.x = position.x;
	Model::translate_vec.y = position.y;
	Model::translate_vec.z = position.z;

	// Update AABB (Collision)
	setAABB();
}

//Method that returns the rotation matrix
glm::mat4 Model::getRotation(float angle, glm::vec3 rotateVec)
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotateVec);
}


//Method that returns the rotation matrix
glm::mat4 Model::getRotationX()
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angleX), glm::vec3(1.0, 0.0, 0.0));
}

glm::mat4 Model::getRotationY()
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angleY), glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Model::getRotationZ()
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angleZ), glm::vec3(0.0, 0.0, 1.0));
}


//Method that returns the translation matrix
glm::mat4 Model::getTranslation() 
{

	return glm::translate(glm::mat4(1.0f), translate_vec);
}

glm::mat4 Model::getReposition()
{
	return glm::translate(glm::mat4(1.0f), translate_vec);
}

//Method that returns the scale matrix
glm::mat4 Model::getScale() 
{
	return glm::scale(glm::mat4(1.0f), scale_vec);
}

//returns shear matrix
glm::mat4 Model::getShearMatrix()
{
	setupShearMatrix();
	return shearMatrix;
}


//Method that calculates the transformation matrix of the model
glm::mat4 Model::getModelMatrix(bool shear)
{

	return getTranslation() * getRotationX() * getRotationY() * getRotationZ() * getScale() * (shear ? getShearMatrix() : glm::mat4(1.0f));
}

//Method that adds a polygon object to the list of polygons that this model is composed of
void Model::addPolygon(Polygon* poly) 
{
	poly->setBoolean(position, texture, color, normal);
	polygons.push_back(poly);
}

//Method that sets the position, texture and color components
void Model::setVertexController(bool position, bool texture, bool color, bool normal) 
{
	Model::position = position;
	Model::texture = texture;
	Model::color = color;
	Model::normal = normal;

	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		(**it).setVertexController(position, texture, color, normal);
	}
}

//Method that returns the sample vertex of the polygon list
Vertex Model::getSampleVertex() 
{
	return polygons.front()->getSampleVertex();
}

//Method that returns the vertex array float count
int Model::getVAFloatCount()
{
	vaComponentCount = 0;
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++) 
	{
		vaComponentCount += (**it).getVAFloatCount();
	}
	return vaComponentCount;
}

//Method that applies the passed tranformation matrix to every polygon in the list
void Model::transform(glm::mat4 transmat)
{
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		(**it).transform(transmat);
	}
}

//Method that retusn the vertex array byte size
int Model::getVAByteSize()
{
	vaByteSize = 0;
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		vaByteSize += (**it).getVAByteSize();
	}

	return vaByteSize;
}

//Method that returns the vertex array count
int Model::getVAVertexCount() 
{
	int vertexCount = 0;
	for (std::vector<Polygon *>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		vertexCount += (**it).getVAVertexCount();
	}
	return vertexCount;
}

//Method that returns the array of vertices that make each polygon
float* Model::getVertexArray()
{
	float* va = new float[getVAFloatCount()];
	int filled = 0;
	for (std::vector<Polygon*>::iterator it = polygons.begin(); it < polygons.end(); it++)
	{
		int vertexCount = (**it).getVAFloatCount();
		float* a = (**it).getVertexArray();
		std::copy(a, a + vertexCount, va + filled);
		filled += vertexCount;
	}
	return va;

}

//Method that returns the origin coordinate of a model
std::map<std::string, glm::vec3> Model::getMinMax()
{
	std::vector<Polygon*>::iterator it = polygons.begin();
	std::map<std::string, glm::vec3> map = (**it).getMinMax();
	it++;

	for (; it < polygons.end(); it++)
	{
		std::map<std::string, glm::vec3> temp = (**it).getMinMax();
		if (map["max"].x < temp["max"].x)
			map["max"].x = temp["max"].x;

		if (map["max"].y < temp["max"].y)
			map["max"].y = temp["max"].y;

		if (map["max"].z < temp["max"].z)
			map["max"].z = temp["max"].z;

		if (map["min"].x > temp["min"].x)
			map["min"].x = temp["min"].x;

		if (map["min"].y > temp["min"].y)
			map["min"].y = temp["min"].y;

		if (map["min"].z > temp["min"].z)
			map["min"].z = temp["min"].z;
	}

	return map;
}

//Method that returns the vertex byte size
int Model::getVertexByteSize() 
{
	return polygons.front()->getVertexByteSize();
}

//Method that translates the model to its origin
void Model::translateToOrigin()
{
	
	std::map<std::string, glm::vec3> map;
	map = getMinMax();

	glm::vec3 temp;
	temp.x = -(map["min"].x + map["max"].x) / 2;
	temp.y = -(map["min"].y + map["max"].y) / 2;
	temp.z = -(map["min"].z + map["max"].z) / 2;

	transform(glm::translate(glm::mat4(1.0f), temp));
	
}

void Model::draw(int mode, Shader* shaderProg)
{
	shaderProg->use();
	this->bind();
	this->material->setShader(shaderProg);
	

	if (textureIndex == -1)
	{
	shaderProg->setInt("fill", textureIndex);
	}

	shaderProg->setMat4("model", this->getModelMatrix(false));
	GLCall(glDrawArrays(mode, 0, this->getVAVertexCount()));

}

void Model::drawMod(int mode, Shader* shaderProg, glm::mat4 modelmat)
{
	shaderProg->use();
	this->bind();
	this->material->setShader(shaderProg);


	if (textureIndex == -1)
	{
		shaderProg->setInt("fill", textureIndex);
	}

	shaderProg->setMat4("model", modelmat);
	GLCall(glDrawArrays(mode, 0, this->getVAVertexCount()));

}


void Model::setAABB()
{
	std::map<std::string, glm::vec3> map = getMinMax();

	aabb.max = glm::vec4(map["max"], 0.0f) * getModelMatrix();// +glm::vec4(translate_vec, 0.0f);
	aabb.min = glm::vec4(map["min"], 0.0f) * getModelMatrix();// +glm::vec4(translate_vec, 0.0f);
}

void Model::insertTextures(std::vector<Texture> tex)
{
	textures.insert(textures.end(), tex.begin(), tex.end());
}


void Model::addModel(Model m) 
{
	polygons.insert(polygons.end(), m.polygons.begin(), m.polygons.end());
}

void Model::print()
{
	for (std::vector<Polygon*>::iterator it = polygons.begin(); it < polygons.end(); it++)
		(*it)->print();
}

bool Model::textureEquals(Model comp) 
{
	if (textures.size() != comp.textures.size())
		return false;
	if (comp.material == nullptr)
		return false; 

	if(!material->equals(comp.material))
		return false;

	bool isEqual = true;
	std::vector<Texture>::iterator it = textures.begin();
	std::vector<Texture>::iterator it2 = comp.textures.begin();
	for (; it < textures.end(); it++, it2++) 
	{
		if (!(*it).equals((*it2)))
			isEqual = false;
	}
	return isEqual;
}

