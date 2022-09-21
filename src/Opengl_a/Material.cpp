#include "Material.h"
#include <iostream>






Material::Material()
{
	ka = glm::vec3(1.0f);
	kd = glm::vec3(1.0f);
	ks = glm::vec3(1.0f);
	ke = glm::vec3(1.0f);
	Ns = 10.00; //shininess
	Ni = 1.45; //optical density
	d = 1.0; //transparency
	illum = 2;
	type = TEXTURE_OFF;
}


Material::Material(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 ke, float Ns, float Ni, float d, int illum, MatType type) : ka(ka), kd(kd), ks(ks), ke(ke), Ns(Ns), Ni(Ni), d(d), illum(illum), type(type)
{
	
}

Material::Material(glm::vec3 kd, glm::vec3 ks, float Ns) : kd(kd), ks(ks), Ns(Ns)
{
	ka = glm::vec3(1.0f);
	ke = glm::vec3(1.0f);
	Ni = 1.45; //optical density
	d = 1.0; //transparency
	illum = 2;
	type = TEXTURE_OFF;
}

Material::Material(glm::vec3 ks, Texture map_kd, float Ns) : ks(ks), Ns(Ns)
{
	addTexture(map_kd);
	type = TEXTURE_D;
}




void Material::setShader(Shader* shader) 
{
	shader->use();
	if (type == TEXTURE_OFF) 
	{
		shader->setInt("loaded", (int)TEXTURE_OFF);
		shader->setFloat("noTex.shininess", Ns);
		shader->setVec3("noTex.specular", ks);
		shader->setVec3("noTex.diffuse", kd);
	}
	else if (type == TEXTURE_D) 
	{
		shader->setInt("loaded", (int)TEXTURE_D);
		shader->setFloat("material.shininess", Ns);
		shader->setVec3("material.specular", ks);
		for (std::vector<Texture>::iterator it = textures.begin(); it < textures.end(); it++)
		{
			if ((*it).type == "texture_diffuse")
			{
				shader->setInt("material.diffuse", (*it).renderer_id - 1);
			}
		}
	}
	else if (type == TEXTURE_DS) 
	{
		shader->setInt("loaded", (int)TEXTURE_DS);
		for (std::vector<Texture>::iterator it = textures.begin(); it < textures.end(); it++)
		{
			if ((*it).type == "texture_diffuse")
			{
				shader->setInt("assimpMat.diffuse", (*it).renderer_id - 1);
			}
			else if ((*it).type == "texture_specular")
			{
				shader->setInt("assimpMat.specular", (*it).renderer_id - 1);
			}
		}
		shader->setFloat("assimpMat.shininess", 225);
	}
	else 
	{
		std::cout << "invalid type " << std::endl;
	}
}



void Material::addTexture(Texture texture)
{
	textures.push_back(texture);
}

bool Material::equals(Material comp)
{
	return textures.size() == comp.textures.size() && type == comp.type && Ns == comp.Ns;
}

bool Material::equals(Material* comp)
{
	return textures.size() == comp->textures.size() && type == comp->type && Ns == comp->Ns;
}

