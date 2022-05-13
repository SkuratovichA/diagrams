/** @file main.cpp
 * @author Skuratovich Aliaksandr <xskura01\@vutbr.cz>
 * @date 30.04.2022
 */


#include "MainWIndow/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;
    w.show();
    return a.exec();
}