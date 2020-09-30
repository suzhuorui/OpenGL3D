#ifndef BEZIERRENDER_H
#define BEZIERRENDER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QColor>
using BNPosition = QPair<GLfloat,GLfloat>;
class BezierRender
{
public:
    BezierRender() = default;
    void inititsize(QVector<BNPosition> points,float span,QColor c);
    void render(QOpenGLExtraFunctions *f,QMatrix4x4 &pMatrix,QMatrix4x4 &vMatrix,QMatrix4x4 &mMatrix);

private:
    QOpenGLShaderProgram program_;
    QOpenGLBuffer vbo_;
    QVector<GLfloat> vertPoints_,colorPoints_;

    long jiecheng(int n);
};

#endif // BEZIERRENDER_H
