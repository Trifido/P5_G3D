#include "BOX.h"
#include "auxiliar.h"
#include "GLSLProgram.h"

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
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;

//Texturas
unsigned int colorTexId;
unsigned int emiTexId;

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);
glm::mat4	model2 = glm::mat4(1.0f);

//Variable cambio intensidad
float valorIntensidad = 0.5f;
glm::vec2 lightCoord = glm::vec2(0.0f);
glm::vec3 lightPos = glm::vec3(1.0f);

//Variable que nos ayudará a rotar la cámara
float rotation = 0.0f;


//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
//Por definir


//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

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


//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName);


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
	glutCreateWindow("Prácticas GLSL");

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

	//Son datos en memoria principal
	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
	view = glm::mat4(1.0f);
	view[3].z = -6;

}
void destroy(){

	programa.Destroy();

	//Activamos un buffer de elementos 0
	//Y un array de elementos 0
	//No es del todo necesario porque ya se encuentra por defecto
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//Borramos los distintos buffers anteriormente creados
	if (programa.getPos() != -1) glDeleteBuffers(1, &posVBO);
	if (programa.getColor() != -1) glDeleteBuffers(1, &colorVBO);
	if (programa.getNormal() != -1) glDeleteBuffers(1, &normalVBO);
	if (programa.getTexCoord() != -1) glDeleteBuffers(1, &texCoordVBO);
	glDeleteBuffers(1, &triangleIndexVBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	//Borramos las texturas
	glDeleteTextures(1, &colorTexId);
	glDeleteTextures(1, &emiTexId);
}

void initObj(){
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
		GL_STATIC_DRAW);

	//Cargo las texturas de color y emisiva
	colorTexId = loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");

	//Inicializo la matriz model en el cliente
	model = glm::mat4(1.0f);
	model2 = glm::mat4(1.0f);
}

GLuint loadShader(const char *fileName, GLenum type){

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
unsigned int loadTex(const char *fileName){

	unsigned char *map;
	unsigned int w, h;
	//Primero se carga la textura desde fichero
	map = loadTexture(fileName, w, h);
	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< fileName << std::endl;
		exit(-1);
	}

	//Creamos una variable con el id de la textura
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	//Tipo de textura a modificar, numero de midmap, formato de canal interno de la tarjeta gráfica, ancho, alto, borde, los canales de los datos que le voy a pasar,
	//tipo de canal, puntero a los datos.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);

	delete[] map;

	//Generar el Mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	//Indicar el acceso a la textura
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	return texId;
}

void renderFunc(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Ejecutamos el programa
	programa.UseProgram();
	// pintado del objeto!!!!

	//Mandamos a dibujar, le decimos al shader que variables uniformes vamos a utilizar
	//Calculamos las distintas matrices
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	//Si el shader requiere de la variable uniforme
	//GLSL almacena las matrices por columnas
	//uModelViewMat presenta el puntero o identificador a la variable uniforme
	programa.AddUnif(modelView, modelViewProj, normal, valorIntensidad, lightPos);

	programa.AddUnifTex(colorTexId, emiTexId);

	//Activamos el VAO del objeto, activandose todos los VBO 
	glBindVertexArray(vao);

	//Pintamos triángulos, cogiendolos de forma disjunta de 3 en 3
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);

	modelView = view * model2;
	modelViewProj = proj * view * model2;
	normal = glm::transpose(glm::inverse(modelView));

	programa.AddUnifMat4fvMV(modelView);
	programa.AddUnifMat4fvMVP(modelViewProj);
	programa.AddUnifMat4fvN(normal);

	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);


	//Nos cepillamos el programa
	glUseProgram(NULL);

	//Siempre la última función
	glutSwapBuffers();

}
void resizeFunc(int width, int height){
	//Función para reajustar el tamaño
	float n = 0.5;
	float f = 20.0;
	float aspectRatio = (float)width / height;
	float invTan = 1.0 / tan(3.14159 / 6.0);

	//Establecemos la matriz de proyección
	proj[0].x = invTan / aspectRatio;
	proj[1].y = invTan;
	proj[2].z = (f + n) / (n - f);
	proj[2].w = -1.0;
	proj[3].z = (-2.0 * f * n) / (f - n);

	glViewport(0, 0, width, height);
}
void idleFunc(){
	//Cambio la matriz model
	static float angle = 0.0;
	angle = (angle > 2.0f * 3.14159) ? 0.0 : angle + 0.0001f;
	model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 0));

	model2 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1.0f, 0));
	model2 *= glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0, 0));
	model2 *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 2.0f, 0));

	glutPostRedisplay();
}
void keyboardFunc(unsigned char key, int x, int y){
	//Primero -> actualizamo el valor

	switch (key)
	{
	case '+':
		valorIntensidad += 0.1;
		break;
	case '-':
		valorIntensidad -= 0.1;
		break;
	case 'w':
		view[3].z++;
		break;
	case 's':
		view[3].z--;
		break;
	case 'a':
		view[3].x++;
		break;
	case 'd':
		view[3].x--;
		break;
	case 'e':
		rotation = 0.0523599f;
		break;
	case 'q':
		rotation = -0.0523599f;
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

	if (key == 'e' || key == 'q'){
		view = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)) * view;
	}

	lightPos[0] = (view * glm::vec4(lightCoord[0], 1.0f, lightCoord[1], 1.0f)).x;
	lightPos[1] = (view * glm::vec4(lightCoord[0], 0.0f, lightCoord[1], 1.0f)).y;
	lightPos[2] = (view * glm::vec4(lightCoord[0], 1.0f, lightCoord[1], 1.0f)).z;
	
}
void mouseFunc(int button, int state, int x, int y){}









