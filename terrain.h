// terrain.h


#include "loadbmp.h"

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <exception>
#include <math.h>
// #include "iglaux.h"
#include "camera.h"


class Terrain {
private:
    typedef const char* pcStr;
    typedef unsigned int uint;
    typedef unsigned char ubyte;

    uint _texId_ground, _texId_water; // 텍스처 ID
    AUX_RGBImageRec* img;
    ubyte* _height;
    GLint _map_width, _map_height;
    GLuint baseTerrain;
    GLubyte minHeight;
    static ubyte* readRawData(pcStr filename, uint width, uint height); // RAW 데이터 읽기


public:
    Terrain(pcStr heightmap, pcStr surfaceTex, GLuint width, GLint height);
    ~Terrain(void);

    void RenderTerrain(GLfloat, GLfloat);
    void TileTerrain(GLint);
    void RenderWater(void);
    GLfloat getHeight(GLfloat, GLfloat);
    GLint getTileX(GLfloat);
    GLint getTileY(GLfloat);
    GLfloat waterLevel;
};

