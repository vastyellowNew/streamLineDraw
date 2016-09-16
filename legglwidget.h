#ifndef LEGGLWIDGET_H
#define LEGGLWIDGET_H

#include <QOpenGLWidget>
#include "colourmap.h"

class legGLWidget : public QOpenGLWidget
{
    Q_OBJECT
private:

    GLuint mTextureList;
    ColourMap map;
    void LoadAllTextures(int choice);

    void mousePressEvent(QMouseEvent *event);

signals:
    void mapNum(int);

public:
    explicit legGLWidget(QWidget *parent = 0);

    int mTextureNo;
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
private slots:
    void mapNumber(int choice);
};

#endif // LEGGLWIDGET_H
