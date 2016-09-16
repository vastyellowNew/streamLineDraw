#ifndef VECFIELD_H
#define VECFIELD_H

//#include <vector.h>

class vecField
{
    int xSize;
    int ySize;
    int zSize;
    float **vectors;

public:
    vecField();
    vecField(int x, int y, int z);
    float *getVec(int x_, int y_, int z_);
    float getXVec(int x_, int y_, int z_);
    float getYVec(int x_, int y_, int z_);
    float getZVec(int x_, int y_, int z_);
    float getXVec(float x_, float y_, float z_);
    float getYVec(float x_, float y_, float z_);
    float getZVec(float x_, float y_, float z_);
    float *getNVec(int x_, int y_, int z_);
    float *getNVec(float x_, float y_, float z_);
    int getXSize();
    int getYSize();
    int getZSize();
    float getSpeed(int x_, int y_, int z_);

    void setSize(int x, int y, int z);
    void setX(int x_, int y_, int z_, float xVal);
    void setY(int x_, int y_, int z_, float yVal);
    void setZ(int x_, int y_, int z_, float zVal);
    //void setVec(int x_, int y_, int z_, vector vec);
    ~vecField();

    float UYgrad(int x_, int y_, int z_);
    float UZgrad(int x_, int y_, int z_);
    float VXgrad(int x_, int y_, int z_);
    float VZgrad(int x_, int y_, int z_);
    float WXgrad(int x_, int y_, int z_);
    float WYgrad(int x_, int y_, int z_);
    float angV(int x_, int y_, int z_);
    float UYgrad(float x_, float y_, float z_);
    float UZgrad(float x_, float y_, float z_);
    float VXgrad(float x_, float y_, float z_);
    float VZgrad(float x_, float y_, float z_);
    float WXgrad(float x_, float y_, float z_);
    float WYgrad(float x_, float y_, float z_);
    float angV(float x_, float y_, float z_);
};

#endif // VECFIELD_H
