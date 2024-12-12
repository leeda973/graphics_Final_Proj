// main.cpp

#include <stdio.h>
#include <math.h>
#include "terrain.h"
#include "Skybox.h"
#include "loadbmp.h"
//#include "inc/GL/glaux.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>





#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "glaux.lib")

// ��ü ����
Terrain* terrain; // ����
Skybox* skybox;   // ��ī�̹ڽ�
Camera cam;       // ī�޶�

bool keyPressed[256]; // Ű���� �Է� ��Ȳ�� ��Ÿ���� �迭

void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }

void dispose();           // ���� �� ����Ǵ� �Լ�
void Display();           // ���÷��� �ݹ�
void Idle();              // ���̵� �ݹ�
void Reshape(GLint w, GLint h); // �������� �ݹ�
void fog();               // �Ȱ� ȿ�� �Լ�

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
    skybox->draw();
    glPopMatrix();
    glScalef(1.0f, 0.2f, 1.0f);

    glPushMatrix();
    terrain->RenderTerrain(cam.eye.x, cam.eye.z); // ������ �׸�
    glPopMatrix();

    fog(); // ���� �Ʒ� �Ȱ� ȿ��
    glFlush();
    glutSwapBuffers(); // ���� ���۸�
}

void Idle() {
    if (keyPressed['1']) glPolygonMode(GL_FRONT, GL_LINE);
    if (keyPressed['2']) glPolygonMode(GL_FRONT, GL_FILL);

    if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
    if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
    if (keyPressed['s']) { cam.slide(0, 0, 0.1); }
    if (keyPressed['w']) { cam.slide(0, 0, -0.1); }
    if (cam.eye.y < terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }

    if (keyPressed['k']) { cam.pitch(-0.5); }
    if (keyPressed['i']) { cam.pitch(0.5); }
    if (keyPressed['q']) { cam.yaw(-0.5); }
    if (keyPressed['e']) { cam.yaw(0.5); }
    if (keyPressed['j']) { cam.roll(0.5); }
    if (keyPressed['l']) { cam.roll(-0.5); }
    else { cam.slide(0, 0, 0); }

    glutPostRedisplay();
}

void Reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h); // �� ��Ʈ ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, static_cast<GLdouble>(w / h), 1.0, 3000.0); // ���� ���� ����
}

void dispose() { // ���α׷� ���� �� ��ü ��ȯ
    delete terrain;
    delete skybox;
    //delete &cam;
}

void fog() {
    //Ȱ��ȭ
   //GL_FOG 
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    GLfloat waterFogColor[4] = { 0.0, 0.6, 0.6, 5.0 };
    GLfloat fogColor[4] = { 0.7, 0.7, 0.7, 0.0 };

    if (cam.eye.y < (terrain->waterLevel - 75)) {
        glFogfv(GL_FOG_COLOR, waterFogColor);
        glFogf(GL_FOG_DENSITY, 0.075);
    }
    else {
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.002);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(400, 30);
    glutCreateWindow("CProject_final");
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutKeyboardFunc(myKeyboard);
    glutKeyboardUpFunc(myKeyboardUp);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    terrain = new Terrain("space/terrain1.raw", "space/snow512.bmp", 257, 257);
    skybox = new Skybox();

    cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0); // �� ��Ʈ�� ī�޶� ����
    cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);
    cam.slide(0, 100, 0);
    cam.roll(0);
    cam.yaw(180);
    cam.pitch(45);
    atexit(dispose);

    printf("W: forward, S: backward, A: Left, D: Right\n");
    printf("JL: Roll, IK: Pitch, QE: Yaw, 1/2: Solid/Wireframe\n");
    glutMainLoop();
    return 0;
}

