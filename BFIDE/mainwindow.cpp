#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(1000, 750);

    openFileAction = new QAction(tr("Open File"));
    runCodeAction = new QAction(tr("Run"));
    newFileAction = new QAction(tr("New File"));
    saveFileAction = new QAction(tr("Save File"));

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
    this->setMenuBar(menuBar);


    CodeEditor *textEdit = new CodeEditor();
    textEdit->setFont(QFont("Courier", 11));
    textEdit->setStyleSheet("QPlainTextEdit { background-color: white; color: grey;}");

    editorsTabWidget = new QTabWidget();
    editorsTabWidget->setTabsClosable(true);
    editorsTabWidget->addTab(textEdit, "Begin");


    BFHighlighter *highlighter = new BFHighlighter(textEdit->document());

    editors.append({textEdit, highlighter, ""});

    layout = new QHBoxLayout();
    layout->addWidget(editorsTabWidget);

    connect(runCodeAction, &QAction::triggered, this, \
            [this] {runCode(editors[editorsTabWidget->currentIndex()].fileName);});
    connect(openFileAction, &QAction::triggered, this, [this] {openFile();});
    connect(newFileAction, &QAction::triggered, this, [this] {newFile();});
    connect(saveFileAction, &QAction::triggered, this, [this] \
    {saveFile();});


    closeTabShortcut = new QShortcut(QKeySequence(tr("Ctrl+W")), this, \
                                     [this] {editorsTabWidget->removeTab(editorsTabWidget->currentIndex());});

    QWidget *w = new QWidget();
    w->setLayout(layout); setCentralWidget(w);
}

MainWindow::~MainWindow()
{
}

void MainWindow::runCode(QString fileName) {
    if(saveCodeToFile(fileName))
        return;

    pid_t pid = fork();
    if (pid == -1) {
        error("Can't clone process for terminal opening");
        return;
    }
    if (!pid) {
        QString command = "brainfuck " + fileName + ";read  -n 1 -p";
        if(execlp("terminator", "terminator", "-e",  command.toStdString().c_str(), NULL) == -1) {
            error("Can't run code via terminal");
            return;
        }
    }
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

void MainWindow::addNewEditor(QFile &file) {
    QString fileName = file.fileName();
    if (editors[editorsTabWidget->currentIndex()].fileName!= fileName) {
        CodeEditor *text = new CodeEditor;
        text->setPlainText(file.readAll());
        BFHighlighter *highlighter = new BFHighlighter(text->document());
        editors.append({text, highlighter, fileName});
        editorsTabWidget->addTab(text, QFileInfo(fileName).baseName());
        editorsTabWidget->setCurrentIndex(editors.size()-1);
    }
    file.close();
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
