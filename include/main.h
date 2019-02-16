#ifndef MAIN_H
#define MAIN_H

//------------------------------------------------------------------------------------------
// ____ INCLUDE ______
//------------------------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <stdlib.h>	// srand i rand
#include <time.h>	// do wywolania srandu

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <ShaderProgram.h>
#include <Texture2D.h>
#include <Camera.h>
#include <Mesh.h>

//------------------------------------------------------------------------------------------
// ____ ZMIENNE GLOBALNE ______
//------------------------------------------------------------------------------------------

int gWindowWidth;
int gWindowHeight;
bool gFullScreen;
bool gWireframe;
bool gFlashlightOn;
GLFWwindow* gWindow;

//OrbitCamera orbitCamera;
//float gYaw;
//float gPitch;
//float gRadius;
//const float MOUSE_SENSITIVITY = 0.25f;

FPSCamera fpsCamera(glm::vec3(0.0f, 3.0f, 10.0f), 0.0f);
const double ZOOM_SENSITIVITY = -1.0;
const float MOVE_SPEED = 5.0; // jednostek na sekunde
const float MOUSE_SENSITIVITY = 0.05f;

//------------------------------------------------------------------------------------------
// ____ METODY I FUNKCJE - DEKLARACJE ______
//------------------------------------------------------------------------------------------

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseMove(GLFWwindow *window, double posX, double posY);
void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();

#endif // MAIN_H