#include "orieglwidget.h"

#include <GL/glut.h>
#include <QMouseEvent>


OrieGLWidget::OrieGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
}

void OrieGLWidget::initializeGL()
{
    glClearColor(1.0,1.0,1.0,1);

    glEnable(GL_COLOR_MATERIAL);
}

void OrieGLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    float lSize = 5;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( 0,0,20, 0,0,0, 0,1,0 );

    glRotatef(m_xORot/16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(m_yORot/16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(m_zORot/16.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(1,0,0); //Red
    glLineWidth(2);

    // Draw axis lines
    glBegin(GL_LINES); //xAxis
    glVertex3f(0, 0, 0 );
    glVertex3f(lSize, 0, 0 );
    glVertex3f(lSize, 0, 0 );
    glVertex3f(lSize-1, +1, 0 );
    glVertex3f(lSize, 0, 0 );
    glVertex3f(lSize-1, -1, 0 );
    glEnd();

    glColor3f(1,1,0); //Yellow
    glBegin(GL_LINES); //yAxis
    glVertex3f(0, 0, 0 );
    glVertex3f(0, lSize, 0 );
    glVertex3f(0, lSize, 0 );
    glVertex3f(+1, lSize-1, 0 );
    glVertex3f(0, lSize, 0 );
    glVertex3f(-1, lSize-1, 0 );
    glEnd();

    glColor3f(0,1,0); //Green
    glBegin(GL_LINES); //zAxis
    glVertex3f(0, 0, 0 );
    glVertex3f(0, 0, lSize );
    glVertex3f(0, 0, lSize );
    glVertex3f(-1, 0, lSize-1 );
    glVertex3f(0, 0, lSize );
    glVertex3f(+1, 0, lSize-1 );
    glEnd();



}

void OrieGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0, w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0, (float)w/h, 0.01, 100.0);

    update();
}

void OrieGLWidget::rot(float xORot, float yORot, float zORot)
{
    m_xORot = xORot;
    m_yORot = yORot;
    m_zORot = zORot;
    update();
}

void OrieGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton )
    {
        m_xORot = 0;
        m_yORot = 0;
        m_zORot = 0;
        emit resetRot(m_xORot, m_yORot);
        update();
    }
    if (event->button() == Qt::LeftButton ) emit resetZoom(1);
}
