#include <GL/glut.h>
#include <cmath>

// Constants
const float PI = 3.1415f;
const float DAY_NIGHT_SPEED = 0.6f;
const float SUN_DISTANCE = 3.0f;

// Global variables
float alpha = 0, beta = 0, delta = 0.5f;
float sunAngle = 0.0f;
float moonAngle = 0.0f;

float sunPosition[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float sunColor[3] = { 1.0f, 1.0f, 0.0f };
float moonPosition[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float moonColor[3] = { 0.1f, 0.1f, 0.8f };

#pragma region Lighting

void updateLightingLamp()
{
	float diffuse[4]
	{
		1,
		1,
		0.1f,
		1.0f,
	};
	float specular[4]
	{
		0.01f,
		0.01f,
		0.01f,
		1.0f
	};
	float ambient[4]
	{
		0.8f,
		0.8f,
		0.1f,
		1.0f
	};

	float position[4]
	{
		0.0f,
		0.85f,
		-1.0f,
		1.0f
	};

	// Set light properties
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

	// Attenuation
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.8f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.8f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.02f);
}

void updateLighting()
{
	// Calculate light intensity based on sun height
	float intensity = std::abs(sunPosition[1] / SUN_DISTANCE); // Will be between -1 and 1
	intensity = (intensity + 1.0f) / 2.0f; // Convert to 0-1 range

	float diffuse[4], specular[4], ambient[4];
	if (sunPosition[1] > 0)
	{
		// Day - warm intense colors
		float sunIntensity = 0.5f;
		diffuse[0] = sunIntensity * intensity;
		diffuse[1] = sunIntensity * intensity / 2;
		diffuse[2] = sunIntensity * intensity / 2;
		diffuse[3] = 1.0f;

		specular[0] = 0.1f;
		specular[1] = 0.1f;
		specular[2] = 0.1f;
		specular[3] = 1.0f;

		ambient[0] = sunIntensity * intensity;
		ambient[1] = sunIntensity * intensity;
		ambient[2] = sunIntensity * intensity / 2;
		ambient[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);
	}
	else
	{
		// Night - cool dim colors
		float moonIntensity = 0.1f;
		diffuse[0] = moonIntensity * intensity / 2;
		diffuse[1] = moonIntensity * intensity / 2;
		diffuse[2] = moonIntensity * intensity;
		diffuse[3] = 1.0f;

		specular[0] = 0.07f;
		specular[1] = 0.07f;
		specular[2] = 0.07f;
		specular[3] = 1.0f;

		ambient[0] = moonIntensity * intensity / 2;
		ambient[1] = moonIntensity * intensity / 2;
		ambient[2] = moonIntensity * intensity;
		ambient[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, moonPosition);
	}

	// Set light properties
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Attenuation
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);


	// Update sky color based on time of day
	float skyR, skyG, skyB;
	if (sunPosition[1] > 0) // Day
	{
		skyR = 0.1f * intensity;
		skyG = 0.5f * intensity;
		skyB = 0.8f * intensity;

		// Set sun color
		sunColor[0] = intensity;
		sunColor[1] = intensity;
		sunColor[2] = intensity / 2;
	}
	else // Night
	{
		skyR = 0.1f * intensity;
		skyG = 0.3f * intensity;
		skyB = 0.6f * intensity;

		// Set lamp light properties
		updateLightingLamp();

		// Set moon color
		moonColor[0] = intensity / 1.8f;
		moonColor[1] = intensity / 1.8f;
		moonColor[2] = intensity;
	}
	glClearColor(skyR, skyG, skyB, 1.0f);


	// Reduce global ambient light to make sunlight more important
	float globalAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

#pragma endregion

#pragma region Utilities

void keyPressed_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		beta += 2;
		break;
	case GLUT_KEY_RIGHT:
		beta -= 2;
		break;
	case GLUT_KEY_UP:
		alpha += 2;
		break;
	case GLUT_KEY_DOWN:
		alpha -= 2;
		break;
	case GLUT_KEY_PAGE_UP:
		delta = delta * 1.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		delta = delta * 0.9;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void timer(int)
{
	// Update sun and moon position
	sunAngle += DAY_NIGHT_SPEED;
	if (sunAngle >= 360.0f) sunAngle = 0.0f;
	moonAngle += DAY_NIGHT_SPEED;
	if (moonAngle >= 360.0f) moonAngle = 0.0f;

	// Update lighting based on new sun position
	updateLighting();

	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

#pragma endregion

#pragma region SceneObjects

void drawHouse()
{
	glPushMatrix();
	glTranslatef(-1.0f, 0.25f, 1.0f);

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

void drawTree()
{
	glPushMatrix();
	glTranslatef(1.0f, 0.0f, 1.0f);

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

void drawLampPost()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.45f, -1.0f);

	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f);
	glScalef(0.05f, 0.8f, 0.05f);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPushMatrix();

	// Disable lighting for the lampost itself (it's a light source)
	glDisable(GL_LIGHTING);

	glColor3f(0.9f, 0.9f, 0.128f);
	glTranslatef(0.0f, 0.40f, 0.0f);
	glutSolidSphere(0.1f, 20, 20);

	// Re-enable lighting
	glEnable(GL_LIGHTING);

	glPopMatrix();

	glPopMatrix();
}

void drawFloor()
{
	glColor3f(0.1, 0.8, 0.1);
	glPushMatrix();
	glScalef(4.0, 0.1, 4.0);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void drawSun()
{
	glPushMatrix();

	// Calculate the sun's orbit position
	sunPosition[0] = cos((sunAngle * PI) / 180.0f) * SUN_DISTANCE; // x
	sunPosition[1] = sin((sunAngle * PI) / 180.0f) * SUN_DISTANCE; // y
	glTranslatef(sunPosition[0], sunPosition[1], 0.0f);

	// Disable lighting for the sun itself (it's a light source)
	glDisable(GL_LIGHTING);

	glColor3f(sunColor[0], sunColor[1], sunColor[2]);
	glutSolidSphere(0.3f, 20, 20);

	// Re-enable lighting
	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void drawMoon()
{
	glPushMatrix();

	// Calculate the moon's orbit position
	moonPosition[0] = cos(((sunAngle - 180) * PI) / 180.0f) * SUN_DISTANCE; // x
	moonPosition[1] = sin(((sunAngle - 180) * PI) / 180.0f) * SUN_DISTANCE; // y
	glTranslatef(moonPosition[0], moonPosition[1], 0.0f);

	// Disable lighting for the moon itself (it's a light source)
	glDisable(GL_LIGHTING);

	glColor3f(moonColor[0], moonColor[1], moonColor[2]);
	glutSolidSphere(0.3f, 20, 20);

	// Re-enable lighting
	glEnable(GL_LIGHTING);

	glPopMatrix();
}

#pragma endregion

int init(void)
{
	// Initialize OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -5.0, 5.0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	updateLighting();

	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float diffuse[4] = { 0.65f, 0.0f, 0.0f, 1.0f }; // k_d = k_a
	float specular[4] = { 0.9f, 0.9f, 0.9f, 1.0f }; // k_s
	float shininess = 0.6f; // n_s

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(beta, 0, 1, 0);
	glRotatef(alpha, 1, 0, 0);
	glScalef(delta, delta, delta);

	// Draw the sun and moon
	drawSun();
	drawMoon();

	// Drawing plane zone
	drawFloor();

	// Draw objects
	drawHouse();
	drawTree();
	drawLampPost();

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Day-Night Cycle Simulation");

	init();
	glutSpecialFunc(keyPressed_special);
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();

	return 0;
}