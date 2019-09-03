#ifndef FIlEDIFF_H
#define FIlEDIFF_H

#include <QMainWindow>
#include <QScrollBar>
#include <QTime>

namespace Ui {
class FileDiff;
}

class FileDiff : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileDiff(QWidget *parent = 0);
    ~FileDiff();

    void showtext(QString s);
    void showtext(int n);
    void showtext(double n);
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
};

#endif // FIlEDIFF_H
