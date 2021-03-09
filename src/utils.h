#pragma once

#include <direct.h>
#include <string>
#include <cassert>
#include <random>


#ifndef	M_PI				// Pi
#define M_PI 3.14159265358979323846
#endif
#ifndef SGN					// Vorzeichen bestimmen
#define SGN(y) (((y) < 0) ? -1 : ((y) > 0))
#endif
#ifndef MIN					// Minimum bestimmen
#define MIN(a,b) ((a) > (b))? (b) : (a)
#endif
#ifndef MAX					// Maximum bestimmen
#define MAX(a,b) ((a) > (b))? (a) : (b)
#endif


char* concatenatedCharPtr(const char* a, const char* b);


/// Concatenates passed path components whilst adding \\ in between 
char* joinPath(const char* a, const char* b);


/// Assures base level of created relative file path to be set to repository root
char* getResourceSubDirPath(char* subDirTitle);


float toRadian(float degrees);


struct Extrema {
public:
	float min, max;

	Extrema(float min, float max);

	float spread() const;
};


struct Measurements {
	float height, width, depth;

	Measurements(float height, float width, float depth);
};


bool toggleFlag(bool flag);


int randInt(const Extrema& bounds);