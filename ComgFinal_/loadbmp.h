// loadbmp.h

#pragma once

#ifndef LOADBMP_H
#define LOADBMP_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

typedef struct _AUX_RGBImageRec {
    GLint sizeX, sizeY;
    unsigned char* data;
} AUX_RGBImageRec;

AUX_RGBImageRec* LoadBMP(const char* filename);

// BMP ������ �ؽ�ó�� ��ȯ�ϴ� �Լ�
int LoadGLTextures(const char* filePath, unsigned int& textureID);


#endif

