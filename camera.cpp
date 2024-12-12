// camera.cpp
#include "camera.h"


#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
// glaux를 사용하지 않고 텍스쳐 비트맵을 로드하기 위해 AUX_RGBImageRec 구조체를 정의하고 LoadBMP도 재작성함
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std; // std::cerr 사용을 편리하게 만듭니다.

Camera::Camera(void) {
    setDefaultCamera();  // 생성자, 초기화 함수를 별도로 호출
}


void Camera::setDefaultCamera(void) {
    setShape(45.0f, 640 / (float)480, 0.1f, 200.0f);  // 화면 구성 값 정의 fov, aspect, near, far
    Point3 eyePoint = Point3(10.0, 0.0, 0.0);  // 카메라 위치 지정
    Point3 lookPoint = Point3(0.0, 0.0, 0.0);
    Vector3 upVector = Vector3(0, 1, 0);
    set(eyePoint, lookPoint, upVector);
}

void Camera::set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ) {
    Point3 Eye = Point3(eyeX, eyeY, eyeZ);
    Point3 look = Point3(lookX, lookY, lookZ);
    Vector3 up = Vector3(upX, upY, upZ);

    eye.set(Eye);
    n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);

    u.set(up.cross(n));
    v.set(n.cross(u));
    u.normalize(); v.normalize(); n.normalize();

    setModelViewMatrix();
}

void Camera::set(Point3 Eye, Point3 look, Vector3 up) {
    eye.set(Eye);
    n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);

    u.set(up.cross(n));
    v.set(n.cross(u));

    u.normalize();
    v.normalize();
    n.normalize(); // 중복 정규화 호출 제거

    setModelViewMatrix();
}

void Camera::setModelViewMatrix(void) {
    float m[16];
    Vector3 eVec(eye.x, eye.y, eye.z);

    m[0] = u.x;  m[4] = u.y;  m[8] = u.z;  m[12] = -eVec.dot(u);
    m[1] = v.x;  m[5] = v.y;  m[9] = v.z;  m[13] = -eVec.dot(v);
    m[2] = n.x;  m[6] = n.y;  m[10] = n.z; m[14] = -eVec.dot(n);
    m[3] = 0;    m[7] = 0;    m[11] = 0;   m[15] = 1.0;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m);
}

void Camera::setShape(float vAngle, float asp, float nr, float fr) {
    viewAngle = vAngle;
    aspect = asp;
    nearDist = nr;
    farDist = fr;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, aspect, nearDist, farDist);
    glMatrixMode(GL_MODELVIEW);
}

void Camera::setAspect(float asp) {
    aspect = asp;
}

void Camera::getShape(float& vAngle, float& asp, float& nr, float& fr) {
    vAngle = viewAngle;
    asp = aspect;
    nr = nearDist;
    fr = farDist; // 원근 투영 값 반환
}

void Camera::slide(double du, double dv, double dn) {
    eye.x += du * u.x + dv * v.x + dn * n.x;
    eye.y += du * u.y + dv * v.y + dn * n.y;
    eye.z += du * u.z + dv * v.z + dn * n.z;
    setModelViewMatrix();
}

void Camera::rotAxes(Vector3& a, Vector3& b, float angle) {
    float ang = 3.14159265 / 180 * angle; // 각도를 라디안으로 변환
    float C = cos(ang), S = sin(ang);
    Vector3 t((C * a.x + S * b.x), (C * a.y + S * b.y), (C * a.z + S * b.z));
    b.set((-S * a.x + C * b.x), (-S * a.y + C * b.y), (-S * a.z + C * b.z));
    a.set(t.x, t.y, t.z);
}

void Camera::roll(float angle) {
    rotAxes(u, v, angle);
    setModelViewMatrix();
}

void Camera::pitch(float angle) {
    rotAxes(n, v, angle);
    setModelViewMatrix();
}

void Camera::yaw(float angle) {
    rotAxes(u, n, angle);
    setModelViewMatrix();
}

Vector3 Vector3::cross(Vector3 b) {
    Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
    return c;
}

float Vector3::dot(Vector3 b) {
    return x * b.x + y * b.y + z * b.z;
}

void Vector3::normalize() {
    double sizeSq = x * x + y * y + z * z;
    if (sizeSq < 0.000001) {
        cerr << "\nnormalize() sees vector (0, 0, 0)!";
        return;
    }
    float scaleFactor = 1.0 / (float)sqrt(sizeSq);
    x *= scaleFactor;
    y *= scaleFactor;
    z *= scaleFactor;
}