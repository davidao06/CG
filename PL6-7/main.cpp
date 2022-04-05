

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <iostream>
#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


float camX = 0, camY = 0, camZ = 0;
int startX, startY, tracking = 0;
float alturaPerson = 1.85f;
int alpha = 0, beta = 45, r = 50;
float angle = 0;
int vertical = 0,horizontal = 0;
float px = 0,py = 0,pz = 0;

std::vector<float> vertices;

GLuint buffers;

float angleCowboy = 0,angleIndio = 0;

unsigned int t, tw, th;
unsigned char *imageData;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
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

float height(float x,float z) {
	float res;
	float x1 = floor(x);
	float z1 = floor(z);
	float fx = x - x1;
	float fz = z - z1;
	int i =  x1 + 127.5;
	int j = z1 + 127.5;
	float h_x1_z = (imageData[th * j + i]) * (1 - fz) + (imageData[th * (j+1) + i]) * fz;
	float h_x2_z = (imageData[th * j + (i+1)]) * (1 - fz) + (imageData[th * (j+1) + i+1]) * fz;

	res = h_x1_z * (1-fx) + h_x2_z * fx;
	return res;
}

void drawTerrain() {

	glBindBuffer(GL_ARRAY_BUFFER, buffers);

    // colocar aqui o código de desenho do terreno usando VBOs com TRIANGLE_STRIPS
	for (float z = -127.5;z < 127.5;z++) {
		for (float x = -127.5;x <= 127.5;x++) {
			float h1 = height(x,z);
			float h2 = height(x,z+1);
			vertices.push_back(x);
			vertices.push_back(h1);
			vertices.push_back(z);

			vertices.push_back(x);
			vertices.push_back(h2);
			vertices.push_back(z+1);			
		}
	}

	glBufferData(GL_ARRAY_BUFFER,sizeof(float) * vertices.size(),vertices.data(),GL_STATIC_DRAW);
}

void drawTree (float x,float z) {
	glPushMatrix();
	glTranslatef(x,height(x,z) - 0.2,z);	
	glColor3f(0.4f,0.2,0);
	glPushMatrix();
	glRotatef(-90,1,0,0);
	glutSolidCone(1,5,4,2);
	glPopMatrix();
	glTranslatef(0,2,0);
	float greenR = (rand()/(float) RAND_MAX) * 0.2;
	float blueR = (rand()/(float) RAND_MAX) * 0.2;
	glColor3f(0,0.6 + greenR,blueR);
	glRotatef(-90,1,0,0);
	glutSolidCone(2,10,4,2);
	glPopMatrix();
}


void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	py = alturaPerson + height(px,pz);
	glLoadIdentity();
	gluLookAt(px, py, pz, 
		      px + sin(angle * 3.14 / 180.0),py,pz + cos(angle * 3.14 / 180.0),
			  0.0f,1.0f,0.0f);

	glColor3f(1,1,1);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBindBuffer(GL_ARRAY_BUFFER, buffers);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	for (int i = 0; i < th - 1; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, tw * 2 * i, tw * 2);
	}

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	srand(2021);
	for (int i = 0; i < 300;) {
		float x = ((rand() / (float) RAND_MAX) * 255) - 127.5;
		float z = ((rand() / (float) RAND_MAX) * 255) - 127.5;
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

// End of frame
	glutSwapBuffers();
}

void processSpecialKeys(int key,int xx,int yy) {
	if (key == GLUT_KEY_RIGHT) {
		angle-= 2;
	}

	if (key == GLUT_KEY_LEFT) {
		angle+= 2;
	}

	glutPostRedisplay();
}

void processKeys(unsigned char key, int xx, int yy) {

	if (key == 87 || key == 119) {
		px = px + 1 * sin(angle * 3.14 / 180.0);
		pz = pz + 1 * cos(angle * 3.14 / 180.0);
	}

	if (key == 83 || key == 115) {
		px = px - 1 * sin(angle * 3.14 / 180.0);
		pz = pz - 1 * cos(angle * 3.14 / 180.0);
	}

	if (key == 68 || key == 100) {
		px = px - sin((angle+90) * 3.14 / 180.0);
		pz = pz - cos((angle+90) * 3.14 / 180.0);
	}
	
	if (key == 65 || key == 97) {
		px = px + sin((angle+90) * 3.14 / 180.0);
		pz = pz + cos((angle+90) * 3.14 / 180.0);
	}

	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}


void init() {
	glewInit();
	ilInit();
	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1,&buffers);
	

// 	Load the height map "terreno.jpg"
	ilGenImages(1,&t);
	ilBindImage(t);
	// terreno.jpg is the image containing our height map
	ilLoadImage((ILstring)"terreno.jpg");
	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	// important: check tw and th values
	// both should be equal to 256
	// if not there was an error loading the image
	// most likely the image could not be found
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();
// 	Build the vertex arrays

// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	drawTerrain();
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
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	init();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

