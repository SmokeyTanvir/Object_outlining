#ifndef PROGRAM_H	
#define PROGRAM_H	

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	
#include <glm/gtc/type_ptr.hpp>	
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <utility/Model.h>
#include <utility/stb_image.h>
#include <utility/Shader.h>
#include <utility/Camera.h>

// deltaTime
float deltaTime;
float lastFrame = 0.0f;

float clearColor[3] = { 0.0f, 0.0f, 0.0f };
// Transformation matrices
glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);

// Camera
float FOV = glm::radians(45.0f);
float lastX, lastY;

// Objects position
glm::vec3 planetPosition(0.0f, 2.0f, 0.0f);

// Outline
float outlineScale = 1.01f;

#endif