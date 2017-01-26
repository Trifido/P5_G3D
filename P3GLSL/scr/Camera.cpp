#include "Camera.h"

Camera::Camera() 
{
	this->proj = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
	this->rotation = 0.0f;
}

void Camera::MoveCamera(unsigned char key) {
	switch (key) {
		case 'w':
			maxZView();
			break;
		case 's':
			minusZView();
			break;
		case 'a':
			maxXView();
			break;
		case 'd':
			minusXView();
			break;
		case 'e':
			//rotation = 0.0523599f;
			IncrementRot();
			break;
		case 'q':
			//rotation = -0.0523599f;
			DecrementRot();
			break;
	}

	if (key == 'e' || key == 'q') {
		//camera.GetView() = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)) * camera.GetView();
		RotateCamera();
	}
}

void Camera::RotateCamera()
{
	this->view = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)) * this->view;
}

bool Camera::InitCamera() 
{
	this->proj = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 500.0f);
	this->view = glm::mat4(1.0f);
	this->view[3].z = -300.0f;

	return true;
}

void Camera::ResizeAspectRatio(float width, float height)
{ 
	this->proj = glm::perspective(glm::radians(60.0f), float(width) / float(height), this->nearp, this->farp);
}

void Camera::SetModelView(glm::mat4 mv)
{
	modelView = mv;
}

void Camera::SetModelViewProj(glm::mat4 mvp)
{
	modelViewProj = mvp;
}

void Camera::SetNormal(glm::mat4 norm)
{
	normal = norm;
}

Camera::~Camera() {}