#ifndef EDITORINTERFACE_H
#define EDITORINTERFACE_H

#include <QMainWindow>

namespace Ui {
class editorInterface;
}

class editorInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit editorInterface(QWidget *parent = nullptr);
    ~editorInterface();

    //void loadFile(const QString &filename);

private:
    Ui::editorInterface *ui;
};

#endif // EDITORINTERFACE_H
