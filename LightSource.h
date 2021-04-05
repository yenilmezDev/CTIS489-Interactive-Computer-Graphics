#pragma once
#include <GL/freeglut.h>

struct LightSource {
	int lightNo;
	float dif_color[4];
	float spec_color[4];
	bool status;
	void init(float attenuate = 0.0f) {
		glLightfv(lightNo, GL_DIFFUSE, dif_color); // Light Source Color
		glLightfv(lightNo, GL_SPECULAR, spec_color);
		glLightfv(lightNo, GL_EMISSION, dif_color);
		if (status) on();
		// setLinearAttenuation(attenuate);

	}
	void addPointLS(float size) {
		float origin[] = { 0,0,0,1 }; // 1: point light source, 0 : directional light source
		glLightfv(lightNo, GL_POSITION, origin);
		show(size);
	}

	void addPointLS() {
		float origin[] = { 0,0,0,1 }; // 1: point light source, 0 : directional light source
		glLightfv(lightNo, GL_POSITION, origin);
	}

	void addDirectionalLS(float x, float y, float z) {
		float origin[] = { x, y, z , 0 };
		glLightfv(lightNo, GL_POSITION, origin);
	}
	void on() {
		glEnable(lightNo);
		status = true;
	}
	void off() {
		glDisable(lightNo);
		status = false;
	}

	void toggle() {
		status = !status;
		if (status) on();
		else off();
	}
	void show(float size) {
		glPushAttrib(GL_ENABLE_BIT); // Save GL_LIGHTING and GL_TEXTURE_2D state 
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		
		if (status) {
			glColor3fv(dif_color);
		}
		else
		{
			glColor3f(0.3, 0.3, 0.3);
		}
		
		glutSolidCube(size);  // representative of light source.
		glPopAttrib();  // Restore Saved GL_LIGTHING state.
	}

	void setLinearAttenuation(float c) {
		glLightf(lightNo, GL_LINEAR_ATTENUATION, c);
	}
};
