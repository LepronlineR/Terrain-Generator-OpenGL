#include "Texture.h"

Texture::Texture(const char* image, TextureType type, GLuint slot) {
	Texture::type = type;
	Texture::slot = slot;
	Texture::path = image;
	glGenTextures(1, &(Texture::ID));

	setupTexture(image);
}

Texture::Texture(const char* image, TextureType type) {
	Texture::type = type;
	Texture::slot = 0;
	Texture::path = image;
	glGenTextures(1, &(Texture::ID));

	setupTexture(image);
}

void Texture::setupTexture(const char* image) {
	int width, height, colorChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(image, &width, &height, &colorChannels, 0);
	if (data) {
		glActiveTexture(GL_TEXTURE0 + Texture::slot);
		glBindTexture(GL_TEXTURE_2D, Texture::ID);

		// set parameter closest
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Set parameter to wrap and repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture
		GLenum format = getFormat(colorChannels);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Textured failed to load from: " << image << std::endl;
	}

	// Clean up and bind
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	shader.Activate();
	glUniform1i(glGetUniformLocation(shader.getID(), uniform), unit);
}

void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, Texture::ID);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &(Texture::ID));
}

TextureType Texture::getType() {
	return Texture::type;
}

std::string Texture::getPath() {
	return Texture::path;
}

GLenum Texture::getFormat(int channels) {
	switch (channels) {
	case 1:
		return GL_RED;
	case 3:
		return GL_RGB;
	case 4:
		return GL_RGBA;
	default:
		std::cout << "ERROR: texture format error" << std::endl;
		break;
	}
	return NULL;
}