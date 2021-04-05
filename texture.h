#ifndef __CTIS_TEXTURE__
#define __CTIS_TEXTURE__

#include <gl/freeglut.h>

class  Texture {
public:
	GLuint id ;
	int width ;
	int height ;
	int mode ;   // 3: RGB, 4: RGBA
	unsigned char *data ; 
	Texture() { id=0; width=0; height=0; data = nullptr; mode = 0; }
	int  Load( const char *file , int clamp = GL_REPEAT  ) ;
	void  Unload() ; 
	void flip() ;
	void use();
	static void replace() {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}

	static void modulate() {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
} ;

#endif