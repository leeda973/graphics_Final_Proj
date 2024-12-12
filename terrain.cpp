// terrain.cpp

#include "terrain.h"
#include "loadbmp.h"

Terrain::Terrain(pcStr heightFile, pcStr surfaceTexFile, GLuint width, GLint height)
{
    _map_width = width;
    _map_height = height; //���� ���� ���� ũ�� ����

    img = NULL;                  //���� ������ ��Ʈ�� �̹��� ������ �ʱ�ȭ �ؽ�ó ID ��
    _texId_ground = 0; _texId_water = 0;
    _height = 0;


    img = LoadBMP(surfaceTexFile);
    glGenTextures(1, &_texId_ground);    //���� �ؽ�ó �̹��� �ε�
    glBindTexture(GL_TEXTURE_2D, _texId_ground); //�ؽ�ó ID�� ���� 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);

    //2���� �ؽ�ó �̹��� ����

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //���ε� ��ü�� ���� ��� �ؽ�ó ���� ���� ���
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //���ε� ��ü�� Ŭ ��� �ؽ�ó�� Ȯ�� ��� ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //�� �ؽ�ó ����
    const char* waterTexFile = "space/water.bmp";
    img = LoadBMP(waterTexFile);   //�� �̹��� �ε�
    glGenTextures(1, &_texId_water);      //�ؽ�ó ID�� ����
    glBindTexture(GL_TEXTURE_2D, _texId_water);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    //���Ϸκ��� ���� ���θ�ŭ�� ���� �����͸� �о� ��.
    _height = readRawData(heightFile, width, height);
    //������ �ּڰ��� ����.
    int i;
    for (i = 0, minHeight = 1024; i < _map_width * height; i++) {
        if (minHeight > _height[i]) { minHeight = _height[i]; }
    }
    //�� ���̴� �ּ� ������ 60 ��ġ

    waterLevel = (GLfloat)minHeight + 100;
    //�ּ� ���̸� �������� ������ �߶�
    for (int i = 0; i < _map_width * _map_height + 1; i++) _height[i] -= minHeight;
    //���� Ÿ�ϸ��ϱ�
    baseTerrain = glGenLists(5);         //4�ܰ� ���� Ÿ��, �� �� 5���� ���÷��� ����Ʈ
    glNewList(baseTerrain, GL_COMPILE);
    TileTerrain(1);      //���� 1 ������ ����
    glEndList();
    glNewList(baseTerrain + 1, GL_COMPILE);
    TileTerrain(4);      //���� 4 ������ ����
    glEndList();
    glNewList(baseTerrain + 2, GL_COMPILE);
    TileTerrain(8);     //���� 8 ������ ����
    glEndList();
    glNewList(baseTerrain + 3, GL_COMPILE);
    TileTerrain(16);    //���� 16 ������ ����
    glEndList();
    glNewList(baseTerrain + 4, GL_COMPILE);
    RenderWater();      //�� �׸���
    glEndList();
}

Terrain:: ~Terrain() {
    //Terrain ��ü�� �Ҹ���.�޸� ��ȯ 1f(Lheight != 0) { deletel] netght; }
    if (_height != 0) { delete[] _height; }
    if (_texId_ground != 0) { glDeleteTextures(1, &_texId_ground); }
    if (_texId_water != 0) { glDeleteTextures(1, &_texId_water); }
    if (img != 0) { free(img->data); free(img); }
}

void Terrain::RenderTerrain(GLfloat x, GLfloat y) {
    int i, j;
    //Ÿ�ϸ��� ���� ���÷��� ����Ʈ�� �ҷ����� �Լ� 
    //���� 1 ������ ����.ī�޶� ��ġ�� ���� �� ��ϸ� �׸�
    glPushMatrix();
    glTranslatef(getTileX(x) * (_map_width - 1), 0.0, getTileY(y) * (_map_height - 1));
    glCallList(baseTerrain);
    glPopMatrix();
    //���� 4 ������ ����
    //���� 1 ����� �����ϰ� 3x3 ������ �׸�.
    for (i = -1; i < 2; i++) {
        for (j = -1; j < 2; j++) {
            if (!(i == 0 && j == 0)) {
                glPushMatrix();
                glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
                glCallList(baseTerrain + 1);
                glPopMatrix();
            }
        }
    }
    //���� 8 ������ ����
    //���� 4 ����� �����ϰ� 5x5 ������ �׸�.
    for (i = -2; i < 3; i++) {
        for (j = -2; j < 3; j++) {
            if ((i < -1 || i>1) || (j < -1 || j>1)) {
                glPushMatrix();
                glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
                glCallList(baseTerrain + 2);
                glPopMatrix();
            }
        }
    }
    //���� 16 ������ ����
    //���� 8����� �����ϰ� 7x7 ������ �׸�.
    for (i = -3; i < 4; i++) {
        for (j = -3; j < 4; j++) {
            if ((i < -2 || i>2) || (j < -2 || j>2)) {
                glPushMatrix();
                glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
                glCallList(baseTerrain + 3);
                glPopMatrix();
            }
        }
    }
    //�� ���÷��� ����Ʈ. 3X3 ������ �׸�.
    for (i = -2; i < 3; i++) {
        for (j = -2; j < 3; j++) {
            glPushMatrix();
            glTranslatef(getTileX(x) * (_map_width - 1) + (i * (_map_width - 1)), 0.0, getTileY(y) * (_map_height - 1) + (j * (_map_height - 1)));
            glCallList(baseTerrain + 4);
            glPopMatrix();
        }
    }
}

void Terrain::TileTerrain(GLint levelOfDetail) {  //LOD �Ķ���͸� �޾� �� Ÿ���� �׸��� �ռ�
    glEnable(GL_TEXTURE_2D);      //�ؽ�ó Ȱ��ȭ
    glMatrixMode(GL_MODELVIEW);     //�� �� ���
    glPushMatrix();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);   //�ؽ�ó ȯ�� ����
    glBindTexture(GL_TEXTURE_2D, _texId_ground);
    //���� �ؽ�ó ID�� ����Ű�� �ؽ�ó�� �Ʒ� ���� ����
    float coordX = (float)1.0 / _map_width;       //���� �̵� ����
    float coordZ = (float)1.0 / _map_height;   //���� �̵� ���� 
    for (int z = 0; z < _map_height; z += levelOfDetail) {   //x, 2���� �������� �ٴڿ� �����Ϸ���
        glBegin(GL_TRIANGLE_STRIP); //���� �ﰢ�� �׸���� �ؽ�ó ���� 
        for (int x = 0; x < _map_width; x += levelOfDetail) {
            glTexCoord2f(coordX * x, coordZ * z);   //�ؽ�ó ������
            glVertex3f(x, _height[z * _map_height + x], z);   //��ü ���� ���� 
            glTexCoord2f(coordX * x, coordZ * (z + levelOfDetail));  //�ؽ�ó ������
            glVertex3f(x, _height[(z + levelOfDetail) * _map_height + x], z + levelOfDetail); //��ü ���� ����
        }
        glEnd();
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void Terrain::RenderWater() {   //�� �׸���
    glEnable(GL_TEXTURE_2D);   //�ؽ�ó Ȱ��ȭ
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   //�ؽ�ó ȯ�� ����


    glEnable(GL_BLEND);         //���� Ȱ��ȭ
    glDisable(GL_CULL_FACE);   //�ĸ� ���� ��Ȱ��ȭ
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   //���� ��� ����
    glColor4f(0.0, 0.4, 0.5, 0.7);

    glBindTexture(GL_TEXTURE_2D, _texId_water);   //�ؽ�ó ����
    glBegin(GL_QUADS);      //�簢�� ������ �׸��� 
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, waterLevel, 0.0);
    //���� ������ �����ϰ� ���� ����
    glTexCoord2f(15.0, 0.0); glVertex3f(0.0, waterLevel, _map_height);
    glTexCoord2f(15.0, 15.0); glVertex3f(_map_height, waterLevel, _map_height);
    glTexCoord2f(0.0, 15.0); glVertex3f(_map_height, waterLevel, 0.0);
    glEnd();
    glEnable(GL_CULL_FACE);      //�ĸ� ���� Ȱ��ȭ
    glDisable(GL_BLEND);      //���� ��Ȱ��ȭ
    glDisable(GL_TEXTURE_2D);
}
//.raM ������ �д� �Լ�.fiLename : ���� �� ������ ���.M; ���� ���� ���� �ڱ�.h: ���� ���� ���� ũ�� ////
Terrain::ubyte* Terrain::readRawData(pcStr filename, uint w, uint h) {
    FILE* f;
    fopen_s(&f, filename, "rb"); //���� ���� ������ raw ���� ����
    if (!f) {   //������ ���� ���
        printf("file not found : %s\n", filename);
        throw std::exception();
    }
    //raw ������ ����� ��� ������ ũ�� = �̹����� �ȼ� ���̱� ������ width height ��ŭ�� ������ �Ҵ��ϸ� ��� �����͸� �о�� �� ����.
    ubyte* raw = new ubyte[w * h]; //raw ũ�⸦ ��� 
    fread(raw, sizeof(ubyte), w * h, f); //���� ������ �о����
    fclose(f);

    //// �����: ���� ������ ���
    printf("Height Data Loaded:\n");
    for (uint i = 0; i < w * h; i++) {
        printf("raw: %d %d\n", i, raw[i]);
        if (i % w == (w - 1)) {  // �� ���� �����͸� ����� �� �ٹٲ�
            printf("\n");
        }
    }
    return raw;
}
GLfloat Terrain::getHeight(GLfloat _x, GLfloat _y) {
    //�ؽ�ó �̹����� �ش�_X, ���� ���� �����͸� �������� �Լ� 
    int x, y;
    if (_x < 0) x = _map_width + ((int)_x % _map_width);   //�ؽ�ó �̹��� ������ �Ѿ�� ���� �ؽ� ó�� �ݺ�
    else x = ((int)_x % _map_width);
    if (_y < 0) y = _map_height + ((int)_y % _map_height);
    else y = ((int)_y % _map_height);
    //��ǥ ���� �ؽ�ó ũ�� �ȿ� ������Ŵ.
    //���� �����ʹ� 1���� �迭�� ����Ǿ� �����Ƿ� ��� ó��
    return (_height[y * _map_height + x] / 5 + 10); //�ش� ��ġ�� ���� ������
}

GLint Terrain::getTileX(GLfloat x) { //���� ī�޶� ��ġ�� �ؽ�ó Ÿ���� ��� ��ǥ�� ���� : ��ǥ
    if (x > 0) return ((int)x) / (int)_map_width;
    else return ((int)x) / (int)_map_width - 1;
}

GLint Terrain::getTileY(GLfloat y) { //���� ī�޶� ��ġ�� �ؽ�ó Ÿ���� ��� ��ǥ�� ���� : y��ǥ
    if (y > 0) return((int)y) / (int)_map_height;
    else return ((int)y) / (int)_map_height - 1;
}