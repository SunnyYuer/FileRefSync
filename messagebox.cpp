#include "messagebox.h"
#include <QMessageBox>

MessageBox::MessageBox()
{

}

void MessageBox::showtext(QWidget *parent, QString title, QString text)
{
    QMessageBox::about(parent, title, text);
}

void MessageBox::showtext(QWidget *parent, QString title, int text)
{
    QMessageBox::about(parent, title, QString::number(text));
}

void MessageBox::showtext(QWidget *parent, QString title, double text)
{
    QMessageBox::about(parent, title, QString::number(text));
}
