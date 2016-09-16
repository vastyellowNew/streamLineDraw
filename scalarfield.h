#ifndef SCALARFIELD_H
#define SCALARFIELD_H


class scalarField
{
    int xSize;
    int ySize;
    int zSize;
    float *scalar;

public:
    scalarField();
    scalarField(int x, int y, int z);

    void setSize(int x, int y, int z);
    void setValue(int x_, int y_, int z_, float val);

    float getValue(int x_, int y_, int z_);
    float getValue(float x_, float y_, float z_);
    int getXSize();
    int getYSize();
    int getZSize();
};

#endif // SCALARFIELD_H
