#ifndef _LIGHT_H
#define _LIGHT_H

#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include <iostream>

class Light
{
	private:
		float valorIntensidad;
		glm::vec3 lightCoord;
		glm::vec3 lightPosShader;
	private:
		void MoveLight(unsigned char key, Camera camera);
		void ChangeIntensity(unsigned char key);

	public:
		Light();
		void LightController(unsigned char key, Camera camera);

		inline glm::vec3 GetPosition() { return lightPosShader; }
		inline float GetIntensity() { return valorIntensidad; }
};

#endif