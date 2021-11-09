#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

enum {COMPILERPATH, LINEEDITSOPTIONSSIZE};

class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

    void setLineEditsValues(QStringList values);

    QStringList getChangedEditsValues() {return changedEditsValues;}
    QVector <int> getChangedEditsIndexes() {return changedEditsIndexes;}
private:
    QString lineEditsTitles[LINEEDITSOPTIONSSIZE] = {"Compiler path"};
    QLineEdit *lineEdits[LINEEDITSOPTIONSSIZE];
    QPushButton *saveButton;

    QStringList changedEditsValues;
    QVector <int> changedEditsIndexes;


    QGridLayout *layout;

    bool isLineEditsChanged[LINEEDITSOPTIONSSIZE] = {};

    void saveChanges();
signals:
    void changesSaved();

    void lineEditsSettingFailed();

};

#endif // SETTINGSWIDGET_H
