#ifndef _GLSLPROGRAM_H
#define _GLSLPROGRAM_H

#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "auxiliar.h"
#include "Light.h"

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
	int inTangent;

	int uModelViewMat;
	int uModelViewProjMat;
	int uNormalMat;

	int uColorTex;
	int uEmiTex;
	int uSpecularTex;
	int uNormalTex;

	//Uniform luz ambiental
	int uIntAmbiental;
	//Uniforms luz puntual
	int uPosPoint, uIntPoint, uNumPoint;
	//Uniforms luz spot
	int uPosSpot, uIntSpot, uNumSpot;
	//Uniform luz direccional
	int uPosDirec, uIntDirec, uNumDirec;

	//Num. total Lights
	int numPoint, numSpot, numDirec;

	std::vector<Light*> lights;
	glm::vec3 ambientLight;

	glm::vec3 *posPointLights;
	glm::vec3 *intPointLights;
	glm::vec3 *posSpotLights;
	glm::vec3 *intSpotLights;
	glm::vec3 *posDirLights;
	glm::vec3 *intDirLights;

private:
	GLuint LoadShader(const char *fileName, GLenum type);
	
	void AddUnifNumLight(int idNum, int num);
	void AddUnifPosLight(int idUnif, glm::vec3 *vect, int sizeArray);
	void AddUnifIntLight(int idUnif, glm::vec3 *vect, int sizeArray);
	void AddUnifTexC(unsigned int color);
	void AddUnifTexE(unsigned int emi);
	void AddUnifTexS(unsigned int spec);
	void AddUnifTexN(unsigned int nor);
	void AddUnif1fAmbiental(glm::vec3 ambient);

public:
	GLSLProgram();
	void InitShader(const char *vname, const char *fname);
	void Destroy();
	void UseProgram();
	void AddUnif(glm::mat4 &modelView, glm::mat4 &modelViewProj, glm::mat4 &normal);
	void AddUnifMat4fvMV(glm::mat4 &mat);
	void AddUnifMat4fvMVP(glm::mat4 &mat);
	void AddUnifMat4fvN(glm::mat4 &mat);
	void AddUnifTex(unsigned int color, unsigned int emi, unsigned int spec, unsigned int nor);
	void AddUnifLight();

	int getPos();
	int getColor();
	int getNormal();
	int getTexCoord();
	int getTangent();

	void AddLight(Light &light);
	inline void AddAmbientLight(glm::vec3 &ai){ ambientLight = ai; }
};




#endif