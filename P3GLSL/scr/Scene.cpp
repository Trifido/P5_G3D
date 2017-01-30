#include "Scene.h"

void Scene::AddObject(Mesh &object) {
	objects.push_back(&object);
}

void Scene::AddLight(Light &light) {
	switch (light.GetType()){
	case 0:
		if (dirPoint.size() < num_max){
			lights.push_back(&light);
			dirPoint.push_back(light.GetPosition());

		}
		break;
	case 1:
		if (dirSpot.size() < num_max){
			lights.push_back(&light);
			dirSpot.push_back(light.GetPosition());

		}
		break;
	case 2:
		if (dirDirect.size() < num_max){
			lights.push_back(&light);
			dirDirect.push_back(light.GetPosition());

		}
	}
	
}

void Scene::AddCamera(Camera &camera) {
	cameras.push_back(&camera);
}

void Scene::Render(){
	for (int i = 0; i < objects.size(); i++){
		(*objects.at(i)).InitRender((*cameras.at(0)));
		(*objects.at(i)).DefaultMeshRender();
	}
}

Mesh Scene::getObject(int i){
	return (*objects.at(i));
}

Camera Scene::getCamera(int i){
	return (*cameras.at(i));
}