#pragma once
/////////////////////////////////////////////
//Shader.h
//Copyright(c) 2019 Luta Vlad
//https://github.com/meemknight/OpenGLEngine
/////////////////////////////////////////////

#include<GL/glew.h>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <cstring>

class ShaderProgram;

template <GLenum shaderType>
struct Shader
{
	Shader() {};
	Shader(const char* name)
	{
		compile(name);
	}
	//	static using shaderType = shaderType;

	unsigned int id = 0;
	void deleteShader();

	void compile(const char* name)
	{
		size_t size;
		FILE *input;
		input = fopen(name, "rb");

		if (input == nullptr)
		{
			std::cout << "couldn't open the shader file: " << name;
		}

		fseek(input, 0, SEEK_END);
		size = ftell(input);
		fseek(input, 0, SEEK_SET);

		char *data = new char[size + 1];

		fread(data, size, 1, input);
		data[size] = 0;
		//

		id = glCreateShader(shaderType);
		glShaderSource(id, 1, &data, 0);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (!result)
		{
			char* message;
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			message = new char[length];
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Error compiling shader:" << name;
			std::cout << message;
			delete[] message;
		}

		delete[] data;

	}

	friend ShaderProgram;
};

using VertexShader = Shader<GL_VERTEX_SHADER>;
using FragmentShader = Shader<GL_FRAGMENT_SHADER>;
using GeometryShader = Shader<GL_GEOMETRY_SHADER>;

struct cmp_str //for compairing the strings literals
{
	bool operator()(const char *a, const char *b)const
	{
		return !std::strcmp(a, b);
	}
};

///bref this class is used for compiling shaders and also using their data
class ShaderProgram
{
	void compileProgram();
	void compileProgramWithGeometryShader();
	std::unordered_map<const char*, int> locations;
	std::unordered_map<const char*, unsigned int> subRoutines;
public:

	ShaderProgram();
	ShaderProgram(const VertexShader &vs, const FragmentShader &fs);
	ShaderProgram(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs);

	unsigned int id = 0;


	void bind();
	void unBind();
	void deleteProgram();

	int getUniformLocation(const char* name);

	unsigned int getSoubRutineLocation(const char* name);

	void uniform(const char* name, float a);
	void uniform(const char* name, float a, float b, float c);
	void uniform(const char* name, float a, float b, float c, float d);
	void uniform(const char* name, int count, float *a);
	void uniformi(const char * name, int a);

	VertexShader vs;
	FragmentShader fs;
	GeometryShader gs;

};

template<GLenum shaderType>
inline void Shader<shaderType>::deleteShader()
{
	glDeleteShader(id);
}


/*
#include<GL/glew.h>
#include<fstream>
#include<vector>
#include<iostream>

template<GLenum S>
struct Shader
{
	Shader(const char *file)
	{
		std::vector<char> shaderData(1000);
		
		std::ifstream f(file);
		if(!f.is_open())
		{
			std::cout << "file error\n";
		}
		std::copy(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>(), std::back_inserter(shaderData));
		shaderData.push_back(0);
		id = glCreateShader(S);
		int length = shaderData.size();
		auto data = &shaderData[0];
		glShaderSource(id, 1, &data, 0);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			char* message;
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			message = new char[length];
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << file << "\n" << message << "\n\n";
			delete[] message;
		}

	}

	GLint id;
};

using  VertexShader = Shader<GL_VERTEX_SHADER>;
using  FragmentShader = Shader<GL_FRAGMENT_SHADER>;

class ShaderProgram
{
public:
	ShaderProgram(VertexShader vs, FragmentShader fs)
	{
		id = glCreateProgram();
		glAttachShader(id, vs.id);
		glAttachShader(id, fs.id);
		glLinkProgram(id);
		glValidateProgram(id);
		char message[1000] = { 0 };
		glGetProgramInfoLog(id,1000,0, message);
		std::cout << message;
		bind();
	}

	void bind()
	{
		glUseProgram(id);
	}

	GLint id;
};
*/