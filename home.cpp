#include <windows.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>

// ================= TEXT FUNCTION =================


int currentScreen = 0;
void drawText(float x, float y, void *font, const char* text)
{
    glRasterPos2f(x, y);

    for(int i = 0; i < strlen(text); i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
}

// ================= AIUB LOGO FUNCTION =================


// ================= DISPLAY =================

void instructionScreen()
{
    glClearColor(0.92f,0.92f,0.96f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // ================= TITLE =================

    glColor3f(0.0f,0.0f,0.6f);

    drawText(250,540,
             GLUT_BITMAP_TIMES_ROMAN_24,
             "PROJECT CONTRIBUTION & INSTRUCTION");

    // line
    glColor3f(0,0,0);

    glLineWidth(2);

    glBegin(GL_LINES);

    glVertex2f(150,520);
    glVertex2f(650,520);

    glEnd();

    // ================= MEMBER 1 =================

    glColor3f(0.6f,0.0f,0.0f);

    drawText(60,470,
             GLUT_BITMAP_HELVETICA_18,
             "1. Quazi Saad");

    glColor3f(0,0,0);

    drawText(90,440,
             GLUT_BITMAP_HELVETICA_18,
             "- Speed - ");

    drawText(90,415,
             GLUT_BITMAP_HELVETICA_18,
             "- Mouse click");

    // ================= MEMBER 2 =================

    glColor3f(0.6f,0.0f,0.0f);

    drawText(60,370,
             GLUT_BITMAP_HELVETICA_18,
             "2. Atifur Rahaman Shohan");

    glColor3f(0,0,0);

    drawText(90,340,
             GLUT_BITMAP_HELVETICA_18,
             "- Speed");

    drawText(90,315,
             GLUT_BITMAP_HELVETICA_18,
             "- Speed");

    // ================= MEMBER 3 =================

    glColor3f(0.6f,0.0f,0.0f);

    drawText(60,270,
             GLUT_BITMAP_HELVETICA_18,
             "3. Zafrin Alam Tanha");

    glColor3f(0,0,0);

    drawText(90,240,
             GLUT_BITMAP_HELVETICA_18,
             "- Speed");

    // ================= MEMBER 4 =================

    glColor3f(0.6f,0.0f,0.0f);

    drawText(60,200,
             GLUT_BITMAP_HELVETICA_18,
             "4. Fariha Tasnim");

    glColor3f(0,0,0);

    drawText(90,170,
             GLUT_BITMAP_HELVETICA_18,
             "- Police station and traffic system.");

    // ================= MEMBER 5 =================

    glColor3f(0.6f,0.0f,0.0f);

    drawText(60,130,
             GLUT_BITMAP_HELVETICA_18,
             "5. Afia Farzana");

    glColor3f(0,0,0);

    drawText(90,100,
             GLUT_BITMAP_HELVETICA_18,
             "- UI design and project presentation.");

    // ================= BACK BUTTON =================

    glColor3f(0.75f,0.1f,0.1f);

    glBegin(GL_POLYGON);

    glVertex2f(20,20);
    glVertex2f(120,20);
    glVertex2f(120,55);
    glVertex2f(20,55);

    glEnd();

    glColor3f(1,1,1);

    drawText(45,33,
             GLUT_BITMAP_HELVETICA_18,
             "BACK");

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON &&
       state == GLUT_DOWN)
    {
        y = 600 - y;

        // instruction button
        if(currentScreen == 0)
        {
            if(x >= 580 && x <= 760 &&
                  y >= 20  && y <= 55)
            {
                currentScreen = 1;
            }
        }

        // back button
        else if(currentScreen == 1)
        {
            if(x >= 20 && x <= 120 &&
               y >= 20 && y <= 55)
            {
                currentScreen = 0;
            }
        }

        glutPostRedisplay();
    }
}



void display()
{
    if(currentScreen == 1)
{
    instructionScreen();
    return;
}
    glClearColor(0.95f, 0.92f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    // ================= TITLE =================

    glColor3f(0.7f, 0.0f, 0.0f);

    drawText(330, 550, GLUT_BITMAP_TIMES_ROMAN_24,
             "Project Title");

    drawText(240, 520, GLUT_BITMAP_TIMES_ROMAN_24,
             "Roads and Movement in Dhaka");

    drawText(310, 485, GLUT_BITMAP_HELVETICA_18,
             "Computer Graphics [I]");

             // ================= BUTTON =================

glColor3f(0.1f, 0.2f, 0.7f);

glBegin(GL_POLYGON);

glVertex2f(580, 20);
glVertex2f(760, 20);
glVertex2f(760, 55);
glVertex2f(580, 55);

glEnd();

// button text
glColor3f(1,1,1);

drawText(615, 33,
         GLUT_BITMAP_HELVETICA_18,
         "INSTRUCTION");

    // ================= TOP LINE =================

    glColor3f(0, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINES);
    glVertex2f(50, 450);
    glVertex2f(750, 450);
    glEnd();

    // ================= GROUP MEMBER LABEL =================

    glColor3f(0, 0, 0);

    drawText(50, 460, GLUT_BITMAP_TIMES_ROMAN_24,
             "Group Member :");

    // ================= TABLE HEADERS =================

    drawText(70, 420, GLUT_BITMAP_HELVETICA_18, "ID");
    drawText(320, 420, GLUT_BITMAP_HELVETICA_18, "Name");
    drawText(630, 420, GLUT_BITMAP_HELVETICA_18, "Roll");

    // header underlines
    glBegin(GL_LINES);

    glVertex2f(60, 410);
    glVertex2f(160, 410);

    glVertex2f(300, 410);
    glVertex2f(490, 410);

    glVertex2f(620, 410);
    glVertex2f(760, 410);

    glEnd();

    // ================= TABLE DATA =================

    // row 1
    drawText(60, 380, GLUT_BITMAP_HELVETICA_18, "23-51964-2");
    drawText(300, 380, GLUT_BITMAP_HELVETICA_18, "Quazi Saad");
    drawText(690, 380, GLUT_BITMAP_HELVETICA_18, "31");

    // row 2
    drawText(60, 350, GLUT_BITMAP_HELVETICA_18, "23-52013-2");
    drawText(300, 350, GLUT_BITMAP_HELVETICA_18, "Atifur Rahaman Shohan ");
    drawText(690, 350, GLUT_BITMAP_HELVETICA_18, "32");

    // row 3
    drawText(60, 320, GLUT_BITMAP_HELVETICA_18, "23-52759-2");
    drawText(300, 320, GLUT_BITMAP_HELVETICA_18, "Zafrin Alam Tanha");
    drawText(690, 320, GLUT_BITMAP_HELVETICA_18, "35");

    // row 4
    drawText(60, 290, GLUT_BITMAP_HELVETICA_18, "23-50325-1");
    drawText(300, 290, GLUT_BITMAP_HELVETICA_18, "Fariha Tasnim");
    drawText(690, 290, GLUT_BITMAP_HELVETICA_18, "15");

    // row 5
    drawText(60, 260, GLUT_BITMAP_HELVETICA_18, "23-51626-2");
    drawText(300, 260, GLUT_BITMAP_HELVETICA_18, "Afia Farzana");
    drawText(690, 260, GLUT_BITMAP_HELVETICA_18, "30");

    // ================= BOTTOM LINE =================

    glBegin(GL_LINES);
    glVertex2f(50, 220);
    glVertex2f(750, 220);
    glEnd();





    // ================= SUPERVISOR =================

    drawText(330, 190, GLUT_BITMAP_TIMES_ROMAN_24, "Supervised by :");

    drawText(330, 160, GLUT_BITMAP_HELVETICA_18, "Mahfujur Rahman");

    drawText(320, 135, GLUT_BITMAP_HELVETICA_18, "Assistant Professor");

    drawText(260, 110, GLUT_BITMAP_HELVETICA_18,
             "Department of Computer Science");

    drawText(250, 85, GLUT_BITMAP_HELVETICA_18,
             "Faculty of Science and Technology");

    drawText(185, 60, GLUT_BITMAP_HELVETICA_18,
             "American International University-Bangladesh(AIUB)");

    glutSwapBuffers();
}

// ================= RESHAPE =================

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
}

// ================= MAIN =================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800, 600);

    glutCreateWindow("Roads and Movement in Dhaka");

    // callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);

    glutMainLoop();

    return 0;
}
