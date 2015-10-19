/*
 tdogl::Program
 
 Copyright 2012 Thomas Dalling - http://tomdalling.com/
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "Program.h"
#include <stdexcept>
#include "lib\glm\gtc\type_ptr.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"
#include "LightComponent.h"

int Program::addLightSource(LightComponent* l)
{
	_lightMap.push_back(l);
	return _lightMap.size();
}

void Program::delLightSource(int index)
{
	_lightMap.erase(_lightMap.begin() + index);
	_lightMap.shrink_to_fit();
}

void Program::updateSkinning()
{
	setUniform("skinning", "gWVP", Camera::getInstance().matrix());
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

	Program::getInstance().setUniform(shadername, "numLights", (int)_lightMap.size());
	for (size_t i = 0; i < _lightMap.size(); ++i){
		SetLightUniform(shadername, "position", i, _lightMap[i]->_pos);
		SetLightUniform(shadername, "intensities", i, _lightMap[i]->_col);
		SetLightUniform(shadername, "attenuation", i, _lightMap[i]->_attenuation);
		SetLightUniform(shadername, "ambientCoefficient", i, _lightMap[i]->_ambientCoefficient);
		SetLightUniform(shadername, "coneAngle", i, _lightMap[i]->_coneAngle);
		SetLightUniform(shadername, "coneDirection", i, _lightMap[i]->_dir);
	}
}

void Program::createShader(char* name, GLenum type, char* filepath)
{
	//std::map<char*, std::pair<std::map<GLenum, Shader*>, GLuint>> _shaderMap;
	if (_shaderMap.find(name) == _shaderMap.end())
	{
		std::pair<std::map<GLenum, Shader>, GLuint> tmp;
		_shaderMap.emplace(name, tmp);
	}
	std::map<GLenum, Shader> &tmpMap = _shaderMap.at(name).first;
	tmpMap.emplace(type, Shader::shaderFromFile(filepath, type));

	GLuint _object = _shaderMap.at(name).second;

	//create the program object
	_object = glCreateProgram();
	if (_object == 0)
		throw std::runtime_error("glCreateProgram failed");
	
	//How to scan through all shaders in a _shaderMap : for (std::map<GLenum, Shader>::iterator iter = _shaderMap.at(name).first.begin(); iter != _shaderMap.at(name).first.end(); ++iter)
	
	//attach all the shaders
	for (std::map<GLenum, Shader>::iterator iter = _shaderMap.at(name).first.begin(); iter != _shaderMap.at(name).first.end(); ++iter)
		glAttachShader(_object, iter->second.object());

	//link the shaders together
	glLinkProgram(_object);

	//detach all the shaders
	for (std::map<GLenum, Shader>::iterator iter = _shaderMap.at(name).first.begin(); iter != _shaderMap.at(name).first.end(); ++iter)
		glDetachShader(_object, iter->second.object());

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

GLint Program::uniform(char* name, const GLchar* uniformName) const {
    if(!uniformName)
        throw std::runtime_error("uniformName was NULL");
    
    GLint uniform = glGetUniformLocation(object(name), uniformName);
    if(uniform == -1)
        throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);
    
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
