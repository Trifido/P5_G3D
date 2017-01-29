#include "Light.h"

Light::Light() {
	valorIntensidad = 0.5f;
	lightCoord = glm::vec2(0.0f);
}

void Light::MoveLight(unsigned char key) {
	switch (key)
	{
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
}

void Light::ChangeIntensity(unsigned char key) {
	switch (key)
	{
		case '+':
			valorIntensidad += 0.1;
			break;
		case '-':
			valorIntensidad -= 0.1;
			break;
		default:
			break;
	}
}

void Light::LightController(unsigned char key) {
	MoveLight(key);
	ChangeIntensity(key);
}