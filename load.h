#ifndef LOAD_H
#define LOAD_H

#include <QDialog>
#include "vecfield.h"
#include <fstream>
#include <iostream>
#include <math.h>

namespace Ui {
class load;
}

class load : public QDialog
{
    Q_OBJECT

public:
    explicit load(vecField *pData, QWidget *parent = 0);
    ~load();

signals:
     void draw(bool sw);

private:
    vecField *mData;
    bool sw;
    float mA = 0.1;
    float mB = 0.5;
    float mC = 0.5;

private slots:

    void on_loadRadioButton_clicked();

    void on_generateRadioButton_clicked();

    void on_tornadoRadio_clicked();

    void on_abcRadio_clicked();

    void on_browseButt_clicked();

    void on_okButton_released();

    void on_modChBox_clicked();

    void on_aSpinBox_valueChanged(double arg1);

    void on_bSpinBox_valueChanged(double arg1);

    void on_cSpinBox_valueChanged(double arg1);

private:
    Ui::load *ui;
    void genTornado(int xs, int ys, int zs, int time, vecField *sallyv);
    void genAbc(int xs, int ys, int zs, vecField *abcv);
    bool readFile(int xs, int ys, int zs, vecField *vecf);
};

#endif // LOAD_H
