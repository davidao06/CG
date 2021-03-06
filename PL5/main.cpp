#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <iostream>

float angleCowboy = 0,angleIndio = 0;
float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawTree (float x,float z) {
	glPushMatrix();
	glTranslatef(x,0,z);
	glColor3f(0.4f,0.2,0);
	glPushMatrix();
	glRotatef(-90,1,0,0);
	glutSolidCone(1,4,4,2);
	glPopMatrix();
	glTranslatef(0,2,0);
	float greenR = (rand()/(float) RAND_MAX) * 0.2;
	float blueR = (rand()/(float) RAND_MAX) * 0.2;
	glColor3f(0,0.6 + greenR,blueR);
	glRotatef(-90,1,0,0);
	glutSolidCone(2,10,4,2);
	glPopMatrix();
}

void drawCowboys(int nrCowboys) {
	float angulo = 0;
	float anguloTurn = -90;
	for (float i = 0; i < nrCowboys; i++) {
		glPushMatrix();
		angulo = i * ((2 * M_PI)/ nrCowboys);
		float x = sin(angulo) * 15;
		float z = cos(angulo) * 15;
		glTranslatef(x,1.5,z);
		glColor3f(0,0,1);
		glRotatef(anguloTurn,0,1,0);
		anguloTurn += 360/(float) nrCowboys;
		glutSolidTeapot(1.5);
		glPopMatrix();
	}
}

void drawIndios(int nrIndios) {
	float angulo = 0;
	float anguloTurn = 0;
	for (float i = 0; i < nrIndios; i++) {
		angulo = i *((2 * M_PI)/ nrIndios);
		glPushMatrix();
		float x = sin(angulo) * 35;
		float z = cos(angulo) * 35;
		glTranslatef(x,1.7,z);
		glColor3f(1,0,0);
		glRotatef(anguloTurn,0,1,0);
		anguloTurn += 360/(float) nrIndios;
		glutSolidTeapot(1.5);
		glPopMatrix();
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	
	srand(2022);
	for (int i = 0; i < 300;) {
		float x = ((rand() / (float) RAND_MAX) * 200) - 100;
		float z = ((rand() / (float) RAND_MAX) * 200) - 100;
		if (pow(x,2) + pow(z,2) > pow(50,2)) {
			drawTree(x,z);
			i++;
		}
	}
	
	glColor3f(1,0.6,0.6);
	glPushMatrix();
	glTranslatef(0,0.8,0);
	glutSolidTorus(2,4,64,64);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angleCowboy,0,1,0);
	angleCowboy -= 1;
	drawCowboys(8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angleIndio,0,1,0);
	angleIndio += 1;
	drawIndios(16);
	glPopMatrix();
	
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
