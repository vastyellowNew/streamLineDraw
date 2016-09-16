#include "scalarfield.h"
#include <stdlib.h>
#include <math.h>

scalarField::scalarField()
{

}

scalarField::scalarField(int x, int y, int z)
{
    xSize = x;
    ySize = y;
    zSize = z;

    //use malloc stdlib.h
    scalar=(float *) malloc(x*y*z*sizeof(float));

}

void scalarField::setSize(int x, int y, int z)
{
    xSize = x;
    ySize = y;
    zSize = z;

    //use malloc stdlib.h
    scalar=(float *) malloc(x*y*z*sizeof(float));
}

void scalarField::setValue(int x_, int y_, int z_, float val)
{
    scalar[z_*xSize*ySize + y_*xSize +x_] = val;
}

float scalarField::getValue(int x_, int y_, int z_)
{
    return scalar[z_*xSize*ySize + y_*xSize +x_];
}

float scalarField::getValue(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;

    //
    //		   n8____________n7
    //		   /|            /|
    //		  / |           / |
    //		 /  |          /  |
    //    n5/___________n6/  _|
    //		|   /n4       |   /n3
    //		|  /          |  /
    //		| /           | /
    //		|/____________|/
    //      n1            n2
    //
    //		+x = n1 to n2
    //		+y = n1 to n4
    //		+z = n1 to n5
    //

    float Cel1Val = scalarField::getValue(xCell, yCell, zCell);

    float Cel2Val = scalarField::getValue(xCell+1, yCell, zCell);

    float Cel3Val = scalarField::getValue(xCell+1, yCell+1, zCell);

    float Cel4Val = scalarField::getValue(xCell, yCell+1, zCell);

    float Cel5Val = scalarField::getValue(xCell, yCell, zCell+1);

    float Cel6Val = scalarField::getValue(xCell+1, yCell, zCell+1);

    float Cel7Val = scalarField::getValue(xCell+1, yCell+1, zCell+1);

    float Cel8Val = scalarField::getValue(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float Edg1Val = Cel1Val+(Cel2Val-Cel1Val)*xLoc;

    float Edg2Val = Cel4Val+(Cel3Val-Cel4Val)*xLoc;

    float Edg3Val = Cel5Val+(Cel6Val-Cel5Val)*xLoc;

    float Edg4Val = Cel8Val+(Cel7Val-Cel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float Fac1 = Edg1Val+(Edg2Val-Edg1Val)*yLoc;

    float Fac2 = Edg3Val+(Edg4Val-Edg3Val)*yLoc;

    //
    //

    float Val = Fac1+(Fac2-Fac1)*zLoc;

    return Val;
}

int scalarField::getXSize()
{
    return xSize;
}

int scalarField::getYSize()
{
    return ySize;
}

int scalarField::getZSize()
{
    return zSize;
}
