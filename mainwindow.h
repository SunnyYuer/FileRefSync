#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollBar>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    Ui::MainWindow *ui;

    QScrollBar *scrolbar1;
    QScrollBar *scrolbar2;
    QStringList filelist1;  //文件1
    QStringList filelist2;  //文件2
    QTime time;
};

#endif // MAINWINDOW_H
