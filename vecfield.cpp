#include "vecfield.h"
#include <stdlib.h>
#include <math.h>

vecField::vecField()
{

}

vecField::vecField(int x, int y, int z)
{
    xSize = x;
    ySize = y;
    zSize = z;

    //use malloc stdlib.h
    vectors=(float **) malloc(x*y*z*sizeof(float *));

    for(int i=0;i<x*y*z;i++)
        vectors[i]=(float *) malloc(3*sizeof(float));
}

void vecField::setSize(int x, int y, int z)
{
    xSize = x;
    ySize = y;
    zSize = z;

    //use malloc stdlib.h
    vectors=(float **) malloc(x*y*z*sizeof(float *));

    for(int i=0;i<x*y*z;i++)
        vectors[i]=(float *) malloc(3*sizeof(float));
}

void vecField::setX(int x_, int y_, int z_, float xVal)
{
    vectors[z_*xSize*ySize + y_*xSize +x_][0] = xVal;
}

void vecField::setY(int x_, int y_, int z_, float yVal)
{
    vectors[z_*xSize*ySize + y_*xSize +x_][1] = yVal;
}

void vecField::setZ(int x_, int y_, int z_, float zVal)
{
    vectors[z_*xSize*ySize + y_*xSize +x_][2] = zVal;
}

/*
void vecField::setVec(int x_, int y_, int z_, float)
{
    vectors[z_*xSize*ySize + y_*xSize +x_] = vec;
}
*/
int vecField::getXSize()
{
    return xSize;
}

int vecField::getYSize()
{
    return ySize;
}

int vecField::getZSize()
{
    return zSize;
}

float vecField::getSpeed(int x_, int y_, int z_)
{
    float xVec = vectors[z_*xSize*ySize + y_*xSize +x_][0];
    float yVec = vectors[z_*xSize*ySize + y_*xSize +x_][1];
    float zVec = vectors[z_*xSize*ySize + y_*xSize +x_][2];

    return sqrt(xVec*xVec + yVec*yVec + zVec*zVec);
}

float* vecField::getVec(int x_, int y_, int z_)
{
    return vectors[z_*xSize*ySize + y_*xSize +x_];
}

float* vecField::getNVec(int x_, int y_, int z_)
{
    float *vec = vectors[z_*xSize*ySize + y_*xSize +x_];
    //normalise vector
    float mag = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
    float normVec[3] = {normVec[0] = vec[0]/mag,
                        normVec[1] = vec[1]/mag,
                        normVec[2] = vec[2]/mag};
    float* returnV;
    returnV = normVec;
    return returnV;
}

float vecField::getXVec(int x_, int y_, int z_)
{
    return vectors[z_*xSize*ySize + y_*xSize +x_][0];
}

float vecField::getYVec(int x_, int y_, int z_)
{
    return vectors[z_*xSize*ySize + y_*xSize +x_][1];
}

float vecField::getZVec(int x_, int y_, int z_)
{
    return vectors[z_*xSize*ySize + y_*xSize +x_][2];
}


float* vecField::getNVec(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
            return vecField::getVec(xCell, yCell, zCell);

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

    float *Cel1Val = vecField::getVec(xCell, yCell, zCell);

    float *Cel2Val = vecField::getVec(xCell+1, yCell, zCell);

    float *Cel3Val = vecField::getVec(xCell+1, yCell+1, zCell);

    float *Cel4Val = vecField::getVec(xCell, yCell+1, zCell);

    float *Cel5Val = vecField::getVec(xCell, yCell, zCell+1);

    float *Cel6Val = vecField::getVec(xCell+1, yCell, zCell+1);

    float *Cel7Val = vecField::getVec(xCell+1, yCell+1, zCell+1);

    float *Cel8Val = vecField::getVec(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float Edg1Val[3] = {(Cel2Val[0]-Cel1Val[0])*xLoc,
                        (Cel2Val[1]-Cel1Val[1])*xLoc,
                        (Cel2Val[2]-Cel1Val[2])*xLoc};

    float Edg2Val[3] = {(Cel3Val[0]-Cel4Val[0])*xLoc,
                        (Cel3Val[1]-Cel4Val[1])*xLoc,
                        (Cel3Val[2]-Cel4Val[2])*xLoc};

    float Edg3Val[3] = {(Cel6Val[0]-Cel5Val[0])*xLoc,
                        (Cel6Val[1]-Cel5Val[1])*xLoc,
                        (Cel6Val[2]-Cel5Val[2])*xLoc};

    float Edg4Val[3] = {(Cel7Val[0]-Cel8Val[0])*xLoc,
                        (Cel7Val[1]-Cel8Val[1])*xLoc,
                        (Cel7Val[2]-Cel8Val[2])*xLoc};

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float Fac1[3] = {(Edg2Val[0]-Edg1Val[0])*yLoc,
                     (Edg2Val[1]-Edg1Val[1])*yLoc,
                     (Edg2Val[2]-Edg1Val[2])*yLoc};

    float Fac2[3] = {(Edg4Val[0]-Edg3Val[0])*yLoc,
                     (Edg4Val[1]-Edg3Val[1])*yLoc,
                     (Edg4Val[2]-Edg3Val[2])*yLoc};

    //
    //

    float vec[3] = {(Fac2[0]-Fac1[0])*zLoc,
                (Fac2[1]-Fac1[1])*zLoc,
                (Fac2[2]-Fac1[2])*zLoc};

    //normalise vector
    float mag = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
    float normVec[3] = {normVec[0] = vec[0]/mag,
                        normVec[1] = vec[1]/mag,
                        normVec[2] = vec[2]/mag};
    float* returnV;
    returnV = normVec;
    return returnV;

}


float vecField::getXVec(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
        return vecField::getXVec(xCell, yCell, zCell);
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

    float xCel1Val = vecField::getXVec(xCell, yCell, zCell);

    float xCel2Val = vecField::getXVec(xCell+1, yCell, zCell);

    float xCel3Val = vecField::getXVec(xCell+1, yCell+1, zCell);

    float xCel4Val = vecField::getXVec(xCell, yCell+1, zCell);

    float xCel5Val = vecField::getXVec(xCell, yCell, zCell+1);

    float xCel6Val = vecField::getXVec(xCell+1, yCell, zCell+1);

    float xCel7Val = vecField::getXVec(xCell+1, yCell+1, zCell+1);

    float xCel8Val = vecField::getXVec(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float xEdg1Val = xCel1Val+(xCel2Val-xCel1Val)*xLoc;

    float xEdg2Val = xCel4Val+(xCel3Val-xCel4Val)*xLoc;

    float xEdg3Val = xCel5Val+(xCel6Val-xCel5Val)*xLoc;

    float xEdg4Val = xCel8Val+(xCel7Val-xCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float xFac1 = xEdg1Val+(xEdg2Val-xEdg1Val)*yLoc;

    float xFac2 = xEdg3Val+(xEdg4Val-xEdg3Val)*yLoc;

    //
    //

    float xVal = xFac1+(xFac2-xFac1)*zLoc;

    return xVal;
}

float vecField::getYVec(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
            return vecField::getYVec(xCell, yCell, zCell);

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

    float yCel1Val = vecField::getYVec(xCell, yCell, zCell);

    float yCel2Val = vecField::getYVec(xCell+1, yCell, zCell);

    float yCel3Val = vecField::getYVec(xCell+1, yCell+1, zCell);

    float yCel4Val = vecField::getYVec(xCell, yCell+1, zCell);

    float yCel5Val = vecField::getYVec(xCell, yCell, zCell+1);

    float yCel6Val = vecField::getYVec(xCell+1, yCell, zCell+1);

    float yCel7Val = vecField::getYVec(xCell+1, yCell+1, zCell)+1;

    float yCel8Val = vecField::getYVec(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float yEdg1Val = yCel1Val+(yCel2Val-yCel1Val)*xLoc;

    float yEdg2Val = yCel4Val+(yCel3Val-yCel4Val)*xLoc;

    float yEdg3Val = yCel5Val+(yCel6Val-yCel5Val)*xLoc;

    float yEdg4Val = yCel8Val+(yCel7Val-yCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float yFac1 = yEdg1Val+(yEdg2Val-yEdg1Val)*yLoc;

    float yFac2 = yEdg3Val+(yEdg4Val-yEdg3Val)*yLoc;

    //
    //

    float yVal = yFac1+(yFac2-yFac1)*zLoc;

    return yVal;

}

float vecField::getZVec(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
            return vecField::getZVec(xCell, yCell, zCell);

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

    float zCel1Val = vecField::getZVec(xCell, yCell, zCell);

    float zCel2Val = vecField::getZVec(xCell+1, yCell, zCell);

    float zCel3Val = vecField::getZVec(xCell+1, yCell+1, zCell);

    float zCel4Val = vecField::getZVec(xCell, yCell+1, zCell);

    float zCel5Val = vecField::getZVec(xCell, yCell, zCell+1);

    float zCel6Val = vecField::getZVec(xCell+1, yCell, zCell+1);

    float zCel7Val = vecField::getZVec(xCell+1, yCell+1, zCell+1);

    float zCel8Val = vecField::getZVec(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float zEdg1Val = zCel1Val+(zCel2Val-zCel1Val)*xLoc;

    float zEdg2Val = zCel4Val+(zCel3Val-zCel4Val)*xLoc;

    float zEdg3Val = zCel5Val+(zCel6Val-zCel5Val)*xLoc;

    float zEdg4Val = zCel8Val+(zCel7Val-zCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float zFac1 = zEdg1Val+(zEdg2Val-zEdg1Val)*yLoc;

    float zFac2 = zEdg3Val+(zEdg4Val-zEdg3Val)*yLoc;

    //
    //

    float zVal = zFac1+(zFac2-zFac1)*zLoc;

    return zVal;

}

//X velocity gradient in y direction
float vecField::UYgrad(int x_, int y_, int z_)
{
    float uYGrad;
    //if at start edge of vector field, take forward difference
    if (y_ == 0)
    {
        float a1 = vecField::getXVec(x_,y_, z_);
        float a2 = vecField::getXVec(x_,y_+1, z_);
        uYGrad = a2-a1;
    }
    //if at end of vector field, take bakward difference
    else if (y_ == ySize -1)
    {
        float a1 = vecField::getXVec(x_,y_-1, z_);
        float a2 = vecField::getXVec(x_, y_, z_);
        uYGrad = a2-a1;
    }
    //middle of vector field, take central difference
    else
    {
        float a1 = vecField::getXVec(x_,y_-1, z_);
        float a2 = vecField::getXVec(x_, y_+1, z_);
        uYGrad = (a2-a1)/2;
    }

    return uYGrad;
}

//X velocity gradient in z direction
float vecField::UZgrad(int x_, int y_, int z_)
{
    float uZGrad;
    //if at start edge of vector field, take forward difference
    if (z_ == 0)
    {
        float a1 = vecField::getXVec(x_,y_, z_);
        float a2 = vecField::getXVec(x_,y_, z_+1);
        uZGrad = a2-a1;
    }
    //if at end of vector field, take bakward difference
    else if (z_ == zSize -1)
    {
        float a1 = vecField::getXVec(x_,y_, z_-1);
        float a2 = vecField::getXVec(x_, y_, z_);
        uZGrad = a2-a1;
    }
    //middle of vector field, take central difference
    else
    {
        float a1 = vecField::getXVec(x_,y_, z_-1);
        float a2 = vecField::getXVec(x_, y_, z_+1);
        uZGrad = (a2-a1)/2;
    }

    return uZGrad;
}

//Y velocity gradient in x direction
float vecField::VXgrad(int x_, int y_, int z_)
{
    float vXGrad;
    //if at start edge of vector field, take forward difference
    if (x_ == 0)
    {
        float a1 = vecField::getYVec(x_,y_, z_);
        float a2 = vecField::getYVec(x_+1,y_, z_);
        vXGrad = a2-a1;
    }
    //if at end of vector field, take bakward difference
    else if (x_ == xSize -1)
    {
        float a1 = vecField::getYVec(x_-1,y_, z_);
        float a2 = vecField::getYVec(x_, y_, z_);
        vXGrad = a2-a1;
    }
    //middle of vector field, take central difference
    else
    {
        float a1 = vecField::getYVec(x_-1,y_, z_);
        float a2 = vecField::getYVec(x_+1, y_, z_);
        vXGrad = (a2-a1)/2;
    }

    return vXGrad;
}

//Y velocity gradient in z direction
float vecField::VZgrad(int x_, int y_, int z_)
{
    float vZGrad;
    //if at start edge of vector field, take forward difference
    if (z_ == 0)
    {
        float a1 = vecField::getYVec(x_,y_, z_);
        float a2 = vecField::getYVec(x_,y_, z_+1);
        vZGrad = a2-a1;
    }
    //if at end of vector field, take bakward difference
    else if (z_ == zSize -1)
    {
        float a1 = vecField::getYVec(x_,y_, z_-1);
        float a2 = vecField::getYVec(x_, y_, z_);
        vZGrad = a2-a1;
    }
    //middle of vector field, take central difference
    else
    {
        float a1 = vecField::getYVec(x_,y_, z_-1);
        float a2 = vecField::getYVec(x_, y_, z_+1);
        vZGrad = (a2-a1)/2;
    }

    return vZGrad;
}

//Z velocity gradient in x direction
float vecField::WXgrad(int x_, int y_, int z_)
{
    float wXGrad;
    //if at start edge of vector field, take forward difference
    if (x_ == 0)
    {
        float a1 = vecField::getZVec(x_, y_, z_);
        float a2 = vecField::getZVec(x_+1, y_, z_);
        wXGrad = a2-a1;
    }
    //if at end of vector field, take bakward difference
    else if (x_ == xSize -1)
    {
        float a1 = vecField::getZVec(x_-1, y_, z_);
        float a2 = vecField::getZVec(x_, y_, z_);
        wXGrad = a2-a1;
    }
    //middle of vector field, take central difference
    else
    {
        float a1 = vecField::getZVec(x_-1, y_, z_);
        float a2 = vecField::getZVec(x_+1, y_, z_);
        wXGrad = (a2-a1)/2;
    }

    return wXGrad;
}

//Z velocity gradient in y direction
float vecField::WYgrad(int x_, int y_, int z_)
{
    float wYGrad;
    //if at start edge of vector field, take forward difference
    if (y_ == 0)
    {
        float a1 = vecField::getZVec(x_, y_, z_);
        float a2 = vecField::getZVec(x_, y_+1, z_);
        wYGrad = a2-a1;
    }
    //if at end of vector field, take bakward difference
    else if (y_ == ySize -1)
    {
        float a1 = vecField::getZVec(x_, y_-1, z_);
        float a2 = vecField::getZVec(x_, y_, z_);
        wYGrad = a2-a1;
    }
    //middle of vector field, take central difference
    else
    {
        float a1 = vecField::getZVec(x_, y_-1, z_);
        float a2 = vecField::getZVec(x_, y_+1, z_);
        wYGrad = (a2-a1)/2;
    }

    return wYGrad;
}

float vecField::UYgrad(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
        return vecField::UYgrad(xCell, yCell, zCell);

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

    float zCel1Val = vecField::UYgrad(xCell, yCell, zCell);

    float zCel2Val = vecField::UYgrad(xCell+1, yCell, zCell);

    float zCel3Val = vecField::UYgrad(xCell+1, yCell+1, zCell);

    float zCel4Val = vecField::UYgrad(xCell, yCell+1, zCell);

    float zCel5Val = vecField::UYgrad(xCell, yCell, zCell+1);

    float zCel6Val = vecField::UYgrad(xCell+1, yCell, zCell+1);

    float zCel7Val = vecField::UYgrad(xCell+1, yCell+1, zCell+1);

    float zCel8Val = vecField::UYgrad(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float zEdg1Val = zCel1Val+(zCel2Val-zCel1Val)*xLoc;

    float zEdg2Val = zCel4Val+(zCel3Val-zCel4Val)*xLoc;

    float zEdg3Val = zCel5Val+(zCel6Val-zCel5Val)*xLoc;

    float zEdg4Val = zCel8Val+(zCel7Val-zCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float zFac1 = zEdg1Val+(zEdg2Val-zEdg1Val)*yLoc;

    float zFac2 = zEdg3Val+(zEdg4Val-zEdg3Val)*yLoc;

    //
    //

    float zVal = zFac1+(zFac2-zFac1)*zLoc;

    return zVal;

}

float vecField::UZgrad(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
        return vecField::UZgrad(xCell, yCell, zCell);

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

    float zCel1Val = vecField::UZgrad(xCell, yCell, zCell);

    float zCel2Val = vecField::UZgrad(xCell+1, yCell, zCell);

    float zCel3Val = vecField::UZgrad(xCell+1, yCell+1, zCell);

    float zCel4Val = vecField::UZgrad(xCell, yCell+1, zCell);

    float zCel5Val = vecField::UZgrad(xCell, yCell, zCell+1);

    float zCel6Val = vecField::UZgrad(xCell+1, yCell, zCell+1);

    float zCel7Val = vecField::UZgrad(xCell+1, yCell+1, zCell+1);

    float zCel8Val = vecField::UZgrad(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float zEdg1Val = zCel1Val+(zCel2Val-zCel1Val)*xLoc;

    float zEdg2Val = zCel4Val+(zCel3Val-zCel4Val)*xLoc;

    float zEdg3Val = zCel5Val+(zCel6Val-zCel5Val)*xLoc;

    float zEdg4Val = zCel8Val+(zCel7Val-zCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float zFac1 = zEdg1Val+(zEdg2Val-zEdg1Val)*yLoc;

    float zFac2 = zEdg3Val+(zEdg4Val-zEdg3Val)*yLoc;

    //
    //

    float zVal = zFac1+(zFac2-zFac1)*zLoc;

    return zVal;

}

float vecField::VXgrad(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
        return vecField::VXgrad(xCell, yCell, zCell);

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

    float zCel1Val = vecField::VXgrad(xCell, yCell, zCell);

    float zCel2Val = vecField::VXgrad(xCell+1, yCell, zCell);

    float zCel3Val = vecField::VXgrad(xCell+1, yCell+1, zCell);

    float zCel4Val = vecField::VXgrad(xCell, yCell+1, zCell);

    float zCel5Val = vecField::VXgrad(xCell, yCell, zCell+1);

    float zCel6Val = vecField::VXgrad(xCell+1, yCell, zCell+1);

    float zCel7Val = vecField::VXgrad(xCell+1, yCell+1, zCell+1);

    float zCel8Val = vecField::VXgrad(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float zEdg1Val = zCel1Val+(zCel2Val-zCel1Val)*xLoc;

    float zEdg2Val = zCel4Val+(zCel3Val-zCel4Val)*xLoc;

    float zEdg3Val = zCel5Val+(zCel6Val-zCel5Val)*xLoc;

    float zEdg4Val = zCel8Val+(zCel7Val-zCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float zFac1 = zEdg1Val+(zEdg2Val-zEdg1Val)*yLoc;

    float zFac2 = zEdg3Val+(zEdg4Val-zEdg3Val)*yLoc;

    //
    //

    float zVal = zFac1+(zFac2-zFac1)*zLoc;

    return zVal;

}

float vecField::VZgrad(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
        return vecField::VZgrad(xCell, yCell, zCell);

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

    float zCel1Val = vecField::VZgrad(xCell, yCell, zCell);

    float zCel2Val = vecField::VZgrad(xCell+1, yCell, zCell);

    float zCel3Val = vecField::VZgrad(xCell+1, yCell+1, zCell);

    float zCel4Val = vecField::VZgrad(xCell, yCell+1, zCell);

    float zCel5Val = vecField::VZgrad(xCell, yCell, zCell+1);

    float zCel6Val = vecField::VZgrad(xCell+1, yCell, zCell+1);

    float zCel7Val = vecField::VZgrad(xCell+1, yCell+1, zCell+1);

    float zCel8Val = vecField::VZgrad(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float zEdg1Val = zCel1Val+(zCel2Val-zCel1Val)*xLoc;

    float zEdg2Val = zCel4Val+(zCel3Val-zCel4Val)*xLoc;

    float zEdg3Val = zCel5Val+(zCel6Val-zCel5Val)*xLoc;

    float zEdg4Val = zCel8Val+(zCel7Val-zCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float zFac1 = zEdg1Val+(zEdg2Val-zEdg1Val)*yLoc;

    float zFac2 = zEdg3Val+(zEdg4Val-zEdg3Val)*yLoc;

    //
    //

    float zVal = zFac1+(zFac2-zFac1)*zLoc;

    return zVal;

}

float vecField::WXgrad(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
        return vecField::WXgrad(xCell, yCell, zCell);

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

    float zCel1Val = vecField::WXgrad(xCell, yCell, zCell);

    float zCel2Val = vecField::WXgrad(xCell+1, yCell, zCell);

    float zCel3Val = vecField::WXgrad(xCell+1, yCell+1, zCell);

    float zCel4Val = vecField::WXgrad(xCell, yCell+1, zCell);

    float zCel5Val = vecField::WXgrad(xCell, yCell, zCell+1);

    float zCel6Val = vecField::WXgrad(xCell+1, yCell, zCell+1);

    float zCel7Val = vecField::WXgrad(xCell+1, yCell+1, zCell+1);

    float zCel8Val = vecField::WXgrad(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float zEdg1Val = zCel1Val+(zCel2Val-zCel1Val)*xLoc;

    float zEdg2Val = zCel4Val+(zCel3Val-zCel4Val)*xLoc;

    float zEdg3Val = zCel5Val+(zCel6Val-zCel5Val)*xLoc;

    float zEdg4Val = zCel8Val+(zCel7Val-zCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float zFac1 = zEdg1Val+(zEdg2Val-zEdg1Val)*yLoc;

    float zFac2 = zEdg3Val+(zEdg4Val-zEdg3Val)*yLoc;

    //
    //

    float zVal = zFac1+(zFac2-zFac1)*zLoc;

    return zVal;

}

float vecField::WYgrad(float x_, float y_, float z_)
{
    int xCell = floor(x_);
    int yCell = floor(y_);
    int zCell = floor(z_);
    float xLoc = x_ - xCell;
    float yLoc = y_ - yCell;
    float zLoc = z_ - zCell;
    if(xLoc == 0 && yLoc ==0 && zLoc ==0)
        return vecField::WYgrad(xCell, yCell, zCell);

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

    float zCel1Val = vecField::WYgrad(xCell, yCell, zCell);

    float zCel2Val = vecField::WYgrad(xCell+1, yCell, zCell);

    float zCel3Val = vecField::WYgrad(xCell+1, yCell+1, zCell);

    float zCel4Val = vecField::WYgrad(xCell, yCell+1, zCell);

    float zCel5Val = vecField::WYgrad(xCell, yCell, zCell+1);

    float zCel6Val = vecField::WYgrad(xCell+1, yCell, zCell+1);

    float zCel7Val = vecField::WYgrad(xCell+1, yCell+1, zCell+1);

    float zCel8Val = vecField::WYgrad(xCell, yCell+1, zCell+1);

    //      e1 = n1-n2
    //      e2 = n4-n3
    //      e3 = n5-n6
    //      e4 = n8-n7

    float zEdg1Val = zCel1Val+(zCel2Val-zCel1Val)*xLoc;

    float zEdg2Val = zCel4Val+(zCel3Val-zCel4Val)*xLoc;

    float zEdg3Val = zCel5Val+(zCel6Val-zCel5Val)*xLoc;

    float zEdg4Val = zCel8Val+(zCel7Val-zCel8Val)*xLoc;

    //      face1 = e1-e2 = n1-n2-n3-n4
    //      face2 = e3-e4 = n5-n6-n7-n8

    float zFac1 = zEdg1Val+(zEdg2Val-zEdg1Val)*yLoc;

    float zFac2 = zEdg3Val+(zEdg4Val-zEdg3Val)*yLoc;

    //
    //

    float zVal = zFac1+(zFac2-zFac1)*zLoc;

    return zVal;

}

float vecField::angV(int x_, int y_, int z_)
{
    float curlx = vecField::WYgrad(x_, y_, z_) - vecField::VZgrad(x_, y_, z_);
    float curly = vecField::UZgrad(x_, y_, z_) - vecField::WXgrad(x_, y_, z_);
    float curlz = vecField::VXgrad(x_, y_, z_) - vecField::UYgrad(x_, y_, z_);
    float xVec = vecField::getXVec(x_, y_, z_);
    float yVec = vecField::getYVec(x_, y_, z_);
    float zVec = vecField::getZVec(x_, y_, z_);

    //normalise
    float mag = sqrt(xVec*xVec + yVec*yVec + zVec*zVec);


    return 5*(curlx*xVec + curly*yVec + curlz*zVec)/mag;
}

float vecField::angV(float x_, float y_, float z_)
{
    float curlx = vecField::WYgrad(x_, y_, z_) - vecField::VZgrad(x_, y_, z_);
    float curly = vecField::UZgrad(x_, y_, z_) - vecField::WXgrad(x_, y_, z_);
    float curlz = vecField::VXgrad(x_, y_, z_) - vecField::UYgrad(x_, y_, z_);
    float xVec = vecField::getXVec(x_, y_, z_);
    float yVec = vecField::getYVec(x_, y_, z_);
    float zVec = vecField::getZVec(x_, y_, z_);

    //magnitude
    float mag = sqrt(xVec*xVec + yVec*yVec + zVec*zVec);


    return 5*(curlx*xVec + curly*yVec + curlz*zVec)/mag;
}
/*
if isempty(x)
  [junk, py, pz] = gradient(u);
  [qx, junk, qz] = gradient(v);
  [rx, ry, junk] = gradient(w);
else
end
curlx = ry-qz;
curly = pz-rx;
curlz = qx-py;
if nargout==4 || nargout==1
  nrm = sqrt(u.^2 + v.^2 + w.^2);
  cav = .5 * (curlx.*u + curly.*v + curlz.*w) ./nrm;
end
*/
vecField::~vecField()
{

}


