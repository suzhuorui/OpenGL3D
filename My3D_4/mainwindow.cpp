#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    widget=new ShowWidget(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    widget->resize(600,400);
    widget->show();
}
