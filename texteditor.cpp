#include "texteditor.h"
#include <QtWidgets>

TextEditor::TextEditor(QWidget *parent) :
    QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);
}

void TextEditor::setPaintState(int sp)
{//0未开始，1开始，2完成
    paintstate = sp;
}

int TextEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {//计算几个数字
        max /= 10;
        ++digits;
    }
    if(paintstate == 0) digits = 0;
    #if (QT_VERSION >= QT_VERSION_CHECK(5,11,0)) //QT5.11版本之后才有horizontalAdvance
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    #else
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    #endif
    return space;
}

void TextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);//设置边距
}

void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void TextEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

    if(paintstate == 1) verticalScrollBar()->setSliderPosition(verticalScrollBar()->maximum());
}

void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(235, 235, 235));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int actualnumber = blockNumber - beforegl(blockNumber) + 1;
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top() && paintstate >= 1)
        {
            QString number = QString::number(actualnumber);
            if(!greyline.contains(blockNumber + 1))
            {
                painter.setPen(QColor(120, 120, 120));
                painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                 Qt::AlignRight, number);
                actualnumber++;
            }
        }
        if(bottom < event->rect().top())
        {
            if(!greyline.contains(blockNumber + 1)) actualnumber++;
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        blockNumber++;
    }
}

void TextEditor::addGreyLine()
{
    greyline.append(blockCount());
}

void TextEditor::clearGreyLine()
{
    greyline.clear();
}

int TextEditor::beforegl(int blocknumber)
{//计算这行之前有多少灰行
    int gl = 0;
    QList<int>::iterator i;
    for (i = greyline.begin(); i != greyline.end(); i++)
    {
      if(*(i) < blocknumber+1) gl++;
    }
    return gl;
}

