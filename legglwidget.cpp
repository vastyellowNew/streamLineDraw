#include "legglwidget.h"
#include <GL/glut.h>
#include <QMouseEvent>
#include "choosemap.h"


legGLWidget::legGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

}


void legGLWidget::LoadAllTextures(int choice)
{
    map.setMap(choice);

    glNewList(mTextureList = glGenLists(1), GL_COMPILE);
    glEnable(GL_TEXTURE_1D);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, 3, map.getSize(), 0, GL_RGB, GL_FLOAT,map.getMap());
    glEndList();
}

void legGLWidget::initializeGL()
{

}

void legGLWidget::paintGL()
{
    legGLWidget::LoadAllTextures(mTextureNo);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_1D);
    glCallList(mTextureList);

    glBegin(GL_QUADS);
    glTexCoord1f(0.0);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0,-1.0);

    glTexCoord1f(1.0);
    glVertex2f(1.0,1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();


}

void legGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0, w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0, (float)w/h, 0.01, 100.0);

    update();
}

void legGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton )
    {
        chooseMap *loadData = new chooseMap();
        loadData->setWindowTitle("Choose legend colours");
        QObject::connect(loadData, SIGNAL(mapNo(int)), this, SLOT(mapNumber(int)));
        loadData->setModal(true);
        loadData->exec();
    }
}

void legGLWidget::mapNumber(int choice)
{
    emit mapNum(choice);
}
