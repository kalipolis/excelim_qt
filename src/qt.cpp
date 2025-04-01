#include "qt.h"

qt::qt(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_qt)
{
    ui->setupUi(this);
}

qt::~qt()
{
    delete ui; 
}