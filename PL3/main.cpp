#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <vector>

struct Coordenada
{
	float x;
	float y;
	float z;
};

float px,py,pz;
float dx,dy,dz;
float raio = 5;
float camAlpha = 0;
float camBeta= 0;

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

void drawCylinder(float radius,float height,int slices) {
	std::vector<Coordenada> coords;
	float alpha = 2 * M_PI;
	float sliceStep = alpha/slices;
	for (int i = 0; i <= slices;i++) {
		float alphaAngle = alpha - i * sliceStep;
		float x = radius * sin(alphaAngle);
		float z = radius * cos(alphaAngle);
		Coordenada c;
		c.x = x;
		c.z = z;
		coords.push_back(c);
	}

	glColor3f(0,0,1);
	glBegin(GL_TRIANGLES);
	for (int i = 0;i < coords.size() - 1;i++) {
		Coordenada c1 = coords[i],c2 = coords[i+1];

		glVertex3f(c1.x,0,c1.z);
		glVertex3f(c2.x,0,c2.z);
		glVertex3f(0,0,0);

		glVertex3f(c2.x,height,c2.z);
		glVertex3f(c1.x,height,c1.z);
		glVertex3f(0,height,0);

		glVertex3f(c2.x,height,c2.z);
		glVertex3f(c2.x,0,c2.z);
		glVertex3f(c1.x,0,c1.z);

		glVertex3f(c1.x,height,c1.z);
		glVertex3f(c2.x,height,c2.z);
		glVertex3f(c1.x,0,c1.z);

		
	}
	glEnd();
}

void drawCone(float raio,float height,float slices,float stacks) {

    float sliceStep = 2 * M_PI / slices;
    float stackStep = height/stacks;
	glBegin(GL_TRIANGLES);
    for (int i = 0; i < stacks;i++) {
        float stackAngle = i * stackStep;
		float stackAngle2 = (i+1) * stackStep;
        float xy = raio * (stacks-i)/stacks; float xy2 = raio * (stacks-i-1)/stacks;
        float y = i * stackStep; float y2 = (i + 1) * stackStep;

        for (int j = 0;j < slices;j++) {
            float sliceAngle = j * sliceStep; float sliceAngle2 = (j+1) * sliceStep;
            // Coordenadas x da stack i
            float x1 = xy * sin(sliceAngle); float x2 = xy * sin(sliceAngle2);
            // Coordenadas x da stack i+1
            float x3 = xy2 * sin(sliceAngle); float x4 = xy2 * sin(sliceAngle2);
            // Coordenadas z da stack i
            float z1 = xy * cos(sliceAngle); float z2 = xy * cos(sliceAngle2);
            // Coordenadas z da stack i+1
            float z3 = xy2 * cos(sliceAngle); float z4 = xy2 * cos(sliceAngle2);

			glVertex3f(x1,y,z1);
			glVertex3f(x2,y,z2);
			glVertex3f(x4,y2,z4);

			glVertex3f(x4,y2,z4);
			glVertex3f(x3,y2,z3);
			glVertex3f(x1,y,z1);
        }
    }

    //BASE DO CONE
	for (int i = 0; i < slices;i++) {
		float alphaAngle = i * sliceStep;
		float x = raio * sin(alphaAngle);
		float z = raio * cos(alphaAngle);
		float x2 = raio * sin(alphaAngle + sliceStep);
		float z2 = raio * cos(alphaAngle + sliceStep);
		
		glVertex3f(x2,0,z2);
		glVertex3f(x,0,z);
		glVertex3f(0,0,0);

	}
	glEnd();
}

void drawSphere(float raio,float slices,float stacks) {
	
	float sliceStep = 2 * M_PI / slices;
    float stackStep = M_PI / stacks;

	for (int i = 0; i < stacks; i++)
	{
		float stackAngle = M_PI_2 - i * stackStep;
		float stackAngle2 = M_PI_2 - (i + 1) * stackStep;
		float xy = raio * cos(stackAngle); float xy2 = raio * cos(stackAngle2);
		float y = raio * sin(stackAngle); float y2 = raio * sin(stackAngle2);
		
		for (int j = 0; j < slices; j++)
		{
			float sliceAngle = j * sliceStep;
			float sliceAngle2 = (j+1) * sliceStep;
			float x1 = xy * sin(sliceAngle);float x2 = xy * sin(sliceAngle2);
			float z1 = xy * cos(sliceAngle);float z2 = xy * cos(sliceAngle2);

			float x3 = xy2 * sin(sliceAngle); float x4 = xy2 * sin(sliceAngle2);
			float z3 = xy2 * cos(sliceAngle); float z4 = xy2 * cos(sliceAngle2);

/*           Stack entre as slides j e j+1
			Para desenhar cada stack basta ligar os vertices formando um quadrado
			(x3,z3) ------------- (x4,z4)
			   |                     |
			   |                     |
			   |                     | 
			   |                     |
			   |                     |
			(x1,z1) ------------- (x2,z2)   */
			
			glVertex3f(x1,y,z1);
			glVertex3f(x2,y,z2);
			glVertex3f(x4,y2,z4);

			glVertex3f(x4,y2,z4);
			glVertex3f(x3,y2,z3);
			glVertex3f(x1,y,z1);

		}
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	px = raio * cos(camBeta) * sin(camAlpha);
	py = raio * sin(camBeta);
	pz = raio * cos(camBeta) * cos(camAlpha);
	gluLookAt(px,py,pz, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
// put the geometric transformations here

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
// put drawing instructions here
	drawCylinder(2,5,20);
	//drawCone(1,2,4,3);
	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void processSpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		if (camBeta < M_PI_2)
			camBeta+= 0.1;
	}
	if (key == GLUT_KEY_DOWN) {
		if (camBeta > -M_PI_2)
			camBeta-= 0.1;
	}

	if (key == GLUT_KEY_RIGHT) {
		camAlpha+= 0.1;
	}

	if (key == GLUT_KEY_LEFT) {
		camAlpha-= 0.1;
	}

	glutPostRedisplay();
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
	glutSpecialFunc(processSpecialKeys);
	
// put here the registration of the keyboard callbacks



//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
