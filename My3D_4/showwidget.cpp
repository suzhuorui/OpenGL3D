#include "showwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include "myglwidget.h"
ShowWidget::ShowWidget(QWidget *parent) : QWidget(parent)
{
    glwidget=new GLWidget();
    glwidget->resize(800,400);
    glwidget->show();


//    MyGLWidget * w=new MyGLWidget();
//    w->resize(1800,900);
//    w->show();
}
