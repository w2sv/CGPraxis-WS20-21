#include "utils.h"


#pragma region Vector3
Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
#pragma endregion

#pragma region Color
Color::Color(double r, double g, double b, double o) {
	this->r = r;
	this->g = g;
	this->b = b;

	this->o = o;
}

Color::Color(int r, int g, int b, double o) {
	this->r = r / 255.;
	this->g = g / 255.;
	this->b = b / 255.;

	this->o = o;
}

const void Color::render() {
	glColor4f(r, g, b, o);
}
#pragma endregion

#pragma region CoordinateExtrema
CoordinateExtrema::CoordinateExtrema(float min, float max) {
	assert(min < max);

	this->min = min;
	this->max = max;
}

CoordinateExtrema::CoordinateExtrema(float absValue) {
	this->min = -absValue;
	this->max = absValue;
}

const float CoordinateExtrema::spread() {
	return abs(this->max - this->min);
}
#pragma endregion

#pragma region Rotation

void glRotatep(float angle, const GLfloat* axis) {
	glRotatef(angle, *(axis), *(axis + 1), *(axis + 2));
}

GLfloat const Axes::X[3] = { 1., 0., 0. };
GLfloat const Axes::Y[3] = { 0., 0., 1. };
GLfloat const Axes::Z[3] = { 0., 1., 0. };
#pragma endregion