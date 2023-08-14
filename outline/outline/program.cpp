#include "program.h"

// global variables
int window_height = 600;
int window_width = 900;

// function prototypes
void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

int main() {
	// window creation
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 8);
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
	glEnable(GL_MULTISAMPLE);
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
	Model floor("resource/models/floor/floor.obj");
	Model planet("resource/models/planet/planet.obj");

	// Shaders
	Shader defaultShader("resource/shaders/default.vert", "resource/shaders/default.frag");
	Shader outlineShader("resource/shaders/outline.vert", "resource/shaders/outline.frag");
	Shader skyboxShader("resource/shaders/skybox.vert", "resource/shaders/skybox.frag");

	// cubemap texture path
	vector<std::string> faces =
	{
		"resource/skybox/posx.jpg",
		"resource/skybox/negx.jpg",
		"resource/skybox/posy.jpg",
		"resource/skybox/negy.jpg",
		"resource/skybox/posz.jpg",
		"resource/skybox/negz.jpg"
	};
	GLuint cubemapTexture = loadCubemap(faces);
	// buffers for skybox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	// vertex configuration
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// unbind VBO, VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
		// draw skybox
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		glm::mat4 skyboxView;
		skyboxView = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
		skyboxShader.setMat4("view", skyboxView);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

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
		planetPosition = glm::vec3(planetPos[0], planetPos[1], planetPos[2]);
		planetModel = glm::translate(planetModel, planetPosition);
		planet.setMVP(defaultShader, planetModel, view, projection);
		planet.Draw(defaultShader);
		glStencilMask(0x00);

		/* Drawing the outline */
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glDisable(GL_DEPTH_TEST);
		planetModel = glm::scale(planetModel, glm::vec3(outlineScale));
		outlineShader.use();
		outlineShader.setMVP(planetModel, view, projection);
		planet.Draw(outlineShader);

		// re-enable depth testing
		glEnable(GL_DEPTH_TEST);
		// re-enable stencil testing
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
		ImGui::SliderFloat3("position", planetPos, -10.0f, 10.0f);
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