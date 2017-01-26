#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "auxiliar.h"
#include <iostream>
#include <gl/glew.h>

class Texture
{
	private:
		unsigned int id;
	public:
		Texture(){};
		Texture(char *filename);
		
		void LoadTexture(char *filename);
		inline unsigned int GetId(){ return this->id; }
		void Destroy();

		inline ~Texture() { Destroy(); }
};

#endif