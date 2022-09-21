#include "ModelContainer.h"
#include <vector>
#include <string>
#include <glm/gtx/transform2.hpp>
#include <iostream>


void ModelContainer::bindArrayBuffer()
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->bindArrayBuffer(true, (*it));
	}
}

void ModelContainer::translateToOrigin()
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->translateToOrigin();
	}
}

void ModelContainer::addModel(Model* model) 
{
	models.push_back(model);
}

void ModelContainer::resetShear()
{
	shearX = glm::vec2(0.0f, 0.0f);
	shearY = glm::vec2(0.0f, 0.0f);
	shearZ = glm::vec2(0.0f, 0.0f);

	setupShearMatrix();
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->resetShear();
	}

}

ModelContainer::ModelContainer() 
{
	rotate_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	translate_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	scale_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate_angle = 0.0;
	rotate_angleX = 0.0;
	rotate_angleY = 0.0;
	rotate_angleZ = 0.0;

	shearX = glm::vec2(0.0f,0.0f);
	shearY = glm::vec2(0.0f, 0.0f);
	shearZ = glm::vec2(0.0f, 0.0f);

	setupShearMatrix();
	

}

//sets up the values of the shear matrix for each axis
void ModelContainer::setupShearMatrix()
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
	shearMatrix[3][3] = 0;
}
Model* ModelContainer::getModelByName(std::string name) 
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		if ((**it).name == name)
			return (*it);
	}
	return nullptr;
}

void ModelContainer::draw(int mode, Shader* shaderProg)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->draw(mode, shaderProg);
	}
}

void ModelContainer::drawMod(int mode, Shader* shaderProg, glm::mat4 modelmat)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->drawMod(mode, shaderProg, modelmat);
	}
}

void ModelContainer::deallocate()
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->deallocate();
	}
}


//Method that updates the values of the x-y-z components of the rotation vector used to calculate the model transformation matrix
void ModelContainer::addRotation(float degrees, glm::vec3 axis)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->addRotation(degrees, axis);
	}

	rotate_vec.x += axis.x;
	rotate_vec.y += axis.y;
	rotate_vec.z += axis.z;
	rotate_angle += degrees;
}

void ModelContainer::addRotation(float degrees, glm::vec3 axis, std::string name)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		if ((**it).name == name)
		{
			(*it)->addRotation(degrees, axis);
			break;
		}
	}
}

void ModelContainer::addRotationX(float degrees)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->addRotationX(degrees);
	}
	rotate_angleX += degrees;
}

void ModelContainer::addRotationY(float degrees)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->addRotationY(degrees);
	}
	rotate_angleY += degrees;
}

void ModelContainer::addRotationZ(float degrees)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->addRotationZ(degrees);
	}
	rotate_angleZ += degrees;
}

void ModelContainer::setRotation(float degrees, glm::vec3 axis)
{
	rotate_vec = axis;
	rotate_angle = degrees;

	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->setRotation(degrees, axis);
	}
}

glm::mat4 ModelContainer::getRotationX()
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angleX), glm::vec3(1.0, 0.0, 0.0));
}

glm::mat4 ModelContainer::getRotationY()
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angleY), glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 ModelContainer::getRotationZ()
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angleZ), glm::vec3(0.0, 0.0, 1.0));
}

//Method that updates the values of the x-y-z components of the scale vector used to calculate the model transformation matrix
void ModelContainer::addScale(glm::vec3 scale)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->addScale(scale);
	}

	if (scale_vec.x >= 0.0 && scale_vec.y >= 0.0 && scale_vec.z >= 0.0) {
		scale_vec.x += scale.x;
		scale_vec.y += scale.y;
		scale_vec.z += scale.z;
	}
	else {
		scale_vec.x = 0.2f;
		scale_vec.y = 0.2f;
		scale_vec.z = 0.2f;
	}

}

void ModelContainer::addScale(glm::vec3 scale, std::string name)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		if ((**it).name == name)
		{
			(*it)->addScale(scale);
			break;
		}
	}

}

//returns the shearing matrix
glm::mat4 ModelContainer::getShearMatrix()
{
	setupShearMatrix();
	return shearMatrix;
}

//Method that calculates the transformation matrix of the model
glm::mat4 ModelContainer::getModelMatrix(bool shear)
{
	return getTranslation() * getRotationX() * getRotationY() * getRotationZ() * getScale() * (shear ? getShearMatrix() : glm::mat4(1.0f));
}

//adds shears to model according to the axis passed, which will determine around which axis it will shear
void ModelContainer::addShearMatrix(glm::vec2 shear, char axis)
{
	if (axis == 'x')
	{
		for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
		{
			(*it)->addShearMatrix(shear, 'x');
		}
		shearX.x += shear.x;
		shearX.y += shear.y;
	}
	else if (axis == 'y')
	{
		for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
		{
			(*it)->addShearMatrix(shear, 'y');
		}
		shearY.x += shear.x;
		shearY.y += shear.y;
	}
	else if (axis == 'z')
	{
		for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
		{
			(*it)->addShearMatrix(shear, 'z');
		}
		shearZ.x += shear.x;
		shearZ.y += shear.y;
	}
}


//Method that updates the values of the x-y-z components of the translation vector used to calculate the model transformation matrix
void ModelContainer::addTranslation(glm::vec3 translate)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->addTranslation(translate);
	}

	ModelContainer::translate_vec.x += translate.x;
	ModelContainer::translate_vec.y += translate.y;
	ModelContainer::translate_vec.z += translate.z;
}

void ModelContainer::addTranslation(glm::vec3 translate, std::string name)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		if ((**it).name == name)
		{
			(*it)->addTranslation(translate);
			break;
		}
	}
}

//Method that returns the rotation matrix
glm::mat4 ModelContainer::getRotation()
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), rotate_vec);
}

//Method that returns the translation matrix
glm::mat4 ModelContainer::getTranslation()
{

	return glm::translate(glm::mat4(1.0f), translate_vec);
}

//Method that returns the scale matrix
glm::mat4 ModelContainer::getScale()
{
	return glm::scale(glm::mat4(1.0f), scale_vec);
}

//Method that updates the position of the model
void ModelContainer::Reposition(glm::vec3 position)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->Reposition(position);
	}

	ModelContainer::translate_vec.x = position.x;
	ModelContainer::translate_vec.y = position.y;
	ModelContainer::translate_vec.z = position.z;
}


glm::mat4 ModelContainer::getReposition()
{
	return glm::translate(glm::mat4(1.0f), translate_vec);
}



void ModelContainer::optimizeModels() 
{	
	std::vector<Model*> m;
	int mCounter = 0;
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
	{
		if (mCounter == 0)
		{
			m.push_back(*it);
			mCounter += 1;
		}
		else
		{
			bool found = false;
			for (std::vector<Model *>::iterator it2 = m.begin(); it2 < m.end(); it2++)
			{
				if ((*it2)->textureEquals(*it)) {
					(*it2)->addModel((**it));
					found = true;
					break;
				}
			}

			if (found == false) {

				m.push_back(*it);
			}

		}
	}

	std::cout << m.size() << std::endl;


	models = m;
}

void ModelContainer::setVertexController(bool position, bool texture, bool color, bool normal)
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
		(*it)->setVertexController(position, texture, color, normal);
}

void ModelContainer::print() 
{
	for (std::vector<Model *>::iterator it = models.begin(); it < models.end(); it++)
		(*it)->print();
}

glm::mat4 ModelContainer::getTranslatedModelMatrix(glm::vec3 position) 
{
	glm::vec3 mid((aabb.max.x + aabb.min.x) / 2, aabb.min.y, (aabb.max.z + aabb.min.z) / 2);
	glm::vec3 translate(position.x - mid.x, position.y - mid.y, position.z - mid.z);
	std::cout << "x: " << translate_vec.x << " y: " << translate_vec.y << " z: " << translate_vec.z << std::endl;
	return glm::translate(glm::mat4(1.0f), translate + translate_vec ) * getRotationX() * getRotationY() * getRotationZ() * getScale();
}

void ModelContainer::calculateMinMax()
{
	std::vector<Model *>::iterator it = models.begin();
	std::map<std::string, glm::vec3> map = (**it).getMinMax();
	it++;
	for (; it < models.end(); it++)
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
	aabb.max = glm::vec4(map["max"], 0.0f) * getModelMatrix();
	aabb.min = glm::vec4(map["min"], 0.0f) * getModelMatrix();

}

