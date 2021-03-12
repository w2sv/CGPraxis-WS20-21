#define _CRT_SECURE_NO_WARNINGS

#ifdef DEBUG
#include <iostream>
#endif

#include "state.h"
#include "help.h"
#include "window.h"
#include "input.h"
#include "light.h"
#include "geometry.h"
#include "utils.h"
#include "robot.h"
#include "camera.h"
#include "image.h"
#include "text.h"

#include "../dependencies/freeglut.h"
#include "../dependencies/glext.h"


const Extrema groundMeasurements(-20, 20);



int main(int argc, char** argv) {
	init(argc, argv);

	// load objects
	Robot::loadObjects();
	Robot::setObjectMaterials();

	// load textures
	Robot::loadTextures();

	// start main loop
	glutMainLoop();

	return 0;
}



void drawScene(const Robot& robot){

	// draw ground
	setColor(.1, .1, .1);
	drawPlane(groundMeasurements, groundMeasurements);
	drawQuadraticGrid(groundMeasurements, 40, Color(.8, .0, .0));

	// draw robot
	robot.draw();
}



void processInput(Robot& robot, Camera& camera) {

	/// ---------------------Robot----------------------

	// axes angle/velocity
	robot.update();

	// reset
	if (cg_key::specialKeyState(GLUT_KEY_F1) == 1)
		robot.reset();

	// set arbitrary configuration
	else if (cg_key::specialKeyState(GLUT_KEY_F2) == 1)
		robot.setArbitraryAxesConfiguration();

	// toggle tcp coord system
	else if (cg_key::specialKeyState(GLUT_KEY_F3) == 1)
		robot.toggleDrawTCPCoordSystem();

	// approach random configuration
	else if (cg_key::specialKeyState(GLUT_KEY_F4) == 1)
		robot.initializeArbitraryAxisConfigurationApproach();

	// approach random configuration infinitely
	else if (cg_key::specialKeyState(GLUT_KEY_F5) == 1)
		robot.toggleInfiniteArbitraryAxisConfigurationApproachMode();

	/// ---------------------Camera----------------------

	// reset
	else if (1 == cg_key::specialKeyState(GLUT_KEY_LEFT))
		camera.reset();

	// orbit mode
	else if (1 == cg_key::specialKeyState(GLUT_KEY_RIGHT))
		camera.toggleMode(Camera::Mode::Orbit);

	// tcp mode
	else if (1 == cg_key::specialKeyState(GLUT_KEY_UP))
		camera.toggleMode(Camera::Mode::TCP);

	// reverse tcp mode
	else if (1 == cg_key::specialKeyState(GLUT_KEY_DOWN))
		camera.toggleMode(Camera::Mode::ReverseTCP);


	/// ---------------------Generic----------------------

	// quit program
	else if (cg_key::keyState('q'))
		exit(0); // Escape -> Programm beenden

	// display help
	else if (1 == cg_key::keyState(27))
		GlobalState::displayHelp = !GlobalState::displayHelp;

	// full screen mode
	else if (1 == cg_key::keyState('p'))
		toggleFullScreenMode();
	
	/// ---------------------Display----------------------

	// display fps
	else if (1 == cg_key::keyState('b'))
		GlobalState::displayFps = !GlobalState::displayFps;

	// global coord system
	else if (1 == cg_key::keyState('n'))
		GlobalState::drawCoordSystem = !GlobalState::drawCoordSystem;

	// display axes states text
	else if (cg_key::keyState('m') == 1)
		robot.toggleDisplayAxesStates();

	/// ---------------------Graphics----------------------

	// wireframe mode
	else if (1 == cg_key::keyState('y'))
		GlobalState::drawMode = (GlobalState::drawMode == GL_FILL) ? GL_LINE : GL_FILL; // Wireframe on/off

	// light mode
	else if (1 == cg_key::keyState('x'))
		GlobalState::lightMode = !GlobalState::lightMode;

	// backfaceculling
	else if (1 == cg_key::keyState('c'))
		GlobalState::cullMode = !GlobalState::cullMode;
}



void displayFunc(){
	static Robot robot;
	static Camera camera(&robot);
	static Text::OrthogonalProjection orthogonalProjection;

	GlobalState::updateFps();
	processInput(robot, camera);

	// Back-Buffer neu initialisieren
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Farbmodus oder Beleuchtungsmodus ?
	setDefaultLightAndMaterial(GlobalState::lightMode);

	// Kamera setzen (spherische Mausnavigation)
	camera.set();

	// Koordinatensystem zeichnen
	if (GlobalState::drawCoordSystem)
		drawCoordSystem(groundMeasurements, groundMeasurements, groundMeasurements);

	// Zeichenmodus einstellen (Wireframe on/off)
	glPolygonMode(GL_FRONT_AND_BACK, GlobalState::drawMode);

	// Backface Culling on/off);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	GlobalState::cullMode ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

	// Modell zeichnen
	drawScene(robot);

	// project text
	if (GlobalState::displayHelp || GlobalState::displayFps || robot.textToBeDisplayed()) {
		orthogonalProjection.activate(true);

		if (GlobalState::displayHelp)
			displayHelp();
		if (GlobalState::displayFps)
			displayFps();
		if (robot.textToBeDisplayed())
			robot.displayText();

		orthogonalProjection.deactivate(true);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);

	glFlush();				// Daten an GL-Server (fuer die Darstellung) schicken
	glutSwapBuffers();		// Buffers wechseln
}