#include "Mesh.h"

Mesh::Mesh()
{
	colorTex.LoadTexture("../img/color2.png");
	emiTex.LoadTexture("../img/emissive.png");
	model = glm::mat4(1.0f);
}

void Mesh::InitRender(Camera &camera)
{
	camera.SetModelView(camera.GetView() * model);
	camera.SetModelViewProj(camera.GetProj() * camera.GetView() * model);
	camera.SetNormal(glm::transpose(glm::inverse(camera.GetModelView())));
}

void Mesh::Render()
{
	//glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);
}

void Mesh::DefaultMeshRender()
{
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);
}

void Mesh::LoadVBO(unsigned int &VBO, int dataSize, float *vertexArray, int idAtrib)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, &vertexArray[0], GL_STATIC_DRAW);
	glVertexAttribPointer(idAtrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(idAtrib);
}

void Mesh::LoadIBO(unsigned int &IBO, int dataSize, float *indexArray)
{
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, &indexArray[0], GL_STATIC_DRAW);
}

void Mesh::Rotate(float angle, glm::vec3 axis)
{
	angle = (angle > 2.0f * 3.14159) ? 0.0 : angle + 0.0001f;
	model *= glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Mesh::Translate(glm::vec3 cord)
{
	model *= glm::translate(glm::mat4(1.0f), cord);
}

void Mesh::Destroy(GLSLProgram &programa) 
{
	//Borramos los distintos buffers anteriormente creados
	if (programa.getPos() != -1) glDeleteBuffers(1, &posVBO);
	if (programa.getColor() != -1) glDeleteBuffers(1, &colorVBO);
	if (programa.getNormal() != -1) glDeleteBuffers(1, &normalVBO);
	if (programa.getTexCoord() != -1) glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &triangleIndexVBO);

	colorTex.Destroy();
	emiTex.Destroy();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &posVBO);
	glDeleteBuffers(1, &colorVBO);
	glDeleteBuffers(1, &normalVBO);
	glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
}