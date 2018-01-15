#include "GLWindow.h"

#include <iostream>

using namespace glm;
using namespace std;

#include "Drawable.h"
#include "SimpleGeometry.h"
#include "SimpleShader.h"
#include "ColorMat.h"
#include "TrackballCamera.h"
#include "SimpleTexManager.h"
#include "simpleTexShader.h"
#include "TextureCreation.h"
#include "TextureMat.h"
#include "MeshInfoLoader.h"
#include "ShadedMat.h"
#include "TorranceSparrowShader.h"
#include "Framebuffer.h"
//Ambient occlusion
#include "AOShader.h"
#include "PosNormalShader.h"
#include "PerlinNoise.h"
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>

using namespace renderlib;

TrackballCamera cam(
	vec3(0, 0, -1), vec3(0, 0, 1),
	glm::perspective(90.f*3.14159f/180.f, 1.f, 0.1f, 3.f));

bool windowResized = false;
int windowWidth, windowHeight;

void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {
	static vec2 lastPos = vec2(0.f, 0.f);
	
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	vec2 mousePos = vec2(float(xpos) / float(vp[2]), 
		float(-ypos) / float(vp[3]))*2.f
		- vec2(1.f, 1.f);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		vec2 diff = mousePos - lastPos;
		cam.trackballRight(-diff.x*3.14159f);
		cam.trackballUp(-diff.y*3.14159f);
	}

	lastPos = mousePos;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	windowResized = true;
	windowWidth = width;
	windowHeight = height;
}


WindowManager::WindowManager() :
window_width(800), window_height(800)
{
	if(!glfwInit()){
		printf("GLFW failed to initialize\n");
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	window = createWindow(window_width, window_height, 
		"Perlin Noise");

	glfwMakeContextCurrent(window);
	initGLExtensions();

	glfwSwapInterval(1);
	glfwSetCursorPosCallback(window, cursorPositionCallback);

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, window_width, window_height);
}

WindowManager::WindowManager(int width, int height, std::string name, glm::vec4 color) :
	window_width(width), window_height(height) 
{
	if(glfwInit()){
		printf("GLFW failed to initialize\n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	window = createWindow(window_width, window_height, name);
	glfwMakeContextCurrent(window);
	initGLExtensions();

	glClearColor(color.r, color.g, color.b, color.a);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, window_width, window_height);
}


void WindowManager::mainLoop() {

	glfwSetWindowSizeCallback(window, windowResizeCallback);

	SimpleTexManager tm;

	//Dragon
	ElementGeometry dragonGeom = objToElementGeometry("models/dragon.obj");
	Drawable dragon(
		new ColorMat(vec3(0.75f, 0.1f, 0.3f)),
		&dragonGeom);
	dragon.addMaterial(new ShadedMat(0.2f, 0.5f, 0.3f, 10.f));

	TorranceSparrowShader tsShader;

	vec3 lightPos(10.f, 10.f, 10.f);

	while (!glfwWindowShouldClose(window)) {

		if (windowResized) {
			window_width = windowWidth;
			window_height = windowHeight;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		tsShader.draw(cam, lightPos, dragon);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void initGLExtensions() {
#ifndef USING_GLEW
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD initialization failed" << std::endl;
	}
#else
	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "GLEW initialization failed" << std::endl;
	}
#endif
}


GLFWwindow *createWindow(int width, int height, std::string name)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(
		width, height, name.c_str(), nullptr, nullptr);
	
	if (window == NULL) {
		glfwTerminate();
		return nullptr;
	}
	else {
		glfwMakeContextCurrent(window);
		return window;
	}
}

