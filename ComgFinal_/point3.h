// point3.h

#pragma once
#ifndef POINT3_H
#define POINT3_H

class Point3 {
public:
    float x, y, z;

    // �⺻ ������
    Point3() : x(0), y(0), z(0) {}

    // �Ű����� ������
    Point3(float x, float y, float z) : x(x), y(y), z(z) {}

    // ����Ʈ ����
    void set(const Point3& p) {
        x = p.x; y = p.y; z = p.z;
    }
};

#endif // POINT3_H

