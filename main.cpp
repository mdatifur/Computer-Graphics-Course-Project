#include <windows.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <cmath>
#include <string>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")
#define PI 3.1416


int currentScreen = 0;


float cloudMove  = 0.0f;
float trainX     = -600.0f;
float wheelAngle = 0.0f;
float speed      = 1.5f;
int   paused     = 0;
bool  greenSignal = true;

float policeCarX   = -120.0f;
bool  policeCarStop = false;


GLfloat f_cloudOffset     = 0.0f;
GLfloat f_train1Offset    = -50.0f;
GLfloat f_trainSpeed      = 0.08f;
GLfloat f_trainWheelAngle = 0.0f;
GLfloat f_car1X           = -20.0f;
GLfloat f_wheelAngle      = 0.0f;
int     f_trafficLightState   = -1;
GLfloat f_trafficLightTimer   = 0.0f;
bool    f_manualLightOverride = true;
int     f_speedLevel          = 1;

float f_getSpeedMultiplier()
{
    return (float)f_speedLevel;
}


float planeX = -400.0f;
float planeY = -40.0f;
float planeAngle = 0.0f;
float speedX = 2.5f;
float speedY = 0.0f;
int flightPhase = 0;

bool ap_paused = false;
bool takeoffEnabled = false;

float cloudAP_X[3] = { -300.0f, 0.0f, 250.0f };
float cloudAP_Y[3] = { 220.0f,  240.0f, 205.0f };
float cloudAP_Speed[3] = { 0.4f, 0.25f, 0.35f };


GLfloat t_cloudPosition = 0.0f;
GLfloat t_cloudSpeed    = 0.05f;
float   t_carOffset     = 0.0f;
int     t_lightState    = 0;
float   t_carSpeed      = 0.2f;


void drawText(float x, float y, void *font, const char* text)
{
    glRasterPos2f(x, y);
    for(int i = 0; i < (int)strlen(text); i++)
        glutBitmapCharacter(font, text[i]);
}

void circle(float cx, float cy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i = 0; i <= 50; i++)
        glVertex2f(cx + r*cos(2*PI*i/50), cy + r*sin(2*PI*i/50));
    glEnd();
}


void f_circle(float x, float y, float radius)
{
    int i;
    int triangleAmount = 360;
    GLfloat twicePi = 2.0f * 3.14159265358979323846f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for(i = 0; i <= triangleAmount; i++)
        glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
                   y + (radius * sin(i * twicePi / triangleAmount)));
    glEnd();
}

void f_drawWheel(float x, float y, float radius, float angle)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    f_circle(0, 0, radius);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(radius, 0);
    glEnd();
    glPopMatrix();
}

void f_drawCar(float r, float g, float b, float currentWheelAngle)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(0.0, 0.0);
    glVertex2f(10.0, 0.0);
    glVertex2f(10.0, 2.0);
    glVertex2f(0.0, 2.0);
    glEnd();
    glColor3f(r + 0.1f, g + 0.1f, b + 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(1.5, 2.0);
    glVertex2f(8.5, 2.0);
    glVertex2f(8.5, 3.5);
    glVertex2f(1.5, 3.5);
    glEnd();
    glColor3f(0.6f, 0.88f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(2.0, 2.2);
    glVertex2f(3.5, 2.2);
    glVertex2f(3.5, 3.2);
    glVertex2f(2.0, 3.2);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(4.25, 2.2);
    glVertex2f(5.75, 2.2);
    glVertex2f(5.75, 3.2);
    glVertex2f(4.25, 3.2);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(6.5, 2.2);
    glVertex2f(8.0, 2.2);
    glVertex2f(8.0, 3.2);
    glVertex2f(6.5, 3.2);
    glEnd();
    f_drawWheel(2.0f, 0.0f, 0.8f, currentWheelAngle);
    f_drawWheel(8.0f, 0.0f, 0.8f, currentWheelAngle);
}

void f_drawCustomTree(float x, float y, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);
    glColor3f(0.54f, 0.27f, 0.07f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f( 1.0f, 0.0f);
    glVertex2f( 1.0f, 5.0f);
    glVertex2f(-1.0f, 5.0f);
    glEnd();
    glColor3f(0.0f, 0.5f, 0.0f);
    f_circle(0.0f, 7.5f, 3.5f);
    glPopMatrix();
}


void ap_drawCircle(float x, float y, float radius)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        float dx = radius * cos(angle);
        float dy = radius * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void ap_drawSky()
{
    glColor3f(0.55f, 0.65f, 0.75f);
    glBegin(GL_POLYGON);
    glVertex2f(-400, 300);
    glVertex2f(400, 300);
    glVertex2f(400, 120);
    glVertex2f(-400, 120);
    glEnd();
}

void ap_drawSun()
{
    float sx = -310.0f;
    float sy = 265.0f;
    glColor3f(1.0f, 0.98f, 0.72f);
    ap_drawCircle(sx, sy, 28.0f);
    glColor3f(1.0f, 0.95f, 0.50f);
    ap_drawCircle(sx, sy, 22.0f);
    glColor3f(1.0f, 1.0f, 0.75f);
    ap_drawCircle(sx, sy, 16.0f);
    glColor3f(1.0f, 0.95f, 0.55f);
    glLineWidth(2.0f);
    int numRays = 12;
    glBegin(GL_LINES);
    for (int i = 0; i < numRays; i++)
    {
        float ang = 2.0f * 3.1416f * i / numRays;
        float x1 = sx + cos(ang) * 20.0f;
        float y1 = sy + sin(ang) * 20.0f;
        float x2 = sx + cos(ang) * 32.0f;
        float y2 = sy + sin(ang) * 32.0f;
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }
    glEnd();
    glLineWidth(1.0f);
}

void ap_drawCloud(float cx, float cy)
{
    glColor3f(0.97f, 0.97f, 0.97f);
    ap_drawCircle(cx - 38.0f, cy,        14.0f);
    ap_drawCircle(cx - 18.0f, cy,        16.0f);
    ap_drawCircle(cx +  4.0f, cy,        16.0f);
    ap_drawCircle(cx + 24.0f, cy,        15.0f);
    ap_drawCircle(cx + 42.0f, cy,        13.0f);
    ap_drawCircle(cx - 28.0f, cy + 14.0f, 17.0f);
    ap_drawCircle(cx -  4.0f, cy + 18.0f, 19.0f);
    ap_drawCircle(cx + 20.0f, cy + 15.0f, 17.0f);
    ap_drawCircle(cx +  2.0f, cy + 30.0f, 14.0f);
}

void ap_drawClouds()
{
    for (int i = 0; i < 3; i++)
        ap_drawCloud(cloudAP_X[i], cloudAP_Y[i]);
}

void ap_drawAirportName()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-195.0f, 140.0f);
    const char* name = "HAZRAT SHAHJALAL INTERNATIONAL AIRPORT";
    for (int i = 0; name[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
}

void ap_drawTerminal()
{
    glColor3f(0.68f, 0.66f, 0.58f);
    glBegin(GL_POLYGON);
    glVertex2f(-400, 150);
    glVertex2f(400, 150);
    glVertex2f(400, 65);
    glVertex2f(-400, 65);
    glEnd();
    glColor3f(0.50f, 0.50f, 0.47f);
    glBegin(GL_POLYGON);
    glVertex2f(-400, 170);
    glVertex2f(400, 170);
    glVertex2f(400, 150);
    glVertex2f(-400, 150);
    glEnd();
    glColor3f(0.10f, 0.16f, 0.19f);
    for (int x = -370; x <= 350; x += 60)
    {
        glBegin(GL_POLYGON);
        glVertex2f(x, 135);
        glVertex2f(x + 40, 135);
        glVertex2f(x + 40, 95);
        glVertex2f(x, 95);
        glEnd();
    }
    glColor3f(0.45f, 0.43f, 0.38f);
    glBegin(GL_POLYGON);
    glVertex2f(-400, 85);
    glVertex2f(400, 85);
    glVertex2f(400, 65);
    glVertex2f(-400, 65);
    glEnd();
    glColor3f(0.55f, 0.55f, 0.52f);
    glBegin(GL_POLYGON);
    glVertex2f(260, 170);
    glVertex2f(310, 170);
    glVertex2f(300, 235);
    glVertex2f(270, 235);
    glEnd();
    glColor3f(0.08f, 0.13f, 0.16f);
    glBegin(GL_POLYGON);
    glVertex2f(255, 235);
    glVertex2f(315, 235);
    glVertex2f(305, 260);
    glVertex2f(265, 260);
    glEnd();
    glColor3f(0.25f, 0.25f, 0.25f);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-400, 150);
    glVertex2f(400, 150);
    glVertex2f(-400, 65);
    glVertex2f(400, 65);
    glEnd();
    glLineWidth(1);
    ap_drawAirportName();
}

void ap_drawRunway()
{
    glColor3f(0.30f, 0.30f, 0.30f);
    glBegin(GL_POLYGON);
    glVertex2f(-400, 50);
    glVertex2f(400, 50);
    glVertex2f(400, -300);
    glVertex2f(-400, -300);
    glEnd();
    glColor3f(0.24f, 0.24f, 0.24f);
    glBegin(GL_POLYGON);
    glVertex2f(-400, -80);
    glVertex2f(400, -80);
    glVertex2f(400, -160);
    glVertex2f(-400, -160);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int x = -350; x <= 350; x += 100)
    {
        glBegin(GL_POLYGON);
        glVertex2f(x, -115);
        glVertex2f(x + 50, -115);
        glVertex2f(x + 50, -125);
        glVertex2f(x, -125);
        glEnd();
    }
}

void ap_drawBarrier()
{
    int x = -400;
    for (int i = 0; i < 40; i++)
    {
        if (i % 2 == 0)
            glColor3f(0.80f, 0.00f, 0.00f);
        else
            glColor3f(1.00f, 1.00f, 1.00f);
        glBegin(GL_POLYGON);
        glVertex2f(x, 65);
        glVertex2f(x + 20, 65);
        glVertex2f(x + 20, 45);
        glVertex2f(x, 45);
        glEnd();
        x += 20;
    }
}

void ap_drawSmallPlane(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.88f, 0.84f, 0.76f);
    glBegin(GL_POLYGON);
    glVertex2f(-65, 0);
    glVertex2f(55, 0);
    glVertex2f(80, 13);
    glVertex2f(55, 26);
    glVertex2f(-65, 26);
    glVertex2f(-85, 13);
    glEnd();
    glColor3f(0.08f, 0.12f, 0.15f);
    glBegin(GL_POLYGON);
    glVertex2f(45, 20);
    glVertex2f(65, 18);
    glVertex2f(72, 13);
    glVertex2f(48, 13);
    glEnd();
    glColor3f(0.45f, 0.45f, 0.50f);
    glBegin(GL_POLYGON);
    glVertex2f(-65, 26);
    glVertex2f(-38, 26);
    glVertex2f(-25, 62);
    glVertex2f(-68, 62);
    glEnd();
    glColor3f(0.60f, 0.60f, 0.56f);
    glBegin(GL_POLYGON);
    glVertex2f(-15, 0);
    glVertex2f(40, 0);
    glVertex2f(10, -30);
    glVertex2f(-45, -30);
    glEnd();
    glColor3f(0.03f, 0.03f, 0.03f);
    ap_drawCircle(-30, -8, 6);
    ap_drawCircle(30, -8, 6);
    glPopMatrix();
}

void ap_drawMainPlane()
{
    glPushMatrix();
    glTranslatef(planeX, planeY, 0);
    glPushMatrix();
    glTranslatef(0, -planeY - 40.0f, 0);
    glColor3f(0.18f, 0.18f, 0.18f);
    glBegin(GL_POLYGON);
    glVertex2f(-150, -55);
    glVertex2f(130, -55);
    glVertex2f(110, -62);
    glVertex2f(-130, -62);
    glEnd();
    glPopMatrix();
    glRotatef(planeAngle, 0, 0, 1);
    glColor3f(0.95f, 0.95f, 0.90f);
    glBegin(GL_POLYGON);
    glVertex2f(-170, 8);
    glVertex2f(-150, 2);
    glVertex2f(115, 2);
    glVertex2f(160, 10);
    glVertex2f(175, 20);
    glVertex2f(160, 30);
    glVertex2f(115, 38);
    glVertex2f(-150, 38);
    glVertex2f(-175, 30);
    glVertex2f(-185, 20);
    glEnd();
    glColor3f(0.04f, 0.07f, 0.10f);
    glBegin(GL_POLYGON);
    glVertex2f(135, 30);
    glVertex2f(155, 25);
    glVertex2f(165, 18);
    glVertex2f(145, 18);
    glEnd();
    glColor3f(0.05f, 0.10f, 0.15f);
    for (int x = -135; x <= 95; x += 22)
    {
        ap_drawCircle((float)x, 24, 4);
    }
    glColor3f(0.80f, 0.00f, 0.00f);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-150, 14);
    glVertex2f(135, 14);
    glEnd();
    glColor3f(0.00f, 0.42f, 0.10f);
    glBegin(GL_LINES);
    glVertex2f(-150, 8);
    glVertex2f(125, 8);
    glEnd();
    glLineWidth(1);
    glColor3f(0.62f, 0.62f, 0.58f);
    glBegin(GL_POLYGON);
    glVertex2f(-25, 38);
    glVertex2f(55, 38);
    glVertex2f(20, 65);
    glVertex2f(-70, 65);
    glEnd();
    glColor3f(0.72f, 0.72f, 0.67f);
    glBegin(GL_POLYGON);
    glVertex2f(-35, 4);
    glVertex2f(75, 4);
    glVertex2f(30, -55);
    glVertex2f(-70, -55);
    glEnd();
    glColor3f(0.72f, 0.72f, 0.68f);
    glBegin(GL_POLYGON);
    glVertex2f(-155, 38);
    glVertex2f(-115, 38);
    glVertex2f(-95, 95);
    glVertex2f(-165, 95);
    glEnd();
    glColor3f(0.70f, 0.70f, 0.65f);
    glBegin(GL_POLYGON);
    glVertex2f(-165, 35);
    glVertex2f(-115, 35);
    glVertex2f(-140, 58);
    glVertex2f(-185, 58);
    glEnd();
    glColor3f(0.82f, 0.82f, 0.78f);
    ap_drawCircle(-15, -32, 16);
    ap_drawCircle(45, -32, 16);
    glColor3f(0.05f, 0.05f, 0.05f);
    ap_drawCircle(-15, -32, 8);
    ap_drawCircle(45, -32, 8);
    if (flightPhase == 0)
    {
        glColor3f(0.10f, 0.10f, 0.10f);
        glLineWidth(2.5f);
        glBegin(GL_LINES);
        glVertex2f(-68, 2);
        glVertex2f(-68, -28);
        glVertex2f(-48, 2);
        glVertex2f(-48, -28);
        glVertex2f(-75, -28);
        glVertex2f(-40, -28);
        glVertex2f(122, 4);
        glVertex2f(122, -26);
        glEnd();
        glLineWidth(1.0f);
        glColor3f(0.05f, 0.05f, 0.05f);
        ap_drawCircle(-68, -34, 7);
        ap_drawCircle(-48, -34, 7);
        ap_drawCircle(122, -32, 5);
        glColor3f(0.75f, 0.75f, 0.72f);
        ap_drawCircle(-68, -34, 2.5f);
        ap_drawCircle(-48, -34, 2.5f);
        ap_drawCircle(122, -32, 1.8f);
        glColor3f(0.25f, 0.25f, 0.25f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        glVertex2f(-75, -34);
        glVertex2f(-61, -34);
        glVertex2f(-55, -34);
        glVertex2f(-41, -34);
        glVertex2f(117, -32);
        glVertex2f(127, -32);
        glEnd();
        glLineWidth(1.0f);
    }
    glPopMatrix();
}

void ap_drawHUD()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-395.0f, -280.0f);
    const char* hint1 = "SPACE: Take Off   P: Pause/Resume   I: Back to Instruction";
    for (int i = 0; hint1[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, hint1[i]);
    if (ap_paused)
    {
        glColor3f(1.0f, 0.85f, 0.20f);
        glRasterPos2f(-30.0f, 0.0f);
        const char* pauseMsg = "PAUSED";
        for (int i = 0; pauseMsg[i] != '\0'; i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, pauseMsg[i]);
    }
}

void airportScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -300, 300);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.60f, 0.60f, 0.60f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ap_drawSky();
    ap_drawSun();
    ap_drawClouds();
    ap_drawTerminal();
    ap_drawSmallPlane(-260, 95);
    ap_drawSmallPlane(120, 95);
    ap_drawRunway();
    ap_drawBarrier();
    ap_drawMainPlane();
    ap_drawHUD();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutSwapBuffers();
}


void metroRailScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 80.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.45f, 0.75f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.8f, 0.0f);
    f_circle(92.0, 69.5, 3.0);

    glPushMatrix();
    glTranslatef(f_cloudOffset, 0.0f, 0.0f);
    glColor3f(0.95f, 0.95f, 0.98f);
    f_circle(44.0, 74.0, 2.0);
    f_circle(46.5, 75.5, 2.5);
    f_circle(49.5, 74.0, 2.0);
    f_circle(46.5, 72.5, 2.0);
    f_circle(60.0, 73.5, 2.0);
    f_circle(62.5, 74.5, 2.5);
    f_circle(65.5, 73.5, 2.0);
    f_circle(62.5, 72.0, 2.0);
    f_circle(75.0, 70.5, 2.0);
    f_circle(77.5, 72.0, 2.5);
    f_circle(80.5, 70.5, 2.0);
    f_circle(77.5, 69.5, 2.0);
    glPopMatrix();

    glColor3f(0.95f, 0.90f, 0.80f);
    glBegin(GL_POLYGON);
    glVertex2f(2.0f,33.5f);
    glVertex2f(22.0f,33.5f);
    glVertex2f(22.0f,65.0f);
    glVertex2f(2.0f,65.0f);
    glEnd();
    glColor3f(0.6f,0.3f,0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(1.0f,65.0f);
    glVertex2f(23.0f,65.0f);
    glVertex2f(23.0f,67.0f);
    glVertex2f(1.0f,67.0f);
    glEnd();
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(11.0f,57.0f);
    glVertex2f(13.0f,57.0f);
    glVertex2f(13.0f,63.0f);
    glVertex2f(11.0f,63.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(8.0f,59.0f);
    glVertex2f(16.0f,59.0f);
    glVertex2f(16.0f,61.0f);
    glVertex2f(8.0f,61.0f);
    glEnd();
    glColor3f(0.38f,0.25f,0.15f);
    glBegin(GL_POLYGON);
    glVertex2f(9.0f,33.5f);
    glVertex2f(15.0f,33.5f);
    glVertex2f(15.0f,38.0f);
    glVertex2f(9.0f,38.0f);
    glEnd();
    glColor3f(0.1f,0.1f,0.1f);
    glBegin(GL_LINES);
    glVertex2f(12.0f,33.5f);
    glVertex2f(12.0f,38.0f);
    glEnd();
    glColor3f(0.82f,0.90f,0.98f);
    for(int j=0; j<4; j++)
    {
        float y=40.0f+j*4.5f;
        glBegin(GL_POLYGON);
        glVertex2f(3.5f,y);
        glVertex2f(5.7f,y);
        glVertex2f(5.7f,y+3.0f);
        glVertex2f(3.5f,y+3.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(6.2f,y);
        glVertex2f(8.4f,y);
        glVertex2f(8.4f,y+3.0f);
        glVertex2f(6.2f,y+3.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(15.6f,y);
        glVertex2f(17.8f,y);
        glVertex2f(17.8f,y+3.0f);
        glVertex2f(15.6f,y+3.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(18.3f,y);
        glVertex2f(20.5f,y);
        glVertex2f(20.5f,y+3.0f);
        glVertex2f(18.3f,y+3.0f);
        glEnd();
    }

    glColor3f(0.45,0.45,0.45);
    glBegin(GL_POLYGON);
    glVertex2f(11.75,67.0);
    glVertex2f(12.25,67.0);
    glVertex2f(12.25,74.0);
    glVertex2f(11.75,74.0);
    glEnd();
    glColor3f(0.0,0.40,0.22);
    glBegin(GL_POLYGON);
    glVertex2f(12.25,70.5);
    glVertex2f(17.25,70.5);
    glVertex2f(17.25,73.5);
    glVertex2f(12.25,73.5);
    glEnd();
    glColor3f(1.0,0.0,0.0);
    f_circle(14.5,72.0,1.0);

    glColor3f(0.2f,0.5f,0.45f);
    glBegin(GL_POLYGON);
    glVertex2f(24.0,33.5);
    glVertex2f(32.0,33.5);
    glVertex2f(32.0,72.0);
    glVertex2f(24.0,72.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(33.0,33.5);
    glVertex2f(41.0,33.5);
    glVertex2f(41.0,58.0);
    glVertex2f(33.0,58.0);
    glEnd();
    glColor3f(0.82f,0.90f,0.98f);
    for(int j=0; j<7; j++)
    {
        float y=38.0f+j*4.5f;
        glBegin(GL_POLYGON);
        glVertex2f(25.3f,y);
        glVertex2f(27.5f,y);
        glVertex2f(27.5f,y+3.0f);
        glVertex2f(25.3f,y+3.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(28.5f,y);
        glVertex2f(30.7f,y);
        glVertex2f(30.7f,y+3.0f);
        glVertex2f(28.5f,y+3.0f);
        glEnd();
    }
    for(int j=0; j<4; j++)
    {
        float y=38.0f+j*4.5f;
        glBegin(GL_POLYGON);
        glVertex2f(34.3f,y);
        glVertex2f(36.5f,y);
        glVertex2f(36.5f,y+3.0f);
        glVertex2f(34.3f,y+3.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(37.5f,y);
        glVertex2f(39.7f,y);
        glVertex2f(39.7f,y+3.0f);
        glVertex2f(37.5f,y+3.0f);
        glEnd();
    }

    glColor3f(0.55,0.55,0.55);
    glBegin(GL_POLYGON);
    glVertex2f(0.0,43.5);
    glVertex2f(100.0,43.5);
    glVertex2f(100.0,45.5);
    glVertex2f(0.0,45.5);
    glEnd();
    glColor3f(0.70,0.70,0.68);
    glBegin(GL_POLYGON);
    glVertex2f(0.0,44.5);
    glVertex2f(100.0,44.5);
    glVertex2f(100.0,45.5);
    glVertex2f(0.0,45.5);
    glEnd();
    glColor3f(0.65,0.65,0.62);
    glBegin(GL_POLYGON);
    glVertex2f(7.5,33.5);
    glVertex2f(9.5,33.5);
    glVertex2f(9.5,43.5);
    glVertex2f(7.5,43.5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(25.5,33.5);
    glVertex2f(27.5,33.5);
    glVertex2f(27.5,43.5);
    glVertex2f(25.5,43.5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(55.5,33.5);
    glVertex2f(57.5,33.5);
    glVertex2f(57.5,43.5);
    glVertex2f(55.5,43.5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(87.5,33.5);
    glVertex2f(89.5,33.5);
    glVertex2f(89.5,43.5);
    glVertex2f(87.5,43.5);
    glEnd();

    glPushMatrix();
    glTranslatef(f_train1Offset, 0.0f, 0.0f);
    glColor3f(0.90,0.98,1.0);
    glBegin(GL_POLYGON);
    glVertex2f(25.0,44.0);
    glVertex2f(43.0,44.0);
    glVertex2f(43.0,48.0);
    glVertex2f(25.0,48.0);
    glEnd();
    glColor3f(0.1,0.65,0.60);
    glBegin(GL_POLYGON);
    glVertex2f(25.0,47.5);
    glVertex2f(43.0,47.5);
    glVertex2f(43.0,48.0);
    glVertex2f(25.0,48.0);
    glEnd();
    glColor3f(0.55f,0.82f,1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(27.5f,45.5f);
    glVertex2f(30.0f,45.5f);
    glVertex2f(30.0f,46.5f);
    glVertex2f(27.5f,46.5f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(31.0f,45.5f);
    glVertex2f(33.5f,45.5f);
    glVertex2f(33.5f,46.5f);
    glVertex2f(31.0f,46.5f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(34.5f,45.5f);
    glVertex2f(37.0f,45.5f);
    glVertex2f(37.0f,46.5f);
    glVertex2f(34.5f,46.5f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(38.0f,45.5f);
    glVertex2f(40.5f,45.5f);
    glVertex2f(40.5f,46.5f);
    glVertex2f(38.0f,46.5f);
    glEnd();
    f_drawWheel(27.5f,43.5f,0.5f,f_trainWheelAngle);
    f_drawWheel(32.0f,43.5f,0.5f,f_trainWheelAngle);
    f_drawWheel(36.5f,43.5f,0.5f,f_trainWheelAngle);
    f_drawWheel(41.0f,43.5f,0.5f,f_trainWheelAngle);
    glPopMatrix();

    glColor3f(0.25,0.45,0.18);
    glBegin(GL_POLYGON);
    glVertex2f(0.0,28.5);
    glVertex2f(100.0,28.5);
    glVertex2f(100.0,33.5);
    glVertex2f(0.0,33.5);
    glEnd();

    f_drawCustomTree(10.0f,29.0f,1.0f);
    f_drawCustomTree(65.0f,29.0f,1.0f);
    f_drawCustomTree(85.0f,29.0f,1.0f);

    glColor3f(0.15,0.15,0.15);
    glBegin(GL_POLYGON);
    glVertex2f(49.75,30.5);
    glVertex2f(50.25,30.5);
    glVertex2f(50.25,35.0);
    glVertex2f(49.75,35.0);
    glEnd();
    glColor3f(0.10,0.10,0.10);
    glBegin(GL_POLYGON);
    glVertex2f(48.75,35.0);
    glVertex2f(51.25,35.0);
    glVertex2f(51.25,41.0);
    glVertex2f(48.75,41.0);
    glEnd();
    if(f_trafficLightState==2)
        glColor3f(1.0f,0.0f,0.0f);
    else
        glColor3f(0.15f,0.15f,0.15f);
    f_circle(50.0,40.0,0.4);
    if(f_trafficLightState==1)
        glColor3f(1.0f,1.0f,0.0f);
    else
        glColor3f(0.15f,0.15f,0.15f);
    f_circle(50.0,38.0,0.4);
    if(f_trafficLightState==0)
        glColor3f(0.0f,1.0f,0.0f);
    else
        glColor3f(0.15f,0.15f,0.15f);
    f_circle(50.0,36.0,0.4);

    glColor3f(0.18,0.18,0.23);
    glBegin(GL_POLYGON);
    glVertex2f(0.0,19.5);
    glVertex2f(100.0,19.5);
    glVertex2f(100.0,28.5);
    glVertex2f(0.0,28.5);
    glEnd();

    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(46.5,20.0);
    glVertex2f(47.5,20.0);
    glVertex2f(47.5,28.0);
    glVertex2f(46.5,28.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(48.5,20.0);
    glVertex2f(49.5,20.0);
    glVertex2f(49.5,28.0);
    glVertex2f(48.5,28.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(50.5,20.0);
    glVertex2f(51.5,20.0);
    glVertex2f(51.5,28.0);
    glVertex2f(50.5,28.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(52.5,20.0);
    glVertex2f(53.5,20.0);
    glVertex2f(53.5,28.0);
    glVertex2f(52.5,28.0);
    glEnd();

    glColor3f(1.0f,1.0f,1.0f);
    float dashY1=23.8f, dashY2=24.2f;
    for(float dx=0.0f; dx<100.0f; dx+=8.0f)
    {
        if(dx>44.0f&&dx<54.0f)
            continue;
        glBegin(GL_POLYGON);
        glVertex2f(dx,dashY1);
        glVertex2f(dx+4.0f,dashY1);
        glVertex2f(dx+4.0f,dashY2);
        glVertex2f(dx,dashY2);
        glEnd();
    }

    glPushMatrix();
    glTranslatef(f_car1X,21.0f,0.0f);
    f_drawCar(0.2f,0.45f,0.9f,f_wheelAngle);
    glPopMatrix();

    glColor3f(0.25f,0.40f,0.18f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0,10.0);
    glVertex2f(100.0,10.0);
    glVertex2f(100.0,19.5);
    glVertex2f(0.0,19.5);
    glEnd();
    glColor3f(0.62f,0.60f,0.55f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0,0.0);
    glVertex2f(100.0,0.0);
    glVertex2f(100.0,10.0);
    glVertex2f(0.0,10.0);
    glEnd();
    glColor3f(0.52f,0.52f,0.48f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0,2.0);
    glVertex2f(100.0,2.0);
    glVertex2f(100.0,2.5);
    glVertex2f(0.0,2.5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0.0,4.5);
    glVertex2f(100.0,4.5);
    glVertex2f(100.0,5.5);
    glVertex2f(0.0,5.5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0.0,7.5);
    glVertex2f(100.0,7.5);
    glVertex2f(100.0,8.0);
    glVertex2f(0.0,8.0);
    glEnd();
    for(float vx=10.0f; vx<100.0f; vx+=10.0f)
    {
        glBegin(GL_POLYGON);
        glVertex2f(vx,0.0f);
        glVertex2f(vx+0.2f,0.0f);
        glVertex2f(vx+0.2f,10.0f);
        glVertex2f(vx,10.0f);
        glEnd();
    }
    f_drawCustomTree(20.0f,12.0f,1.0f);
    f_drawCustomTree(75.0f,12.0f,1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,800,0,600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutSwapBuffers();
}

float t_mapX(float x)
{
    return (x + 1) * 50;
}
float t_mapY(float y)
{
    return (y + 1) * 40;
}

void t_drawPineTree(float x, float y)
{
    float X = t_mapX(x);
    float Y = t_mapY(y);
    glBegin(GL_QUADS);
    glColor3f(0.42f, 0.26f, 0.15f);
    glVertex2f(X - 0.01f * 50, Y);
    glVertex2f(X + 0.01f * 50, Y);
    glVertex2f(X + 0.01f * 50, Y + 0.08f * 40);
    glVertex2f(X - 0.01f * 50, Y + 0.08f * 40);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.08f, 0.30f, 0.12f);
    glVertex2f(X - 0.07f * 50, Y + 0.08f * 40);
    glVertex2f(X + 0.07f * 50, Y + 0.08f * 40);
    glVertex2f(X,              Y + 0.24f * 40);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.12f, 0.45f, 0.18f);
    glVertex2f(X - 0.05f * 50, Y + 0.18f * 40);
    glVertex2f(X + 0.05f * 50, Y + 0.18f * 40);
    glVertex2f(X,              Y + 0.33f * 40);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.20f, 0.65f, 0.25f);
    glVertex2f(X - 0.04f * 50, Y + 0.27f * 40);
    glVertex2f(X + 0.04f * 50, Y + 0.27f * 40);
    glVertex2f(X,              Y + 0.37f * 40);
    glEnd();
}

void t_drawCrosswalkStripe(float x1, float x2, float y)
{
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(x1, y);
    glVertex2f(x2, y);
    glVertex2f(x2, y - 0.8f);
    glVertex2f(x1, y - 0.8f);
    glEnd();
}

void t_drawCrosswalk(float x1, float x2, float startY)
{
    for (int i = 0; i < 10; i++)
    {
        float y = startY - (i * 1.6f);
        t_drawCrosswalkStripe(x1, x2, y);
    }
}

void t_drawCircle(float cx, float cy, float r, float R, float G, float B)
{
    int i;
    int triangleAmount = 100;
    GLfloat twicePi = 2.0f * 3.14159265358979323846f;
    float CX = (cx + 1) * 50;
    float CY = (cy + 1) * 40;
    float radius = r * 60;
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(CX, CY);
    for(i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            CX + radius * cos(i * twicePi / triangleAmount),
            CY + radius * sin(i * twicePi / triangleAmount)
        );
    }
    glEnd();
}

void tanhaCityScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 80.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int i;
    GLfloat twicePi = 2.0f * 3.14159265358979323846f;
    int triangleAmount = 100;

    // sky gradient top
    glBegin(GL_QUADS);
    glColor3f(0.447f, 0.792f, 0.933f);
    glVertex2f(0.0f, 80.0f);
    glVertex2f(100.0f, 80.0f);
    glColor3f(0.804f, 0.910f, 0.890f);
    glVertex2f(100.0f, 64.0f);
    glVertex2f(0.0f, 64.0f);
    glEnd();

    // sky gradient bottom
    glBegin(GL_QUADS);
    glColor3f(0.804f, 0.910f, 0.890f);
    glVertex2f(0.0f, 64.0f);
    glVertex2f(100.0f, 64.0f);
    glColor3f(0.894f, 0.945f, 0.875f);
    glVertex2f(100.0f, 36.4f);
    glVertex2f(0.0f, 36.4f);
    glEnd();

    // clouds
    glPushMatrix();
    glTranslatef(t_cloudPosition, 0.0f, 0.0f);
    float cx, cy, cRadius;

    glColor3f(0.92f, 0.96f, 0.98f);
    cx = 20.0f;
    cy = 74.0f;
    cRadius = 3.2f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();
    cx = 25.0f;
    cy = 75.0f;
    cRadius = 4.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();
    cx = 30.0f;
    cy = 73.6f;
    cRadius = 3.2f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();

    cx = 60.0f;
    cy = 75.2f;
    cRadius = 3.6f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();
    cx = 65.0f;
    cy = 76.0f;
    cRadius = 4.4f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();
    cx = 70.0f;
    cy = 74.8f;
    cRadius = 3.6f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();

    cx = 42.0f;
    cy = 70.5f;
    cRadius = 2.2f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();
    cx = 45.0f;
    cy = 72.0f;
    cRadius = 2.8f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();
    cx = 48.5f;
    cy = 70.8f;
    cRadius = 2.3f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(cx+cRadius*cos(i*twicePi/triangleAmount),cy+cRadius*sin(i*twicePi/triangleAmount));
    glEnd();
    glPopMatrix();

    // ground
    glBegin(GL_QUADS);
    glColor3f(0.365f, 0.592f, 0.290f);
    glVertex2f(0.0f, 36.0f);
    glVertex2f(100.0f, 36.0f);
    glVertex2f(100.0f, 30.0f);
    glVertex2f(0.0f, 30.0f);
    glEnd();

    // sidewalk base
    glBegin(GL_QUADS);
    glColor3f(0.792f, 0.761f, 0.843f);
    glVertex2f(0.0f, 30.0f);
    glVertex2f(100.0f, 30.0f);
    glVertex2f(100.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glEnd();

    // road
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(0.0f, 26.0f);
    glVertex2f(100.0f, 26.0f);
    glVertex2f(100.0f, 8.8f);
    glVertex2f(0.0f, 8.8f);
    glEnd();

    glLineWidth(6.0);
    glBegin(GL_LINES);
    glColor3f(0.537f, 0.529f, 0.616f);
    glVertex2f(0.0f, 25.6f);
    glVertex2f(100.0f, 25.6f);
    glEnd();

    glLineWidth(4.0);
    glBegin(GL_LINES);
    glColor3f(0.314f, 0.502f, 0.298f);
    glVertex2f(100.0f, 36.0f);
    glVertex2f(0.0f, 36.0f);
    glEnd();

    // road dashes
    glColor3f(1.0f, 1.0f, 1.0f);
    float y1 = 18.0f, y2 = 17.2f;
    glBegin(GL_QUADS);
    glVertex2f(2.5f,  y1);
    glVertex2f(7.5f,  y1);
    glVertex2f(7.5f,  y2);
    glVertex2f(2.5f,  y2);
    glVertex2f(12.5f, y1);
    glVertex2f(17.5f, y1);
    glVertex2f(17.5f, y2);
    glVertex2f(12.5f, y2);
    glVertex2f(22.5f, y1);
    glVertex2f(27.5f, y1);
    glVertex2f(27.5f, y2);
    glVertex2f(22.5f, y2);
    glVertex2f(32.5f, y1);
    glVertex2f(37.5f, y1);
    glVertex2f(37.5f, y2);
    glVertex2f(32.5f, y2);
    glVertex2f(62.5f, y1);
    glVertex2f(67.5f, y1);
    glVertex2f(67.5f, y2);
    glVertex2f(62.5f, y2);
    glVertex2f(72.5f, y1);
    glVertex2f(77.5f, y1);
    glVertex2f(77.5f, y2);
    glVertex2f(72.5f, y2);
    glVertex2f(82.5f, y1);
    glVertex2f(87.5f, y1);
    glVertex2f(87.5f, y2);
    glVertex2f(82.5f, y2);
    glVertex2f(92.5f, y1);
    glVertex2f(97.5f, y1);
    glVertex2f(97.5f, y2);
    glVertex2f(92.5f, y2);
    glEnd();

    t_drawCrosswalk(47.5f, 55.0f, 24.4f);

    // red car
    glPushMatrix();
    glTranslatef(-t_carOffset, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(0.85f, 0.25f, 0.20f);
    glVertex2f(88.5f, 24.4f);
    glVertex2f(88.5f, 20.4f);
    glVertex2f(72.5f, 20.4f);
    glVertex2f(72.5f, 24.4f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(86.0f, 24.4f);
    glVertex2f(75.0f, 24.4f);
    glVertex2f(78.0f, 28.0f);
    glVertex2f(83.5f, 28.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.78f, 0.25f, 0.22f);
    glVertex2f(76.25f, 24.4f);
    glVertex2f(83.5f, 24.4f);
    glVertex2f(83.5f, 20.8f);
    glVertex2f(76.25f, 20.8f);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(82.0f, 24.0f);
    glVertex2f(81.0f, 24.0f);
    glVertex2f(78.5f, 24.0f);
    glVertex2f(77.5f, 24.0f);
    glEnd();
    glLineWidth(8.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.75f, 0.2f);
    glVertex2f(72.5f, 23.6f);
    glVertex2f(73.0f, 23.6f);
    glVertex2f(88.0f, 23.6f);
    glVertex2f(88.5f, 23.6f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.55f, 0.75f, 0.92f);
    glVertex2f(83.5f, 27.2f);
    glVertex2f(83.5f, 24.8f);
    glVertex2f(78.0f, 24.8f);
    glVertex2f(78.0f, 27.2f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.55f, 0.75f, 0.92f);
    glVertex2f(83.5f, 24.8f);
    glVertex2f(76.0f, 24.8f);
    glVertex2f(78.0f, 27.2f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.55f, 0.75f, 0.92f);
    glVertex2f(85.0f, 24.8f);
    glVertex2f(83.5f, 24.8f);
    glVertex2f(83.5f, 27.2f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.85f, 0.25f, 0.20f);
    glVertex2f(83.5f, 24.8f);
    glVertex2f(83.5f, 27.2f);
    glVertex2f(80.0f, 20.8f);
    glVertex2f(80.0f, 27.2f);
    glEnd();
    // red car wheels
    float wx = 76.0f, wy = 20.4f, wr = 2.0f;
    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(wx, wy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(wx+wr*cos(i*twicePi/triangleAmount),wy+wr*sin(i*twicePi/triangleAmount));
    glEnd();
    glColor3f(0.75f, 0.75f, 0.75f);
    wr=1.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(wx, wy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(wx+wr*cos(i*twicePi/triangleAmount),wy+wr*sin(i*twicePi/triangleAmount));
    glEnd();
    wx=85.0f;
    wr=2.0f;
    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(wx, wy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(wx+wr*cos(i*twicePi/triangleAmount),wy+wr*sin(i*twicePi/triangleAmount));
    glEnd();
    glColor3f(0.75f, 0.75f, 0.75f);
    wr=1.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(wx, wy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(wx+wr*cos(i*twicePi/triangleAmount),wy+wr*sin(i*twicePi/triangleAmount));
    glEnd();
    glPopMatrix();

    // taxi
    glPushMatrix();
    glTranslatef(t_carOffset, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.84f, 0.0f);
    glVertex2f(11.0f,18.8f);
    glVertex2f(11.0f,14.8f);
    glVertex2f(27.5f,14.8f);
    glVertex2f(27.5f,18.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.84f, 0.0f);
    glVertex2f(13.0f,18.8f);
    glVertex2f(24.0f,18.8f);
    glVertex2f(21.0f,22.8f);
    glVertex2f(15.5f,22.8f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.70f,0.85f,0.95f);
    glVertex2f(15.5f,22.0f);
    glVertex2f(15.5f,19.2f);
    glVertex2f(14.0f,19.2f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.70f,0.85f,0.95f);
    glVertex2f(21.0f,22.0f);
    glVertex2f(23.0f,19.2f);
    glVertex2f(21.0f,19.2f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.70f,0.85f,0.95f);
    glVertex2f(15.5f,22.0f);
    glVertex2f(15.5f,19.2f);
    glVertex2f(21.0f,19.2f);
    glVertex2f(21.0f,22.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(1.0f,0.92f,0.45f);
    glVertex2f(15.5f,18.8f);
    glVertex2f(15.5f,14.8f);
    glVertex2f(23.0f,14.8f);
    glVertex2f(23.0f,18.8f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(1.0f,0.84f,0.0f);
    glVertex2f(15.5f,22.0f);
    glVertex2f(15.5f,19.2f);
    glVertex2f(19.0f,22.0f);
    glVertex2f(19.0f,14.8f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(16.0f,18.4f);
    glVertex2f(16.5f,18.4f);
    glVertex2f(19.0f,18.4f);
    glVertex2f(19.5f,18.4f);
    glVertex2f(11.5f,17.6f);
    glVertex2f(27.0f,17.6f);
    glEnd();
    glLineWidth(4.0);
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(19.0f,22.8f);
    glVertex2f(17.5f,22.8f);
    glEnd();
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(18.25f,22.8f);
    glVertex2f(18.25f,24.8f);
    glEnd();
    glLineWidth(4.0);
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(10.5f,15.6f);
    glVertex2f(12.5f,15.6f);
    glVertex2f(28.0f,15.6f);
    glVertex2f(26.0f,15.6f);
    glEnd();
    glLineWidth(8.0);
    glBegin(GL_LINES);
    glColor3f(0.6745f,0.3059f,0.2588f);
    glVertex2f(11.0f,17.2f);
    glVertex2f(11.5f,17.2f);
    glEnd();
    // taxi wheels
    wx=15.0f;
    wy=14.8f;
    wr=2.0f;
    glColor3f(0.05f,0.05f,0.05f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(wx,wy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(wx+wr*cos(i*twicePi/triangleAmount),wy+wr*sin(i*twicePi/triangleAmount));
    glEnd();
    glColor3f(0.7f,0.7f,0.7f);
    wr=1.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(wx,wy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(wx+wr*cos(i*twicePi/triangleAmount),wy+wr*sin(i*twicePi/triangleAmount));
    glEnd();
    wx=24.5f;
    wr=2.0f;
    glColor3f(0.05f,0.05f,0.05f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(wx,wy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(wx+wr*cos(i*twicePi/triangleAmount),wy+wr*sin(i*twicePi/triangleAmount));
    glEnd();
    glColor3f(0.7f,0.7f,0.7f);
    wr=1.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(wx,wy);
    for(i=0; i<=triangleAmount; i++)
        glVertex2f(wx+wr*cos(i*twicePi/triangleAmount),wy+wr*sin(i*twicePi/triangleAmount));
    glEnd();
    glPopMatrix();

    // pink building
    glBegin(GL_QUADS);
    glColor3f(0.945f,0.706f,0.769f);
    glVertex2f(66.0f,31.2f);
    glVertex2f(54.0f,31.2f);
    glVertex2f(54.0f,60.8f);
    glVertex2f(66.0f,60.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.85f,0.58f,0.65f);
    glVertex2f(70.0f,31.2f);
    glVertex2f(65.5f,31.2f);
    glVertex2f(65.5f,60.8f);
    glVertex2f(70.0f,60.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.78f,0.74f,0.88f);
    glVertex2f(70.0f,31.2f);
    glVertex2f(79.0f,31.2f);
    glVertex2f(79.0f,47.2f);
    glVertex2f(70.0f,47.2f);
    glEnd();
    glColor3f(0.92f,0.92f,0.95f);
    {
        float startX=71.5f, startY=32.8f, w=1.5f, h=2.4f, gapX=2.5f, gapY=3.2f;
        for(int row=0; row<4; row++)
            for(int col=0; col<3; col++)
            {
                float bx=startX+col*gapX, by=startY+row*gapY;
                glBegin(GL_QUADS);
                glVertex2f(bx,by);
                glVertex2f(bx+w,by);
                glVertex2f(bx+w,by+h);
                glVertex2f(bx,by+h);
                glEnd();
            }
    }
    glBegin(GL_QUADS);
    glColor3f(0.65f,0.62f,0.78f);
    glVertex2f(79.0f,31.2f);
    glVertex2f(82.5f,31.2f);
    glVertex2f(82.5f,47.2f);
    glVertex2f(79.0f,47.2f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.95f,0.94f,0.98f);
    glVertex2f(70.0f,47.2f);
    glVertex2f(82.5f,47.2f);
    glVertex2f(82.5f,46.0f);
    glVertex2f(70.0f,46.0f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.7f,0.7f,0.7f);
    glVertex2f(70.0f,46.4f);
    glVertex2f(82.5f,46.4f);
    glEnd();
    // pink columns
    glBegin(GL_QUADS);
    glColor3f(0.788f,0.459f,0.561f);
    glVertex2f(55.0f,31.2f);
    glVertex2f(57.5f,31.2f);
    glVertex2f(57.5f,60.8f);
    glVertex2f(55.0f,60.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(55.5f,32.4f);
    glVertex2f(57.0f,32.4f);
    glVertex2f(57.0f,59.6f);
    glVertex2f(55.5f,59.6f);
    glEnd();
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glColor3f(0.788f,0.459f,0.561f);
    glVertex2f(55.0f,57.2f);
    glVertex2f(57.5f,57.2f);
    glVertex2f(55.0f,54.4f);
    glVertex2f(57.5f,54.4f);
    glVertex2f(55.0f,51.6f);
    glVertex2f(57.5f,51.6f);
    glVertex2f(55.0f,48.8f);
    glVertex2f(57.5f,48.8f);
    glVertex2f(55.0f,46.0f);
    glVertex2f(57.5f,46.0f);
    glVertex2f(55.0f,43.2f);
    glVertex2f(57.5f,43.2f);
    glVertex2f(55.0f,40.4f);
    glVertex2f(57.5f,40.4f);
    glVertex2f(55.0f,37.6f);
    glVertex2f(57.5f,37.6f);
    glVertex2f(55.0f,34.8f);
    glVertex2f(57.5f,34.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.788f,0.459f,0.561f);
    glVertex2f(58.5f,31.2f);
    glVertex2f(61.0f,31.2f);
    glVertex2f(61.0f,60.8f);
    glVertex2f(58.5f,60.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(59.0f,32.4f);
    glVertex2f(60.5f,32.4f);
    glVertex2f(60.5f,59.6f);
    glVertex2f(59.0f,59.6f);
    glEnd();
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glColor3f(0.788f,0.459f,0.561f);
    glVertex2f(58.5f,56.8f);
    glVertex2f(61.0f,56.8f);
    glVertex2f(58.5f,54.0f);
    glVertex2f(61.0f,54.0f);
    glVertex2f(58.5f,51.2f);
    glVertex2f(61.0f,51.2f);
    glVertex2f(58.5f,48.4f);
    glVertex2f(61.0f,48.4f);
    glVertex2f(58.5f,45.6f);
    glVertex2f(61.0f,45.6f);
    glVertex2f(58.5f,42.8f);
    glVertex2f(61.0f,42.8f);
    glVertex2f(58.5f,40.0f);
    glVertex2f(61.0f,40.0f);
    glVertex2f(58.5f,37.2f);
    glVertex2f(61.0f,37.2f);
    glVertex2f(58.5f,34.4f);
    glVertex2f(61.0f,34.4f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.788f,0.459f,0.561f);
    glVertex2f(64.5f,31.2f);
    glVertex2f(62.0f,31.2f);
    glVertex2f(62.0f,60.8f);
    glVertex2f(64.5f,60.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(64.0f,32.4f);
    glVertex2f(62.5f,32.4f);
    glVertex2f(62.5f,59.6f);
    glVertex2f(64.0f,59.6f);
    glEnd();
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glColor3f(0.788f,0.459f,0.561f);
    glVertex2f(64.5f,56.8f);
    glVertex2f(62.0f,56.8f);
    glVertex2f(64.5f,54.0f);
    glVertex2f(62.0f,54.0f);
    glVertex2f(64.5f,51.2f);
    glVertex2f(62.0f,51.2f);
    glVertex2f(64.5f,48.4f);
    glVertex2f(62.0f,48.4f);
    glVertex2f(64.5f,45.6f);
    glVertex2f(62.0f,45.6f);
    glVertex2f(64.5f,42.8f);
    glVertex2f(62.0f,42.8f);
    glVertex2f(64.5f,40.0f);
    glVertex2f(62.0f,40.0f);
    glVertex2f(64.5f,37.2f);
    glVertex2f(62.0f,37.2f);
    glVertex2f(64.5f,34.4f);
    glVertex2f(62.0f,34.4f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.988f,0.910f,0.953f);
    glVertex2f(53.5f,60.8f);
    glVertex2f(65.5f,60.8f);
    glVertex2f(65.5f,62.0f);
    glVertex2f(53.5f,62.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.859f,0.784f,0.859f);
    glVertex2f(70.0f,60.8f);
    glVertex2f(65.5f,60.8f);
    glVertex2f(65.5f,62.0f);
    glVertex2f(70.0f,62.0f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.698f,0.635f,0.675f);
    glVertex2f(53.5f,61.2f);
    glVertex2f(70.0f,61.2f);
    glEnd();

    // dark grey building
    glBegin(GL_QUADS);
    glColor3f(0.510f,0.667f,0.706f);
    glVertex2f(14.0f,32.0f);
    glVertex2f(26.5f,32.0f);
    glVertex2f(26.5f,64.0f);
    glVertex2f(14.0f,64.0f);
    glEnd();
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glColor3f(0.376f,0.510f,0.549f);
    glVertex2f(15.5f,32.0f);
    glVertex2f(15.5f,64.0f);
    glVertex2f(17.0f,32.0f);
    glVertex2f(17.0f,64.0f);
    glVertex2f(18.5f,32.0f);
    glVertex2f(18.5f,64.0f);
    glVertex2f(20.0f,32.0f);
    glVertex2f(20.0f,64.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.86f,0.91f,0.92f);
    glVertex2f(13.0f,62.4f);
    glVertex2f(21.0f,62.4f);
    glVertex2f(21.0f,64.0f);
    glVertex2f(13.0f,64.0f);
    glEnd();

    // light grey building
    glBegin(GL_QUADS);
    glColor3f(0.694f,0.816f,0.894f);
    glVertex2f(21.0f,32.0f);
    glVertex2f(30.0f,32.0f);
    glVertex2f(30.0f,64.0f);
    glVertex2f(21.0f,64.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.369f,0.475f,0.510f);
    glVertex2f(22.0f,32.0f);
    glVertex2f(23.5f,32.0f);
    glVertex2f(23.5f,64.0f);
    glVertex2f(22.0f,64.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.369f,0.475f,0.510f);
    glVertex2f(27.5f,32.0f);
    glVertex2f(29.0f,32.0f);
    glVertex2f(29.0f,64.0f);
    glVertex2f(27.5f,64.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(22.0f,32.8f);
    glVertex2f(23.0f,32.8f);
    glVertex2f(23.0f,61.2f);
    glVertex2f(22.0f,61.2f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(27.5f,32.8f);
    glVertex2f(28.5f,32.8f);
    glVertex2f(28.5f,61.2f);
    glVertex2f(27.5f,61.2f);
    glEnd();
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glColor3f(0.369f,0.475f,0.510f);
    glVertex2f(22.0f,58.4f);
    glVertex2f(23.5f,58.4f);
    glVertex2f(22.0f,55.2f);
    glVertex2f(23.5f,55.2f);
    glVertex2f(22.0f,52.0f);
    glVertex2f(23.5f,52.0f);
    glVertex2f(22.0f,48.8f);
    glVertex2f(23.5f,48.8f);
    glVertex2f(22.0f,45.6f);
    glVertex2f(23.5f,45.6f);
    glVertex2f(22.0f,42.4f);
    glVertex2f(23.5f,42.4f);
    glVertex2f(22.0f,39.2f);
    glVertex2f(23.5f,39.2f);
    glVertex2f(22.0f,36.0f);
    glVertex2f(23.5f,36.0f);
    glVertex2f(22.0f,32.8f);
    glVertex2f(23.5f,32.8f);
    glVertex2f(27.5f,58.4f);
    glVertex2f(28.5f,58.4f);
    glVertex2f(27.5f,55.2f);
    glVertex2f(28.5f,55.2f);
    glVertex2f(27.5f,52.0f);
    glVertex2f(28.5f,52.0f);
    glVertex2f(27.5f,48.8f);
    glVertex2f(28.5f,48.8f);
    glVertex2f(27.5f,45.6f);
    glVertex2f(28.5f,45.6f);
    glVertex2f(27.5f,42.4f);
    glVertex2f(28.5f,42.4f);
    glVertex2f(27.5f,39.2f);
    glVertex2f(28.5f,39.2f);
    glVertex2f(27.5f,36.0f);
    glVertex2f(28.5f,36.0f);
    glVertex2f(27.5f,32.8f);
    glVertex2f(28.5f,32.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.922f,0.969f,0.969f);
    glVertex2f(30.5f,62.4f);
    glVertex2f(21.0f,62.4f);
    glVertex2f(21.0f,64.0f);
    glVertex2f(30.5f,64.0f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.655f,0.725f,0.741f);
    glVertex2f(13.5f,62.8f);
    glVertex2f(30.5f,62.8f);
    glEnd();

    // brown buildings
    glBegin(GL_QUADS);
    glColor3f(0.608f,0.259f,0.196f);
    glVertex2f(28.5f,32.0f);
    glVertex2f(31.0f,32.0f);
    glVertex2f(31.0f,50.0f);
    glVertex2f(28.5f,50.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.780f,0.384f,0.275f);
    glVertex2f(31.0f,32.0f);
    glVertex2f(37.5f,32.0f);
    glVertex2f(37.5f,50.0f);
    glVertex2f(31.0f,50.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(31.5f,33.6f);
    glVertex2f(32.5f,33.6f);
    glVertex2f(32.5f,48.4f);
    glVertex2f(31.5f,48.4f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(33.5f,33.6f);
    glVertex2f(34.5f,33.6f);
    glVertex2f(34.5f,48.4f);
    glVertex2f(33.5f,48.4f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(35.5f,33.6f);
    glVertex2f(36.5f,33.6f);
    glVertex2f(36.5f,48.4f);
    glVertex2f(35.5f,48.4f);
    glEnd();

    // yellow buildings
    glBegin(GL_QUADS);
    glColor3f(0.757f,0.475f,0.153f);
    glVertex2f(6.0f,32.0f);
    glVertex2f(11.0f,32.0f);
    glVertex2f(11.0f,52.8f);
    glVertex2f(6.0f,52.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.925f,0.659f,0.333f);
    glVertex2f(11.0f,32.0f);
    glVertex2f(19.0f,32.0f);
    glVertex2f(19.0f,52.8f);
    glVertex2f(11.0f,52.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(12.0f,33.2f);
    glVertex2f(13.0f,33.2f);
    glVertex2f(13.0f,52.5f);
    glVertex2f(12.0f,52.5f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(13.8f,33.2f);
    glVertex2f(14.8f,33.2f);
    glVertex2f(14.8f,52.5f);
    glVertex2f(13.8f,52.5f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(15.6f,33.2f);
    glVertex2f(16.6f,33.2f);
    glVertex2f(16.6f,52.5f);
    glVertex2f(15.6f,52.5f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(17.4f,33.2f);
    glVertex2f(18.4f,33.2f);
    glVertex2f(18.4f,52.5f);
    glVertex2f(17.4f,52.5f);
    glEnd();
    glLineWidth(8.0);
    glBegin(GL_LINES);
    glColor3f(0.925f,0.659f,0.333f);
    glVertex2f(11.0f,52.0f);
    glVertex2f(19.0f,52.0f);
    glVertex2f(11.0f,48.8f);
    glVertex2f(19.0f,48.8f);
    glVertex2f(11.0f,45.6f);
    glVertex2f(19.0f,45.6f);
    glVertex2f(11.0f,42.4f);
    glVertex2f(19.0f,42.4f);
    glVertex2f(11.0f,39.2f);
    glVertex2f(19.0f,39.2f);
    glVertex2f(11.0f,36.0f);
    glVertex2f(19.0f,36.0f);
    glVertex2f(11.0f,33.2f);
    glVertex2f(19.0f,33.2f);
    glEnd();

    // left brown building
    glBegin(GL_QUADS);
    glColor3f(0.518f,0.220f,0.271f);
    glVertex2f(0.0f,31.6f);
    glVertex2f(7.5f,31.6f);
    glVertex2f(7.5f,42.0f);
    glVertex2f(0.0f,42.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(2.0f,33.2f);
    glVertex2f(3.0f,33.2f);
    glVertex2f(3.0f,40.8f);
    glVertex2f(2.0f,40.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(4.0f,33.2f);
    glVertex2f(5.0f,33.2f);
    glVertex2f(5.0f,40.8f);
    glVertex2f(4.0f,40.8f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.914f,0.961f,0.961f);
    glVertex2f(6.0f,33.2f);
    glVertex2f(7.0f,33.2f);
    glVertex2f(7.0f,40.8f);
    glVertex2f(6.0f,40.8f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.518f,0.220f,0.271f);
    glVertex2f(2.0f,40.0f);
    glVertex2f(7.0f,40.0f);
    glVertex2f(2.0f,38.0f);
    glVertex2f(7.0f,38.0f);
    glVertex2f(2.0f,36.0f);
    glVertex2f(7.0f,36.0f);
    glVertex2f(2.0f,34.0f);
    glVertex2f(7.0f,34.0f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.75f,0.45f,0.48f);
    glVertex2f(1.0f,31.6f);
    glVertex2f(1.0f,42.0f);
    glEnd();
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glColor3f(0.906f,0.882f,0.827f);
    glVertex2f(0.0f,42.0f);
    glVertex2f(7.5f,42.0f);
    glEnd();

    // trees
    t_drawPineTree(-0.85f, -0.22f);
    t_drawPineTree(-0.43f, -0.22f);
    t_drawPineTree(-0.20f, -0.22f);
    t_drawPineTree( 0.05f, -0.22f);
    t_drawPineTree( 0.68f, -0.22f);
    t_drawPineTree( 0.82f, -0.22f);

    // traffic light stand
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(49.5f,28.0f);
    glVertex2f(49.5f,44.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(46.0f,38.4f);
    glVertex2f(48.5f,38.4f);
    glVertex2f(48.5f,44.0f);
    glVertex2f(46.0f,44.0f);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(48.5f,42.8f);
    glVertex2f(49.5f,42.8f);
    glVertex2f(48.5f,41.2f);
    glVertex2f(49.5f,41.2f);
    glVertex2f(48.5f,40.0f);
    glVertex2f(49.5f,40.0f);
    glEnd();

    // sidewalk diagonal lines
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0.3f,0.3f,0.3f);
    glVertex2f(0.0f,0.0f);
    glVertex2f(6.0f,8.8f);
    glVertex2f(12.5f,0.0f);
    glVertex2f(16.5f,8.8f);
    glVertex2f(24.0f,0.0f);
    glVertex2f(26.5f,8.8f);
    glVertex2f(35.0f,0.0f);
    glVertex2f(36.5f,8.8f);
    glVertex2f(45.0f,0.0f);
    glVertex2f(46.0f,8.8f);
    glVertex2f(55.0f,0.0f);
    glVertex2f(55.0f,8.8f);
    glVertex2f(65.5f,0.0f);
    glVertex2f(64.0f,8.8f);
    glVertex2f(76.5f,0.0f);
    glVertex2f(74.0f,8.8f);
    glVertex2f(88.5f,0.0f);
    glVertex2f(85.0f,8.8f);
    glVertex2f(100.0f,0.0f);
    glVertex2f(95.0f,8.8f);
    glEnd();

    // traffic lights
    if (t_lightState == 0)
    {
        t_drawCircle(-0.055f,  0.07f, 0.015f, 1.0f, 0.0f, 0.0f);
    }
    else
    {
        t_drawCircle(-0.055f,  0.07f, 0.015f, 0.3f, 0.0f, 0.0f);
    }
    t_drawCircle(-0.055f,  0.03f, 0.015f, 0.3f, 0.3f, 0.0f);
    if (t_lightState == 1)
    {
        t_drawCircle(-0.055f, -0.01f, 0.015f, 0.0f, 1.0f, 0.0f);
    }
    else
    {
        t_drawCircle(-0.055f, -0.01f, 0.015f, 0.0f, 0.3f, 0.0f);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutSwapBuffers();
}

//  HOME SCREEN
void homeScreen()
{
    glClearColor(0.95f, 0.92f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.7f, 0.0f, 0.0f);
    drawText(330, 550, GLUT_BITMAP_TIMES_ROMAN_24, "Project Title");
    drawText(150, 520, GLUT_BITMAP_TIMES_ROMAN_24, "Dhaka in Motion: Various Transport System Simulation");
    drawText(310, 485, GLUT_BITMAP_HELVETICA_18,   "Computer Graphics [I]");

    glColor3f(0.1f, 0.2f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2f(580, 20);
    glVertex2f(760, 20);
    glVertex2f(760, 55);
    glVertex2f(580, 55);
    glEnd();
    glColor3f(1,1,1);
    drawText(615, 33, GLUT_BITMAP_HELVETICA_18, "INSTRUCTION");

    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(50, 450);
    glVertex2f(750, 450);
    glEnd();

    glColor3f(0, 0, 0);
    drawText(50, 460, GLUT_BITMAP_TIMES_ROMAN_24, "Group Member :");

    drawText(70,  420, GLUT_BITMAP_HELVETICA_18, "ID");
    drawText(320, 420, GLUT_BITMAP_HELVETICA_18, "Name");
    drawText(630, 420, GLUT_BITMAP_HELVETICA_18, "Roll");

    glBegin(GL_LINES);
    glVertex2f(60,  410);
    glVertex2f(160, 410);
    glVertex2f(300, 410);
    glVertex2f(490, 410);
    glVertex2f(620, 410);
    glVertex2f(760, 410);
    glEnd();

    drawText(60,  380, GLUT_BITMAP_HELVETICA_18, "23-51964-2");
    drawText(300, 380, GLUT_BITMAP_HELVETICA_18, "Quazi Saad");
    drawText(690, 380, GLUT_BITMAP_HELVETICA_18, "31");

    drawText(60,  350, GLUT_BITMAP_HELVETICA_18, "23-52013-2");
    drawText(300, 350, GLUT_BITMAP_HELVETICA_18, "Atifur Rahaman Shohan");
    drawText(690, 350, GLUT_BITMAP_HELVETICA_18, "32");

    drawText(60,  320, GLUT_BITMAP_HELVETICA_18, "23-52759-2");
    drawText(300, 320, GLUT_BITMAP_HELVETICA_18, "Zafrin Alam Tanha");
    drawText(690, 320, GLUT_BITMAP_HELVETICA_18, "35");

    drawText(60,  290, GLUT_BITMAP_HELVETICA_18, "23-50325-1");
    drawText(300, 290, GLUT_BITMAP_HELVETICA_18, "Fariha Tasnim");
    drawText(690, 290, GLUT_BITMAP_HELVETICA_18, "15");

    drawText(60,  260, GLUT_BITMAP_HELVETICA_18, "23-51626-2");
    drawText(300, 260, GLUT_BITMAP_HELVETICA_18, "Afia Farzana");
    drawText(690, 260, GLUT_BITMAP_HELVETICA_18, "30");

    glBegin(GL_LINES);
    glVertex2f(50, 220);
    glVertex2f(750, 220);
    glEnd();

    drawText(330, 190, GLUT_BITMAP_TIMES_ROMAN_24, "Supervised by :");
    drawText(330, 160, GLUT_BITMAP_HELVETICA_18,   "Mahfujur Rahman");
    drawText(320, 135, GLUT_BITMAP_HELVETICA_18,   "Assistant Professor");
    drawText(260, 110, GLUT_BITMAP_HELVETICA_18,   "Department of Computer Science");
    drawText(250,  85, GLUT_BITMAP_HELVETICA_18,   "Faculty of Science and Technology");
    drawText(185,  60, GLUT_BITMAP_HELVETICA_18,   "American International University-Bangladesh(AIUB)");

    glColor3f(0.0f, 0.50f, 0.0f);


    glutSwapBuffers();
}

//  INSTRUCTION SCREEN

void instructionScreen()
{
    glClearColor(0.92f, 0.92f, 0.96f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.6f);
    drawText(220, 560, GLUT_BITMAP_TIMES_ROMAN_24,
             "PROJECT CONTRIBUTION & INSTRUCTION");

    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(50, 542);
    glVertex2f(750, 542);
    glEnd();

    // ---- MEMBER 1 ----
    glColor3f(0.6f, 0.0f, 0.0f);
    drawText(60, 516, GLUT_BITMAP_HELVETICA_18, "1. Quazi Saad  (Metro Rail - Press K)");
    glColor3f(0, 0, 0);
    drawText(90, 496, GLUT_BITMAP_HELVETICA_12,
             "Q/S : Speed   |   Mouse Left-click : Red   |  Mouse Right-click : Green   |   Y : Yellow");

    // ---- MEMBER 2 ----
    glColor3f(0.6f, 0.0f, 0.0f);
    drawText(60, 470, GLUT_BITMAP_HELVETICA_18, "2. Atifur Rahaman Shohan  (Railway Station - Press G)");
    glColor3f(0, 0, 0);
    drawText(90, 450, GLUT_BITMAP_HELVETICA_12,
             "W/S : Train speed   |   Space : Pause   |   R : Reset   |   Mouse : Signal");

    // ---- MEMBER 3 ----
    glColor3f(0.6f, 0.0f, 0.0f);
    drawText(60, 424, GLUT_BITMAP_HELVETICA_18, "3. Zafrin Alam Tanha  (City Scene - Press H)");
    glColor3f(0, 0, 0);
    drawText(90, 404, GLUT_BITMAP_HELVETICA_12,
             "G : Green light (cars move)   |   R : Red light (cars stop)");
    drawText(90, 388, GLUT_BITMAP_HELVETICA_12,
             "L-click : Cloud faster   |   R-click : Cloud slower   |   I : Back");

    // ---- MEMBER 4 ----
    glColor3f(0.6f, 0.0f, 0.0f);
    drawText(60, 362, GLUT_BITMAP_HELVETICA_18, "4. Fariha Tasnim  (Airport Scene - Press J)");
    glColor3f(0, 0, 0);
    drawText(90, 342, GLUT_BITMAP_HELVETICA_12,
             "Space : Take Off   |   P : Pause/Resume   |   I : Back");

    // ---- MEMBER 5 ----
    glColor3f(0.6f, 0.0f, 0.0f);
    drawText(60, 316, GLUT_BITMAP_HELVETICA_18, "5. Afia Farzana");
    glColor3f(0, 0, 0);
    drawText(90, 296, GLUT_BITMAP_HELVETICA_12, "- (add contribution here)");

    // ---- BACK BUTTON ----
    glColor3f(0.75f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(20, 20);
    glVertex2f(120, 20);
    glVertex2f(120, 55);
    glVertex2f(20, 55);
    glEnd();
    glColor3f(1, 1, 1);
    drawText(45, 33, GLUT_BITMAP_HELVETICA_18, "BACK");

    glColor3f(0.0f, 0.45f, 0.0f);
    drawText(135, 42, GLUT_BITMAP_HELVETICA_12,
             "Press A -> Railway   |   K -> Metro Rail   |   J -> Airport   |   H -> City Scene");

    glutSwapBuffers();
}

//  RAILWAY STATION – all draw functions

void drawSky()
{
    glBegin(GL_POLYGON);
    glColor3f(0.28f, 0.62f, 0.95f);
    glVertex2f(0,600);
    glVertex2f(800,600);
    glColor3f(0.65f, 0.88f, 1.0f);
    glVertex2f(800,300);
    glVertex2f(0,300);
    glEnd();
}

void drawSun()
{
    glColor3f(1.0f, 0.97f, 0.45f);
    circle(700, 540, 50);
}

void drawClouds()
{
    glColor3f(1,1,1);
    circle(150+cloudMove, 520, 22);
    circle(178+cloudMove, 534, 28);
    circle(210+cloudMove, 520, 22);
    glColor3f(0.96f,0.96f,0.99f);
    circle(440+cloudMove, 540, 20);
    circle(468+cloudMove, 553, 26);
    circle(500+cloudMove, 540, 20);
}

void drawHills()
{
    glColor3f(0.42f, 0.62f, 0.28f);
    circle(100,310,90);
    circle(300,300,110);
    circle(520,305,95);
    circle(710,302,85);
}

void drawGround()
{
    glColor3f(0.38f, 0.58f, 0.22f);
    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(800,0);
    glVertex2f(800,310);
    glVertex2f(0,310);
    glEnd();
}

void drawBuildings()
{
    glColor3f(0.28f,0.40f,0.62f);
    glBegin(GL_POLYGON);
    glVertex2f(20,310);
    glVertex2f(110,310);
    glVertex2f(110,510);
    glVertex2f(20,510);
    glEnd();
    glColor3f(0.85f,0.92f,1.0f);
    for(int r=0; r<5; r++)
        for(int c=0; c<3; c++)
        {
            glBegin(GL_POLYGON);
            glVertex2f(30+c*26,318+r*36);
            glVertex2f(48+c*26,318+r*36);
            glVertex2f(48+c*26,338+r*36);
            glVertex2f(30+c*26,338+r*36);
            glEnd();
        }
    glColor3f(0.33f,0.46f,0.65f);
    glBegin(GL_POLYGON);
    glVertex2f(120,310);
    glVertex2f(205,310);
    glVertex2f(205,465);
    glVertex2f(120,465);
    glEnd();
    glColor3f(0.85f,0.92f,1.0f);
    for(int r=0; r<4; r++)
        for(int c=0; c<2; c++)
        {
            glBegin(GL_POLYGON);
            glVertex2f(130+c*32,320+r*36);
            glVertex2f(152+c*32,320+r*36);
            glVertex2f(152+c*32,340+r*36);
            glVertex2f(130+c*32,340+r*36);
            glEnd();
        }
    glColor3f(0.30f,0.42f,0.60f);
    glBegin(GL_POLYGON);
    glVertex2f(610,310);
    glVertex2f(698,310);
    glVertex2f(698,500);
    glVertex2f(610,500);
    glEnd();
    glColor3f(0.85f,0.92f,1.0f);
    for(int r=0; r<5; r++)
        for(int c=0; c<2; c++)
        {
            glBegin(GL_POLYGON);
            glVertex2f(620+c*30,318+r*36);
            glVertex2f(640+c*30,318+r*36);
            glVertex2f(640+c*30,338+r*36);
            glVertex2f(620+c*30,338+r*36);
            glEnd();
        }
    glColor3f(0.36f,0.50f,0.68f);
    glBegin(GL_POLYGON);
    glVertex2f(708,310);
    glVertex2f(790,310);
    glVertex2f(790,460);
    glVertex2f(708,460);
    glEnd();
    glColor3f(0.85f,0.92f,1.0f);
    for(int r=0; r<4; r++)
        for(int c=0; c<2; c++)
        {
            glBegin(GL_POLYGON);
            glVertex2f(716+c*28,318+r*36);
            glVertex2f(734+c*28,318+r*36);
            glVertex2f(734+c*28,336+r*36);
            glVertex2f(716+c*28,336+r*36);
            glEnd();
        }
}

void drawStation()
{
    glColor3f(0.65f,0.65f,0.65f);
    glBegin(GL_POLYGON);
    glVertex2f(298,152);
    glVertex2f(644,152);
    glVertex2f(644,212);
    glVertex2f(298,212);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(295,156);
    glVertex2f(640,156);
    glVertex2f(640,186);
    glVertex2f(295,186);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(295,186);
    glVertex2f(640,186);
    glVertex2f(640,215);
    glVertex2f(295,215);
    glEnd();
    glColor3f(0.95f,0.82f,0.08f);
    glBegin(GL_POLYGON);
    glVertex2f(295,158);
    glVertex2f(640,158);
    glVertex2f(640,165);
    glVertex2f(295,165);
    glEnd();
    for(int px=315; px<=615; px+=75)
    {
        glColor3f(0.50f,0.50f,0.50f);
        glBegin(GL_POLYGON);
        glVertex2f(px,215);
        glVertex2f(px+8,215);
        glVertex2f(px+8,260);
        glVertex2f(px,260);
        glEnd();
    }
    glColor3f(0.30f,0.38f,0.56f);
    glBegin(GL_POLYGON);
    glVertex2f(288,254);
    glVertex2f(650,254);
    glVertex2f(650,268);
    glVertex2f(288,268);
    glEnd();
    glColor3f(0.22f,0.28f,0.44f);
    glBegin(GL_POLYGON);
    glVertex2f(288,268);
    glVertex2f(650,268);
    glVertex2f(650,275);
    glVertex2f(288,275);
    glEnd();
    glColor3f(0.52f,0.34f,0.14f);
    glBegin(GL_POLYGON);
    glVertex2f(370,174);
    glVertex2f(420,174);
    glVertex2f(420,179);
    glVertex2f(370,179);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(372,166);
    glVertex2f(376,166);
    glVertex2f(376,175);
    glVertex2f(372,175);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(416,166);
    glVertex2f(420,166);
    glVertex2f(420,175);
    glVertex2f(416,175);
    glEnd();
    glColor3f(0.28f,0.28f,0.32f);
    glBegin(GL_POLYGON);
    glVertex2f(286,156);
    glVertex2f(290,156);
    glVertex2f(290,236);
    glVertex2f(286,236);
    glEnd();
    glColor3f(0.20f,0.20f,0.24f);
    glBegin(GL_POLYGON);
    glVertex2f(281,228);
    glVertex2f(297,228);
    glVertex2f(297,248);
    glVertex2f(281,248);
    glEnd();
    if(!greenSignal)
        glColor3f(1.0f,0.12f,0.08f);
    else
        glColor3f(0.3f,0.0f,0.0f);
    circle(289,244,4);
    if(greenSignal)
        glColor3f(0.10f,0.92f,0.25f);
    else
        glColor3f(0.0f,0.3f,0.0f);
    circle(289,233,4);
}

void drawTrees()
{
    float treeX[]= {220,290,350,420,510,720,770};
    for(int i=0; i<7; i++)
    {
        float x=treeX[i], y=310;
        glColor3f(0.45f,0.24f,0.06f);
        glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+10,y);
        glVertex2f(x+10,y+55);
        glVertex2f(x,y+55);
        glEnd();
        glColor3f(0.12f,0.60f,0.18f);
        circle(x+5,y+55,24);
        circle(x+5,y+75,18);
    }
}

void drawPineTree(float x,float y)
{
    glColor3f(0.45f,0.24f,0.06f);
    glBegin(GL_POLYGON);
    glVertex2f(x,y);
    glVertex2f(x+10,y);
    glVertex2f(x+10,y+40);
    glVertex2f(x,y+40);
    glEnd();
    glColor3f(0.05f,0.45f,0.10f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x-20,y+35);
    glVertex2f(x+30,y+35);
    glVertex2f(x+5,y+75);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x-16,y+55);
    glVertex2f(x+26,y+55);
    glVertex2f(x+5,y+95);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x-12,y+72);
    glVertex2f(x+22,y+72);
    glVertex2f(x+5,y+112);
    glEnd();
}

void drawBushyTree(float x,float y)
{
    glColor3f(0.45f,0.24f,0.06f);
    glBegin(GL_POLYGON);
    glVertex2f(x,y);
    glVertex2f(x+12,y);
    glVertex2f(x+12,y+50);
    glVertex2f(x,y+50);
    glEnd();
    glColor3f(0.10f,0.60f,0.15f);
    circle(x-8,y+55,20);
    circle(x+18,y+55,20);
    circle(x+5,y+75,24);
    circle(x+5,y+95,16);
}

void drawFlowerBed(float x,float y)
{
    glColor3f(0.10f,0.55f,0.10f);
    circle(x,y,18);
    circle(x+18,y+5,20);
    circle(x+36,y,18);
    glColor3f(1.0f,0.2f,0.4f);
    circle(x+8,y+6,3);
    glColor3f(1.0f,1.0f,0.2f);
    circle(x+20,y+10,3);
    glColor3f(0.9f,0.3f,1.0f);
    circle(x+30,y+5,3);
    glColor3f(0.2f,0.9f,1.0f);
    circle(x+15,y-2,3);
}

void drawTrack(float y)
{
    glColor3f(0.38f,0.26f,0.12f);
    for(int x=0; x<800; x+=28)
    {
        glBegin(GL_POLYGON);
        glVertex2f(x,y-2);
        glVertex2f(x+18,y-2);
        glVertex2f(x+18,y+16);
        glVertex2f(x,y+16);
        glEnd();
    }
    glColor3f(0.58f,0.55f,0.50f);
    glBegin(GL_POLYGON);
    glVertex2f(0,y);
    glVertex2f(800,y);
    glVertex2f(800,y+4);
    glVertex2f(0,y+4);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0,y+10);
    glVertex2f(800,y+10);
    glVertex2f(800,y+14);
    glVertex2f(0,y+14);
    glEnd();
}

void drawWheel(float cx,float cy,float r,float angleDeg)
{
    glPushMatrix();
    glTranslatef(cx,cy,0);
    glRotatef(angleDeg,0,0,1);
    glColor3f(0.12f,0.12f,0.12f);
    circle(0,0,r);
    glColor3f(0.42f,0.42f,0.42f);
    circle(0,0,r*0.72f);
    glColor3f(0.55f,0.55f,0.55f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for(int s=0; s<6; s++)
    {
        float a=s*PI/3.0f;
        glVertex2f(0,0);
        glVertex2f((r*0.68f)*cos(a),(r*0.68f)*sin(a));
    }
    glEnd();
    glLineWidth(1.0f);
    glColor3f(0.65f,0.65f,0.65f);
    circle(0,0,r*0.22f);
    glColor3f(0.88f,0.88f,0.88f);
    circle(0,0,r*0.10f);
    glPopMatrix();
}

void drawBogie(float offset,int isEngine)
{
    float ty=156.0f,tw=150.0f,th=44.0f,wy=ty-6.0f,wr=10.0f;
    glColor3f(0.22f,0.22f,0.22f);
    glBegin(GL_POLYGON);
    glVertex2f(offset+4,ty-4);
    glVertex2f(offset+tw-4,ty-4);
    glVertex2f(offset+tw-4,ty+2);
    glVertex2f(offset+4,ty+2);
    glEnd();
    glColor3f(0.93f,0.97f,0.98f);
    glBegin(GL_POLYGON);
    glVertex2f(offset,ty);
    glVertex2f(offset+tw,ty);
    glVertex2f(offset+tw,ty+th);
    glVertex2f(offset,ty+th);
    glEnd();
    glColor3f(0.10f,0.20f,0.42f);
    glBegin(GL_POLYGON);
    glVertex2f(offset,ty+th-9);
    glVertex2f(offset+tw,ty+th-9);
    glVertex2f(offset+tw,ty+th);
    glVertex2f(offset,ty+th);
    glEnd();
    glColor3f(0.08f,0.72f,0.22f);
    glBegin(GL_POLYGON);
    glVertex2f(offset,ty);
    glVertex2f(offset+tw,ty);
    glVertex2f(offset+tw,ty+9);
    glVertex2f(offset,ty+9);
    glEnd();
    float winW=tw*0.19f, winH=th*0.42f, winY=ty+14;
    float gaps[3]= {0.12f,0.42f,0.72f};
    for(int i=0; i<3; i++)
    {
        float wx=offset+tw*gaps[i];
        glColor3f(0.50f,0.58f,0.68f);
        glBegin(GL_POLYGON);
        glVertex2f(wx-1,winY-1);
        glVertex2f(wx+winW+1,winY-1);
        glVertex2f(wx+winW+1,winY+winH+1);
        glVertex2f(wx-1,winY+winH+1);
        glEnd();
        glColor3f(0.68f,0.85f,0.95f);
        glBegin(GL_POLYGON);
        glVertex2f(wx,winY);
        glVertex2f(wx+winW,winY);
        glVertex2f(wx+winW,winY+winH);
        glVertex2f(wx,winY+winH);
        glEnd();
        glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_POLYGON);
        glVertex2f(wx+2,winY+winH-5);
        glVertex2f(wx+winW*0.5f,winY+winH-5);
        glVertex2f(wx+winW*0.5f,winY+winH-2);
        glVertex2f(wx+2,winY+winH-2);
        glEnd();
    }
    glColor3f(0.60f,0.70f,0.80f);
    glLineWidth(1.2f);
    glBegin(GL_LINES);
    glVertex2f(offset+tw*0.34f,ty);
    glVertex2f(offset+tw*0.34f,ty+th);
    glVertex2f(offset+tw*0.86f,ty);
    glVertex2f(offset+tw*0.86f,ty+th);
    glEnd();
    glLineWidth(1.0f);
    glColor3f(0.55f,0.55f,0.55f);
    glBegin(GL_POLYGON);
    glVertex2f(offset-9,ty+th*0.38f);
    glVertex2f(offset,ty+th*0.38f);
    glVertex2f(offset,ty+th*0.38f+8);
    glVertex2f(offset-9,ty+th*0.38f+8);
    glEnd();
    if(isEngine)
    {
        glColor3f(0.28f,0.28f,0.28f);
        glBegin(GL_POLYGON);
        glVertex2f(offset+tw*0.25f-7,ty+th+1);
        glVertex2f(offset+tw*0.25f+7,ty+th+1);
        glVertex2f(offset+tw*0.25f+7,ty+th+5);
        glVertex2f(offset+tw*0.25f-7,ty+th+5);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(offset+tw*0.25f-9,ty+th+3);
        glVertex2f(offset+tw*0.25f+9,ty+th+3);
        glVertex2f(offset+tw*0.25f+9,ty+th+8);
        glVertex2f(offset+tw*0.25f-9,ty+th+8);
        glEnd();
        glColor3f(0.40f,0.40f,0.40f);
        glBegin(GL_POLYGON);
        glVertex2f(offset+tw-24,ty+th+0.5f);
        glVertex2f(offset+tw-6,ty+th+0.5f);
        glVertex2f(offset+tw-6,ty+th+2);
        glVertex2f(offset+tw-24,ty+th+2);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(offset+tw-22,ty+th+2);
        glVertex2f(offset+tw-8,ty+th+2);
        glVertex2f(offset+tw-8,ty+th+12);
        glVertex2f(offset+tw-22,ty+th+12);
        glEnd();
        glColor3f(0.78f,0.62f,0.22f);
        glBegin(GL_POLYGON);
        glVertex2f(offset+tw,ty+4);
        glVertex2f(offset+tw+10,ty+4);
        glVertex2f(offset+tw+10,ty+th-4);
        glVertex2f(offset+tw,ty+th-4);
        glEnd();
        glColor3f(1.0f,0.96f,0.60f);
        circle(offset+tw+8,ty+th*0.55f,6.5f);
        glColor3f(1.0f,1.0f,0.85f);
        circle(offset+tw+8,ty+th*0.55f,4.5f);
        glColor3f(0.55f,0.55f,0.55f);
        glBegin(GL_POLYGON);
        glVertex2f(offset+tw+10,ty+th*0.35f);
        glVertex2f(offset+tw+18,ty+th*0.35f);
        glVertex2f(offset+tw+18,ty+th*0.35f+8);
        glVertex2f(offset+tw+10,ty+th*0.35f+8);
        glEnd();
    }
    else
    {
        glColor3f(0.55f,0.55f,0.55f);
        glBegin(GL_POLYGON);
        glVertex2f(offset+tw,ty+th*0.38f);
        glVertex2f(offset+tw+9,ty+th*0.38f);
        glVertex2f(offset+tw+9,ty+th*0.38f+8);
        glVertex2f(offset+tw,ty+th*0.38f+8);
        glEnd();
    }
    drawWheel(offset+25,wy,wr,wheelAngle);
    drawWheel(offset+75,wy,wr,wheelAngle);
    drawWheel(offset+125,wy,wr,wheelAngle);
}

void drawLampPosts()
{
    float lampX[]= {250,690};
    for(int i=0; i<2; i++)
    {
        float x=lampX[i];
        glColor3f(0.20f,0.20f,0.20f);
        glBegin(GL_POLYGON);
        glVertex2f(x,180);
        glVertex2f(x+6,180);
        glVertex2f(x+6,280);
        glVertex2f(x,280);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2f(x,275);
        glVertex2f(x+24,275);
        glVertex2f(x+24,280);
        glVertex2f(x,280);
        glEnd();
        glColor3f(1.0f,0.95f,0.6f);
        circle(x+24,270,8);
    }
}

void drawStationBoard()
{
    glColor3f(0.30f,0.30f,0.30f);
    glBegin(GL_POLYGON);
    glVertex2f(520,215);
    glVertex2f(525,215);
    glVertex2f(525,260);
    glVertex2f(520,260);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(620,215);
    glVertex2f(625,215);
    glVertex2f(625,260);
    glVertex2f(620,260);
    glEnd();
    glColor3f(0.10f,0.20f,0.60f);
    glBegin(GL_POLYGON);
    glVertex2f(500,250);
    glVertex2f(645,250);
    glVertex2f(645,285);
    glVertex2f(500,285);
    glEnd();
}

void drawBushes()
{
    glColor3f(0.10f,0.55f,0.12f);
    circle(180,175,16);
    circle(195,180,18);
    circle(210,175,16);
    glColor3f(1.0f,0.2f,0.4f);
    circle(190,180,3);
    glColor3f(1.0f,1.0f,0.2f);
    circle(205,175,3);
    glColor3f(0.10f,0.55f,0.12f);
    circle(700,175,16);
    circle(715,180,18);
    circle(730,175,16);
    glColor3f(1.0f,0.3f,0.3f);
    circle(715,180,3);
}

void drawBarrier()
{
    glColor3f(0.35f,0.35f,0.35f);
    glBegin(GL_POLYGON);
    glVertex2f(90,156);
    glVertex2f(100,156);
    glVertex2f(100,200);
    glVertex2f(90,200);
    glEnd();
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(100,190);
    glVertex2f(210,190);
    glVertex2f(210,196);
    glVertex2f(100,196);
    glEnd();
    glColor3f(1,1,1);
    for(int i=0; i<5; i++)
    {
        glBegin(GL_POLYGON);
        glVertex2f(110+i*20,190);
        glVertex2f(118+i*20,190);
        glVertex2f(118+i*20,196);
        glVertex2f(110+i*20,196);
        glEnd();
    }
}

void drawTrain()
{
    float gap=6.0f, cw=150.0f;
    glColor3f(0.50f,0.50f,0.50f);
    glBegin(GL_POLYGON);
    glVertex2f(trainX+cw,164);
    glVertex2f(trainX+cw+gap,164);
    glVertex2f(trainX+cw+gap,170);
    glVertex2f(trainX+cw,170);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(trainX+2*cw+gap,164);
    glVertex2f(trainX+2*cw+2*gap,164);
    glVertex2f(trainX+2*cw+2*gap,170);
    glVertex2f(trainX+2*cw+gap,170);
    glEnd();
    drawBogie(trainX,0);
    drawBogie(trainX+cw+gap,0);
    drawBogie(trainX+2*(cw+gap),1);
}

void drawPolice(float x,float y)
{
    glColor3f(1.0f,0.8f,0.6f);
    circle(x,y+28,6);
    glColor3f(0.10f,0.10f,0.35f);
    glBegin(GL_POLYGON);
    glVertex2f(x-6,y+32);
    glVertex2f(x+6,y+32);
    glVertex2f(x+6,y+36);
    glVertex2f(x-6,y+36);
    glEnd();
    glColor3f(0.10f,0.20f,0.60f);
    glBegin(GL_POLYGON);
    glVertex2f(x-6,y+5);
    glVertex2f(x+6,y+5);
    glVertex2f(x+6,y+22);
    glVertex2f(x-6,y+22);
    glEnd();
    glColor3f(0.1f,0.1f,0.1f);
    glBegin(GL_LINES);
    glVertex2f(x-3,y+5);
    glVertex2f(x-3,y-8);
    glVertex2f(x+3,y+5);
    glVertex2f(x+3,y-8);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x-10,y+16);
    glVertex2f(x+10,y+16);
    glEnd();
}

void drawPoliceCar(float x,float y)
{
    glColor3f(0.10f,0.10f,0.45f);
    glBegin(GL_POLYGON);
    glVertex2f(x,y);
    glVertex2f(x+90,y);
    glVertex2f(x+90,y+25);
    glVertex2f(x,y+25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(x+15,y+25);
    glVertex2f(x+65,y+25);
    glVertex2f(x+55,y+45);
    glVertex2f(x+25,y+45);
    glEnd();
    glColor3f(0.75f,0.90f,1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x+22,y+28);
    glVertex2f(x+40,y+28);
    glVertex2f(x+35,y+42);
    glVertex2f(x+25,y+42);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(x+42,y+28);
    glVertex2f(x+58,y+28);
    glVertex2f(x+52,y+42);
    glVertex2f(x+38,y+42);
    glEnd();
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(x+35,y+46);
    glVertex2f(x+45,y+46);
    glVertex2f(x+45,y+50);
    glVertex2f(x+35,y+50);
    glEnd();
    glColor3f(0,0,1);
    glBegin(GL_POLYGON);
    glVertex2f(x+45,y+46);
    glVertex2f(x+55,y+46);
    glVertex2f(x+55,y+50);
    glVertex2f(x+45,y+50);
    glEnd();
    glColor3f(0.1f,0.1f,0.1f);
    circle(x+20,y,10);
    circle(x+70,y,10);
    glColor3f(0.5f,0.5f,0.5f);
    circle(x+20,y,4);
    circle(x+70,y,4);
}

void drawMiniTicketBooth(float x,float y)
{
    glColor3f(0.90f,0.40f,0.20f);
    glBegin(GL_POLYGON);
    glVertex2f(x,y);
    glVertex2f(x+70,y);
    glVertex2f(x+70,y+60);
    glVertex2f(x,y+60);
    glEnd();
    glColor3f(0.30f,0.90f,0.50f);
    glBegin(GL_POLYGON);
    glVertex2f(x-5,y+60);
    glVertex2f(x+75,y+60);
    glVertex2f(x+75,y+70);
    glVertex2f(x-5,y+70);
    glEnd();
    glColor3f(0.75f,0.90f,1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x+15,y+25);
    glVertex2f(x+55,y+25);
    glVertex2f(x+55,y+45);
    glVertex2f(x+15,y+45);
    glEnd();
}

void drawRoad()
{
    glColor3f(0.25f,0.25f,0.25f);
    glBegin(GL_POLYGON);
    glVertex2f(0,230);
    glVertex2f(800,230);
    glVertex2f(800,290);
    glVertex2f(0,290);
    glEnd();
    glColor3f(1.0f,1.0f,0.0f);
    for(int x=0; x<800; x+=35)
    {
        glBegin(GL_POLYGON);
        glVertex2f(x+5,272);
        glVertex2f(x+20,272);
        glVertex2f(x+20,276);
        glVertex2f(x+5,276);
        glEnd();
    }
}

void drawPoliceStation(float x,float y)
{
    glColor3f(0.72f,0.60f,0.40f);
    glBegin(GL_POLYGON);
    glVertex2f(x,y);
    glVertex2f(x+90,y);
    glVertex2f(x+90,y+70);
    glVertex2f(x,y+70);
    glEnd();
    glColor3f(0.10f,0.20f,0.65f);
    glBegin(GL_POLYGON);
    glVertex2f(x-5,y+70);
    glVertex2f(x+95,y+70);
    glVertex2f(x+95,y+82);
    glVertex2f(x-5,y+82);
    glEnd();
    glColor3f(0.35f,0.20f,0.10f);
    glBegin(GL_POLYGON);
    glVertex2f(x+35,y);
    glVertex2f(x+55,y);
    glVertex2f(x+55,y+35);
    glVertex2f(x+35,y+35);
    glEnd();
    glColor3f(0.75f,0.90f,1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x+10,y+28);
    glVertex2f(x+30,y+28);
    glVertex2f(x+30,y+48);
    glVertex2f(x+10,y+48);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(x+60,y+28);
    glVertex2f(x+80,y+28);
    glVertex2f(x+80,y+48);
    glVertex2f(x+60,y+48);
    glEnd();
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x+20,y+55);
    glVertex2f(x+70,y+55);
    glVertex2f(x+70,y+66);
    glVertex2f(x+20,y+66);
    glEnd();
}

void drawGrassTexture()
{
    glColor3f(0.15f,0.45f,0.10f);
    for(int x=0; x<800; x+=20)
    {
        glBegin(GL_LINES);
        glVertex2f(x,120);
        glVertex2f(x+3,128);
        glVertex2f(x+5,120);
        glVertex2f(x+8,130);
        glVertex2f(x+10,120);
        glVertex2f(x+13,127);
        glEnd();
    }
}

void drawSceneHint() {}

void railwayScene()
{
    glClearColor(0.53f,0.81f,0.98f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky();
    drawSun();
    drawClouds();
    drawHills();
    drawGround();
    drawGrassTexture();
    drawBuildings();
    drawStation();
    drawTrees();
    drawLampPosts();
    drawStationBoard();
    drawBushes();
    drawBarrier();
    drawBuildings();
    drawRoad();
    drawStation();
    drawLampPosts();
    drawStationBoard();
    drawBushes();
    drawBarrier();
    drawTrees();
    drawPineTree(10,300);
    drawPineTree(700,300);
    drawBushyTree(580,310);
    drawBushyTree(620,310);
    drawFlowerBed(750,180);
    drawFlowerBed(20,180);
    drawPoliceCar(policeCarX,265);
    drawMiniTicketBooth(550,180);
    drawPoliceStation(170,290);
    drawPolice(190,230);
    drawPolice(215,230);
    drawTrack(60);
    drawTrack(100);
    drawTrack(140);
    drawTrain();
    drawSceneHint();
    glutSwapBuffers();
}


//  SOUND HELPERS

void playTrainSound()
{
    PlaySound(TEXT("horn.wav"),NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);
}
void stopTrainSound()
{
    PlaySound(NULL,NULL,0);
}

//  GLUT CALLBACKS

void display()
{
    if(currentScreen == 0)
    {
        homeScreen();
        return;
    }
    if(currentScreen == 1)
    {
        instructionScreen();
        return;
    }
    if(currentScreen == 3)
    {
        metroRailScene();
        return;
    }
    if(currentScreen == 4)
    {
        airportScene();
        return;
    }
    if(currentScreen == 5)
    {
        tanhaCityScene();
        return;
    }
    railwayScene();
}

void mouse(int button,int state,int x,int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        y = 600-y;

        if(currentScreen==0)
        {
            if(x>=580 && x<=760 && y>=20 && y<=55)
            {
                currentScreen=1;
                glutPostRedisplay();
            }
        }
        else if(currentScreen==1)
        {
            if(x>=20 && x<=120 && y>=20 && y<=55)
            {
                currentScreen=0;
                glutPostRedisplay();
            }
        }
        else if(currentScreen==2)
        {
            if(x>=285&&x<=293&&y>=240&&y<=248)
            {
                greenSignal=false;
                stopTrainSound();
            }
            if(x>=285&&x<=293&&y>=229&&y<=237)
            {
                greenSignal=true;
                playTrainSound();
            }
            glutPostRedisplay();
        }
    }

    // Metro Rail scene mouse (screen 3)
    if(currentScreen==3 && state==GLUT_DOWN)
    {
        if(button==GLUT_LEFT_BUTTON)
        {
            f_manualLightOverride=true;
            f_trafficLightState=2;
        }
        else if(button==GLUT_RIGHT_BUTTON)
        {
            f_manualLightOverride=true;
            f_trafficLightState=0;
        }
        glutPostRedisplay();
    }

    // Tanha City scene mouse (screen 5)
    if(currentScreen==5 && state==GLUT_DOWN)
    {
        if(button==GLUT_LEFT_BUTTON)
        {
            t_cloudSpeed += 0.01f;
            if(t_cloudSpeed > 0.3f)
                t_cloudSpeed = 0.3f;
        }
        else if(button==GLUT_RIGHT_BUTTON)
        {
            t_cloudSpeed -= 0.01f;
            if(t_cloudSpeed < 0.0f)
                t_cloudSpeed = 0.0f;
        }
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key,int x,int y)
{
    // I → return to Instruction page from any scene
    if((key=='i'||key=='I') && (currentScreen==2||currentScreen==3||currentScreen==4||currentScreen==5))
    {
        stopTrainSound();
        PlaySound(NULL,0,0);
        currentScreen=1;
        glutPostRedisplay();
        return;
    }

    // K → launch Metro Rail scene from instruction screen
    if((key=='k'||key=='K') && currentScreen==1)
    {
        currentScreen=3;
        f_cloudOffset=0.0f;
        f_train1Offset=-50.0f;
        f_trainWheelAngle=0.0f;
        f_car1X=-20.0f;
        f_wheelAngle=0.0f;
        f_trafficLightState=-1;
        f_trafficLightTimer=0.0f;
        f_manualLightOverride=true;
        f_speedLevel=1;
        PlaySound(TEXT("MS.wav"),NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);
        glutPostRedisplay();
        return;
    }

    // J → launch Airport scene from instruction screen
    if((key=='j'||key=='J') && currentScreen==1)
    {
        currentScreen=4;
        planeX = -400.0f;
        planeY = -40.0f;
        planeAngle = 0.0f;
        speedX = 2.5f;
        speedY = 0.0f;
        flightPhase = 0;
        ap_paused = false;
        takeoffEnabled = false;
        cloudAP_X[0] = -300.0f;
        cloudAP_X[1] = 0.0f;
        cloudAP_X[2] = 250.0f;
        cloudAP_Y[0] = 220.0f;
        cloudAP_Y[1] = 240.0f;
        cloudAP_Y[2] = 205.0f;
        PlaySound(TEXT("plane.wav"),NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);
        glutPostRedisplay();
        return;
    }

    // H → launch Tanha City scene from instruction screen
    if((key=='h'||key=='H') && currentScreen==1)
    {
        currentScreen=5;
        t_cloudPosition=0.0f;
        t_cloudSpeed=0.05f;
        t_carOffset=0.0f;
        t_lightState=0;
        t_carSpeed=0.2f;
        PlaySound(TEXT("traffic.wav"),NULL,SND_ASYNC|SND_FILENAME);
        glutPostRedisplay();
        return;
    }

    // A or a → launch railway scene from home or instruction
    if((key=='g'||key=='G') && currentScreen!=2 && currentScreen!=3 && currentScreen!=4 && currentScreen!=5)
    {
        currentScreen=2;
        trainX=-600;
        wheelAngle=0;
        cloudMove=0;
        policeCarX=-120;
        policeCarStop=false;
        paused=0;
        speed=1.5f;
        greenSignal=true;
        playTrainSound();
        glutPostRedisplay();
        return;
    }

    if(currentScreen==2)
    {
        switch(key)
        {
        case 'w':
        case 'W':
            if(speed<12.0f)
                speed+=0.5f;
            break;
        case 's':
        case 'S':
            if(speed>0.0f)
                speed-=0.5f;
            break;
        case ' ':
            paused=!paused;
            break;
        case 'r':
        case 'R':
            trainX=-600;
            wheelAngle=0;
            break;
        case 27:
            exit(0);
        }
    }

    if(currentScreen==3)
    {
        switch(key)
        {
        case 'q':
        case 'Q':
            if(f_speedLevel>0)
                f_speedLevel--;
            break;
        case 's':
        case 'S':
            if(f_speedLevel<5)
                f_speedLevel++;
            break;
        case 'y':
        case 'Y':
            f_manualLightOverride=true;
            f_trafficLightState=1;
            break;
        case 'a':
        case 'A':
            f_manualLightOverride=false;
            f_trafficLightTimer=0.0f;
            break;
        case 'm':
        case 'M':
            PlaySound(NULL,0,0);
            break;
        case 'u':
        case 'U':
            PlaySound(TEXT("MS.wav"),NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);
            break;
        }
    }

    if(currentScreen==4)
    {
        switch(key)
        {
        case ' ':
            takeoffEnabled = true;
            break;
        case 'p':
        case 'P':
            ap_paused = !ap_paused;
            break;
        }
    }

    // Tanha City scene keyboard (screen 5)
    if(currentScreen==5)
    {
        switch(key)
        {
        case 'g':
        case 'G':
            t_lightState = 1;
            break;
        case 'r':
        case 'R':
            t_lightState = 0;
            break;
        }
    }
}

void specialKeys(int key,int x,int y)
{
    if(currentScreen==2)
    {
        switch(key)
        {
        case GLUT_KEY_UP:
            if(speed<12.0f)
                speed+=0.5f;
            break;
        case GLUT_KEY_DOWN:
            if(speed>0.0f)
                speed-=0.5f;
            break;
        }
    }
}

void update(int value)
{
    if(currentScreen==2 && !paused)
    {
        cloudMove+=0.5f;
        if(cloudMove>800)
            cloudMove=-800;

        if(greenSignal)
        {
            trainX+=speed;
            wheelAngle-=speed*3.0f;
            if(wheelAngle<-360.0f)
                wheelAngle+=360.0f;
        }
        if(trainX>900)
            trainX=-600;

        if(!policeCarStop)
        {
            policeCarX+=1.0f;
            if(policeCarX>=70)
            {
                policeCarX=70;
                policeCarStop=true;
            }
        }
    }

    if(currentScreen==3)
    {
        if(!f_manualLightOverride)
        {
            f_trafficLightTimer+=0.02f;
            if(f_trafficLightTimer>15.0f)
                f_trafficLightTimer=0.0f;
            if(f_trafficLightTimer<7.0f)
                f_trafficLightState=0;
            else if(f_trafficLightTimer<9.0f)
                f_trafficLightState=1;
            else
                f_trafficLightState=2;
        }
        f_cloudOffset+=0.015f;
        if(f_cloudOffset>100.0f)
            f_cloudOffset=-90.0f;
        float mult=f_getSpeedMultiplier();
        float trainActualSpeed=f_trainSpeed*mult;
        f_train1Offset+=trainActualSpeed;
        f_trainWheelAngle-=(trainActualSpeed*80.0f);
        if(f_train1Offset>100.0f)
            f_train1Offset=-50.0f;
        bool stopCar1=(f_trafficLightState==1||f_trafficLightState==2)&&
                      (f_car1X+10.0f>42.0f)&&(f_car1X+10.0f<46.5f);
        if(!stopCar1)
        {
            float carSpeed=0.05f*mult;
            f_car1X+=carSpeed;
            f_wheelAngle-=(carSpeed*45.0f);
            if(f_car1X>110.0f)
                f_car1X=-30.0f;
        }
    }

    // Airport scene animation
    if(currentScreen==4 && !ap_paused)
    {
        for (int i = 0; i < 3; i++)
        {
            cloudAP_X[i] -= cloudAP_Speed[i];
            if (cloudAP_X[i] < -550.0f)
                cloudAP_X[i] = 550.0f;
        }
        planeX += speedX;
        planeY += speedY;
        if (takeoffEnabled && flightPhase == 0)
            flightPhase = 1;
        if (flightPhase == 1)
        {
            planeAngle += 0.2f;
            speedY += 0.05f;
            if (planeAngle >= 15.0f)
                flightPhase = 2;
        }
        if (planeX > 600)
        {
            planeX = -500.0f;
            planeY = -40.0f;
            speedY = 0.0f;
            planeAngle = 0.0f;
            flightPhase = 0;
            takeoffEnabled = false;
        }
    }

    // Tanha City scene animation
    if(currentScreen==5)
    {
        t_cloudPosition += t_cloudSpeed;
        if(t_cloudPosition > 120.0f)
            t_cloudPosition = -40.0f;
        if(t_lightState == 1)
        {
            t_carOffset += t_carSpeed;
            if(t_carOffset > 120.0f)
                t_carOffset = -120.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16,update,0);
}

void reshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,800,0,600);
    glMatrixMode(GL_MODELVIEW);
}

//  MAIN
int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(800,600);
    glutCreateWindow("Roads and Movement in Dhaka");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutTimerFunc(0,update,0);

    glutMainLoop();
    return 0;
}
