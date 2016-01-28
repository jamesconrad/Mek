#include "Shader.h"
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

Shader::Shader(const std::string& shaderCode, GLenum shaderType) :
    _object(0),
    _refCount(NULL)
{
    //create the shader object
    _object = glCreateShader(shaderType);
    if(_object == 0)
        throw std::runtime_error("glCreateShader failed");
    
    //set the source code
    const char* code = shaderCode.c_str();
    glShaderSource(_object, 1, (const GLchar**)&code, NULL);
    
    //compile
    glCompileShader(_object);
    
    //throw exception if compile error occurred
    GLint status;
    glGetShaderiv(_object, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteShader(_object); _object = 0;
        throw std::runtime_error(msg);
    }
    
    _refCount = new unsigned;
    *_refCount = 1;
}

Shader::Shader(const Shader& other) :
    _object(other._object),
    _refCount(other._refCount)
{
    _retain();
}

Shader::~Shader() {
    //_refCount will be NULL if constructor failed and threw an exception
    if(_refCount) _release();
}

GLuint Shader::object() const {
    return _object;
}

Shader& Shader::operator = (const Shader& other) {
    _release();
    _object = other._object;
    _refCount = other._refCount;
    _retain();
    return *this;
}

std::string ReadShader(char* filepath)
{
	std::fstream f;
	f.open(filepath, std::ios::in | std::ios::binary);
	if (!f.is_open())
		throw std::runtime_error(std::string("Failed to open file : ") + filepath);
	std::stringstream buffer;
	buffer << f.rdbuf();
	std::string shader;
	shader = buffer.str();
	
	//Read my file and look for a "#include "
	size_t pos = shader.find("#include", 0);
	while (pos != std::string::npos)
	{
		//search for the ">" tag
		size_t ebrace = shader.find(">", pos + 1);
		//grab the filepath then the subfile
		std::string subfilepath = shader.substr(pos + 10, ebrace - (pos + 10));
		//recusivley include incase of a tree
		std::string subfile = ReadShader((char*)subfilepath.c_str());

		//expand/insert ontop of the #include
		shader.replace(pos, ebrace - pos, subfile);

		pos = shader.find("#include", pos + 1);
	}
	
	return shader;
}

Shader* Shader::shaderFromFile(const std::string& filePath, GLenum shaderType) {
    //open file
    //std::ifstream f;
    //f.open(filePath.c_str(), std::ios::in | std::ios::binary);
    //if(!f.is_open()){
    //    throw std::runtime_error(std::string("Failed to open file: ") + filePath);
    //}
	//
    ////read whole file into stringstream buffer
    //std::stringstream buffer;
    //buffer << f.rdbuf();
	//
	//
    ////return new shader
	
	//get shader code
    Shader* shader = new Shader(ReadShader((char*)filePath.c_str()), shaderType);
    return shader;
}

void Shader::_retain() {
    assert(_refCount);
    *_refCount += 1;
}

void Shader::_release() {
    assert(_refCount && *_refCount > 0);
    *_refCount -= 1;
    if(*_refCount == 0){
        glDeleteShader(_object); _object = 0;
        delete _refCount; _refCount = NULL;
    }
}

