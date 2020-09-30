#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>
#include "glwidget.h"
class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = nullptr);




private:
    GLWidget *glwidget;
signals:

public slots:
};

#endif // SHOWWIDGET_H
