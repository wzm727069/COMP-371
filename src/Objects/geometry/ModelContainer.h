#pragma once
#include <vector>
#include <string>
#include "Model.h"

/*
*Model Container 
*
*A container for many models, the model container allows many models to be rendered in the same method
*
*This class was created to avoid future rendering conflicts
*
*/
class ModelContainer 
{
public: 
	std::vector<Model*> models;
	AABB aabb;
	ModelContainer();

	Model* getModelByName(std::string name);
	void addModel(Model* model);
	void draw(int mode, Shader* shaderProg);
	void bindArrayBuffer();
	void deallocate();
	void calculateMinMax();
	void drawMod(int mode, Shader* shaderProg, glm::mat4 modelmat);

	
	glm::mat4 getTranslatedModelMatrix(glm::vec3 position);
	void Reposition(glm::vec3 position);
	void addRotation(float radians, glm::vec3 axis);
	void addRotationX(float radians);
	void addRotationY(float radians);
	void addRotationZ(float radians);
	void addScale(glm::vec3 scale);
	void addTranslation(glm::vec3 translate);
	void addRotation(float radians, glm::vec3 axis, std::string name);
	void setRotation(float radians, glm::vec3 axis);
	void addScale(glm::vec3 scale, std::string name); 
	void addShearMatrix(glm::vec2 shear, char axis);
	void addTranslation(glm::vec3 translate, std::string name);
	void setupShearMatrix();
	void resetShear();
	void setVertexController(bool position, bool texture, bool color, bool normal);
	void print();
	void translateToOrigin();

	void optimizeModels();

	glm::mat4 getShearMatrix();
	glm::mat4 getRotation();
	glm::mat4 getRotationX();
	glm::mat4 getRotationY();
	glm::mat4 getRotationZ();
	glm::mat4 getTranslation();
	glm::mat4 getScale();
	glm::mat4 getModelMatrix(bool shear = false);
	glm::mat4 getReposition();
  
	glm::vec3 shear_vec; 
	glm::vec3 rotate_vec;
	glm::vec3 translate_vec;
	glm::vec3 scale_vec;

	glm::mat4 shearMatrix;
	glm::vec2 shearX;
	glm::vec2 shearY;
	glm::vec2 shearZ;

	float rotate_angle, rotate_angleX, rotate_angleY, rotate_angleZ;
};