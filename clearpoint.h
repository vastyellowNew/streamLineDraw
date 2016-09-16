#ifndef CLEARPOINT_H
#define CLEARPOINT_H


class clearPoint
{
    int mXSize;
    int mYSize;
    int mZSize;
    bool *mLocationClear;
    bool *mTempLocationClear;
    int mClearance = 2;
    bool mDataSet = false;
public:
    clearPoint();
    clearPoint(int x, int y, int z);
    void setSize(int x, int y, int z);
    void setClearance(int clearance);
    void setPoint(int x, int y, int z);
    void setPoint(float x, float y, float z);
    void clearTemp();
    bool checkSetPoint(float x, float y, float z);
    void newRibbon();
    bool isClear(int x, int y, int z);
    bool isClear(float x, float y, float z);
    void reset();
    int size();
};

#endif // CLEARPOINT_H
