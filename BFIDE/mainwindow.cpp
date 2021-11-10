#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(1000, 750);

    openFileAction = new QAction(tr("Open File"));
    runCodeAction = new QAction(tr("Run"));
    newFileAction = new QAction(tr("New File"));
    saveFileAction = new QAction(tr("Save File"));
    settingsAction = new QAction(tr("Settings"));

    openFileAction->setShortcut(tr("Ctrl+O"));
    runCodeAction->setShortcut(tr("F5"));
    newFileAction->setShortcut(tr("Ctrl+N"));
    saveFileAction->setShortcut(tr("Ctrl+S"));

    fileActionsMenu = new QMenu(tr("&File"));
    fileActionsMenu->addAction(openFileAction);
    fileActionsMenu->addAction(newFileAction);
    fileActionsMenu->addAction(saveFileAction);

    menuBar = new QMenuBar;
    menuBar->addMenu(fileActionsMenu);
    menuBar->addAction(runCodeAction);
    menuBar->addAction(settingsAction);
    this->setMenuBar(menuBar);


    CodeEditor *textEdit = new CodeEditor();
    textEdit->setFont(QFont("Courier", 11));
    textEdit->setStyleSheet("QPlainTextEdit { background-color: white; color: grey;}");

    editorsTabWidget = new QTabWidget();
    editorsTabWidget->setTabsClosable(true);
    editorsTabWidget->addTab(textEdit, "Begin");
    connect(editorsTabWidget->tabBar(), &QTabBar::tabCloseRequested, this, &MainWindow::removeTab);

    BFHighlighter *highlighter = new BFHighlighter(textEdit->document());

    editors.append({textEdit, highlighter, ""});

    layout = new QHBoxLayout();
    layout->addWidget(editorsTabWidget);

    connect(runCodeAction, &QAction::triggered, this, \
            [this] {runCode(editors[editorsTabWidget->currentIndex()].fileName);});
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::openSettings);


    closeTabShortcut = new QShortcut(QKeySequence(tr("Ctrl+W")), this, \
                                     [this] {editorsTabWidget->removeTab(editorsTabWidget->currentIndex());});

    loadSettings();

    settingsWidget = new SettingsWidget;
    settingsWidget->setWindowFlag(Qt::WindowStaysOnTopHint);
    connect (settingsWidget, &SettingsWidget::lineEditsSettingFailed, this, [this] {error("Invalid parameters passed for QLineEdits values");});
    connect(settingsWidget, &SettingsWidget::changesSaved, this, &MainWindow::processChanges);

    QWidget *w = new QWidget();
    w->setLayout(layout); setCentralWidget(w);
}

MainWindow::~MainWindow()
{
}

void MainWindow::runCode(QString fileName) {
    if (fileName == "") {
        saveFile();
        fileName = editors[editorsTabWidget->currentIndex()].fileName;
    }
    if(saveCodeToFile(fileName)) {
        return;
    }

    QString command = compilerPath + ' ' + fileName + ";read  -n 1 -p";
    QStringList arguments = execArguments;
    arguments.append(command);

    QProcess *process = new QProcess;
    process->start(terminalPath, arguments);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "/home", tr("BrainFuck files (*.txt *.bf)"));
    if(fileName.isEmpty()) {
        error("File is not chosen");
        return;
    }
    QFile file(fileName);
    if(openQFile(file, QFile::ReadOnly))
        return;

    addNewEditor(file);
}

void MainWindow::newFile() {
    QString strFile = QFileDialog::getSaveFileName(NULL, "Create New File","/home","BrainFuck files (*.txt *.bf)");
    QFile file(strFile);
    if(openQFile(file, QFile::WriteOnly))
        return;
    addNewEditor(file);
}

int MainWindow::saveCodeToFile(QString fileName) {
    if (fileName.isEmpty())
        fileName = QDir::currentPath() + "/buf.bf";


    QFile file(fileName);
    if(openQFile(file, QFile::WriteOnly))
        return 1;
    file.write(editors[editorsTabWidget->currentIndex()].edit->toPlainText().toLatin1());
    file.close();
    return 0;
}

void MainWindow::saveFile() {
    QString currentFileName = editors[editorsTabWidget->currentIndex()].fileName;
    if (currentFileName.isEmpty()) {
        QString strFile = QFileDialog::getSaveFileName(NULL, "Create New File","/home","BrainFuck files (*.txt *.bf)");
        QFile file(strFile);
        if(openQFile(file, QFile::WriteOnly))
            return;
        editors[editorsTabWidget->currentIndex()].fileName = strFile;
        editorsTabWidget->setTabText(editorsTabWidget->currentIndex(), QFileInfo(file).baseName());
        file.write(editors[editorsTabWidget->currentIndex()].edit->toPlainText().toStdString().c_str());
    } else {
        QFile file(currentFileName);
        if(openQFile(file, QFile::WriteOnly))
            return;
        file.write(editors[editorsTabWidget->currentIndex()].edit->toPlainText().toStdString().c_str());
    }

}

void MainWindow::loadSettings() {
    compilerPath = settings.value("MainWindow/compilerPath").toString();
    if (compilerPath.isEmpty()) {
        compilerPath = "/usr/bin/brainfuck";
        settings.setValue("MainWindow/compilerPath", compilerPath);
    }

    terminalPath = settings.value("MainWindow/terminalPath").toString();
    if (terminalPath.isEmpty()) {
        terminalPath = "/usr/bin/xterm";
        settings.setValue("MainWindow/terminalPath", terminalPath);
    }

    execCommand = settings.value("MainWindow/execCommand").toString();
    if (execCommand.isEmpty()) {
        execCommand = "{terminal} -e {command}";
        settings.setValue("MainWindow/execCommand", execCommand);
    }
    fillExecArguments();
}

void MainWindow::openSettings() {
    settingsWidget->setLineEditsValues({compilerPath, terminalPath, execCommand});
    settingsWidget->show();
}

void MainWindow::processChanges() {
    QStringList changedLineEditsValues = settingsWidget->getChangedEditsValues();
    QVector <int> changedIndexes = settingsWidget->getChangedEditsIndexes();
    QFile file;
    for (int i = 0; i < changedIndexes.length(); ++i) {
        switch (changedIndexes[i]) {
            case 0:
                file.setFileName(changedLineEditsValues[i]);
                if (!file.open(QFile::ReadOnly) || QFileInfo(file).completeSuffix() != "") {
                    error("Указанный путь для компилятора не существует\nили указанный файл не является исполняемым");
                    return;
                }
                compilerPath = changedLineEditsValues[i];
                settings.setValue("MainWindow/compilerPath", compilerPath);
            break;
            case 1:
            file.setFileName(changedLineEditsValues[i]);
            if (!file.open(QFile::ReadOnly) || QFileInfo(file).completeSuffix() != "") {
                error("Указанный путь для терминала не существует\nили указанный файл не является исполняемым");
                return;
            }
            terminalPath = changedLineEditsValues[i];
            settings.setValue("MainWindow/terminalPath", terminalPath);
            break;
        case 2:
            QString command = changedLineEditsValues[i];
            int bracketsCount = 0;
            for (int ii = 0; ii < command.length(); ++ii) {
                if (command[ii] == '{') {
                    ++bracketsCount;
                    int j = ii+ 1;
                     for (;j < command.length() && command[j] != '}'; ++j) {}
                    if (j == command.length()) {
                        error("Задана некорректная команда для запуска кода в терминале");
                        return;
                    }
                }
            }
            if (bracketsCount != 2) {
                error("Invalid command to run code");
                return;
            }
            execCommand = command;
            settings.setValue("MainWindow/execCommand", execCommand);

            fillExecArguments();


            break;
        }
    }
}

void MainWindow::fillExecArguments() {
    execArguments.clear();
    int left = execCommand.indexOf('}'), right = execCommand.lastIndexOf('{');
    execArguments = execCommand.mid(left + 1, right - left - 1).split(' ');
    execArguments.removeAll("");
}

void MainWindow::addNewEditor(QFile &file) {
    QString fileName = file.fileName();
    CodeEditor *text = new CodeEditor;
    text->setPlainText(file.readAll());
    BFHighlighter *highlighter = new BFHighlighter(text->document());
    editors.append({text, highlighter, fileName});
    editorsTabWidget->addTab(text, QFileInfo(fileName).baseName());
    editorsTabWidget->setCurrentIndex(editors.size() - 1);
    file.close();
}

void MainWindow::removeTab(int index) {
    int startingIndex = editorsTabWidget->currentIndex();
    if (editorsTabWidget->count() < 2)
        return;
    editors.remove(index);
    editorsTabWidget->clear();
    for (auto &i: editors) {
        editorsTabWidget->addTab(i.edit, QFileInfo(i.fileName).baseName());
    }
    if (startingIndex == editors.length()) --startingIndex;
    editorsTabWidget->setCurrentIndex(startingIndex);
}


void MainWindow::error(QString errorString) {
    QString messageText = errorString + "\nSystem output:\n" + QString(strerror(errno));
    QMessageBox box(QMessageBox::Icon::Warning, "Error!", messageText, QMessageBox::Ok);
    box.exec();
}

bool MainWindow::openQFile(QFile &file, QIODeviceBase::OpenMode mode) {
    if (!file.open(mode)) {
        error("Can't open file (" + file.fileName() +')');
        return 1;
    }
    return 0;

}
