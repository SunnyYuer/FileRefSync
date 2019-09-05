#include "folderdiff.h"
#include "ui_folderdiff.h"

FolderDiff::FolderDiff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderDiff)
{
    ui->setupUi(this);
}

FolderDiff::~FolderDiff()
{
    delete ui;
}
