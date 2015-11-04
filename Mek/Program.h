#pragma once

#include <vector>
#include <map>
#include "lib\glm\glm.hpp"

#include "Shader.h"
#include "Camera.h"

class LightComponent;
struct PointLight;
/**
 Represents an OpenGL program made by linking shaders.
 */

//the light locations
struct LocBaseLight
{
	GLint Color;
	GLint AmbientIntensity;
	GLint DiffuseIntensity;
};

struct LocDirectionalLight
{
	LocBaseLight Base;
	GLint Direction;
};

struct LocAttenuation
{
	GLint Constant;
	GLint Linear;
	GLint Exp;
};

struct LocPointLight
{
	LocBaseLight Base;
	GLint Position;
	LocAttenuation Atten;
};

struct LocSpotLight
{
	LocPointLight Base;
	GLint Direction;
	GLint Cutoff;
};

enum SkinningShaderIndex
{
	gNumPointLights = 0,
	gNumSpotLights = 1,
	gDirectionalLightBaseColor = 2,
	gDirectionalLightDirection = 3,
	gDirectionalLightBaseAmbientIntensity = 4,
	gDirectionalLightBaseDiffuseIntensity = 5,
	gMatSpecularIntensity = 6,
	gSpecularPower = 7
};

class Program {
public:
	 /**
	 The program's object ID, as returned from glCreateProgram
	  */
	GLuint object(char* name) const;

	void use(char* name) const;

	bool isInUse(char* name) const;

	void stopUsing(char* name) const;

	void createShader(char* name, GLenum type, char* filepath);

	/**
	The attribute index for the given name, as returned from glGetAttribLocation.
	 */
	GLint attrib(char* shaderName, const GLchar* attribName) const;


	/**
	 The uniform index for the given name, as returned from glGetUniformLocation.
	 */
	GLint uniform(char* shaderName, const GLchar* uniformName) const;

	//NO DELETE FUNCTION YET TODO:: DELETE FUNCTION ON LIGHTCOMPONENT DECONSTRUCTOR ALSO ADD THE LIGHTSOURCE ON CONSTRUCTOR
	int addLightSource(LightComponent*);
	void delLightSource(int);

	void updateSkinning();

	void updateLighting(char* shadername);

	/**
	 Setters for attribute and uniform variables.

	 These are convenience methods for the glVertexAttrib* and glUniform* functions.
	 */
#define _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(SHADER_NAME, OGL_TYPE) \
        void setAttrib(char*, const GLchar* attribName, OGL_TYPE v0); \
        void setAttrib(char*, const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
        void setAttrib(char*, const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setAttrib(char*, const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
        void setAttrib1v(char*, const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib2v(char*, const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib3v(char*, const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib4v(char*, const GLchar* attribName, const OGL_TYPE* v); \
\
        void setUniform(char*, const GLchar* uniformName, OGL_TYPE v0); \
        void setUniform(char*, const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
        void setUniform(char*, const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setUniform(char*, const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
        void setUniform1v(char*, const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform2v(char*, const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform3v(char*, const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform4v(char*, const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \

	_TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(char*, GLfloat)
		_TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(char*, GLdouble)
		_TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(char*, GLint)
		_TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(char*, GLuint)

		void setUniformMatrix2(char* shaderName, const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniformMatrix3(char* shaderName, const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniformMatrix4(char* shaderName, const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniform(char* shaderName, const GLchar* uniformName, const glm::mat2& m, GLboolean transpose = GL_FALSE);
	void setUniform(char* shaderName, const GLchar* uniformName, const glm::mat3& m, GLboolean transpose = GL_FALSE);
	void setUniform(char* shaderName, const GLchar* uniformName, const glm::mat4& m, GLboolean transpose = GL_FALSE);
	void setUniform(char* shaderName, const GLchar* uniformName, const glm::vec3& v);
	void setUniform(char* shaderName, const GLchar* uniformName, const glm::vec4& v);
	
	static Program& getInstance()
	{
		static Program instance;
		return instance;
	}

private:

	Program() {};

	// 3 spooky 5 me
	std::map<char*, std::pair<std::map<GLenum, Shader*>, GLuint>> _shaderMap;

	std::vector<LightComponent*> _lightMap;
	std::vector<void *> _vlightMapLoc;
	std::vector<int> _vlightVarLoc;
	
	unsigned int numPointLights;
	unsigned int numSpotLights;
	
	void setPointLight(PointLight* l, char* shadername, std::string &base);
	void updateLightLocations(char* shadername);

	//copying disabled
	Program(const Program&);
	const Program& operator=(const Program&);
};