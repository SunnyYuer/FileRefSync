#ifndef FIlEDIFF_H
#define FIlEDIFF_H

#include "messagebox.h"
#include <QScrollBar>
#include <QTime>
#include <QWidget>

namespace Ui {
class FileDiff;
}

class FileDiff : public QWidget
{
    Q_OBJECT

public:
    explicit FileDiff(QWidget *parent = nullptr);
    ~FileDiff();

    int getfilelist();
    const char *detectEncoding(QString buffer);
    QMap<int, int> lcs();
    QMap<int, int> lcsx();
    void setTextColor(int texteditnum, const QColor &color);
    void hilightLines(int texteditnum, const QColor &color, int &emptyhl, int rep);

private slots:
    void on_button1_clicked();
    void on_button2_clicked();
    void on_btncon_clicked();
    void scrolbar1Moved(int value);
    void scrolbar2Moved(int value);

private:
    Ui::FileDiff *ui;
    QScrollBar *scrolbar1;
    QScrollBar *scrolbar2;
    QStringList filelist1;  //文件1
    QStringList filelist2;  //文件2
    QTime time;
    MessageBox *messageBox;
};

#endif // FIlEDIFF_H
