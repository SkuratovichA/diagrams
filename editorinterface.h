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
    enum newWindowType {
        // just open a new canvas with default template
        NO_FILE,

        // open a dialogue file window to open a file and edit it
        OPEN_FILE,

        // load one of the provided examples to open
        EXAMPLE_FILE
    };

    explicit editorInterface(QWidget *parent = nullptr,
                             // name of an example
                             // Must be provided only if newType is EXAMPLE_FILE
                             QString exampleName = nullptr,

                             // default option for window is to create it
                             // without a type - just a plain canvas
                             newWindowType newType = NO_FILE
                             );
    ~editorInterface();

    //void loadFile(const QString &filename);

private slots:
    void on_actionNew_Project_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionAll_triggered();

    void on_actionAll_Objects_triggered();

    void on_actionAll_Lines_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionReset_Zoom_triggered();

    void on_actionNew_Diagram_triggered();

    void on_actionImport_diagram_triggered();

    void on_actionSave_As_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

private:
    Ui::editorInterface *ui;
    QString filename = "";
    QString filenameFilter = "Diagram Files (*.gae)";
};

#endif // EDITORINTERFACE_H
