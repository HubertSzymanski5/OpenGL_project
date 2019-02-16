#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H


#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <map>
using std::string;

class ShaderProgram {

public:

	// konstruktor
	ShaderProgram();
	// destruktor
	~ShaderProgram();

	enum ShaderType {
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	// ladowanie shaderow
	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void use();

	void setUniform(const GLchar* name, const glm::vec2 &v);
	void setUniform(const GLchar* name, const glm::vec3 &v);
	void setUniform(const GLchar* name, const glm::vec4 &v);
	void setUniform(const GLchar* name, const glm::mat4 &m);
	void setUniform(const GLchar* name, const GLfloat &f);
	void setUniform(const GLchar* name, const GLint v);
	void setUniformSampler(const GLchar* name, const GLuint &slot);

	GLuint getProgram();

private:
	
	// zamiana plikow shaderow na string
	string fileToString(const string &filename);
	// sprawdzenie czy compilacja przebiegla pomyslnie
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name);

	GLuint mHandle;
	std::map<string, GLint> mUniformLocations;

}; // class ShaderProgram

#endif // SHADER_PROGRAM_H