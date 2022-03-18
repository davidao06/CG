#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

float xPos = 0,yPos = 0,zPos = 0;
float xScale = 1,yScale = 1,zScale = 1;
float angle = 0;
GLenum mode = GL_FILL;


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


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f( 100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

// put the geometric transformations here
	glTranslatef(xPos,yPos,zPos);
	glRotatef(angle,0,1,0);
	glScalef(xScale,yScale,zScale);
	glPolygonMode(GL_FRONT,mode);

// put drawing instructions here
	glBegin(GL_TRIANGLES);
	glColor3f(0,0,1.0);
	glVertex3f(xPos+ 1.0f, 0.0f, zPos-1.0f);
	glVertex3f(xPos+ 1.0f, 0.0f, zPos+1.0f);
	glVertex3f(xPos- 1.0f, 0.0f, zPos-1.0f);

	glColor3f(1.0,1.0,1.0);
	glVertex3f(xPos- 1.0f, 0.0f, zPos+1.0f);
	glVertex3f(xPos- 1.0f, 0.0f, zPos-1.0f);
	glVertex3f(xPos+ 1.0f, 0.0f, zPos+1.0);

	glColor3f(1.0f,0,0);
	glVertex3f(xPos+ 1.0f, 0.0f, zPos+1.0f);
	glVertex3f(xPos+ 1.0f, 0.0f, zPos-1.0f);
	glVertex3f(xPos, 2.0f, zPos);

	glColor3f(0,1.0f,0);
	glVertex3f(xPos - 1.0f, 0.0f, zPos+1.0f);
	glVertex3f(xPos+ 1.0f, 0.0f, zPos+1.0f);
	glVertex3f(xPos, 2.0f, zPos);

	glColor3f(1.0,1.0,0);
	glVertex3f(xPos- 1.0f, 0.0f, zPos-1.0f);
	glVertex3f(xPos- 1.0f, 0.0f, zPos+1.0f);
	glVertex3f(xPos, 2.0f, zPos);

	glColor3f(0,1.0,1.0);
	glVertex3f(xPos+ 1.0f, 0.0f, zPos-1.0f);
	glVertex3f(xPos- 1.0f, 0.0f, zPos-1.0f);
	glVertex3f(xPos, 2.0f, zPos);
	glEnd();
	

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void specialKeys (int key,int x,int y) {
	if (key == GLUT_KEY_UP) {
		yScale += 0.2;
	}

	if (key == GLUT_KEY_DOWN) {
		yScale -= 0.2;
	}

	glutPostRedisplay();
}

void normalKeys(unsigned char key,int x,int y) {
	if (key == 65 || key == 97) {
		xPos++;
	}
	if (key == 68 || key == 100) {
		xPos--;
	}
	if (key == 83 || key == 115) {
		zPos--;
	}
	if (key == 87 || key == 119) {
		zPos++;
	}
	if (key == 76 || key == 108) {
		angle += 5;
	}
	if (key == 82 || key == 114) {
		angle -= 5;
	}
	if (key == 90 || key == 122) {
		mode = GL_FILL;
	}
	if (key == 88 || key == 120 ){
		mode = GL_LINE;
	}
	if (key == 67 || key == 99) {
		mode = GL_POINT;
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

	
// put here the registration of the keyboard callbacks
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(normalKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
