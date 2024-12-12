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

// 객체 선언
Terrain* terrain; // 지형
Skybox* skybox;   // 스카이박스
Camera cam;       // 카메라

bool keyPressed[256]; // 키보드 입력 상황을 나타내는 배열

void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }

void dispose();           // 종료 시 수행되는 함수
void Display();           // 디스플레이 콜백
void Idle();              // 아이들 콜백
void Reshape(GLint w, GLint h); // 리셰이프 콜백
void fog();               // 안개 효과 함수

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
    skybox->draw();
    glPopMatrix();
    glScalef(1.0f, 0.2f, 1.0f);

    glPushMatrix();
    terrain->RenderTerrain(cam.eye.x, cam.eye.z); // 지형을 그림
    glPopMatrix();

    fog(); // 수면 아래 안개 효과
    glFlush();
    glutSwapBuffers(); // 더블 버퍼링
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
    glViewport(0, 0, w, h); // 뷰 포트 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, static_cast<GLdouble>(w / h), 1.0, 3000.0); // 원근 투상 설정
}

void dispose() { // 프로그램 종료 시 객체 반환
    delete terrain;
    delete skybox;
    //delete &cam;
}

void fog() {
    //활성화
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

    cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0); // 뷰 포트와 카메라 설정
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

