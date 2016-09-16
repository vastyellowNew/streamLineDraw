#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QTimer>
#include "vecfield.h"
#include <QVector3D>
#include "ordered.h"
#include "scalarfield.h"
#include "colourmap.h"

//struct vecA {
//    float x;
//    float y;
//    float z;
//    float ang;
//    bool ok;
//};

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    float xSize,ySize,zSize;

    float seedX,seedY,seedZ; //Seed position
    QVector3D m_seed;
    bool m_dSw = false;
    bool m_line = false;
    bool m_VArrows = false;
    bool m_GArrows = false;
    bool m_streArrow = false;
    bool m_normArrow = false;
    float m_aScale = 1;
    vecField *mData;
    vecField *m_speedFGrad;
    int xSpace = 10;
    int ySpace = 10;
    int zSpace = 10;
    float minScal, maxScal;
    scalarField m_heliField;
    int m_colourHeli = 0;
    float m_rLight = 0.2;
    float m_gLight = 0.2;
    float m_bLight = 0.2;
    float m_alLight = 1.0;

    QVector<QList<QVector3D> > m_ribbon;
    QVector<QVector3D> mRibSeed;
    float mSphereRad = 0.5f;
    bool mDrawSeed = false;
    float mL0X = 0.5f;
    float mL0Y = 0.5f;
    float mL0Z = 0.1f;
    float mL1X = -0.5f;
    float mL1Y = 0.5f;
    float mL1Z = 0.3f;
    float mL2X = 0.2f;
    float mL2Y = -0.5f;
    float mL2Z = 0.7f;
    GLfloat mlight0_position[3];
    GLfloat mlight1_position[3];
    GLfloat mlight2_position[3];
    float mAmbient = 0.05;
    float mDiffuse = 0.5;
    float mSpecular = 0.6;
    GLfloat mlight_diffuse[4];
    GLfloat mlight_specular[4];
    GLfloat mlight_ambient[4];
    bool mL0 = true;
    bool mL1 = true;
    bool mL2 = true;
    int mTextureNo;
    int mPCRibbons = 100;
    int mNoOfRibbs = 100;

signals:
    void rotate(float xRot, float yRot, float zRot);

public slots:
    void dSw(bool sw1);
    void resetRot(float xORot, float yORot);
    void resetZoom(float zoomFactor);


private:
    QTimer timer;
    float m_zoomFactor =1;
    float camDelta[3] = {0.0,0.0,0.0};
    float lookAtDelta[3] = {0.0,0.0,0.0};
    float zoom = 0.0f;
    QPointF lastPos;
    float xRot =0.0f;
    float yRot =0.0f;
    float zRot =0.0f;

    void setLight();
    void matRibbon(float x, float y, float z);
    //vecA rk4(float x, float y, float z);
    void twoRibbon();
    void drawArrow(float x2, float y2, float z2, float x1, float y1, float z1);
    void drawFrame();
    void drawBox(int x, int y, int z);
};

#endif // GLWIDGET_H
