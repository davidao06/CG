#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <iostream>


GLuint verticesB,indicesB, indiceCount;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;
int timebase;
float frame;


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

struct Coordenada {
	float x,y,z;
};

int findVerticeInVector(std::vector<float> vertices,float cx,float cy,float cz) {
	int indice = 0;
	bool encontrado = false;
	for (int i = 0;i < vertices.size() - 2 && !encontrado;i += 3) {
		float x = vertices[i];
		float y = vertices[i+1];
		float z = vertices[i+2];
		if (cx == x && cy == y && cz == z) encontrado = true;
		else indice++;
	}
	if (!encontrado) indice = -1;
	return indice;
}

void drawCylinder(float radius,float height,int slices) {
	std::vector<Coordenada> coords;

	std::vector<float> vertices;
	std::vector<GLuint> indices;
	float alpha = 2 * M_PI;
	vertices.push_back(0);vertices.push_back(0);vertices.push_back(0);
	vertices.push_back(0);vertices.push_back(height/2);vertices.push_back(0);
	vertices.push_back(radius*sin(alpha));vertices.push_back(0);vertices.push_back(radius*cos(alpha));
	vertices.push_back(radius*sin(alpha));vertices.push_back(height/2);vertices.push_back(radius*cos(alpha));
	int index = 4;

	float sliceStep = alpha/slices;
	for (int i = 0; i < slices;i++) {
		float alphaAngle = alpha - i * sliceStep;
		float alphaAngle2 = alpha - (i+1) * sliceStep;
		float x2 = radius * sin(alphaAngle2);
		float z2 = radius * cos(alphaAngle2);
		indices.push_back(index-2);
		vertices.push_back(x2);
		vertices.push_back(0);
		vertices.push_back(z2);
		indices.push_back(index);
		indices.push_back(0);

		indices.push_back(index-1);
		indices.push_back(1);
		vertices.push_back(x2);
		vertices.push_back(height/2);
		vertices.push_back(z2);
		indices.push_back(index+1);

		indices.push_back(index-2);
		indices.push_back(index-1);
		indices.push_back(index+1);

		indices.push_back(index+1);
		indices.push_back(index);
		indices.push_back(index-2);

		index +=2;
	}

	glGenBuffers(1,&verticesB);
	glBindBuffer(GL_ARRAY_BUFFER,verticesB);

	//copiar o vector para o buffer
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		vertices.data(),
		GL_STATIC_DRAW
	);

	glGenBuffers(1,&indicesB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indicesB);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(GLuint),
		indices.data(),
		GL_STATIC_DRAW
	);

	indiceCount = indices.size();
}

void prepareData() {

	//Criar o VBO
	
	drawCylinder(1,2,1000);
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER,verticesB);
	glVertexPointer(3,GL_FLOAT,0,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indicesB);
	glDrawElements(
		GL_TRIANGLES,
		indiceCount,
		GL_UNSIGNED_INT,
		0
	);

	frame++;
	float fps;
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame*1000.0/(time-timebase);
		timebase = time;
		frame = 0;
	}

	char tmp[50];
	sprintf(tmp,"FPS:%.10f",fps);
	glutSetWindowTitle(tmp);

	// End of frame
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

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin");
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

	// init GLEW
#ifndef __APPLE__
	glewInit();
#endif


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);

	spherical2Cartesian();

	printInfo();

	prepareData();
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
