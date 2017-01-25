#include "GLSLProgram.h"
#include "auxiliar.h"

#include <windows.h>

#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void GLSLProgram::InitShader(const char *vname, const char *fname){
	//Creamos un shader de vertices y de fragmentos
	vshader = LoadShader(vname, GL_VERTEX_SHADER);
	fshader = LoadShader(fname, GL_FRAGMENT_SHADER);

	//Ahora hay que vincularlos a un programa.

	//Creamos un programa y vinculamos los shaders
	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	//Los Atributos SIEMPRE se añaden antes de linkar el programa, se proponen los valores
	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");

	//LINKADO
	glLinkProgram(program);

	//El linkado le da los valores si puede, sino le da un -1
	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");

	//Las variable uniform se declaran despues del linkado
	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
	//Inicializamos las uniform que se van a usar
	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");
	//Uniform Luz
	posLuz = glGetUniformLocation(program, "posLuz");
	inLuz = glGetUniformLocation(program, "inLuz");

	//Preguntamos el status de linkado
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}
}

GLuint GLSLProgram::LoadShader(const char *fileName, GLenum type){
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);
	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);

	//Se le asigna al shader, la cadena de caracteres recien cargadas con una longitud
	glShaderSource(shader, 1,
		(const GLchar **)&source, (const GLint *)&fileLen);

	//Compilación del shader
	glCompileShader(shader);

	//Se libera el recurso de la cadena del shader
	delete source;

	//Comprobamos que se compiló bien
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "FILENAME: " << fileName << std::endl;
		std::cout << "Error: " << logString << std::endl;
		delete logString;
		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

void GLSLProgram::Destroy(){

	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(this->vshader);
	glDeleteShader(this->fshader);
	glDeleteProgram(program);
}

void GLSLProgram::UseProgram(){
	glUseProgram(program);
}

void GLSLProgram::AddUnif(glm::mat4 &modelView, glm::mat4 &modelViewProj, glm::mat4 &normal, float &intensidad, glm::vec3 &posLigth){

	AddUnifMat4fvMV(modelView);
	AddUnifMat4fvMVP(modelViewProj);
	AddUnifMat4fvN(normal);
	AddUnif3fv(posLigth);
	AddUnif1f(intensidad);
}

void GLSLProgram::AddUnifMat4fvMV(glm::mat4 &mat){
	glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(mat[0][0]));
}

void GLSLProgram::AddUnifMat4fvMVP(glm::mat4 &mat){
	glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(mat[0][0]));
}

void GLSLProgram::AddUnifMat4fvN(glm::mat4 &mat){
	glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(mat[0][0]));
}

void GLSLProgram::AddUnif3fv(glm::vec3 &vect){
	glUniform3fv(posLuz, 1, &(vect[0]));
}

void GLSLProgram::AddUnif1f(float &f){
	glUniform1f(inLuz, f);
}

void GLSLProgram::AddUnifTex(unsigned int color, unsigned int emi){
	AddUnifTexC(color);
	AddUnifTexE(emi);
}

void GLSLProgram::AddUnifTexC(unsigned int color){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color);
	//Le digo al shader que la textura de color la tiene que coger en la 0
	glUniform1i(uColorTex, 0);
}

void GLSLProgram::AddUnifTexE(unsigned int emi){
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, emi);
	glUniform1i(uEmiTex, 1);
}

int GLSLProgram::getPos(){
	return inPos;
}

int GLSLProgram::getColor(){
	return inColor;
}

int GLSLProgram::getNormal(){
	return inNormal;
}

int GLSLProgram::getTexCoord(){
	return inTexCoord;
}