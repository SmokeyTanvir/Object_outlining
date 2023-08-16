# OpenGL Planet Outline Program with Stencil Buffers 🪐✨

This is a simple OpenGL program that demonstrates how to create an outline effect around a 3D object using stencil buffers. The program uses the GLFW library for window creation, GLAD for OpenGL function loading, and ASSIMP for loading a simple planet model. The outline effect is achieved by rendering the object twice: once as a solid object and once as an outline, using stencil testing to mask out the interior. 🎮🌌

## Features 🚀

- Displays a 3D planet model with an outline effect using stencil buffers. 🪐🎨
- Allows you to move around the scene using mouse click and drag along with WASD controls. 🖱️⌨️

## Prerequisites 🛠️

Before running this program, you'll need to have the following libraries installed:

- GLFW (for window and input management)
- GLAD (for loading OpenGL functions)
- ASSIMP (for model loading)

The libraries are included with the project. But if you want, you can install these libraries using your preferred package manager or by building them from source.

## Installation ⚙️

1. Clone or download this repository to your local machine.
2. Make sure you have Visual Studio 2017 installed.

## Usage 🎮
1. Open the project/solution using Visual Studio 2017.
2. Build and run the project.
3. A window will appear displaying a 3D planet model.
4. Click and hold the left mouse button to move around the scene.
5, Use the WASD keys to control the movement of the camera.

## Implementation Details 📝
The program follows these general steps:

1. Initialize GLFW and create a window.
2. Initialize GLAD to load OpenGL functions.
3. Load the planet model using ASSIMP.
4. Set up vertex and fragment shaders.
5. Set up a stencil buffer and configure OpenGL settings for the outline effect.
6. Render the planet model twice:
- First, render the model as solid objects to set the stencil buffer.</li>
- Then, render the model as outlines using the stencil buffer to mask interior areas.</li>


## Credits 🙌
- This program uses the [GLFW library](https://github.com/glfw/glfw) for window management 🪟🎮
- OpenGL function loading is done using the [GLAD library](https://github.com/Dav1dde/glad) 🎮🧰
- The planet model used in this program is loaded using the [ASSIMP library](https://github.com/assimp/assimp) 🪐📦



