
#include "core.h"
#include <math.h>
#include "cMesh.h"
#include "cCamera.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

using namespace std;
using namespace glm;

/////////////////////////////////////////////////////////////////////////////////////
// global variables
/////////////////////////////////////////////////////////////////////////////////////
cScene Scene;

unsigned int initWidth = 800;
unsigned int initHeight = 600;

/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window
}

vec3 GetForwardVector(float velocity, float yrot)
{
	return velocity * glm::vec3{
				glm::cos(glm::radians(yrot + 90)),
				0.0f,
				glm::sin(glm::radians(yrot + 90)) };
}


/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	// ignore key up (key release) events
	if (action == GLFW_RELEASE) 
		return;	

	// pointer to the currently selected camera..
	cCamera* pCamera = &Scene.m_Camera[Scene.m_cameraId];

	// we get the forward vector using the yaw radiant
	float velocity = 1;
	vec3 dir = GetForwardVector(velocity, pCamera->m_rot.y);

	// process keyboard inputs here..
	if (key == GLFW_KEY_ESCAPE) {}
	if (key == GLFW_KEY_X) { glfwSetWindowShouldClose(window, true); }
	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) { pCamera->m_rot.y -= 5.0f; }
	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) { pCamera->m_rot.y += 5.0f; }
	if (key == GLFW_KEY_W || key == GLFW_KEY_UP) { pCamera->m_pos -= dir; }
	if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) { pCamera->m_pos += dir; }


	/*switch (key)
	{
		//std::cout << "Forward x = " << dir.x << " && forward Y = " << dir.y << " && Forward Z = " << dir.z << std::endl;
		//std::cout << "____________________________________________________________" << std::endl;
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_X:
			glfwSetWindowShouldClose(window, true);
		break;


		// simple camera controls to help navigate the scene
		case GLFW_KEY_A:
		{
			pCamera->m_rot.y -= 5.0f;
			// rotate camera about y-axis
		}break;

		case GLFW_KEY_D:
		{
			pCamera->m_rot.y += 5.0f;
			// rotate camera about y-axis
		}break;

		case GLFW_KEY_W:
		{
			//pCamera->m_pos.x += (cos((pCamera->m_rot.y + 90.0f) * (3.14f / 180.0f)) / 0.5f) * -1.0f;
			//pCamera->m_pos.z -= (sin((pCamera->m_rot.y + 90.0f) * (3.14f / 180.0f)) / 5.0f) * -1.0f;

			pCamera->m_pos -= dir;

			//pCamera->m_pos.x = pCamera->m_pos.x + dir.x;
			//pCamera->m_pos.z = pCamera->m_pos.z + dir.z;

			//pCamera->m_pos.z -= 0.5f;
			// rotate camera about x-axis
			std::cout << "Forward x = " << dir.x << " && forward Y = " << dir.y << " && Forward Z = " << dir.z << std::endl;
			std::cout << "____________________________________________________________" << std::endl;

		}break;

		case GLFW_KEY_S:
		{
			//pCamera->m_pos.x += (cos((pCamera->m_rot.y + 90.0f + 180.0f) * (3.14f / 180.0f)) / 0.5f) * -1.0f;
			//pCamera->m_pos.z -= (sin((pCamera->m_rot.y + 90.0f + 180.0f) * (3.14f / 180.0f)) / 5.0f) * -1.0f;

			pCamera->m_pos += dir;
			//pCamera->m_pos.x -= dir.x;
			//pCamera->m_pos.z -= dir.z;
			//pCamera->m_pos.z += 0.5f;
			// rotate camera about x-axis
			std::cout << "Forward x = " << dir.x << " && forward Y = " << dir.y << " && Forward Z = " << dir.z << std::endl;
			std::cout << "____________________________________________________________" << std::endl;

		}break;

		case GLFW_KEY_UP:
		{
			pCamera->m_pos.z -= 0.5f;
			// zoom out along z-axis
			std::cout << "Forward x = " << dir.x << " && forward Y = " << dir.y << " && Forward Z = " << dir.z << std::endl;
			std::cout << "____________________________________________________________" << std::endl;
		}break;

		case GLFW_KEY_DOWN:
		{
			pCamera->m_pos.z += 0.5f;
			// zoom in along z-axis
			std::cout << "Forward x = " << dir.x << " && forward Y = " << dir.y << " && Forward Z = " << dir.z << std::endl;
			std::cout << "____________________________________________________________" << std::endl;
		}break;

		default:
		{
			std::cout << "Forward x = " << dir.x << " && forward Y = " << dir.y << " && Forward Z = " << dir.z << std::endl;
			std::cout << "____________________________________________________________" << std::endl;
		}
	}*/
}

/////////////////////////////////////////////////////////////////////////////////////
// renderScene
/////////////////////////////////////////////////////////////////////////////////////
void renderScene()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(true);

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	////////////////////////////////////////////////////////////////////////////////
	// Render mesh objects here..
	////////////////////////////////////////////////////////////////////////////////
	Scene.render();

	glBindVertexArray(0);       	
}

/////////////////////////////////////////////////////////////////////////////////////
// printDebugInfo() - print version info to console
/////////////////////////////////////////////////////////////////////////////////////
void printDebugInfo()
{
	// Print info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

#ifdef GL_SHADING_LANGUAGE_VERSION
	printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

	printf("Using GLEW version %s.\n", glewGetString(GLEW_VERSION));
	printf("------------------------------\n");
	printf("Press ESCAPE or 'X' or 'x' to exit.\n");
}

/////////////////////////////////////////////////////////////////////////////////////
// DebugOutput
// Used to output openGL debug info
// KHR_debug runs on all systems running openGL4.3 or later
/////////////////////////////////////////////////////////////////////////////////////
void DebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data)
{
	cout << "debug call: " << msg << endl;
}

/////////////////////////////////////////////////////////////////////////////////////
// main()
/////////////////////////////////////////////////////////////////////////////////////
int main() 
{
	// initialise glfw
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "GraphicsTemplate", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stdout, "Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	
	// initialise glew
	glewInit();

	// print version info 
	//printDebugInfo();

	glDebugMessageCallback(DebugOutput, NULL);

	// Set callback function for resizing the window
	glfwSetFramebufferSizeCallback(window, resize);

	// Set callback for keyboard events
	glfwSetKeyCallback(window, keyboard);

	resize(window, initWidth, initHeight);

	////////////////////////////////////////////////////
	// initialise scene - geometry and shaders etc
	////////////////////////////////////////////////////
	Scene.init(initWidth, initHeight);

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {

		renderScene();					// Render into the current buffer
		glfwSwapBuffers(window);		// Displays what was just rendered (using double buffering).

		glfwPollEvents();				// Use this version when animating as fast as possible
	}

	glfwTerminate();
	return 0;
}

