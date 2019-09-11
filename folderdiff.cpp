#include "folderdiff.h"
#include "ui_folderdiff.h"
#include "messagebox.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QSettings>
#include <QDebug>

FolderDiff::FolderDiff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderDiff)
{
    ui->setupUi(this);
    scrolbar1 = ui->treeWidget1->verticalScrollBar();
    scrolbar2 = ui->treeWidget2->verticalScrollBar();
    connect(static_cast<QWidget*>(scrolbar1), SIGNAL(sliderMoved(int)), this,  SLOT(scrolbar1Moved(int)));
    connect(static_cast<QWidget*>(scrolbar2), SIGNAL(sliderMoved(int)), this,  SLOT(scrolbar2Moved(int)));
    ui->treeWidget1->setColumnWidth(0, 250);
    ui->treeWidget2->setColumnWidth(0, 250);
    time.start();
}

FolderDiff::~FolderDiff()
{
    delete ui;
}

void FolderDiff::on_button1_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);
    QString lastPath = setting.value("Folder1Path").toString();
    if(lastPath.isEmpty()) lastPath = "./";
    QString s = QFileDialog::getExistingDirectory(this, "选择文件夹", lastPath);
    if(s.isEmpty()) return;
    ui->lineEdit1->setText(s);
    setting.setValue("Folder1Path", s);
}

void FolderDiff::on_button2_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);
    QString lastPath = setting.value("Folder2Path").toString();
    if(lastPath.isEmpty()) lastPath = "./";
    QString s = QFileDialog::getExistingDirectory(this, "选择文件夹", lastPath);
    if(s.isEmpty()) return;
    ui->lineEdit2->setText(s);
    setting.setValue("Folder2Path", s);
}

int FolderDiff::checklineEdit()
{
    QString s1 = ui->lineEdit1->text();
    QDir dir1(s1);
    if(s1.isEmpty() || !dir1.exists())
    {
        MessageBox::showtext(this, "提示", "文件夹1不存在");
        return 0;
    }
    QString s2 = ui->lineEdit2->text();
    QDir dir2(s2);
    if(s2.isEmpty() || !dir2.exists())
    {
        MessageBox::showtext(this, "提示", "文件夹2不存在");
        return 0;
    }
    return 1;
}

QList<QFileInfo> FolderDiff::getfilelist(QDir dir, int filetype)
{
    if(filetype == 0)
    {
        QList<QFileInfo> filelist(dir.entryInfoList(QDir::Dirs));
        filelist.removeFirst();//删除.
        filelist.removeFirst();//删除..
        return filelist;
    }
    else
    {
        QList<QFileInfo> filelist(dir.entryInfoList(QDir::Files));
        return filelist;
    }
}

QStringList FolderDiff::fileinfolist(QFileInfo fileinfo)
{
    QStringList infolist;
    if(fileinfo.isDir())
    {
        infolist<<fileinfo.fileName()<<"";
        infolist<<fileinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    }
    if(fileinfo.isFile())
    {
        infolist<<fileinfo.fileName();
        double size = fileinfo.size();
        QString suffix = "B";
        if (size >= 1024)
        {
            size /= 1024;
            suffix = "KB";
            if (size >= 1024)
            {
                size /= 1024;
                suffix = "MB";
                if (size >= 1024)
                {
                    size /= 1024;
                    suffix = "GB";
                }
            }
        }
        if(suffix == "B") infolist<<QString::number(size) + " " + suffix;
        else infolist<<QString::number(size, 'f', 1) + " " + suffix;
        infolist<<fileinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    }
    return infolist;
}

void FolderDiff::setItemIcon(QTreeWidgetItem *item, int filetype)
{
    if(filetype == 0) item->setIcon(0, QIcon(":/pic/image/folder.png"));
    else item->setIcon(0, QIcon(":/pic/image/file.png"));
}

bool FolderDiff::fileCompare(QFileInfo fileinfo1, QFileInfo fileinfo2)
{
    if(fileinfo1.size() != fileinfo2.size()) return false;
    QFile file1(fileinfo1.filePath());
    file1.open(QIODevice::ReadOnly | QIODevice::Text);
    QString sha256file1 = QCryptographicHash::hash(file1.readAll(), QCryptographicHash::Sha256).toHex();
    file1.close();
    QFile file2(fileinfo2.filePath());
    file2.open(QIODevice::ReadOnly | QIODevice::Text);
    QString sha256file2 = QCryptographicHash::hash(file2.readAll(), QCryptographicHash::Sha256).toHex();
    file2.close();
    //qDebug()<<sha256file1<<sha256file2;
    return sha256file1==sha256file2;
}

bool FolderDiff::linkCompare(QFileInfo fileinfo1, QFileInfo fileinfo2)
{
    //qDebug()<<fileinfo1.symLinkTarget()<<fileinfo2.symLinkTarget();
    return fileinfo1.symLinkTarget()==fileinfo2.symLinkTarget();
}

QMap<int, int> FolderDiff::getMatchMap(QList<QFileInfo> list1, QList<QFileInfo> list2)
{
    QMap<int, int> matchmap;
    for(int i=0; i<list1.count(); i++)
    {
        for(int j=0; j<list2.count(); j++)
        {
            if(list1[i].fileName() == list2[j].fileName())
            {
                matchmap.insert(i, j);
            }
        }
    }
    return matchmap;
}

void FolderDiff::on_btndiff_clicked()
{
    if(!checklineEdit()) return;

    ui->treeWidget1->clear();
    ui->treeWidget2->clear();
    QApplication::processEvents();

    QDir dir1(ui->lineEdit1->text());
    QDir dir2(ui->lineEdit2->text());
    QList<QFileInfo> filelist1;
    QList<QFileInfo> filelist2;
    QMap<int, int> matchmap;
    QMap<int, int>::iterator it;
    int filetype = 0;  //0为文件夹，1为文件
    int delline = 0; //folder1删掉的行数
    int addline = 0; //folder2增加的行数

    while(filetype <= 1)
    {
        qDebug()<<"获取filelist开始"<<time.elapsed()/1000.0<<"s";
        filelist1 = getfilelist(dir1, filetype);
        filelist2 = getfilelist(dir2, filetype);
        qDebug()<<"获取filelist结束"<<time.elapsed()/1000.0<<"s";

        qDebug()<<"获取matchmap开始"<<time.elapsed()/1000.0<<"s";
        matchmap = getMatchMap(filelist1, filelist2);
        qDebug()<<"获取matchmap结束"<<time.elapsed()/1000.0<<"s";
        matchmap.insert(filelist1.length(), filelist2.length());

        qDebug()<<"输出开始"<<time.elapsed()/1000.0<<"s";
        int i=0, j=0;
        for (it = matchmap.begin(); it != matchmap.end(); it++)
        {
            if(i < it.key())
            {
                while(i < it.key())
                {
                    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget1, fileinfolist(filelist1[i]));
                    setItemIcon(item, filetype);
                    item->setForeground(0, QColor(237, 28, 36));//红色
                    delline++;
                    i++;
                }
            }
            if(i == it.key())
            {
                if(j < it.value())
                {
                    while(j < it.value())
                    {
                        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget2, fileinfolist(filelist2[j]));
                        setItemIcon(item, filetype);
                        item->setForeground(0, QColor(34, 177, 76));//绿色
                        addline++;
                        j++;
                    }
                }
                if(addline <= delline)
                {
                    delline = delline - addline;
                    addline = 0;
                }
                if(addline > delline)
                {
                    addline = addline - delline;
                    delline = 0;
                }
                while(addline > 0)
                {
                    new QTreeWidgetItem(ui->treeWidget1, QStringList(""));
                    addline--;
                }
                while(delline > 0)
                {
                    new QTreeWidgetItem(ui->treeWidget2, QStringList(""));
                    delline--;
                }
                if(j == it.value() && j != filelist2.length())
                {
                    bool diff = false;
                    if(filelist1[i].isSymLink()) diff = !linkCompare(filelist1[i], filelist2[j]);
                    else
                    {
                        if(filetype == 1) diff = !fileCompare(filelist1[i], filelist2[j]);
                    }
                    QTreeWidgetItem *item1 = new QTreeWidgetItem(ui->treeWidget1, fileinfolist(filelist1[i]));
                    setItemIcon(item1, filetype);
                    if(diff) item1->setForeground(0, QColor(0, 162, 232));//蓝色
                    QTreeWidgetItem *item2 = new QTreeWidgetItem(ui->treeWidget2, fileinfolist(filelist2[j]));
                    setItemIcon(item2, filetype);
                    if(diff) item2->setForeground(0, QColor(0, 162, 232));//蓝色
                    QApplication::processEvents();
                    i++;
                    j++;
                }
            }
        }
        qDebug()<<"输出结束"<<time.elapsed()/1000.0<<"s";
        filetype++;
    }
}

void FolderDiff::scrolbar1Moved(int value)
{  //鼠标按住滚动条移动才会调用
    scrolbar2->setValue(value);
}

void FolderDiff::scrolbar2Moved(int value)
{
    scrolbar1->setValue(value);
}
