// camera.h
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <windows.h>
#include <strstream>
#include <math.h>
//#include "glut.h"


class Point3 {  // ���� Ŭ����
public:
    float x, y, z; // ��ǥ
    void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; } // �� ��ǥ ���� �޼���
    void set(Point3& p) { x = p.x; y = p.y; z = p.z; }  // �ٸ� ���� ��ǥ�� ���� �޼���
    Point3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; } // ������: �� ��ǥ �ʱ�ȭ
    Point3() { x = y = z = 0; } // ������: �ʱ�ȭ ���� �� ����
};

class Vector3 {
public:
    float x, y, z; // ���� ����
    void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; } // ���� ���� �޼���
    void set(const Vector3& v) { x = v.x; y = v.y; z = v.z; } // ���� ���� �޼���
    void flip() { x = -x; y = -y; z = -z; } // �ݴ� ���� ����
    void setDiff(Point3& a, Point3& b) { x = a.x - b.x; y = a.y - b.y; z = a.z - b.z; } // �� ������ ��
    void normalize(); // ���� ����ȭ
    Vector3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; } // ������
    Vector3(const Vector3& v) { x = v.x; y = v.y; z = v.z; } // ���� ������
    Vector3() { x = y = z = 0.0; } // �⺻ ������
    Vector3 cross(const Vector3 b); // ����
    float dot(const Vector3 b); // ����
};

class Camera {
public:
    Point3 eye; // ���� ��ǥ ����
    Vector3 u, v, n; // ���� ��ǥ�踦 �����ϴ� ���� ����(Basis Vector)
    float aspect, nearDist, farDist, viewAngle; // gluPerspective�� �Ķ���͵�

    void setModelViewMatrix(); // ���� ��ȯ�� ���� �� �� ��� ����
    Camera(void); // ������

    // ���� �ùķ��̼��� ȸ��/��ȯ �Լ���
    void roll(float angle);
    void pitch(float angle);
    void yaw(float angle);

    void slide(double du, double dv, double dn);

    // ī�޶� ��ġ, ����, ī�޶� �� ���� ����
    void set(Point3 Eye, Point3 look, Vector3 up);
    void set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ);
    void setShape(float viewAngle, float aspect, float near, float far);
    void setAspect(float asp); // ��Ⱦ�� ����
    void getShape(float& vAngle, float& aspect, float& near, float& far); // ���� ���� ����
    void rotAxes(Vector3& a, Vector3& b, float angle); // ���� ��ǥ�� �� ȸ��
    void setDefaultCamera(); // ī�޶� �ʱ� ���� �Լ�
};