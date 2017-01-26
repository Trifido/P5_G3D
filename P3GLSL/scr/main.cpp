#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "GLSLProgram.h"
#include "BOX.h"
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
#include <iostream>


//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//VAO
unsigned int vao;
//VBOs que forman parte del objeto
//unsigned int posVBO;
//unsigned int colorVBO;
//unsigned int normalVBO;
//unsigned int texCoordVBO;
//unsigned int triangleIndexVBO;

//Camera
Camera camera;

//Meshes
Mesh cube1, cube2;

//Variable cambio intensidad
float valorIntensidad = 0.5f;
glm::vec2 lightCoord = glm::vec2(0.0f);
glm::vec3 lightPos = glm::vec3(1.0f);

GLSLProgram programa;

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initObj();
void destroy();

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();
	programa.InitShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag"); //v0 sin textura, V1 con textura
	initObj();

	glutMainLoop();

	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv){
	//Crea un contexto básico
	glutInit(&argc, argv);
	//Genera un contexto de opengl 3.3
	glutInitContextVersion(3, 3);
	//Que no tenga recompatibilidad hacia atrás
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//ELEMENTOS DEL FRAMEBUFFER
	//Crea el framebuffer con profundidad
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Práctica 5");

	//Se pone esto porque glew es muy antigua, para poner funcionalidades experimentales
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	// Preguntamos con que versión de opengl estamos trabajando
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
}

void initOGL(){
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	//Activar Culling, pintar cara front y back rellenas.
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	camera.InitCamera();
}

void destroy(){

	programa.Destroy();

	//Activamos un buffer de elementos 0
	//Y un array de elementos 0
	//No es del todo necesario porque ya se encuentra por defecto
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	cube1.Destroy(programa);
	cube2.Destroy(programa);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
}

void initObj()
{
	//Creo el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (programa.getPos() != -1)
	{
		//Creo y subo datos
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 3, cubeVertexPos, GL_STATIC_DRAW);
		//Quiero meter el VBO activo en inPos, coges vectores de 3 elementos, cada elemento es de tipo float,
		//antes de pasarlo al shader no lo normalizas, straigh y offset
		glVertexAttribPointer(programa.getPos(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		//Le digo al VAO que use el atributo inPos
		glEnableVertexAttribArray(programa.getPos());
	}
	if (programa.getColor() != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(programa.getColor(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(programa.getColor());
	}
	if (programa.getNormal() != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(programa.getNormal(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(programa.getNormal());
	}
	if (programa.getTexCoord() != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(programa.getTexCoord(), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(programa.getTexCoord());
	}

	//Creo un buffer de posiciones, en el último element array buffer activo
	//aún no está configurado, se configurará cuando se pase a la etapa de pintar
	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex*sizeof(unsigned int)* 3, cubeTriangleIndex,
		GL_STATIC_DRAW)
}

void renderFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Ejecutamos el programa
	programa.UseProgram();
	// pintado del objeto!!!!

	//Mandamos a dibujar, le decimos al shader que variables uniformes vamos a utilizar
	//Calculamos las distintas matrices

	cube1.InitRender(camera);

	//Si el shader requiere de la variable uniforme
	//GLSL almacena las matrices por columnas
	//uModelViewMat presenta el puntero o identificador a la variable uniforme
	programa.AddUnif(camera.GetModelView(), camera.GetModelViewProj(), camera.GetNormal(), valorIntensidad, lightPos);

	programa.AddUnifTex(cube1.GetColorId(), cube1.GetEmiteId());

	//Activamos el VAO del objeto, activandose todos los VBO 
	glBindVertexArray(vao);

	//Pintamos triángulos, cogiendolos de forma disjunta de 3 en 3
	cube1.DefaultMeshRender();

	cube2.InitRender(camera);

	programa.AddUnifMat4fvMV(camera.GetModelView());
	programa.AddUnifMat4fvMVP(camera.GetModelViewProj());
	programa.AddUnifMat4fvN(camera.GetNormal());

	cube2.DefaultMeshRender();

	//Nos cepillamos el programa
	glUseProgram(NULL);

	//Siempre la última función
	glutSwapBuffers();
}

void resizeFunc(int width, int height)
{
	camera.ResizeAspectRatio(width, height);
	glViewport(0, 0, width, height);
}

void idleFunc()
{
	//Cambio la matriz model
	static float angle = 0.0;
	//angle = (angle > 2.0f * 3.14159) ? 0.0 : angle + 0.1f;
	angle += 0.1f;

	cube1.Rotate(angle, glm::vec3(1.0f, 1.0f, 0));

	cube2.Rotate(angle, glm::vec3(1.0f, 1.0f, 0));
	cube2.Translate(glm::vec3(3.0f, 0, 0));
	cube2.Rotate(angle, glm::vec3(0, 2.0f, 0));

	glutPostRedisplay();
}
void keyboardFunc(unsigned char key, int x, int y){
	//Primero -> actualizamo el valor

	camera.MoveCamera(key);

	switch (key)
	{
	case '+':
		valorIntensidad += 0.1;
		break;
	case '-':
		valorIntensidad -= 0.1;
		break;
	case 'i':
		lightCoord[1] += 0.1f;
		break;
	case 'k':
		lightCoord[1] -= 0.1f;
		break;
	case 'j':
		lightCoord[0] -= 0.1f;
		break;
	case 'l':
		lightCoord[0] += 0.1f;
		break;
	default:
		break;
	}

	lightPos[0] = (camera.GetView() * glm::vec4(lightCoord[0], 1.0f, lightCoord[1], 1.0f)).x;
	lightPos[1] = (camera.GetView() * glm::vec4(lightCoord[0], 0.0f, lightCoord[1], 1.0f)).y;
	lightPos[2] = (camera.GetView() * glm::vec4(lightCoord[0], 1.0f, lightCoord[1], 1.0f)).z;
	
}
void mouseFunc(int button, int state, int x, int y){}