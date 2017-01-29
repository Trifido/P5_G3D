#ifndef _MESH_H
#define _MESH_H

#include "BOX.h"
#include "GLSLProgram.h"
#include "Texture.h"
#include "Camera.h"

//enum Type
//{
//		ASSIMP_MODEL = 0,
//		PRIMITIVE_MODEL = 1
//};

class Mesh
{
	private:
		//VBOs que forman parte del objeto
		unsigned int posVBO;
		unsigned int colorVBO;
		unsigned int normalVBO;
		unsigned int texCoordVBO;
		unsigned int triangleIndexVBO;
		//Textures
		Texture colorTex;
		Texture emiTex;
		//Model matrix
		glm::mat4 model;
		//ProgramShaders
		GLuint vao;
		GLSLProgram *programa;


	private:
		void LoadVBO(unsigned int &VBO, int dataSize, const float *vertexArray, GLint size, int idAtrib);
		void LoadIBO(unsigned int &IBO, int dataSize, const unsigned int *indexArray);
		void Rotate(float &angle, glm::vec3 axis);
		void Translate(glm::vec3 cord);

	public:
		Mesh();	//Default Mesh (Cube)
//		Mesh(Type type, const char *filename);
//		Mesh(Type type, const char *filename, const char *fileColorTex);
////		Mesh(Type type, const char *filename, const char *fileEmiTex);
//		Mesh(Type type, const char *filename, const char *fileColorTex, const char *fileEmiTex);
		~Mesh();
		
		void Destroy(GLSLProgram &programa);

		void Rotation(float &angle, glm::vec3 axis);
		void Orbit(float &internAngle, float &externAngle, glm::vec3 externRadius);
		void InitRender(Camera &camera);
		void Render();
		void DefaultMeshRender();
		void InitDefaultMesh();

		inline void AddShader(GLSLProgram &ps) { programa = &ps; }
		inline unsigned int GetEmiteId() { return emiTex.GetId(); }
		inline unsigned int GetColorId() { return colorTex.GetId(); }
};

#endif