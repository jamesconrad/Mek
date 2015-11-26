#include "Program.h"
#include <stdexcept>
#include "lib\glm\gtc\type_ptr.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"
#include "ComponentLight.h"

int Program::addLightSource(LightComponent* l)
{
	_lightMap.push_back(l);
	if (l->type == lSPOT)
	{
		numSpotLights++;
	}
	else if (l->type == lPOINT)
	{
		numPointLights++;
	}

	updateLightLocations("skinning");

	return _lightMap.size();
}

void Program::delLightSource(int index)
{
	if (_lightMap[index]->type == lSPOT)
		numSpotLights--;
	else if (_lightMap[index]->type == lPOINT)
		numPointLights--;
	_lightMap.erase(_lightMap.begin() + index);
	_lightMap.shrink_to_fit();
	_vlightMapLoc.erase(_vlightMapLoc.begin() + index);
	_vlightMapLoc.shrink_to_fit();
}

void Program::updateLightLocations(char* shadername)
{
	GLint shader = object(shadername);
	char Name[128];
	memset(Name, 0, sizeof(Name));
	for (int i = 0, s = _lightMap.size(); i < s; i++)
	{
		if (_lightMap[i]->type == lPOINT)
		{
			LocPointLight* loc = new LocPointLight;
			_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
			loc->Base.Color = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
			loc->Base.AmbientIntensity = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Position", i);
			loc->Position = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
			loc->Base.DiffuseIntensity = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
			loc->Atten.Constant = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
			loc->Atten.Linear = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
			loc->Atten.Exp = glGetUniformLocation(shader, Name);

			_vlightMapLoc.push_back(loc);
		}
		else if (_lightMap[i]->type == lSPOT)
		{
			LocSpotLight* loc = new LocSpotLight;
			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
			loc->Base.Base.Color = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
			loc->Base.Base.AmbientIntensity = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
			loc->Base.Base.DiffuseIntensity = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
			loc->Base.Position = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
			loc->Direction = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
			loc->Cutoff = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
			loc->Base.Atten.Constant = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
			loc->Base.Atten.Linear = glGetUniformLocation(shader, Name);

			_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
			loc->Base.Atten.Exp = glGetUniformLocation(shader, Name);

			_vlightMapLoc.push_back(loc);
		}
	}
	//now that we have our light sources done, lets do the other variables
	_vlightVarLoc.push_back(glGetUniformLocation(shader, "gNumPointLights"));
	_vlightVarLoc.push_back(glGetUniformLocation(shader, "gNumSpotLights"));
	_vlightVarLoc.push_back(glGetUniformLocation(shader, "gDirectionalLight.Base.Color"));
	_vlightVarLoc.push_back(glGetUniformLocation(shader, "gDirectionalLight.Direction"));
	_vlightVarLoc.push_back(glGetUniformLocation(shader, "gDirectionalLight.Base.AmbientIntensity"));
	_vlightVarLoc.push_back(glGetUniformLocation(shader, "gDirectionalLight.Base.DiffuseIntensity"));
	_vlightVarLoc.push_back(glGetUniformLocation(shader, "gMatSpecularIntensity"));
	_vlightVarLoc.push_back(glGetUniformLocation(shader, "gSpecularPower"));
}

void Program::updateSkinning()
{
	//
	//uniform float materialShininess;
	//uniform vec3 materialSpecularColor;
	//
	//setUniform("skinning", "materialShininess", 0.5f);
	//setUniform("skinning", "meterialSpecularColor", glm::vec3(0.5,0.5,0.5));

	setUniform("skinning", "gWVP", Camera::getInstance().matrix());
	//setUniform("skinning", "cameraPosition", Camera::getInstance().position());
	setUniform("skinning", "gWorld", glm::translate(glm::mat4(), glm::vec3(0, 0, 0)));

}
//helper function
template <typename T>
void SetLightUniform(char* shaderName, const char* propertyName, size_t lightIndex, const T& value)
{
	char tmp[128];
	sprintf_s(tmp, "allLights[%i].%s", lightIndex, propertyName);
	std::string uniformName(tmp);

	Program::getInstance().setUniform("standard", uniformName.c_str(), value);
}

void Program::updateLighting(char* shadername)
{
	//MODEL SETS : //currently set as 0.5 by shader
	//uniform float gMatSpecularIntensity;
	//uniform float gSpecularPower;

	glUniform1i(_vlightVarLoc[gNumPointLights], numPointLights);
	glUniform1i(_vlightVarLoc[gNumSpotLights], numSpotLights);
	glUniform3f(_vlightVarLoc[gDirectionalLightBaseColor], 0.25f, 0.25f, 0.25f);
	glUniform3f(_vlightVarLoc[gDirectionalLightDirection], 0.0f, -1.0f, -1.0f);
	glUniform1f(_vlightVarLoc[gDirectionalLightBaseAmbientIntensity], 0.55f);
	glUniform1f(_vlightVarLoc[gDirectionalLightBaseDiffuseIntensity], 0.9f);
	glUniform1f(_vlightVarLoc[gMatSpecularIntensity], 0.5f);
	glUniform1f(_vlightVarLoc[gSpecularPower], 0.5f);
	
	//set the variables
	//index in lightmap are relative to indicies in vlightMapLoc
	for (int i = 0, s = _lightMap.size(); i < s; i++)
	{
		if (_lightMap[i]->type == lPOINT)
		{
			LocPointLight* loc = static_cast<LocPointLight*>(_vlightMapLoc[i]);
			PointLight* l = static_cast<PointLight*>(_lightMap[i]->lightSource);

			glUniform1f(loc->Atten.Constant, l->Atten.Constant);
			glUniform1f(loc->Atten.Linear, l->Atten.Linear);
			glUniform1f(loc->Atten.Exp, l->Atten.Exp);
			glUniform3f(loc->Base.Color, l->Base.Color.x, l->Base.Color.y, l->Base.Color.z);
			glUniform1f(loc->Base.AmbientIntensity, l->Base.AmbientIntensity);
			glUniform1f(loc->Base.DiffuseIntensity, l->Base.DiffuseIntensity);
			glUniform3f(loc->Position, l->Position.x, l->Position.y, l->Position.z);
		}
		else if (_lightMap[i]->type == lSPOT)
		{
			LocSpotLight* loc = static_cast<LocSpotLight*>(_vlightMapLoc[i]);
			SpotLight* l = static_cast<SpotLight*>(_lightMap[i]->lightSource);

			glUniform1f(loc->Base.Atten.Constant, l->Base.Atten.Constant);
			glUniform1f(loc->Base.Atten.Linear, l->Base.Atten.Linear);
			glUniform1f(loc->Base.Atten.Exp, l->Base.Atten.Exp);
			glUniform3f(loc->Base.Base.Color, l->Base.Base.Color.x, l->Base.Base.Color.y, l->Base.Base.Color.z);
			glUniform1f(loc->Base.Base.AmbientIntensity, l->Base.Base.AmbientIntensity);
			glUniform1f(loc->Base.Base.DiffuseIntensity, l->Base.Base.DiffuseIntensity);
			glUniform3f(loc->Base.Position, l->Base.Position.x, l->Base.Position.y, l->Base.Position.z);
			glUniform3f(loc->Direction, l->Direction.x, l->Direction.y, l->Direction.z);
			glUniform1f(loc->Cutoff, l->Cutoff);
		}
	}
}

void Program::createShader(char* name, GLenum type, char* filepath)
{
	//std::map<char*, std::pair<std::map<GLenum, Shader*>, GLuint>> _shaderMap;
	if (_shaderMap.find(name) == _shaderMap.end())
	{
		std::pair<std::map<GLenum, Shader*>, GLuint> tmp;
		_shaderMap.emplace(name, tmp);
	}
	std::map<GLenum, Shader*> &tmpMap = _shaderMap.at(name).first;
	tmpMap.emplace(type, Shader::shaderFromFile(filepath, type));

	GLuint _object = _shaderMap.at(name).second;

	//create the program object
	_object = glCreateProgram();
	if (_object == 0)
		throw std::runtime_error("glCreateProgram failed");
	
	//How to scan through all shaders in a _shaderMap : for (std::map<GLenum, Shader>::iterator iter = _shaderMap.at(name).first.begin(); iter != _shaderMap.at(name).first.end(); ++iter)
	
	//attach all the shaders
	for (std::map<GLenum, Shader*>::iterator iter = _shaderMap.at(name).first.begin(); iter != _shaderMap.at(name).first.end(); ++iter)
		glAttachShader(_object, iter->second->object());

	//link the shaders together
	glLinkProgram(_object);

	//detach all the shaders
	for (std::map<GLenum, Shader*>::iterator iter = _shaderMap.at(name).first.begin(); iter != _shaderMap.at(name).first.end(); ++iter)
		glDetachShader(_object, iter->second->object());

	//throw exception if linking failed
	GLint status;
	glGetProgramiv(_object, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		std::string msg("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(_object, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteProgram(_object); _object = 0;
		throw std::runtime_error(msg);
	}
	_shaderMap.at(name).second = _object;
}

GLuint Program::object(char* name) const {
	return _shaderMap.at(name).second;
}

void Program::use(char* name) const {
    glUseProgram(object(name));
}

bool Program::isInUse(char* name) const {
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return (currentProgram == (GLint)object(name));
}

void Program::stopUsing(char* name) const {
    assert(isInUse(name));
    glUseProgram(0);
}

GLint Program::attrib(char* name, const GLchar* attribName) const {
    if(!attribName)
        throw std::runtime_error("attribName was NULL");
    
    GLint attrib = glGetAttribLocation(object(name), attribName);
    if(attrib == -1)
        throw std::runtime_error(std::string("Program attribute not found: ") + attribName);
    
    return attrib;
}

GLint Program::uniform(char* shadername, const GLchar* uniformName) const {
    if(!uniformName)
        throw std::runtime_error("uniformName was NULL");
    
    GLint uniform = glGetUniformLocation(object(shadername), uniformName);
	if (uniform == -1)
	{
		uniform = glGetUniformLocation(_shaderMap.at(shadername).first.at(GL_FRAGMENT_SHADER)->object(), uniformName);
		if (uniform = -1)
			throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);
	}
    
    return uniform;
}

#define ATTRIB_N_UNIFORM_SETTERS(SHADER_NAME, OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
    void Program::setAttrib(SHADER_NAME sName, const GLchar* name, OGL_TYPE v0) \
        { assert(isInUse(sName)); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (attrib(sName, name), v0); } \
    void Program::setAttrib(SHADER_NAME sName, const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(isInUse(sName)); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (attrib(sName, name), v0, v1); } \
    void Program::setAttrib(SHADER_NAME sName, const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(isInUse(sName)); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (attrib(sName, name), v0, v1, v2); } \
    void Program::setAttrib(SHADER_NAME sName, const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(isInUse(sName)); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (attrib(sName, name), v0, v1, v2, v3); } \
\
    void Program::setAttrib1v(SHADER_NAME sName, const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse(sName)); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (attrib(sName, name), v); } \
    void Program::setAttrib2v(SHADER_NAME sName, const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse(sName)); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (attrib(sName, name), v); } \
    void Program::setAttrib3v(SHADER_NAME sName, const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse(sName)); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (attrib(sName, name), v); } \
    void Program::setAttrib4v(SHADER_NAME sName, const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse(sName)); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (attrib(sName, name), v); } \
\
    void Program::setUniform(SHADER_NAME sName, const GLchar* name, OGL_TYPE v0) \
        { assert(isInUse(sName)); glUniform1 ## TYPE_SUFFIX (uniform(sName, name), v0); } \
    void Program::setUniform(SHADER_NAME sName, const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(isInUse(sName)); glUniform2 ## TYPE_SUFFIX (uniform(sName, name), v0, v1); } \
    void Program::setUniform(SHADER_NAME sName, const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(isInUse(sName)); glUniform3 ## TYPE_SUFFIX (uniform(sName, name), v0, v1, v2); } \
    void Program::setUniform(SHADER_NAME sName, const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(isInUse(sName)); glUniform4 ## TYPE_SUFFIX (uniform(sName, name), v0, v1, v2, v3); } \
\
    void Program::setUniform1v(SHADER_NAME sName, const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse(sName)); glUniform1 ## TYPE_SUFFIX ## v (uniform(sName, name), count, v); } \
    void Program::setUniform2v(SHADER_NAME sName, const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse(sName)); glUniform2 ## TYPE_SUFFIX ## v (uniform(sName, name), count, v); } \
    void Program::setUniform3v(SHADER_NAME sName, const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse(sName)); glUniform3 ## TYPE_SUFFIX ## v (uniform(sName, name), count, v); } \
    void Program::setUniform4v(SHADER_NAME sName, const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse(sName)); glUniform4 ## TYPE_SUFFIX ## v (uniform(sName, name), count, v); }

ATTRIB_N_UNIFORM_SETTERS(char*, GLfloat, , f);
ATTRIB_N_UNIFORM_SETTERS(char*, GLdouble, , d);
ATTRIB_N_UNIFORM_SETTERS(char*, GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(char*, GLuint, I, ui);

void Program::setUniformMatrix2(char* shaderName, const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    assert(isInUse(shaderName));
    glUniformMatrix2fv(uniform(shaderName, name), count, transpose, v);
}

void Program::setUniformMatrix3(char* shaderName, const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    assert(isInUse(shaderName));
    glUniformMatrix3fv(uniform(shaderName, name), count, transpose, v);
}

void Program::setUniformMatrix4(char* shaderName, const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    assert(isInUse(shaderName));
    glUniformMatrix4fv(uniform(shaderName, name), count, transpose, v);
}

void Program::setUniform(char* shaderName, const GLchar* name, const glm::mat2& m, GLboolean transpose) {
    assert(isInUse(shaderName));
    glUniformMatrix2fv(uniform(shaderName, name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(char* shaderName, const GLchar* name, const glm::mat3& m, GLboolean transpose) {
    assert(isInUse(shaderName));
    glUniformMatrix3fv(uniform(shaderName, name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(char* shaderName, const GLchar* name, const glm::mat4& m, GLboolean transpose) {
    assert(isInUse(shaderName));
    glUniformMatrix4fv(uniform(shaderName, name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(char* shaderName, const GLchar* uniformName, const glm::vec3& v) {
    setUniform3v(shaderName, uniformName, glm::value_ptr(v));
}

void Program::setUniform(char* shaderName, const GLchar* uniformName, const glm::vec4& v) {
    setUniform4v(shaderName, uniformName, glm::value_ptr(v));
}
