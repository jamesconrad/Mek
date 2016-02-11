#pragma once

#include "include\GL\glew.h"
#include <string>
#include <vector>

/**
 Represents a compiled OpenGL shader.
 */
class Shader {
public:

	static Shader* shaderFromFile(const std::string& filePath, GLenum shaderType);
	Shader(const std::string& shaderCode, GLenum shaderType);
	GLuint object() const;
	~Shader();

private:
	GLuint _object;
	unsigned* _refCount;
};
