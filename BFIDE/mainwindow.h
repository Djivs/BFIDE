#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>
#include <errno.h>

#include <QMainWindow>
#include <QMenuBar>
#include <QDebug>
#include <QAction>
#include <QActionGroup>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QTabWidget>
#include <QShortcut>
#include <QSettings>

#include "bfhighlighter.h"
#include "codeeditor.h"
#include "settingswidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QMenuBar *menuBar;

    QMenu *fileActionsMenu;

    QAction *openFileAction;
    QAction *runCodeAction;
    QAction *newFileAction;
    QAction *saveFileAction;
    QAction *settingsAction;

    QShortcut *closeTabShortcut;

    QTabWidget *editorsTabWidget;

    struct Editor {
        CodeEditor *edit;
        BFHighlighter *highlighter;
        QString fileName;
    };

    QVector <Editor> editors;

    SettingsWidget *settingsWidget;

    QHBoxLayout *layout;

    QString compilerPath;

    QSettings settings;


    void openFile();
    bool openQFile(QFile &file, QIODeviceBase::OpenMode mode);
    void newFile();
    void saveFile();
    int saveCodeToFile(QString fileName);

    void loadSettings();
    void openSettings();
    void processChanges();

    void addNewEditor(QFile &file);

    void runCode(QString fileName);

    void error(QString errorString);
};
#endif // MAINWINDOW_H
