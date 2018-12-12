#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uchardet/uchardet.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColor>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scrolbar1 = ui->plainTextEdit1->verticalScrollBar();
    scrolbar2 = ui->plainTextEdit2->verticalScrollBar();
    connect(static_cast<QWidget*>(scrolbar1), SIGNAL(sliderMoved(int)), this,  SLOT(scrolbar1Moved(int)));
    connect(static_cast<QWidget*>(scrolbar2), SIGNAL(sliderMoved(int)), this,  SLOT(scrolbar2Moved(int)));
    ui->label3->setText("");
    ui->label4->setText("");
    time.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showtext(QString s)
{
    QMessageBox msg(this);
    msg.setWindowTitle("提示");
    msg.setText(s);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void MainWindow::showtext(int n)
{
    QMessageBox msg(this);
    msg.setWindowTitle("提示");
    msg.setText(QString::number(n));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void MainWindow::showtext(double n)
{
    QMessageBox msg(this);
    msg.setWindowTitle("提示");
    msg.setText(QString::number(n));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void MainWindow::on_button1_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);
    QString lastPath = setting.value("File1Path").toString();
    if(lastPath.isEmpty()) lastPath = "./";
    QString s = QFileDialog::getOpenFileName(this,"选择文件",lastPath,"files(*.*)");
    ui->lineEdit1->setText(s);
    if(!s.isEmpty()) setting.setValue("File1Path",s);
}

void MainWindow::on_button2_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);
    QString lastPath = setting.value("File2Path").toString();
    if(lastPath.isEmpty()) lastPath = "./";
    QString s = QFileDialog::getOpenFileName(this,"选择文件",lastPath,"files(*.*)");
    ui->lineEdit2->setText(s);
    if(!s.isEmpty()) setting.setValue("File2Path",s);
}

const char *MainWindow::detectEncoding(QString fname)
{//使用开源代码uchardet
    uchardet_t  handle = uchardet_new();
    const char *charset = "";
    char buffer[65536];

    QByteArray fba = fname.toLocal8Bit();
    char *filename = fba.data();
    //qDebug()<<filename;
    FILE *fp = fopen(filename, "r");

    while (!feof(fp))
    {
        size_t len = fread(buffer, 1, 65536, fp);
        int retval = uchardet_handle_data(handle, buffer, len);
        if (retval != 0)
        {
            showtext("uchardet错误");
            return charset;
        }
    }
    fclose(fp);
    uchardet_data_end(handle);

    charset = strdup(uchardet_get_charset(handle));

    uchardet_delete(handle);

    return charset;
}

int MainWindow::getfilelist()
{
    QString s1 = ui->lineEdit1->text();
    QFile file1(s1);
    if(!file1.exists())
    {
        showtext("文件1不存在");
        return 0;
    }
    QString s2 = ui->lineEdit2->text();
    QFile file2(s2);
    if(!file2.exists())
    {
        showtext("文件2不存在");
        return 0;
    }

    if(!file1.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;

    const char *file1code = detectEncoding(s1);
    const char *file2code = detectEncoding(s2);
    ui->label3->setText(file1code);
    ui->label4->setText(file2code);

    QTextStream in1(&file1);
    in1.setCodec(file1code);
    QString all1 = in1.readAll();
    filelist1 = all1.split("\n");

    QTextStream in2(&file2);
    in2.setCodec(file2code);
    QString all2 = in2.readAll();
    filelist2 = all2.split("\n");

    file1.close();
    file2.close();
    return 1;
}

void MainWindow::setTextColor(int texteditnum, const QColor &color)
{
    QTextCharFormat fmt;//文本字符格式
    fmt.setBackground(color);// 设为color色
    if(texteditnum == 1) ui->plainTextEdit1->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
    if(texteditnum == 2) ui->plainTextEdit2->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
}

void MainWindow::hilightLines(int texteditnum, const QColor &color, int &emptyhl, int rep)
{
    QList<QTextEdit::ExtraSelection> extraSelections;//提供一种方式显示选择的文本
    int currentline;
    if(texteditnum == 1) extraSelections = ui->plainTextEdit1->extraSelections();
    if(texteditnum == 2) extraSelections = ui->plainTextEdit2->extraSelections();
    if(emptyhl!=-1)
    {
        QTextEdit::ExtraSelection sc;
        sc.format.setBackground(color);
        sc.format.setProperty(QTextFormat::FullWidthSelection, true);
        if(texteditnum == 1) sc.cursor = ui->plainTextEdit1->textCursor();
        if(texteditnum == 2) sc.cursor = ui->plainTextEdit2->textCursor();
        currentline = sc.cursor.blockNumber();
        sc.cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, currentline-emptyhl);
        extraSelections.append(sc);
        emptyhl = -1;
    }
    if(rep == 0)//补红绿行就不运行
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(color);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        if(texteditnum == 1) selection.cursor = ui->plainTextEdit1->textCursor();
        if(texteditnum == 2) selection.cursor = ui->plainTextEdit2->textCursor();
        currentline = selection.cursor.blockNumber();
        if(selection.cursor.columnNumber()!=0)
        {
            selection.cursor.movePosition(QTextCursor::StartOfLine);
            extraSelections.append(selection);
        }
        else emptyhl = currentline;
    }
    if(texteditnum == 1) ui->plainTextEdit1->setExtraSelections(extraSelections);//设置高亮
    if(texteditnum == 2) ui->plainTextEdit2->setExtraSelections(extraSelections);//设置高亮
}

QMap<int, int> MainWindow::lcs()
{
    QMap<int, int> matchmap;
    QString linetext1;
    QString linetext2;
    int M = filelist1.length();
    int N = filelist2.length();
    int **array;
    array = new int *[M];
    for (int i = 0; i < M; i++)
    {
        array[i] = new int[N];
    }

    int i=0, j=0;
    while(j!=N)
    {//lcs算法开始
        linetext2 = filelist2.at(j);
        while(i!=M)
        {
            linetext1 = filelist1.at(i);
            if(linetext1 == linetext2)
            {
                if(i == 0 || j == 0) array[i][j] = 1;
                else array[i][j] = array[i-1][j-1] + 1;
            }
            else
            {
                if(i == 0 && j==0) array[i][j] = 0;
                else if(i == 0) array[i][j] = array[i][j-1];
                else if(j == 0) array[i][j] = array[i-1][j];
                else array[i][j] = (array[i-1][j]>=array[i][j-1])?array[i-1][j]:array[i][j-1];
            }
            i++;
        }
        i = 0;
        j++;
    }

    i = M-1;
    j = N-1;
    int maxmatch = array[i][j];
    while(maxmatch > 0)
    {//开始回溯
        while(j >= 0)
        {
            j--;
            if(j==-1 || array[i][j]<maxmatch)
            {
                j++;
                while(i >= 0)
                {
                    i--;
                    if(i==-1 || array[i][j]<maxmatch)
                    {
                        i++;
                        matchmap.insert(i, j);
                        maxmatch--;
                        break;
                    }
                }
                break;
            }
        }
        i--;j--;
    }

    for (int i = 0; i < M; i++)
    {
        delete[]array[i];

    }
    delete[]array;

    return matchmap;
}

QMap<int, int> MainWindow::lcsx()
{//采用自主改进的LCSX算法
    QMap<int, int> matchmap;
    QString linetext1;
    QString linetext2;
    QStringList flist1;
    QStringList flist2;
    int M,N;
    if(filelist1.length() >= filelist2.length())
    {
        M = filelist1.length();
        N = filelist2.length();
        flist1 = filelist1;
        flist2 = filelist2;
    }
    else
    {
        M = filelist2.length();
        N = filelist1.length();
        flist1 = filelist2;
        flist2 = filelist1;
    }
    int **array;//M>=N
    array = new int *[M];
    for (int i = 0; i < M; i++)
    {
        array[i] = new int[N];
    }
    qDebug()<<"初始化开始"<<time.elapsed()/1000.0<<"s";
    for (int i = 0; i < M; i++)
    {
        for(int j = 0; j < N; j++)
        {//全部初始化为-1
            array[i][j] = -1;
        }
    }
    qDebug()<<"初始化结束"<<time.elapsed()/1000.0<<"s";

    double rate = 1.0*N/M;
    int del = (M==N)?1:0;//rate为1时，next要减1
    int next;
    int i=0, j=0;
    int itmp=0, jtmp=0;
    int maxmatch = 0, matchadd = 0;
    int locai = 0, locaj = 0;
    qDebug()<<"算法开始"<<time.elapsed()/1000.0<<"s";
    while(i!=M || j!=N)
    {//lcsx算法开始
        itmp = i;
        jtmp = j;
        while(array[i][j] == -1)
        {
            linetext1 = flist1.at(i);
            linetext2 = flist2.at(j);
            if(i == maxmatch) locai = i+1;
            if(j == maxmatch) locaj = j+1;
            if(linetext1 == linetext2)
            {
                if(i == 0 || j == 0) array[i][j] = 1;
                else array[i][j] = array[i-1][j-1] + 1;
                if(array[i][j] > maxmatch) matchadd = 1;
            }
            else
            {
                if(i == 0 && j==0) array[i][j] = 0;
                else if(i == 0) array[i][j] = array[i][j-1];
                else if(j == 0) array[i][j] = array[i-1][j];
                else array[i][j] = (array[i-1][j]>=array[i][j-1])?array[i-1][j]:array[i][j-1];
            }
            next = static_cast<int>(rate*(i+1)) - del;//下一行要算到的位置
            if(next <= maxmatch) break;
            else
            {
                i = next-1;
                j++;
                if(j == N) break;
            }
        }
        if(matchadd == 1)
        {//匹配数增加了，前maxmatch行列补余格子
            maxmatch++;
            for (; locai < M; locai++)
            {
                if(array[locai][maxmatch-1] == -1) array[locai][maxmatch-1] = array[locai-1][maxmatch-1];
            }
            for (; locaj < N; locaj++)
            {
                if(array[maxmatch-1][locaj] == -1) array[maxmatch-1][locaj] = array[maxmatch-1][locaj-1];
            }
            matchadd = 0;
        }
        if(itmp+1 == M || jtmp < maxmatch)
        {//这行算满了   或者    这行补余了
         //跳到下一行
            if(jtmp+1 != N) itmp = static_cast<int>(rate*(itmp+1)) - del - 1;
            else itmp = M-1;
            jtmp++;
        }
        i = itmp+1;
        j = jtmp;
    }
    qDebug()<<"算法结束"<<time.elapsed()/1000.0<<"s";

    qDebug()<<"回溯开始"<<time.elapsed()/1000.0<<"s";
    i--;j--;
    while(maxmatch > 0)
    {//开始回溯
        while(j >= 0)
        {
            j--;
            if(j==-1 || array[i][j]<maxmatch)
            {
                j++;
                while(i >= 0)
                {
                    i--;
                    if(i==-1 || array[i][j]<maxmatch)
                    {
                        i++;
                        if(filelist1.length() >= filelist2.length()) matchmap.insert(i, j);
                        else matchmap.insert(j, i);
                        maxmatch--;
                        break;
                    }
                }
                break;
            }
        }
        i--;j--;
    }
    qDebug()<<"回溯结束"<<time.elapsed()/1000.0<<"s";

    for (int i = 0; i < M; i++)
    {
        delete[]array[i];

    }
    delete[]array;

    return matchmap;
}

void MainWindow::on_btncon_clicked()
{
    qDebug()<<"获取filelist开始"<<time.elapsed()/1000.0<<"s";
    if(!getfilelist()) return;
    qDebug()<<"获取filelist结束"<<time.elapsed()/1000.0<<"s";

    ui->plainTextEdit1->setStartPaint(0);
    ui->plainTextEdit2->setStartPaint(0);//要在clear之前
    ui->plainTextEdit1->clear();
    ui->plainTextEdit2->clear();
    ui->plainTextEdit1->clearGreyLine();
    ui->plainTextEdit2->clearGreyLine();
    QApplication::processEvents();//让程序处理那些还没有处理的事件，然后再把使用权返回给调用者

    int i=0, j=0;
    int delline = 0; //file1删掉的行数
    int addline = 0; //file2增加的行数
    QString linetext1;
    QString linetext2;
    int emptyhl1 = -1;//空行高亮位置记录
    int emptyhl2 = -1;//空行高亮位置记录
    setTextColor(1, QColor(255, 255, 255));//白色
    setTextColor(2, QColor(255, 255, 255));//白色

    qDebug()<<"lcsx开始"<<time.elapsed()/1000.0<<"s";
    QMap<int, int> matchmap = lcsx();
    qDebug()<<"lcsx结束"<<time.elapsed()/1000.0<<"s";

    matchmap.insert(filelist1.length(), filelist2.length());

    qDebug()<<"输出开始"<<time.elapsed()/1000.0<<"s";
    ui->plainTextEdit1->setStartPaint(1);
    ui->plainTextEdit2->setStartPaint(1);
    QMap<int, int>::iterator it;
    for (it = matchmap.begin(); it != matchmap.end(); it++)
    {//map会自动按key升序排序
        if(i!=filelist1.length()) linetext1 = filelist1.at(i);
        if(j!=filelist2.length()) linetext2 = filelist2.at(j);
        if(i < it.key())
        {
            while(i < it.key())
            {
                ui->plainTextEdit1->appendPlainText(linetext1);
                hilightLines(1, QColor(255, 210, 210), emptyhl1, 0);//红色
                delline++;
                i++;
                if(i == filelist1.length()) break;
                linetext1 = filelist1.at(i);
            }
        }
        if(i == it.key())
        {
            if(j < it.value())
            {
                while(j < it.value())
                {
                    ui->plainTextEdit2->appendPlainText(linetext2);
                    hilightLines(2, QColor(210, 255, 210), emptyhl2, 0);//绿色
                    addline++;
                    j++;
                    if(j == filelist2.length()) break;
                    linetext2 = filelist2.at(j);
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
                ui->plainTextEdit1->appendPlainText(" ");
                if(emptyhl1 != -1) hilightLines(1, QColor(255, 210, 210), emptyhl1, 1);//红色
                hilightLines(1, QColor(230, 230, 230), emptyhl1, 0);//灰色
                ui->plainTextEdit1->addGreyLine();
                addline--;
            }
            while(delline > 0)
            {
                ui->plainTextEdit2->appendPlainText(" ");
                if(emptyhl2 != -1) hilightLines(2, QColor(210, 255, 210), emptyhl2, 1);//绿色
                hilightLines(2, QColor(230, 230, 230), emptyhl2, 0);//灰色
                ui->plainTextEdit2->addGreyLine();
                delline--;
            }
            if(j == it.value() && j != filelist2.length())
            {
                ui->plainTextEdit1->appendPlainText(linetext1);
                ui->plainTextEdit2->appendPlainText(linetext2);
                //QApplication::processEvents();//实时显示，特别费时
                i++;
                j++;
            }
            if(emptyhl1 != -1) hilightLines(1, QColor(255, 210, 210), emptyhl1, 1);//红色
            if(emptyhl2 != -1) hilightLines(2, QColor(210, 255, 210), emptyhl2, 1);//绿色
        }
    }
    qDebug()<<"输出结束"<<time.elapsed()/1000.0<<"s";
}

void MainWindow::scrolbar1Moved(int value)
{  //鼠标按住滚动条移动才会调用
    scrolbar2->setValue(value);
}

void MainWindow::scrolbar2Moved(int value)
{
    scrolbar1->setValue(value);
}
