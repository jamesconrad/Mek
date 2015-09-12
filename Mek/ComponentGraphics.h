#pragma once
// engine includes
#include "Component.h"

// assimp includes
#include "include\assimp\Importer.hpp"
#include "include\assimp\postprocess.h"
#include "include\assimp\scene.h"

// devil includes
#include "include\IL\il.h"

// gl includes
#include "include\GL\glfw3.h"
#include "lib\glm\glm.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"

// std includes
#include <map>


class ComponentGraphics : public Component
{
public:
	void update();
	void loadModel(char* model);
	void render();
private:
	// model loader vars
	const aiScene* _scene;
	Assimp::Importer _importer;
	std::map<std::string, GLuint*> _textureIdMap;
	GLuint* _textureIds;

	// model rendering vars
	GLuint _vbo;
	GLuint _vao;

	glm::mat4 _transform;
};