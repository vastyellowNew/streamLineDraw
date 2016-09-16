#include "ordered.h"

ordered::ordered()
{

}

void ordered::clear() //deletes all values
{
    order.clear();
}

void ordered::set(int x, int y, int z, float val)
{
    //scalarField[z*xSize*ySize + y*xSize +x] = val;
    QVector3D pos = {(float)x, (float)y, (float)z};
    order.insert(val, pos);
}

int ordered::length()
{
    return order.size();
}

QVector3D ordered::getFQVec(int pos)
{
    QMap<float, QVector3D>::iterator it = order.begin();
    int j = 0;
    while (it != order.end() )
    {
        if (j == pos) break;
        else
        {
            ++it;
            ++j;
        }
    }
    return it.value();
}

QVector3D ordered::getLQVec(int pos) //returns the pos-th largest location
{
    QMap<float, QVector3D>::iterator it = order.end();
    int j = 0;
    --it;
    while (it != order.begin())
    {
        if (j == pos) break;
        else
        {
            --it;
            ++j;
        }
    }
    return it.value();
}

void ordered::convert(QVector<QVector3D> *orderFast)
{
    QMap<float, QVector3D>::iterator it = order.end();
    --it;
    while (it != order.begin())
    {
        orderFast->append(it.value());
        --it;
    }
    orderFast->append(it.value());
}

float ordered::getScal(int x, int y, int z)
{
    QVector3D pos = {(float)x, (float)y, (float)z};
    return order.key(pos);
    //return scalarField[z*xSize*ySize + y*xSize + x];
}

float ordered::getFScal(int pos) //returns the pos-th smallest value
{
    QMap<float, QVector3D>::iterator it = order.begin();
    int j = 0;
    while (it != order.end() )
    {
        if (j == pos) break;
        else
        {
            ++it;
            ++j;
        }
    }
    return it.key();
}

float ordered::getLScal(int pos) //returns the pos-th largest value
{
    QMap<float, QVector3D>::iterator it = order.end();
    int j = 0;
    --it;
    while (it != order.begin())
    {
        if (j == pos) break;
        else
        {
            --it;
            ++j;
        }
    }
    return it.key();
}

float ordered::getLCS() //returns the smallest value to be used for the colour scaling
{
    int pc = ceil(order.size()/1000); // 0.1 % position
    //QMap<float, QVector3D>::iterator it = order.begin();
    float pb = (order.begin()).key();
    float pm = (order.begin()+pc).key();
    float pe = (order.end()-1).key();
    if ((pe-pm)/pe < (pm-pb)/pm) return pm;
    return pb;
}

float ordered::getUCS() //returns the largest value to be used for the colour scaling
{
    int pc = order.size() - floor(order.size()/1000) -1; //99.9%position
    //QMap<float, QVector3D>::iterator it = order.end();
    float pb = (order.begin()).key();
    float pm = (order.begin()+pc).key();
    float pe = (order.end()-1).key();
    if ((pe-pm)/pe > (pm-pb)/pm) return pm;
    return pe;
}
