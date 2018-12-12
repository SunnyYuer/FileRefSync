#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TextEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void setStartPaint(int sp);
    void addGreyLine();
    void clearGreyLine();
    int beforegl(int blocknumber);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;
    int startpaint = 0;
    QList<int> greyline;
};

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    LineNumberArea(TextEditor *editor) : QWidget(editor)
    {
        textEditor = editor;
    }

    QSize sizeHint() const override
    {
        return QSize(textEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        textEditor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEditor *textEditor;
};

#endif // TEXTEDITOR_H
