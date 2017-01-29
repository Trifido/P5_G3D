#ifndef _LIGHT_H
#define _LIGHT_H

#include <gl/glew.h>
#include <gl/gl.h>
#include <glm/glm.hpp>

class Light
{
	private:
		float valorIntensidad;
		glm::vec3 lightCoord;

	private:
		void MoveLight(unsigned char key);
		void ChangeIntensity(unsigned char key);

	public:
		Light();
		void LightController(unsigned char key);

		inline glm::vec3 GetPosition() { return lightCoord; }
		inline float GetIntensity() { return valorIntensidad; }
};

#endif