#pragma once
#include <string>

/*
*Texture 
*
*Description: provides a simple interface for creating textures and binding them to interfaces in opengl
*
*/
class Texture
{

public: 
	unsigned int renderer_id;
	unsigned char* localBuffer;
	int width, height, BPP;
	std::string filePath, id, type, path;
	bool bound;

	bool equals(Texture comp);
	void setBound(bool value);
	void TextureFromFile(const char* path);
	Texture(const std::string& path, std::string type  = "texture_diffuse");
	Texture();
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }


};