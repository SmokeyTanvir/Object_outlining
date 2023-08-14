#include "program.h"

// global variables
int window_height = 600;
int window_width = 900;

// function prototypes
void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

int main() {
	// window creation
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "program", nullptr, nullptr);
	if (window == NULL) {
		std::cout << "ERROR::WINDOW CREATION:FAILED\n\n";
		return -1;
	}
	// Create Context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR::FAILED TO INITIALIZE GLAD\n\n";
	}
	// Enable depth, stencil testing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// imGUI style
	ImGui::StyleColorsDark();
	// setup Renerer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// Camera
	Camera cam(glm::vec3(0.0f, 3.0f, 10.0f));
	lastX = window_width / 2;
	lastY = window_height / 2;

	// 3d Models
	Model floor("models/floor/floor.obj");
	Model planet("models/planet/planet.obj");

	// Shaders
	Shader defaultShader("shaders/default.vert", "shaders/default.frag");
	Shader outlineShader("shaders/outline.vert", "shaders/outline.frag");

	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glfwPollEvents();

		/* Setting up deltaTime */
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/* Process Camera Movements */
		if(!io.WantCaptureMouse)
			cam.ProcessMouseMovement(window, lastX, lastY);
		cam.processKeyInputs(window, deltaTime);

		// Setting up transformation matrices
		view = cam.GetViewMatrix();
		projection = glm::perspective(FOV, (float)window_width / window_height, 0.1f, 100.0f);

		// Drawing Stuff
		// ----------------------
		glStencilMask(0x00);
		/* Drawing the floor */
		glm::mat4 floorModel(1.0f);
		floor.setMVP(defaultShader, floorModel, view, projection);
		floor.Draw(defaultShader);

		/* Drawing the planet */
		// Enable stencil writing
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		// Draw the model
		glm::mat4 planetModel(1.0f);
		planetModel = glm::translate(planetModel, planetPosition);
		planet.setMVP(defaultShader, planetModel, view, projection);
		planet.Draw(defaultShader);

		/* Drawing the outline */
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		planetModel = glm::scale(planetModel, glm::vec3(outlineScale));
		outlineShader.use();
		outlineShader.setMVP(planetModel, view, projection);
		planet.Draw(outlineShader);

		// re-enable depth testing and stencil testing
		glEnable(GL_DEPTH_TEST);
		glStencilMask(0xFF);


		// start ImGUI frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// window
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(300.0f, 100.0f));
		ImGui::Begin("Window");
		ImGui::Text("Framerate: %.1f FPS", io.Framerate);
		ImGui::ColorEdit3("clear color", clearColor);
		ImGui::End();

		// planet
		ImGui::SetNextWindowPos(ImVec2(0.0f, 100.0f));
		ImGui::SetNextWindowSize(ImVec2(300.0f, 100.0f));
		ImGui::Begin("planet");
		ImGui::SliderFloat("outline", &outlineScale, 1.001f, 1.05f);
		ImGui::End();


		// ImGUI rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// cleanup
	glfwDestroyWindow(window);

	return 0;
}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;
}