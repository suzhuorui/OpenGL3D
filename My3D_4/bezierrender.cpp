#include "bezierrender.h"
#include <QtMath>



void BezierRender::inititsize(QVector<BNPosition> points, float span, QColor c)
{
    const GLchar *vshader_source =
            "#version 330\n"
            "uniform mat4 uPMatrix,uVMatrix,uMMatrix;\n"
            "layout(location = 0) in vec3 aPosition;\n"
            "layout(location = 1) in vec3 aColor;\n"
            "smooth out vec4 vColor;\n"
            "void main(void)\n"
            "{\n"
            "   gl_Position = uPMatrix * uVMatrix * uMMatrix * vec4(aPosition,1);\n"
            "   vColor = vec4(aColor,1);\n"
            "}\n";

        const GLchar *cshader_source =
                "#version 330\n"
                "in vec4 vColor;\n"
                "out vec4 fragColor;\n"
                "\n"
                "void main(void)\n"
                "{\n"
                "   fragColor = vColor;"
                "}\n";

    program_.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,vshader_source);
    program_.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,cshader_source);

    program_.link();

    int n = points.count() - 1;
    if(n < 1){
        return;
    }

    int steps = static_cast<int>(1.0f/span);
    long *jiechengNA = new long[n+1];
    for(int i = 0; i <= n; i++){
        jiechengNA[i] = jiecheng(i);
    }

    for(int i = 0;i <= steps; i++){
        float t=i*span;
        if(t>1)
        {
            t=1;
        }
        float xf=0;
        float yf=0;

        float *tka = new float[n+1];
        float *otka = new float[n+1];
        for(int j = 0; j <= n; j++){
            tka[j]=static_cast<float>(::pow(t,j));
            otka[j]=static_cast<float>(::pow(1-t,j));
        }

        for(int k =0;k<=n;k++){
            float xs=(jiechengNA[n]/(jiechengNA[k]*jiechengNA[n-k]))*tka[k]*otka[n-k];
            xf=xf+points.at(k).first * xs;
            yf=yf+points.at(k).second * xs;
        }

        vertPoints_ << xf << yf << 0.0;
        colorPoints_ << c.redF() << c.greenF() << c.blueF();
        delete tka;
        delete otka;
    }
    QVector<GLfloat> alPoints;
    alPoints << vertPoints_ << colorPoints_;
    vbo_.create();
    vbo_.bind();
    vbo_.allocate(alPoints.data(),alPoints.count() * sizeof(GLfloat));
    delete jiechengNA;
}

void BezierRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix, QMatrix4x4 &mMatrix)
{
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);

    program_.bind();
    vbo_.bind();
    program_.setUniformValue("uPMatrix",pMatrix);
    program_.setUniformValue("uVMatrix",vMatrix);
    program_.setUniformValue("uMMatrix",mMatrix);

    program_.enableAttributeArray(0);
    program_.enableAttributeArray(1);
    program_.setAttributeBuffer(0,GL_FLOAT,0,3,3*sizeof(GLfloat));
    program_.setAttributeBuffer(1,GL_FLOAT,vertPoints_.count() * sizeof(GLfloat),3,3*sizeof(GLfloat));
    f->glDrawArrays(GL_LINE_STRIP,0,vertPoints_.count() / 3);
    program_.disableAttributeArray(0);
    program_.disableAttributeArray(1);
    vbo_.release();
    program_.release();

    f->glDisable(GL_DEPTH_TEST);
    f->glDisable(GL_CULL_FACE);

}

long BezierRender::jiecheng(int n)
{
    long result = 1;
        if( n == 0){
            return 1;
        }

        for(int i = 2; i <= n; i++){
            result = result * i;
        }

        return result;
}
