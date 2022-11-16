#include <iostream>
#include <glad/glad.h>
#include <windows.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <filesystem>
#include <random>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Custom includes
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Cubemap.h"
#include "cube.h"
#include "Plane.h"
#include "framebuffer.h"
#include "terraingeneration.h"
#include "terrainoptions.h"

#pragma warning( disable : 26451 )
#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>

#define RED 0
#define GREEN 1
#define BLUE 2

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

unsigned int MSAAsamples = 16;;

VAO setupTerrain(const char* dir, unsigned int& totalStrips, unsigned int& totalTrianglesPerStrip, terrainOptions settings);

void drawTerrain(Shader & shader, Camera & camera, 
	VAO terrainVAO, Texture texture, unsigned int totalStrips, unsigned int totalTrianglesPerStrip);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}

void drawIMGUI() {
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static float f = 0.0f;

	ImGui::Begin("Terrain Generator");  // Create a window and append into it.

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void drawIMGUICustom(const char* name) {
	ImGui::SetNextWindowPos(ImVec2(0.0f, SCR_HEIGHT/2));
	ImGui::Begin(name);
}

void changeSizeIMGUI(Shader& shader, float size) {
	glUseProgram(shader.getID());
	glUniform1f(glGetUniformLocation(shader.getID(), "size"), size);
}

void activateAll(int lightOptions, std::vector<Shader> shaders, std::vector<glm::mat4> models, glm::vec4 lightColor, glm::vec3 lightPos) {
	int count = 0;
	for (Shader shader : shaders) {
		shader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(models[count]));
		glUniform4f(glGetUniformLocation(shader.getID(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shader.getID(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform1i(glGetUniformLocation(shader.getID(), "lightOption"), lightOptions);
		count++;
	}
}

void renderModels(std::vector<Model> models, Shader& shader, Camera& camera) {
	for (Model model : models) {
		model.Draw(shader, camera);
	}
}

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, MSAAsamples);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain Generator", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// setup IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Data
	std::vector<Texture> textures;
	std::vector<Model> allModels;
	std::vector<glm::mat4> models;
	std::vector<Shader> shaders;

	// ==================================== CUBEMAP ====================================

	std::string parentCubemapDir = "Resources/Cubemaps";
	//					loads a cubemap texture in order
	// +X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back)
	std::string spaceDir[] = {
		parentCubemapDir + "/blue/right.png",
		parentCubemapDir + "/blue/left.png",
		parentCubemapDir + "/blue/top.png",
		parentCubemapDir + "/blue/bot.png",
		parentCubemapDir + "/blue/front.png",
		parentCubemapDir + "/blue/back.png"
	};

	Shader skyboxShader("cubemap.vert", "cubemap.frag");
	Cubemap cubemap(spaceDir, SCR_WIDTH, SCR_HEIGHT);

	// ==================================== LIGHT ====================================

	// Shader for light object
	Shader lightShader("light.vert", "light.frag");
	Cube box;
	Transform light(box.cubeVertices(), box.cubeIndices(), textures);
	Mesh lightMesh(light);

	// Light model
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(60.5f, 62.5f, 60.2f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	// add to data
	shaders.push_back(lightShader);
	models.push_back(lightModel);

	// ==================================== MODEL ====================================

	// Shader for model
	Shader modelShader("default.vert", "default.frag");
	Model moon("Moon.obj", "Resources/Moon/");

	allModels.push_back(moon);

	// model model
	glm::vec3 modelPos = glm::vec3(0.0f, 2.0f, 10.0f);
	glm::mat4 modelModel = glm::mat4(1.0f);
	modelModel = glm::translate(modelModel, modelPos);
	changeSizeIMGUI(modelShader, 10.0f);

	// add to data
	shaders.push_back(modelShader);
	models.push_back(modelModel);

	// ==================================== TERRAIN ====================================
	// Shader/Model for terrain
	Shader terrainShader("terrain.vert", "terrain.frag");
	glm::vec3 terrainPos = glm::vec3(0.0f, -10.0f, 0.0f);
	glm::mat4 terrainModel = glm::mat4(1.0f);
	terrainModel = glm::translate(terrainModel, terrainPos);

	// Terrain texture
	Texture terrainTexture("Resources/mars.jpg", DIFFUSE, 0);
	
	// add to data
	shaders.push_back(terrainShader);
	models.push_back(terrainModel);

	// setup terrain
	std::string parentDirMaps = "resources/heightmaps";
	unsigned int totalStrips = 0;
	unsigned int totalTrianglesPerStrip = 0;
	VAO terrainVAO;
	// set size to 1
	changeSizeIMGUI(terrainShader, 1.0);



	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Creates camera object
	Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.75f, 6.0f));

	bool generate = false;
	bool wireframe = false;
	bool procedural = false;
	// Camera initialization
	float camRotationSpeed = 0.5f;
	float camSpeed = 0.01f;
	float camFOVdeg = 45.0f;
	float camNearPlane = 0.1f;
	float camFarPlane = 450.0f;

	terrainOptions settings = {};
	settings.size = 1;
	settings.ySteep = 0.15f; 
	settings.minRandomAmount = -128;
	settings.maxRandomAmount = 128;
	settings.yHeight = 100.0f;
	settings.algorithm = DIAMONDSQUARE;
	// very common configuration for fractal noise
	settings.frequency = 0.05f;
	settings.octaves = 3;
	settings.persistence = 0.5f;
	settings.lacunarity = 2.0f;

	const char* result_map = "map.png";

	// Main while loop
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.85f, 0.85f, 0.90f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateCameraMatrix(camFOVdeg, camNearPlane, camFarPlane);

		// IMGUI
		drawIMGUI();
		ImGui::Text("Settings/Menu");

		// lights
		static const char* choices[]{ "Point Light", "Directional Light", "Spot Light" };
		static int selected = 0;
		ImGui::Combo("Light Choice", &selected, choices, IM_ARRAYSIZE(choices));
		activateAll(selected, shaders, models, lightColor, lightPos);

		// ================ DRAWING ==============

		lightMesh.Draw(lightShader, camera);

		// Draw models
		renderModels(allModels, modelShader, camera);
	
		// Draw Cubemap
		cubemap.Draw(skyboxShader, camera);

		ImGui::Checkbox("Wireframe Mode", &wireframe);
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// ================ Generating/Drawing/Options Terrain ==============

		ImGui::Checkbox("Procedural Generation", &procedural);

		// Default Terrain Options
		ImGui::SliderFloat("Scale of Y (Magnitude/Steepness)", &settings.ySteep, 1.0f, 5.0f);
		ImGui::SliderFloat("Height", &settings.yHeight, 5.0f, 1000.0f);
		ImGui::SliderInt("Subdivisions in Generation", &settings.size, 1, 5);

		// choose between procedural terrains or manual/image-based terrains
		if (!procedural) {
			static const char* choicesMap[]{ "Default Map", "Australia Heightmap", "USA Heightmap" };
			static int selectedMap = 0;
			ImGui::Combo("Heightmap Choice", &selectedMap, choicesMap, IM_ARRAYSIZE(choicesMap));

			switch (selectedMap) {
			case 0:
				result_map = "/map.png";
				break;
			case 1:
				result_map = "/australia.png";
				break;
			case 2:
				result_map = "/usa.png";
				break;
			default:
				break;
			}
			settings.algorithm = NONE;
		} else {
			static const char* choicesAlgo[]{ "Diamond Square", "Fractal Noise" };
			static int selectedAlgorithm = 0;
			ImGui::Combo("Algorithm Choice", &selectedAlgorithm, choicesAlgo, IM_ARRAYSIZE(choicesAlgo));

			switch (selectedAlgorithm) {
			case 0:
				settings.algorithm = DIAMONDSQUARE;
				break;
			case 1:
				settings.algorithm = FRACTAL;
				break;
			default:
				break;
			}
			
			// Algorithmic Options

			ImGui::SliderInt("Minimum Random Num (For algorithms)", &settings.minRandomAmount, -200, 200);
			ImGui::SliderInt("Maximum Random Num (For algorithms)", &settings.maxRandomAmount, -200, 200);

			// Fractal Noise Only
			if (selectedAlgorithm == 1) {
				// Fractal Options
				ImGui::SliderFloat("Frequency (Fractal)", &settings.frequency, 0.0, 1.0);
				ImGui::SliderInt("Octaves (Fractal)", &settings.octaves, 0, 10);
				ImGui::SliderFloat("Lacunarity (Fractal)", &settings.lacunarity, 0.0, 10.0);
				ImGui::SliderFloat("Persistence (Fractal)", &settings.persistence, 0.0, 5.0);
			}
		}

		if (ImGui::Button("Generate Terrain")) {
			terrainVAO = setupTerrain((parentDirMaps + result_map).c_str(), totalStrips, totalTrianglesPerStrip, settings);
			generate = true;
		}

		// Draw Terrain
		if (generate)
			drawTerrain(terrainShader, camera, terrainVAO, terrainTexture, totalStrips, totalTrianglesPerStrip);

		ImGui::End();

		// camera options
		drawIMGUICustom("Camera Settings");

		// camera rotation speed
		ImGui::Text("Settings for the Camera");
		ImGui::SliderFloat("Camera Speed", &camSpeed, 0.001f, 0.1f);
		ImGui::SliderFloat("Camera Rotation Speed", &camRotationSpeed, 0.01f, 1.0f);
		ImGui::SliderFloat("Camera FOV degree", &camFOVdeg, 0.0f, 180.0f);
		ImGui::SliderFloat("Camera Near Plane", &camNearPlane, 0.1f, 100.0f);
		ImGui::SliderFloat("Camera Far Plane", &camFarPlane, 100.0f, 1000.0f);

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Events
		camera.Inputs(window, camRotationSpeed, camSpeed);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// Deleting/Cleaning up
	for (Shader shader : shaders) {
		shader.Delete();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}