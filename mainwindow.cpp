#include "filediff.h"
#include "folderdiff.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_filediffButton_clicked()
{
    FileDiff *filediff = new FileDiff();
    ui->tabWidget->insertTab(tabnum, filediff, "文件比较");
    ui->tabWidget->setCurrentIndex(tabnum);
    tabnum++;
}

void MainWindow::on_folderdiffButton_clicked()
{
    FolderDiff *folderdiff = new FolderDiff();
    ui->tabWidget->insertTab(tabnum, folderdiff, "文件夹比较");
    ui->tabWidget->setCurrentIndex(tabnum);
    tabnum++;
}

void MainWindow::on_btnabout_clicked()
{
    MessageBox::showtext(this, "关于", "123");
}
