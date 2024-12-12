//vector3.h

#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3 {
public:
    float x, y, z;

    // �⺻ ������
    Vector3() : x(0), y(0), z(0) {}

    // �Ű����� ������
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // ���� ����
    void set(float nx, float ny, float nz) {
        x = nx; y = ny; z = nz;
    }

    // ���� ����
    Vector3 cross(const Vector3& b) const {
        return Vector3(
            y * b.z - z * b.y,
            z * b.x - x * b.z,
            x * b.y - y * b.x

        );
    }

    // ���� ����
    float dot(const Vector3& b) const {
        return x * b.x + y * b.y + z * b.z;
    }

    // ���� ����ȭ
    void normalize() {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length > 0.0f) {
            x /= length;
            y /= length;
            z /= length;
        }
        else {
            std::cerr << "Warning: Attempting to normalize a zero-length vector.\n";
        }
    }
};

#endif // VECTOR3_H