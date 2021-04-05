#pragma once
#include <GL/freeglut.h>
struct Material {
	float ambient_color[4];  // color of pixel that does not see light sources (color of dark areas)
	float dif_color[4]; // color of the object indepent from where you look at it.
	float spec_color[4];
	float shininess; // flatness/roughness of the object, 0 - 128 (metalic shiny surface)
	void use() {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dif_color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif_color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_color);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); // 0 (opaque) - 128 (metalic)
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, dif_color);
	}
};



// You can find Material Parameters from internet.
Material Default = {
	{0.2f, 0.2f, 0.2f, 1.0f  },
	{0.8f, 0.8f, 0.8f, 1.0f },
	{0.0f, 0.0f, 0.0f, 1.0f },
	0.0f
};


Material PolishedBronze = {
	{0.25f, 0.148f, 0.06475f, 1.0f  },
	{0.4f, 0.2368f, 0.1036f, 1.0f },
	{0.774597f, 0.458561f, 0.200621f, 1.0f },
	76.8f
};

Material Golden = {
	{ 0.24725f, 0.1995f, 0.0745f, 1.0f },
	{0.75164f, 0.60648f, 0.22648f, 1.0f },
	{0.628281f, 0.555802f, 0.366065f, 1.0f },
	51.2f
};

Material Perl = {
	{ 0.25f, 0.20725f, 0.20725f, 0.922f },
	{1.0f, 0.829f, 0.829f, 0.922f },
	{0.296648f, 0.296648f, 0.296648f, 0.922f },
	11.264f
};

Material GreenRubber = {
	{ 0.0f,0.05f,0.0f,1.0f },
	{ 0.4f,0.5f,0.4f,1.0f},
	{0.04f,0.7f,0.04f,1.0f },
	10.0f
};

Material GreenRubberTransparent = {
	{ 0.0f,0.05f,0.0f,0.5f },
	{ 0.4f,0.5f,0.4f,0.5f},
	{0.04f,0.7f,0.04f,0.5f },
	10.0f
};


Material PolishedSilver = {
	{ 0.23125f, 0.23125f, 0.23125f, 1.0f },
	{0.2775f, 0.2775f, 0.2775f, 1.0f },
	{0.773911f, 0.773911f, 0.773911f, 1.0f },
	89.6f
};
