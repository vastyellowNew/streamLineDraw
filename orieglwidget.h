#ifndef ORIEGLWIDGET_H
#define ORIEGLWIDGET_H

#include <QOpenGLWidget>

class OrieGLWidget : public QOpenGLWidget
{
        Q_OBJECT

private:
    float m_xORot, m_yORot, m_zORot;

    void mousePressEvent(QMouseEvent *event);
public:
    explicit OrieGLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

signals:
    void resetRot(float m_xORot, float m_yORot);
    void resetZoom(float zoomFactor);

public slots:
    void rot(float xORot, float yORot, float zORot);
};

#endif // ORIEGLWIDGET_H
