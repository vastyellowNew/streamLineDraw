#include "clearpoint.h"
#include "stdlib.h"
#include "math.h"

clearPoint::clearPoint()
{

}

clearPoint::clearPoint(int x, int y, int z)
{
    mXSize = x;
    mYSize = y;
    mZSize = z;

    //use malloc stdlib.h
    mLocationClear=(bool *) malloc(x*y*z*sizeof(bool));
    for (int i = 0; i < x*y*z; ++i) mLocationClear[i] = true;

    mTempLocationClear=(bool *) malloc(x*y*z*sizeof(bool));
    for (int i = 0; i < x*y*z; ++i) mTempLocationClear[i] = true;
    mDataSet = true;
}

void clearPoint::setSize(int x, int y, int z)
{
    mXSize = x;
    mYSize = y;
    mZSize = z;

    //use malloc stdlib.h
    mLocationClear=(bool *) malloc(x*y*z*sizeof(bool));
    for (int i = 0; i < x*y*z; ++i) mLocationClear[i] = true;

    mTempLocationClear=(bool *) malloc(x*y*z*sizeof(bool));
    for (int i = 0; i < x*y*z; ++i) mTempLocationClear[i] = true;
    mDataSet = true;
}

void clearPoint::setClearance(int clearance)
{
    mClearance = ceil(clearance);
}

void clearPoint::setPoint(int x, int y, int z)
{
    for (int i = x-mClearance; i <= x+mClearance; ++i)
    {
        for (int j = y-mClearance; j <= y+mClearance; ++j)
        {
            for (int k = z-mClearance; k <= z+mClearance; ++k)
            {
                if (!(i<0 || j<0 || k<0 || i>mXSize-2 || j>mYSize-2 || k>mZSize-2))
                {
                    mTempLocationClear[k*mXSize*mYSize + j*mXSize +i] = false;
                }
            }
        }
    }
}

void clearPoint::setPoint(float x, float y, float z)
{
    int xF = floor(x);
    int xC = ceil(x);
    int yF = floor(y);
    int yC = ceil(y);
    int zF = floor(z);
    int zC = ceil(z);

    this->setPoint(xF, yF, zF);
    this->setPoint(xF, yF, zC);
    this->setPoint(xF, yC, zF);
    this->setPoint(xF, yC, zC);
    this->setPoint(xC, yF, zF);
    this->setPoint(xC, yF, zC);
    this->setPoint(xC, yC, zF);
    this->setPoint(xC, yC, zC);
}

void clearPoint::clearTemp()
{
    if (mDataSet) for (int i = 0; i < mXSize*mYSize*mZSize; ++i) mTempLocationClear[i] = true;
}

bool clearPoint::checkSetPoint(float x, float y, float z)
{
    this->setPoint(x, y, z);
    return this->isClear(x, y, z);
}

void clearPoint::newRibbon()
{
    for (int i = 0; i < mXSize*mYSize*mZSize; ++i)
    {
        if (!mTempLocationClear[i]) mLocationClear[i] = false;
        mTempLocationClear[i] = true;
    }
}

bool clearPoint::isClear(int x, int y, int z)
{
    if (x<0 || y<0 || z<0 || x>mXSize-1 || y>mYSize-1 || z>mZSize-1)
    {
        return false;
    }
    return mLocationClear[z*mXSize*mYSize + y*mXSize +x];
}

bool clearPoint::isClear(float x, float y, float z)
{
    int xF = floor(x);
    int xC = ceil(x);
    int yF = floor(y);
    int yC = ceil(y);
    int zF = floor(z);
    int zC = ceil(z);

    return (this->isClear(xF, yF, zF) &&
            this->isClear(xF, yF, zC) &&
            this->isClear(xF, yC, zF) &&
            this->isClear(xF, yC, zC) &&
            this->isClear(xC, yF, zF) &&
            this->isClear(xC, yF, zC) &&
            this->isClear(xC, yC, zF) &&
            this->isClear(xC, yC, zC));
}

void clearPoint::reset()
{
    if (mDataSet) for (int i = 0; i < mXSize*mYSize*mZSize; ++i) mLocationClear[i] = true;
}

int clearPoint::size()
{
    return mXSize*mYSize*mZSize;
}
