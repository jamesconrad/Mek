#pragma once

#include <vector>
#include <map>
#include "lib\glm\glm.hpp"

#include "Shader.h"
#include "Camera.h"

class LightComponent;
struct PointLight;

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
//Represents a compiled shader program.
struct CompiledShader
{
	GLuint programid;//The id of the program
	std::map<GLenum, Shader*> shaders;//List of shaders indexed by type
	std::map<char*, GLint> uniformLoc;//List of uniform locations indexed by name
	std::map<char*, GLint> attribLoc;//List of attributes, currently unused
};

class Program {
public:
	GLuint object(char* name);

	void use(char* name);
	void bind(char* name);
	void unbind();

	bool isInUse(char* name);

	void stopUsing(char* name);

	void createShader(char* name, GLenum type, char* filepath);

	GLint attrib(char* attribName);

	GLint uniform(char* uniformName);

	//NO DELETE FUNCTION YET TODO:: DELETE FUNCTION ON LIGHTCOMPONENT DECONSTRUCTOR ALSO ADD THE LIGHTSOURCE ON CONSTRUCTOR
	int addLightSource(LightComponent*);
	void delLightSource(int);

	void updateSkinning();

	void updateLighting(char* shadername);

	void setUniformMatrix2(char* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniformMatrix3(char* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniformMatrix4(char* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniform(char* uniformName, const glm::mat2& m, GLboolean transpose = GL_FALSE);
	void setUniform(char* uniformName, const glm::mat3& m, GLboolean transpose = GL_FALSE);
	void setUniform(char* uniformName, const glm::mat4& m, GLboolean transpose = GL_FALSE);
	void setUniform(char* uniformName, const glm::vec2& v);
	void setUniform(char* uniformName, const glm::vec3& v);
	void setUniform(char* uniformName, const glm::vec4& v);
	void setUniform(char* uniformName, const GLfloat& d);
	void setUniform(char* uniformName, const GLdouble& d);
	void setUniform(char* uniformName, const GLint& d);
	void setUniform(char* uniformName, const GLuint& d);
	
	static Program& getInstance()
	{
		static Program instance;
		return instance;
	}

private:

	Program() {};

	// 3 spooky 5 me
	std::map<char*, CompiledShader> _smap;
	CompiledShader* _bound;

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