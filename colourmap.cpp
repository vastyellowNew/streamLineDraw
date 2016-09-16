#include "colourmap.h"


ColourMap::ColourMap()
{

}

ColourMap::ColourMap(int tex)
{
    mChoice = tex;
}

float(* ColourMap::getMap())[3]
{
    //if (mChoice == 1) return mTextureArray1;
    if (mChoice == 2) return mTextureArray2;
    else if (mChoice == 3) return mTextureArray3;
    else if (mChoice == 4) return mTextureArray4;
    else if (mChoice == 5) return mTextureArray5;
    else return mTextureArray0;
}

int ColourMap::getSize()
{
    //if (mChoice == 1) return mSize1;
    if (mChoice == 2) return mSize2;
    else if (mChoice == 3) return mSize3;
    else if (mChoice == 4) return mSize4;
    else if (mChoice == 5) return mSize5;
    else return mSize0;
}

void ColourMap::setMap(int tex)
{
    mChoice = tex;
}
