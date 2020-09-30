#include "myglwidget.h"
#include <QTimer>
MyGLWidget::MyGLWidget(QWidget *parent) : QGLWidget(parent)
{
    QTimer *timer=new QTimer();
    timer->setInterval(20);
    rTri=0;
    connect(timer,&QTimer::timeout,this,&MyGLWidget::updataShow);
    timer->start();

}

void MyGLWidget::initializeGL()
{
    render_.inititsize(QVector<BNPosition>() << qMakePair(0.0,0.0)<< qMakePair(0.4,-0.3) << qMakePair(0.4,0.3) << qMakePair(0.8,-0.6),0.05,Qt::white);
    camera_.setX(0);
    camera_.setY(0);
    camera_.setZ(2);

//    glEnable( GL_TEXTURE_2D );// 启用2D纹理
//    glClearColor(0.1f, 0.1f, 0.4f, 0.1f);               //黑色背景
//    glShadeModel(GL_SMOOTH);                            //启用阴影平滑
//    glClearDepth(1.0);                                  //设置深度缓存
//    glEnable(GL_DEPTH_TEST);                            //启用深度测试
//    glEnable(GL_CULL_FACE);                             //开启剔除操作
//    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
}

void MyGLWidget::resizeGL(int w,int h)
{
    pMatrix_.setToIdentity();
        pMatrix_.perspective(45,static_cast<float>(w)/h,0.01f,100.0f);
}

void MyGLWidget::paintGL()
{

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存

//    glLoadIdentity();                                   //重置模型观察矩阵
//    glTranslatef(0.0f, 0.2f, -3.0f);
//    glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);
//    //glRotatef(m_Rot, 0.0f, 0.0f, 1.0f);                 //绕z轴旋转

//    glRotatef(rTri, 0.0f, 0.0f, 1.0f); //注意：X轴旋转，X,Y,Z可调整绕哪个轴
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
        f->glClearColor(0.0, 0.0, 0.0, 0.0);
        f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        QMatrix4x4 vMatrix;
        vMatrix.lookAt(camera_,QVector3D(0.0,0.0,0.0),QVector3D(0.0,1.0,0.0));

        QMatrix4x4 mMatrix;
        mMatrix.translate(0,0.4,0);
        int n = 360 / 5;
        for(int i = 0; i < n; i++){
            QMatrix4x4 yangleMatrix;
            yangleMatrix.rotate(i * 5,0,1,0);
            QMatrix4x4 asd=yangleMatrix * mMatrix;
            render_.render(f,pMatrix_,vMatrix,asd);
        }
}

void MyGLWidget::updataShow()
{
    rTri+=1;
    update();
}
