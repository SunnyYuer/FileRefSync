#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>
#include <QString>

class MessageBox : public QDialog
{
public:
    explicit MessageBox(QWidget *parent);//父控件不设置为空
    void showtext(QString s);
    void showtext(int n);
    void showtext(double n);

private:
    QWidget *parentWidget;
};

#endif // MESSAGEBOX_H
