#include <GL/freeglut_std.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_W 800
#define WINDOW_H 500

static GLfloat screenLeft = -49.5;
static GLfloat screenRight = 49.5;
static const int FPS = 60;
static GLfloat stick1Position = 0.0;
static GLfloat stickThick = 1.0;
static GLfloat stickSize = 15;

static GLfloat stick2Position = 0.0;
static GLfloat stick2Thick = -1.0;

static char score_1[20], score_2[20];
static GLint player1_score = 0, player2_score = 0;



static GLfloat moveXFactor = 0.2;
static GLfloat moveYFactor = 0.1;

struct _ball
{
	GLfloat radius = 1.5;
	GLfloat X = 0.0;
	GLfloat Y = 0.0;
	int directionX = -1;
	int directionY = 1;
}ball;



void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0 / aspect, 50.0 / aspect, -1.0, 1.0);
}

void draw_circle(float x, float y, float radius) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	static const int circle_points = 100;
	static const float angle = 2.0f * 3.1416f / circle_points;

	// this code (mostly) copied from question:
	glBegin(GL_POLYGON);
	double angle1 = 0.0;
	glVertex2d(radius * cos(0.0), radius * sin(0.0));
	int i;
	for (i = 0; i < circle_points; i++)
	{
		glVertex2d(radius * cos(angle1), radius * sin(angle1));
		angle1 += angle;
	}
	glEnd();
	glPopMatrix();
}

void drawStrokeText(char* string, float x, float y, float z)
{
	char* c;
	glRasterPos3f(x, y, z);
	
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRectf(screenLeft, stick1Position, screenLeft + stickThick, stick1Position + stickSize);
	glRectf(screenRight + stick2Thick, stick2Position, screenRight, stick2Position + stickSize);

	snprintf(score_1, sizeof(score_1), "%d", player1_score);
	drawStrokeText(score_1, -5, 25, 0);

	
	snprintf(score_2, sizeof(score_2), "%d", player2_score);
	drawStrokeText(score_2, 5, 25 ,0);
	draw_circle(ball.X, ball.Y, ball.radius);
	
	
	
	glFlush();
	glutSwapBuffers();
}


void timer(int v)
{
	glutPostRedisplay();
	
	if (ball.X < screenLeft + stickThick + ball.radius - 0.1)
	{
		if (((stick1Position) < ball.Y) && (ball.Y < (stick1Position + stickSize)) && (ball.X > screenLeft + stickThick))
		{
			ball.directionX = ball.directionX * (-1);
			moveXFactor = moveXFactor + 0.5;
			
		}
		else if (ball.X < screenLeft + stickThick)
		{
			ball.X = 0;
			ball.Y = 0;
			moveXFactor = 0.2;
			player2_score++;
		}
	}
	if (ball.X > screenRight + stick2Thick - ball.radius - 0.1)
	{
		
		
		if (((stick2Position) < ball.Y) && (ball.Y < (stick2Position + stickSize)) && (ball.X > screenRight + stick2Thick -5))
		{
			ball.directionX = ball.directionX * (-1);
			moveXFactor = moveXFactor + 0.5;
		
		}
		else if (ball.X > screenRight + stickThick)
		{
			ball.X = 0;
			ball.Y = 0;
			moveXFactor = 0.2;
			
			player1_score++;

		}
	}

	if (ball.Y > 29.5 || ball.Y < -29.5)
	{
		ball.directionY = ball.directionY * -1;
	}
	ball.X = ball.X + (moveXFactor * ball.directionX);
	ball.Y = ball.Y + (moveYFactor * ball.directionY);
	
	glutTimerFunc(1000 / FPS, timer, v);
}




void kbSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (stick1Position + stickSize < 31)
		{
			stick1Position = stick1Position + 1;
			
		}
		break;
	case GLUT_KEY_DOWN:
		if (stick1Position > -31)
		{
			stick1Position = stick1Position - 1;
		
		}
		break;
	case GLUT_KEY_F1:
		if (stick2Position + stickSize < 31)
		{
			stick2Position = stick2Position + 1;
			
		}
		break;
		
	case GLUT_KEY_F2:
		if (stick2Position > -31)
		{
			stick2Position = stick2Position - 1;
			
		}
		break;
	}
}




int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(260, 140);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutCreateWindow("The pong game");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);

	glutSpecialFunc(kbSpecial);
	
	glutMainLoop();
	return 0;
}