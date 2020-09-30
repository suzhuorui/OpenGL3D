#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include "bezierrender.h"
class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = nullptr);



protected:

    virtual void resizeGL(int w,int h);
    virtual void initializeGL();
    virtual void paintGL();

private:
    BezierRender render_;
    QMatrix4x4 pMatrix_;
    QVector3D camera_;
    GLfloat rTri;

signals:

public slots:
    void updataShow();
};

#endif // MYGLWIDGET_H
