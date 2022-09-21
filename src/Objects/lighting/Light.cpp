#include "Light.h"

#include <string>
#include <iostream>

// Default Constructor
Light::Light() {}

// Directional
Light::Light(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
{
	Light::direction = dir;

	Light::ambient = amb;
	Light::diffuse = diff;
	Light::specular = spec;
}

// SpotLight (Default)
Light::Light(glm::vec3 startingPos, bool active)
{
	Light::direction = glm::vec3(-5.75f, 0.0f, 5.0f);
	Light::position = startingPos;

	Light::ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	Light::diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	Light::specular = glm::vec3(0.1f, 0.1f, 0.1f);

	Light::constant = 1.0f;
	Light::linear = 0.0014f;
	Light::quadratic = 0.000007f;

	Light::active = active;
}

// SpotLight (Parameterized)
Light::Light(glm::vec3 startingPos, glm::vec3 dir, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec, float line, float quad, bool active)
{
	Light::position = startingPos;
	Light::direction = dir;

	Light::ambient = amb;
	Light::diffuse = dif;
	Light::specular = spec;

	Light::constant = 1.0f;
	Light::linear = line;
	Light::quadratic = quad;

	Light::active = active;
}

// Deconstructor
Light::~Light() {};

void Light::setFlashLightShaderValues(Shader * shader, Camera * camera)
{
	shader->setVec3("spotLight.position", camera->position);
	shader->setVec3("spotLight.direction", camera->front);
	shader->setVec3("spotLight.ambient", ambient.x, ambient.y, ambient.z);
	shader->setVec3("spotLight.diffuse", diffuse.x, diffuse.y, diffuse.z);
	shader->setVec3("spotLight.specular", specular.x, specular.y, specular.z);
	shader->setFloat("spotLight.constant", constant);
	shader->setFloat("spotLight.linear", linear);
	shader->setFloat("spotLight.quadratic", quadratic);
	shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}

// Sets all the values in the fragment shader specific to that light
void Light::setShaderValues(Shader * shader, bool isSpotLight)
{
	if (isSpotLight)
	{
		shader->setVec3("spotLight.position", position);
		shader->setVec3("spotLight.direction", direction);
		shader->setVec3("spotLight.ambient", ambient.x, ambient.y, ambient.z);
		shader->setVec3("spotLight.diffuse", diffuse.x, diffuse.y, diffuse.z);
		shader->setVec3("spotLight.specular", specular.x, specular.y, specular.z);
		shader->setFloat("spotLight.constant", constant);
		shader->setFloat("spotLight.linear", linear);
		shader->setFloat("spotLight.quadratic", quadratic);
		shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	}
	else
	{
		shader->setVec3("dirLight.direction", direction);
		shader->setVec3("dirLight.ambient", ambient.x, ambient.y, ambient.z);
		shader->setVec3("dirLight.diffuse", diffuse.x, diffuse.y, diffuse.z);
		shader->setVec3("dirLight.specular", specular.x, specular.y, specular.z);
	}
}