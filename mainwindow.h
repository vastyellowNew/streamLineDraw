#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vecfield.h>
#include <ordered.h>
#include <QVector3D>
#include "scalarfield.h"
#include "clearpoint.h"

struct vecA {
    float x;
    float y;
    float z;
    float ang;
    bool ok;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    vecField *mData = new vecField;
    bool dSw = false;
    float *speedField;
    vecField *speedFGrad = new vecField;
    float **m_curl;
    scalarField m_heliField;
    ordered m_heliF;
    ordered m_heliMagF;
    QVector3D m_seed;
    ordered speedF;

    int xSize,ySize,zSize;
    float dt = 0.15;
    float ribWidth = 1;
    QList<QVector3D> m_ribbon;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_X_valueChanged(double arg1);

    void on_Y_valueChanged(double arg1);

    void on_Z_valueChanged(double arg1);

    void on_loadData_released();

    void dataP();

    void on_rLenBox_valueChanged(QString arg1);

    void on_sNoBox_valueChanged(int arg1);

    void on_rX_valueChanged(double arg1);

    void on_rY_valueChanged(double arg1);

    void on_rZ_valueChanged(double arg1);

    void on_speedMBox_clicked(bool checked);

    void on_helMBox_clicked(bool checked);

    void on_hideLBox_clicked(bool checked);

    void on_helMagMBox_clicked(bool checked);

    void on_maxSlider_valueChanged(int value);

    void on_minSlider_valueChanged(int value);

    void on_rescaleBox_clicked(bool checked);

    void mapNumber(int choice);

    void on_pushButton_2_clicked();

    void on_showSeedsBox_clicked(bool checked);

    void on_progressSlider_valueChanged(int value);

    void on_forwardButt_clicked();

    void on_backButt_clicked();

private:
    Ui::MainWindow *ui;
    void calculations();
    float grad(int x, int y, int z, int pos);
    void rake();
    void matRibbon(float x, float y, float z);
    vecA rk4(float x, float y, float z);
    vecA rk4Back(float x, float y, float z);
    bool calc = false;
    float m_minScal, m_maxScal;
    float m_modMinScal, m_modMaxScal;
    clearPoint mSeedPoints;
    QVector<QVector3D> mheliMagFFast;
    int mRibbonMinLength = 5;
};

#endif // MAINWINDOW_H
