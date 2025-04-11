#include <GL/glut.h>

float alpha = 0, beta = 0, delta = 1;

void keyPressed_special(int key, int x, int y) {
	switch (key) {

	case GLUT_KEY_PAGE_UP:
		delta = delta * 1.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		delta = delta * 0.9;
		break;
	case GLUT_KEY_LEFT:
		beta += 1;
		break;
	case GLUT_KEY_RIGHT:
		beta -= 1;
		break;
	case GLUT_KEY_UP:
		alpha -= 1;
		break;
	case GLUT_KEY_DOWN:
		alpha += 1;
		break;
	default:
		break;

	}
	glutPostRedisplay();
}


void drawHouse() {
	glPushMatrix();
	glTranslatef(-1.0f, -0.70f, 1.0f);
	
	glPushMatrix();
	glColor3f(0.2f, 0.173f, 0.127f);
	glScalef(0.5f, 0.5f, 0.5f);     
	glutSolidCube(1.0f);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.82f, 0.61f, 0.53f);
	glTranslatef(0.0f, 0.25f, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.6f, 0.5f, 10, 10);
	glPopMatrix();

	glPopMatrix();
}

void drawTree() {
	glPushMatrix();
	glTranslatef(1.0f, -0.95f, 1.0f);

	glPushMatrix();
	glColor3f(0.55f, 0.27f, 0.07f);
	glTranslatef(0.0f, 0.25f, 0.0f);
	glScalef(0.1f, 0.5f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.62f, 0.160f, 0.85f); 
	glTranslatef(0.0f, 0.50f, 0.0f);
	glutSolidSphere(0.3f, 20, 20);
	glPopMatrix();

	glPopMatrix();
}

void drawLampPost() {
	glPushMatrix();
	glTranslatef(0.0f, -0.55f, -1.0f);

	glPushMatrix();
	glColor3f(0.8f, 0.8f, 0.8f);
	glScalef(0.05f, 0.8f, 0.05f);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.255f, 0.243f, 0.128f);
	glTranslatef(0.0f, 0.40f, 0.0f);
	glutSolidSphere(0.1f, 20, 20);
	glPopMatrix();

	glPopMatrix();
}

void drawFloor() {
	glColor3f(0.1, 0.8, 0.1);
	glPushMatrix();
	glTranslatef(0, -1, 0);
	glScalef(4.0, 0.1, 4.0);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float diffuse[4] = { 0.65f, 0.0f, 0.0f, 1.0f }; //k_d = k_a
	float specular[4] = { 0.9f, 0.9f,0.9f, 1.0f }; //k_s
	float shininess = 65.0f; //n_s

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(beta, 0, 1, 0);
	glRotatef(alpha, 1, 0, 0);
	glScalef(delta, delta, delta);
	
	// Drawing plane zone
	drawFloor();

	// Draw objects
	drawHouse();
	drawTree();
	drawLampPost();
	

	glFlush();

}


void lighting() {

	float position[4] = { 2.0f , 2.0f, 2.0f, 1.0 };
	float white[4] = { 1.0f , 1.0f, 1.0f, 1.0f };
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//attenuation
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f); //a_0
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5f); // a_1
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);  //a_2


	//
	float globalAmbient[4] = { 0.9f, 0.9f,0.9f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

}

int init(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);

	lighting();

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	return 0;
}


int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Illumination model");

	init();
	glutSpecialFunc(keyPressed_special);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;

}