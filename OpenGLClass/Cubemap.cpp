#include "Cubemap.h"

Cubemap::Cubemap(std::string directories[], const unsigned int width, const unsigned int height) {
	Cubemap::SCR_WIDTH = width;
	Cubemap::SCR_HEIGHT = height;

	GLuint skyboxIndices[] = {
		// Right
		6, 5, 1,
		1, 2, 6,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	GLfloat skyboxVertices[] = {
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	Cubemap::VAO.Bind();
	GLuint VBO, EBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	std::vector<std::string> faces;
	faces.assign(directories, directories + 6);
	generateTexture();
	load(faces);
}

/*					loads a cubemap texture in order
	+X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back)		*/
void Cubemap::load(std::vector<std::string> faces) {
	// apply textures
	int width, height, colorChannels;
	for (int x = 0; x < faces.size(); x++) {
		unsigned char* data = stbi_load(faces[x].c_str(), &width, &height, &colorChannels, 0);
		if (data) {
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + x, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		} else {
			std::cout << "Textured failed to load from: " << faces[x] << std::endl;
			stbi_image_free(data);
		}
	}
}

void Cubemap::generateTexture() {
	glGenTextures(1, &(Cubemap::ID));
	glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap::ID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Cubemap::Draw(Shader& shader, Camera& camera) {
	glDepthFunc(GL_LEQUAL);
	shader.Activate();
	glm::mat4 viewpoint = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	viewpoint = glm::mat4(glm::mat3(glm::lookAt(camera.getCamPos(), camera.getCamPos() + camera.getCamOrientation(), camera.getCamUp())));
	projection = glm::perspective(glm::radians(45.0f), (float)Cubemap::SCR_WIDTH / Cubemap::SCR_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(viewpoint));
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// skybox cube
	Cubemap::VAO.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap::ID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

GLuint Cubemap::getID() {
	return Cubemap::ID;
}

void Cubemap::bindVAO() {
	Cubemap::VAO.Bind();
}

void Cubemap::activate(Shader& shader) {
	shader.Activate();
	glUniform1i(glGetUniformLocation(shader.getID(), "cubemap"), 0);
}