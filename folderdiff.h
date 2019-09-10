#ifndef FOLDERDIFF_H
#define FOLDERDIFF_H

#include "messagebox.h"
#include <QFileDialog>
#include <QScrollBar>
#include <QTreeWidgetItem>
#include <QTime>
#include <QWidget>

namespace Ui {
class FolderDiff;
}

class FolderDiff : public QWidget
{
    Q_OBJECT

public:
    explicit FolderDiff(QWidget *parent = nullptr);
    ~FolderDiff();

    int checklineEdit();
    QList<QFileInfo> getfilelist(QDir dir, int filetype);
    QStringList fileinfolist(QFileInfo fileinfo);
    void setItemIcon(QTreeWidgetItem *item, int filetype);
    bool fileCompare(QFileInfo fileinfo1, QFileInfo fileinfo2);
    bool linkCompare(QFileInfo fileinfo1, QFileInfo fileinfo2);
    QMap<int, int> getMatchMap(QList<QFileInfo> list1, QList<QFileInfo> list2);

private slots:
    void on_button1_clicked();
    void on_button2_clicked();
    void on_btndiff_clicked();
    void scrolbar1Moved(int value);
    void scrolbar2Moved(int value);

private:
    Ui::FolderDiff *ui;
    MessageBox *messageBox;
    QTime time;
    QScrollBar *scrolbar1;
    QScrollBar *scrolbar2;
};

#endif // FOLDERDIFF_H
