#ifndef ORDERED_H
#define ORDERED_H

#include <QList>
#include <QVector3D>
#include <QMultiMap>
#include <QVector>

class ordered
{
    float* scalarField;
    QMultiMap<float, QVector3D> order;

public:
    ordered();
    void set(int x, int y, int z, float val);
    int length();
    float getFScal(int pos);
    float getLScal(int pos);
    float getUCS();
    float getLCS();
    void clear();
    float getScal(int x, int y, int z);
    QVector3D getFQVec(int pos);
    QVector3D getLQVec(int pos);
    void convert(QVector<QVector3D> *orderFast);
};

#endif // ORDERED_H
