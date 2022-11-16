#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Shader.h"

enum TextureType {
	DIFFUSE, SPECULAR, NORMAL, HEIGHT, DEFAULT
};

class Texture {
public:
	// constructor
	Texture(const char* image, TextureType type, GLuint slot);
	Texture(const char* image, TextureType type);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	void Delete();

	TextureType getType();
	std::string getPath();

private:
	GLuint ID;
	GLuint slot;
	TextureType type;
	std::string path;

	// get format of texture
	GLenum getFormat(int channels);

	// setup the texture given file location 
	void setupTexture(const char* image);
};
#endif