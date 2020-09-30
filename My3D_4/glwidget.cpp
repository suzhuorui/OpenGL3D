/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QtMath>
#include <QTimer>
#include <QImage>


GLWidget::GLWidget(QWidget *parent): QGLWidget(parent)
{
//    m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
//    // --transparent causes the clear color to be transparent. Therefore, on systems that
//    // support it, the widget will become transparent apart from the logo.
//    if (m_transparent) {
//        QSurfaceFormat fmt = format();
//        fmt.setAlphaBufferSize(8);
//        setFormat(fmt);
//    }

          m_xRot=0;
          m_yRot=0;
          m_zRot=0;
    fullscreen = false;
    m_ShowCPoints = true;
    m_Rot = 0.0f;
    m_Divs = 7;
    initBezier();


    QTimer *timer = new QTimer(this);                   //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    //timer->start(10);
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::cleanup()
{
    glDeleteLists(m_Mybezier.dlBPatch, 1);              //删除显示列表
}

void GLWidget::updateGL()
{
    m_Rot+=1;
    update();
}

void GLWidget::initializeGL()
{
    //initializeOpenGLFunctions();                         //启用纹理映射
    QPixmap *a=new QPixmap();
    if(!a->load("G:/WorkSpace/Qt/My3D_4/asd.bmp"))
    {
        qDebug("图片未加载成功");
    }
    m_Mybezier.texture = bindTexture(*a);
    m_Mybezier.dlBPatch = genBezier();
    glEnable( GL_TEXTURE_2D );// 启用2D纹理
    glClearColor(0.1f, 0.1f, 0.4f, 0.1f);               //背景
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑
    glClearDepth(1.0);                                  //设置深度缓存
    glEnable(GL_DEPTH_TEST);                            //启用深度测试
    glEnable(GL_CULL_FACE);                             //开启剔除操作
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
}

Point_3D GLWidget::bernstein(float u, Point_3D *p)
{
    Point_3D a = p[0] * pow(u, 3);
    Point_3D b = p[1] * (3*pow(u, 2)*(1-u));
    Point_3D c = p[2] * (3*u*pow(1-u, 2));
    Point_3D d = p[3] * pow(1-u, 3);

    Point_3D r = a + b + c + d;
    return r;
}

GLuint GLWidget::genBezier()
{
    GLuint drawlist = glGenLists(1);                    //分配1个显示列表的空间
    Point_3D temp[4];
    //根据每一条曲线的细分数，分配相应的内存
    Point_3D *last = (Point_3D*)malloc(sizeof(Point_3D)*(m_Divs+1));

    if (m_Mybezier.dlBPatch != 0)                       //如果显示列表存在，则删除
    {
        glDeleteLists(m_Mybezier.dlBPatch, 1);
    }

    temp[0] = m_Mybezier.anchors[0][3];                 //获得u方向的四个控制点
    temp[1] = m_Mybezier.anchors[1][3];
    temp[2] = m_Mybezier.anchors[2][3];
    temp[3] = m_Mybezier.anchors[3][3];

    for (int v=0; v<=m_Divs; v++)                       //根据细分数，创建各个分割点的参数
    {
        float py = ((float)v)/((float)m_Divs);
        last[v] = bernstein(py, temp);                  //使用bernstein函数求得分割点坐标
    }

    glNewList(drawlist, GL_COMPILE);                    //绘制一个新的显示列表
    glBindTexture(GL_TEXTURE_2D, m_Mybezier.texture);   //绑定纹理
    for (int u=1; u<=m_Divs; u++)
    {
        float px = ((float)u)/((float)m_Divs);          //计算v方向上的细分点的参数
        float pxold = ((float)u-1.0f)/((float)m_Divs);  //上一个v方向的细分点的参数

        temp[0] = bernstein(px, m_Mybezier.anchors[0]); //计算每个细分点v方向上贝塞尔曲面的控制点
        temp[1] = bernstein(px, m_Mybezier.anchors[1]);
        temp[2] = bernstein(px, m_Mybezier.anchors[2]);
        temp[3] = bernstein(px, m_Mybezier.anchors[3]);

        glBegin(GL_TRIANGLE_STRIP);                     //开始绘制三角形带
        for (int v=0; v<=m_Divs; v++)
        {
            float py = ((float)v)/((float)m_Divs);  //沿着u方向顺序绘制
            glTexCoord2f(pxold, py);                //设置纹理坐标并绘制一个顶点
            glVertex3d(last[v].x(), last[v].y(), last[v].z());

            last[v] = bernstein(py, temp);          //计算下一个顶点
            glTexCoord2f(px, py);                   //设置纹理坐标并绘制新的顶点
            glVertex3d(last[v].x(), last[v].y(), last[v].z());
        }
        glEnd();                                        //结束三角形带的绘制
    }
    glEndList();                                        //显示列表绘制结束

    free(last);                                         //释放分配的内存
    return drawlist;                                    //返回创建的显示列表
}

void GLWidget::initBezier()
{
    m_Mybezier.anchors[0][0] = Point_3D(-0.75, -0.75, -0.50);
    m_Mybezier.anchors[0][1] = Point_3D(-0.25, -0.75,  0.00);
    m_Mybezier.anchors[0][2] = Point_3D( 0.25, -0.75,  0.00);
    m_Mybezier.anchors[0][3] = Point_3D( 0.75, -0.75, -0.50);

    m_Mybezier.anchors[1][0] = Point_3D(-0.75, -0.25, -0.75);
    m_Mybezier.anchors[1][1] = Point_3D(-0.25, -0.25,  0.50);
    m_Mybezier.anchors[1][2] = Point_3D( 0.25, -0.25,  0.50);
    m_Mybezier.anchors[1][3] = Point_3D( 0.75, -0.25, -0.75);

    m_Mybezier.anchors[2][0] = Point_3D(-0.75,  0.25,  0.00);
    m_Mybezier.anchors[2][1] = Point_3D(-0.25,  0.25, -0.50);
    m_Mybezier.anchors[2][2] = Point_3D( 0.25,  0.25, -0.50);
    m_Mybezier.anchors[2][3] = Point_3D( 0.75,  0.25,  0.00);

    m_Mybezier.anchors[3][0] = Point_3D(-0.75,  0.75, -0.50);
    m_Mybezier.anchors[3][1] = Point_3D(-0.25,  0.75, -1.00);
    m_Mybezier.anchors[3][2] = Point_3D( 0.25,  0.75, -1.00);
    m_Mybezier.anchors[3][3] = Point_3D( 0.75,  0.75, -0.50);

    m_Mybezier.dlBPatch = 0;                            //默认的显示列表为0
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存

    glLoadIdentity();                                   //重置模型观察矩阵
    glTranslatef(0.0f, 0.2f, -3.0f);
    glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(m_Rot, 0.0f, 0.0f, 1.0f);                 //绕z轴旋转

    glCallList(m_Mybezier.dlBPatch);                    //调用显示列表，绘制贝塞尔曲面

    glDisable(GL_TEXTURE_2D);                       //禁用纹理贴图
    glColor3f(1.0f, 0.0f, 0.0f);                    //设置颜色为红色
    for (int i=0; i<4; i++)                         //绘制水平线
    {
        glBegin(GL_LINE_STRIP);
            for (int j=0; j<4; j++)
            {
                glVertex3d(m_Mybezier.anchors[i][j].x(),m_Mybezier.anchors[i][j].y(),m_Mybezier.anchors[i][j].z());
            }
        glEnd();
    }
    for (int i=0; i<4; i++)                         //绘制垂直线
    {
        glBegin(GL_LINE_STRIP);
            for (int j=0; j<4; j++)
            {
                glVertex3d(m_Mybezier.anchors[j][i].x(),m_Mybezier.anchors[j][i].y(),m_Mybezier.anchors[j][i].z());
            }
        glEnd();
    }
    glColor3f(1.0f, 1.0f, 1.0f);                    //恢复OpenGL属性
    glEnable(GL_TEXTURE_2D);

}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}
