#include "messagebox.h"
#include <QMessageBox>

MessageBox::MessageBox(QWidget *parent) :
    QDialog(parent)
{
    parentWidget = parent;
}

void MessageBox::showtext(QString s)
{
    QMessageBox msg(parentWidget);
    msg.setWindowTitle("提示");
    msg.setText(s);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void MessageBox::showtext(int n)
{
    QMessageBox msg(parentWidget);
    msg.setWindowTitle("提示");
    msg.setText(QString::number(n));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void MessageBox::showtext(double n)
{
    QMessageBox msg(parentWidget);
    msg.setWindowTitle("提示");
    msg.setText(QString::number(n));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}
