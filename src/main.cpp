// File: main.cpp
// Author: Skuratovich Aliaksandr <xskura01@vutbr.cz>
// Date: 24.04.2022

#include "MainWIndow/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;
    w.show();
    return a.exec();
}