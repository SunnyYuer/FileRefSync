#ifndef FOLDERDIFF_H
#define FOLDERDIFF_H

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

private:
    Ui::FolderDiff *ui;
};

#endif // FOLDERDIFF_H
