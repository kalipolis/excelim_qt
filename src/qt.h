#pragma once
#include "ui_qt.h"
#include <QMainWindow>

class qt : public QMainWindow {
    Q_OBJECT
    
public:
    qt(QWidget* parent = nullptr);
    ~qt();

private:
    Ui_qt* ui;
};