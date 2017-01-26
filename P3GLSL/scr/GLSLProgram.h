#ifndef _GLSLPROGRAM_H
#define _GLSLPROGRAM_H

#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "auxiliar.h"

class GLSLProgram
{
private:
	char *vname, *fname;

	unsigned int vshader;
	unsigned int fshader;
	unsigned int program;

	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;

	int inLuz;
	int posLuz;

	int uModelViewMat;
	int uModelViewProjMat;
	int uNormalMat;

	int uColorTex;
	int uEmiTex;

	GLuint LoadShader(const char *fileName, GLenum type);
	
	void AddUnif3fv(glm::vec3 &vect);
	void AddUnif1f(float &f);
	void AddUnifTexC(unsigned int color);
	void AddUnifTexE(unsigned int emi);

public:
	void InitShader(const char *vname, const char *fname);
	void Destroy();
	void UseProgram();
	void AddUnif(glm::mat4 &modelView, glm::mat4 &modelViewProj, glm::mat4 &normal, float &intensidad, glm::vec3 &posLigth);
	void AddUnifMat4fvMV(glm::mat4 &mat);
	void AddUnifMat4fvMVP(glm::mat4 &mat);
	void AddUnifMat4fvN(glm::mat4 &mat);
	void AddUnifTex(unsigned int color, unsigned int emi);

	int getPos();
	int getColor();
	int getNormal();
	int getTexCoord();

};




#endif