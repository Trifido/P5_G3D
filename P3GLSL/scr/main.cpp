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

//Camera
Camera camera;

//Meshes
Mesh cube1, cube2;

//Variable cambio intensidad
Light light1;
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	cube1.Destroy(programa);
	cube2.Destroy(programa);
}

void initObj()
{
	programa.AddLight(light1);
	cube1.AddShader(programa);
	cube2.AddShader(programa);
	cube1.InitDefaultMesh();
	cube2.InitDefaultMesh();
}

void renderFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cube1.InitRender(camera);
	cube1.DefaultMeshRender();

	cube2.InitRender(camera);
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
	angle += 0.1f;

	cube1.Rotation(angle, glm::vec3(1.0f, 1.0f, 0));
	cube2.Orbit(angle, angle, glm::vec3(3.0f, 0, 0));

	glutPostRedisplay();
}
void keyboardFunc(unsigned char key, int x, int y){
	camera.MoveCamera(key);
	light1.LightController(key, camera);
}
void mouseFunc(int button, int state, int x, int y){}