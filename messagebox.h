#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>
#include <QString>

class MessageBox
{
public:
    MessageBox();
    static void showtext(QWidget *parent, QString title, QString text);
    static void showtext(QWidget *parent, QString title, int text);
    static void showtext(QWidget *parent, QString title, double text);
};

#endif // MESSAGEBOX_H
