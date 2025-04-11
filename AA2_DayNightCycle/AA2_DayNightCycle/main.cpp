#include <GL/glut.h>



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

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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

	return 0;
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
	glutSolidSphere(1.5, 40, 40);

	glFlush();

}


int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Illumination model");

	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;

}