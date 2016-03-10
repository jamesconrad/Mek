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
	bind("skinning");
	setUniform("gWVP", Camera::getInstance().matrix());
//	setUniform("skinning", "cameraPosition", Camera::getInstance().position());
	setUniform("gWorld", glm::translate(glm::mat4(), glm::vec3(0, 0, 0)));

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
	glUniform1f(_vlightVarLoc[gDirectionalLightBaseAmbientIntensity], 0.1f);
	glUniform1f(_vlightVarLoc[gDirectionalLightBaseDiffuseIntensity], 0.1f);
	glUniform1f(_vlightVarLoc[gMatSpecularIntensity], 0.1f);
	glUniform1f(_vlightVarLoc[gSpecularPower], 0.1f);
	
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
	std::string debfp = "../Debug/";
	debfp.append(filepath);
	debfp = debfp.substr(17);
	filepath = (char*)debfp.c_str();
	GLuint _object;

	//New method

	//=============================================
	//MUST RECOMPILE UPON ADDING NEW SHADER
	//=============================================
	if (_smap.find(name) == _smap.end())
		_smap.emplace(name, CompiledShader());

	//Create the actual shader
	_smap.at(name).shaders.emplace(type, Shader::shaderFromFile(filepath, type));

	//create the program object
	_object = glCreateProgram();
	if (_object == 0)
		throw std::runtime_error("glCreateProgram failed");
	
	//store programid
	_smap.at(name).programid = _object;

	//for (int i = 0, s = _smap.at(name).shaders.size(); i < s; i++)
	for (auto i = _smap.at(name).shaders.begin(); i != _smap.at(name).shaders.end(); ++i)
		glAttachShader(_object, i->second->object());

	//link the shaders together
	glLinkProgram(_object);

	for (auto i = _smap.at(name).shaders.begin(); i != _smap.at(name).shaders.end(); ++i)
		glDetachShader(_object, i->second->object());

	//throw exception if linking failed
	GLint status;
	glGetProgramiv(_object, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
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
	//_shaderMap.at(name).second = _object;
}

GLuint Program::object(char* name)
{
	return _smap.at(name).programid;
	//return _shaderMap.at(name).second;
}

void Program::use(char* name)
{
	_bound = &_smap.at(name);
    glUseProgram(_bound->programid);
}

void Program::bind(char* name)
{
	_bound = &_smap.at(name);
	glUseProgram(_bound->programid);
}

void Program::unbind()
{
	_bound = nullptr;
	glUseProgram(0);
}

bool Program::isInUse(char* name)
{
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return (currentProgram == (GLint)object(name));
}

void Program::stopUsing(char* name)
{
    glUseProgram(0);
}

GLint Program::attrib(char* attribName)
{
    if(!attribName)
        throw std::runtime_error("attribName was NULL");
	if (_bound->attribLoc.find((char*)attribName) == _bound->attribLoc.end())
	{
		GLint attrib = glGetAttribLocation(_bound->programid, attribName);
		if (attrib == -1)
			throw std::runtime_error(std::string("Program attribute not found: ") + attribName);

		//cache the result
		_bound->attribLoc.emplace(attribName, attrib);
		return attrib;
	}
	else
		return _bound->attribLoc.at(attribName);
}

GLint Program::uniform(char* uniformName)
{
    if(!uniformName)
        throw std::runtime_error("uniformName was NULL");
    

	if (_bound->uniformLoc.find(uniformName) == _bound->uniformLoc.end())
	{
		GLint uniform = glGetUniformLocation(_bound->programid, uniformName);
		//if (uniform == -1)
		//	throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);

		//cache the result
		_bound->uniformLoc.emplace(uniformName, uniform);
		return uniform;
	}
	else
		return _bound->uniformLoc.at(uniformName);
}

/*
Remaining:
	
Complete:
	mat3
	mat4
	vec2
	vec3
	vec4
	float
	double
	int
	unsigned int

*/
void Program::setUniformMatrix2(char* name, const GLfloat* v, GLsizei count, GLboolean transpose)
{
    glUniformMatrix2fv(uniform(name), count, transpose, v);
}

void Program::setUniformMatrix3(char* name, const GLfloat* v, GLsizei count, GLboolean transpose)
{
    glUniformMatrix3fv(uniform(name), count, transpose, v);
}

void Program::setUniformMatrix4(char* name, const GLfloat* v, GLsizei count, GLboolean transpose)
{
    glUniformMatrix4fv(uniform(name), count, transpose, v);
}

void Program::setUniform(char* name, const glm::mat2& m, GLboolean transpose)
{
    glUniformMatrix2fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(char* name, const glm::mat3& m, GLboolean transpose)
{
    glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(char* name, const glm::mat4& m, GLboolean transpose)
{
    glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(char* uniformName, const glm::vec2& v)
{
	glUniform2fv(uniform(uniformName), 1, glm::value_ptr(v));
}

void Program::setUniform(char* uniformName, const glm::vec3& v)
{
	glUniform3fv(uniform(uniformName), 1, glm::value_ptr(v));
}

void Program::setUniform(char* uniformName, const glm::vec4& v)
{
	glUniform4fv(uniform(uniformName), 1, glm::value_ptr(v));
}

void Program::setUniform(char* uniformName, const GLfloat& d)
{
	glUniform1f(uniform(uniformName), d);
}

void Program::setUniform(char* uniformName, const GLdouble& d)
{
	glUniform1d(uniform(uniformName), d);
}

void Program::setUniform(char* uniformName, const GLint& d)
{
	glUniform1i(uniform(uniformName), d);
}

void Program::setUniform(char* uniformName, const GLuint& d)
{
	glUniform1ui(uniform(uniformName), d);
}
