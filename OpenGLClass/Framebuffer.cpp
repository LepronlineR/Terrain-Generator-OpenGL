#include "Framebuffer.h"

Framebuffer::Framebuffer(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT) {
	
	Shader frameBufferShader("framebuffer.vert", "framebuffer.frag");
	Framebuffer::FBShader = frameBufferShader;

	// frame buffer
	glGenFramebuffers(1, &(Framebuffer::ID));
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer::ID);

	// texture
	glGenBuffers(1, &(Framebuffer::Texture));
	glBindTexture(GL_TEXTURE_2D, Framebuffer::Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	// set parameter closest
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Set parameter to wrap and clamp
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Framebuffer::Texture, 0);
	
	// render buffer
	glGenRenderbuffers(1, &(Framebuffer::RBO));
	glBindBuffer(GL_RENDERBUFFER, Framebuffer::RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Framebuffer::RBO);

	switch (auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cout << "FRAMEBUFFER ERROR: (INCOMPLETE ATTACHMENT)\n" << result << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "FRAMEBUFFER ERROR: (INCOMPLETE DRAW BUFFER)\n" << result << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		std::cout << "FRAMEBUFFER ERROR: (INCOMPLETE LAYER TARGETS)\n" << result << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cout << "FRAMEBUFFER ERROR: (INCOMPLETE MISSING ATTACHMENT)\n" << result << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		std::cout << "FRAMEBUFFER ERROR: (INCOMPLETE MULTISAMPLE)\n" << result << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cout << "FRAMEBUFFER ERROR: (INCOMPLETE READ BUFFER)\n" << result << std::endl;
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		break;
	default:
		std::cout << "FRAMEBUFFER ERROR: (UNKNOWN ERROR)\n" << result << std::endl;
		break;
	}
}

void Framebuffer::Initialize() {
	Cube box;
	std::vector<Vertex> boxVert = box.cubeVertices();
	std::vector<GLuint> boxInd = box.cubeIndices();
	Transform t(boxVert, boxInd);
	Framebuffer::transform = t;
	Framebuffer::VAO.Bind();
	VBO VBO(Framebuffer::transform);
	EBO EBO(Framebuffer::transform);
	Framebuffer::VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	Framebuffer::VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	Framebuffer::VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	Framebuffer::VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	Framebuffer::VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();

	Framebuffer::FBShader.Activate();
	glUniform1i(glGetUniformLocation(Framebuffer::FBShader.getID(), "screen"), 0);
}

void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer::ID);
	glEnable(GL_DEPTH_TEST);
}

void Framebuffer::Draw() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	FBShader.Activate();
	Framebuffer::VAO.Bind();
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, Framebuffer::transform.getIndices().size(), GL_UNSIGNED_INT, 0);
}

Shader Framebuffer::getShader() {
	return Framebuffer::FBShader;
}