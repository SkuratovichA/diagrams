#ifndef EDITORTAB_H
#define EDITORTAB_H

#include <QWidget>

namespace Ui {
class editorTab;
}

class editorTab : public QWidget
{
    Q_OBJECT

public:
    explicit editorTab(QWidget *parent = nullptr);
    ~editorTab();

private:
    Ui::editorTab *ui;
};

#endif // EDITORTAB_H
