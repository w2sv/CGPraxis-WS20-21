#pragma once

#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>  // !
#include <limits>
#include <functional>
#include <map>
#include <vector>
#include <random>
#include <sstream>
#include <string>
#include <math.h>

#include "../dependencies/freeglut.h"

#include "geometry.h"
#include "input.h"
#include "wavefront.h"
#include "glutils.h"
#include "utils.h"
#include "help.h"
#include "image.h"
#include "window.h"


void loadTextures();


struct OrientationDimension {
	public:
		OrientationDimension(char incrementationKey, char decrementationKey, float startAngle, Extrema&& angleLimits, char velocityAlterationKey);

		void reset();
		float getAngle() const;
		bool angleLimitReached() const;
		void setArbitraryAngle();

		void update();
	private:
		const float maxVelocity = 3;

		float angle;
		float velocity;

		bool angleLimitReached_b;
		bool velocityLimitReached_b;

		const char velocityAlterationKey;
		const float startAngle;
		const Extrema angleLimits;
		const bool isFullRangeOfMotionDim;
		const char incrementationKey, decrementationKey;

		void updatePosition();
		void updateVelocity();

		void clipAngle();
};


#pragma region Axis
struct Axis {
	OrientationDimension orientation;

	Axis(OrientationDimension&& orientation);
	void adjustModelMatrixOrientationAccordingly() const;
	void adjustModelMatrixOrientationInversely() const;
};


struct RotationAxis : public Axis {
	using Axis::Axis;
};


struct TiltAxis : public Axis {
	using Axis::Axis;
};
#pragma endregion


std::vector<Vector2> discrete2DCircleRadiusPoints(float radius, int nPoints);



class Robot
{
public:
	static void loadObjects();
	static void setObjectMaterials();

	Robot();
	~Robot();

	void draw();  // TODO: make const again
	void update();
	void reset();
	void setArbitraryAxesConfiguration();

	void toggleDrawTCPCoordSystem();
	void toggleDisplayAxesAngles();
	void assumeSpatialTCPConfiguration() const;
private:
	const static Color BASE_COLOR;
	bool drawTCPCoordSystem;
	bool displayAxesAngles_b;

	void displayAxesAngles() const;

	/* ------------OBJECTS----------------- */

	const static int N_OBJECTS = 4;
	static cg_object3D objects[N_OBJECTS];

	enum Object {
		RotationAxis1,
		ScrewHead,
		TiltAxis1,
		TiltAxis2
	};

	/* -------------PARTS------------------ */

	void drawAxisWeight() const;

	static const std::vector<Vector2> SCREW_POSITIONS;
	void drawScrewCircle() const;

	/* ------------COMPONENTS------------------- */

	void drawPedestal() const;
	const float PEDASTEL_HEIGHT = 3.;

	void drawBase() const;
	const float LOWER_STEEL_CYLINDER_HEIGHT = 1.6;

		/* --------------AXES------------------ */

	std::vector<Axis*> axes;
	std::map<Axis*, std::function<const void()>> axis2DrawFunction;

	void drawFirstAxis() const;
	void drawSecondAxis() const;
	void drawThirdAxis() const;
	void drawFourthAxis() const;
};
#endif