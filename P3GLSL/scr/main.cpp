#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "GLSLProgram.h"
#include "BOX.h"
#include "auxiliar.h"
#include "Scene.h"

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
Mesh ogre("../img/diffuse.png", "../img/bump.png");
Mesh cube1;
Mesh cube2;
Mesh cube3;
Mesh cube4;

//Variable cambio intensidad
Light light1;
Light light2(DIRECTIONAL_LIGHT);
Light light3(SPOT_LIGHT);

float valorIntensidad = 0.5f;
glm::vec2 lightCoord = glm::vec2(0.0f);
glm::vec3 lightPos = glm::vec3(1.0f);

GLSLProgram programa;
GLSLProgram programa2;
GLSLProgram programa3;

Scene scene1;

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
	programa2.InitShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag");
	programa3.InitShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag");
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
	programa2.Destroy();
	programa3.Destroy();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	ogre.Destroy(programa);
	cube1.Destroy(programa2);
	cube2.Destroy(programa2);
	cube3.Destroy(programa3);
	cube4.Destroy(programa3);
}

void initObj()
{
	light2.SetIntensity(glm::vec3(1.0, 0.0, 0.0));
	light3.SetIntensity(glm::vec3(0.0, 1.0, 0.0));

	programa.AddLight(light1);
	programa.AddAmbientLight(scene1.getAmbientLight());

	programa2.AddLight(light2);
	programa2.AddAmbientLight(scene1.getAmbientLight());
	
	programa3.AddLight(light3);
	programa3.AddAmbientLight(scene1.getAmbientLight());

	ogre.AddShader(programa);
	cube1.AddShader(programa2);
	cube2.AddShader(programa2);
	cube3.AddShader(programa3);
	cube4.AddShader(programa3);
	
	ogre.InitMesh("../Mallas/ogre.ply");
	cube1.InitDefaultMesh();
	cube2.InitDefaultMesh();
	cube3.InitDefaultMesh();
	cube4.InitDefaultMesh();

	scene1.AddObject(ogre);
	scene1.AddObject(cube1);
	scene1.AddObject(cube2);
	scene1.AddObject(cube3);
	scene1.AddObject(cube4);

	scene1.AddLight(light1);
	scene1.AddLight(light2);
	scene1.AddLight(light3);
	
	scene1.AddCamera(camera);

}

void renderFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene1.Render();

	glutSwapBuffers();
}

void resizeFunc(int width, int height)
{
	camera.ResizeAspectRatio(width, height);
	glViewport(0, 0, width, height);
}

void idleFunc()
{
	scene1.Animation();

	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){
	camera.MoveCamera(key);
	light1.LightController(key, camera);
}

void mouseFunc(int button, int state, int x, int y){}