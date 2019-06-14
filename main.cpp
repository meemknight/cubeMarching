#include <iostream>
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "Camera.h"
#include "worldControll.h"

int main()
{


	if(glfwInit() == false)
	{
		std::cout << "glfwerror";
		Sleep(1000);
	}
	float width = 640;
	float heigth = 480;
	GLFWwindow* window = glfwCreateWindow(width, heigth, "My Title", NULL, NULL);
	
	if (!window)
	{
		std::cout << "glfwerror window";
		Sleep(1000);
	}

	glfwMakeContextCurrent(window);
	glewInit();

	glEnable(GL_PROGRAM_POINT_SIZE);

	ShaderProgram program{"vert.vert", "geometry.geom","frag.frag"};

	World3d world;
	world.scale = 1;
	world.create(150, 150, 150);
	world.popultate(circleFunctionCreator(10, 70, 70, 70));
	world.calculateGpuData();
	world.bind();


	//glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, (void*)3);
	//glEnableVertexAttribArray(1);
	auto windowNative = (HWND)glfwGetWin32Window(window);
	Camera camera(windowNative, 85.f, &width, &heigth, 0.1f, 100);
	camera.rSpeed = 0.2;
	camera.position = { 0,0, 10 };

	float deltaTime = 0;
	float lastTime = GetTickCount();
	float cameraSpeed = 0.5f;


	while (!glfwWindowShouldClose(window))
	{
		deltaTime = GetTickCount() - lastTime;
		lastTime = GetTickCount();

		POINT cursorPos;
		GetCursorPos(&cursorPos);
		RECT windowRect;
		GetWindowRect(windowNative, &windowRect);

		if(GetAsyncKeyState('W'))
		{
			camera.moveFront(cameraSpeed * deltaTime);
		}
		if (GetAsyncKeyState('S'))
		{
			camera.moveBack(cameraSpeed * deltaTime);
		}
		if (GetAsyncKeyState('A'))
		{
			camera.moveLeft(cameraSpeed * deltaTime);
		}
		if (GetAsyncKeyState('D'))
		{
			camera.moveRight(cameraSpeed * deltaTime);
		}
		if (GetAsyncKeyState('R'))
		{
			camera.moveUp(cameraSpeed * deltaTime);
		}
		if (GetAsyncKeyState('F'))
		{
			camera.moveDown(cameraSpeed * deltaTime);
		}


		camera.mouseUpdate({ cursorPos.x - windowRect.top, cursorPos.y - windowRect.left});
		
		world.bind();
		program.bind();
		auto uniformId = program.getUniformLocation("modelToWorldToview");
		glUniformMatrix4fv(uniformId, 1, false, &camera.getProjectionViewMatrix()[0][0]);

		program.uniform("u_color", 0, 0.5, 0.2, 1);

		glDrawArrays(GL_POINTS, 0, world.getTrueCount());

		glfwPollEvents();		
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glfwTerminate();

	return 0;
}

/*
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

static const struct
{
	float x, y;
	float r, g, b;
} vertices[3] =
{
	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
	{  0.6f, -0.4f, 0.f, 1.f, 0.f },
	{   0.f,  0.6f, 0.f, 0.f, 1.f }
};
static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
int main(void)
{
	GLFWwindow* window;
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSwapInterval(1);
	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
		sizeof(vertices[0]), (void*)0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(vertices[0]), (void*)(sizeof(float) * 2));
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glm::mat4 m, p, mvp;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		m = glm::mat4(1.0f);
		m = glm::rotate(m, (float)glfwGetTime(), {0.3,0.2,0.1});
		p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mvp =  p * m;
		glUseProgram(program);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
*/