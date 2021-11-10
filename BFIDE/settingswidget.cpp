#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumWidth(400);
    layout = new QGridLayout();
    layout->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < LINEEDITSOPTIONSSIZE; ++i) {
        QLineEdit *edit = new QLineEdit;
        connect(edit, &QLineEdit::textEdited, this, [this, i] {isLineEditsChanged[i] = true; saveButton->setEnabled(true);});
        layout->addWidget(new QLabel(lineEditsTitles[i] + ':'), i, 0);
        layout->addWidget(edit, i, 1);
        lineEdits[i] = edit;
    }

    saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, this, &SettingsWidget::saveChanges);
    saveButton->setEnabled(false);

    layout->addWidget(saveButton, LINEEDITSOPTIONSSIZE, 0, 1, 2);

    setLayout(layout);

    for (int i = 0; i < LINEEDITSOPTIONSSIZE; ++i) {
        isLineEditsChanged[i] = false;
    }
}

void SettingsWidget::saveChanges() {
    changedEditsIndexes.clear();
    changedEditsValues.clear();
    int changesCount = 0;
    for (int i = 0; i < LINEEDITSOPTIONSSIZE; ++i) {
        if (isLineEditsChanged[i]) {
            changedEditsIndexes.append(i);
            changedEditsValues.append(lineEdits[i]->text());
            ++changesCount;
        }
    }
    emit changesSaved();
    saveButton->setEnabled(false);
}

void SettingsWidget::setLineEditsValues(QStringList values) {
    if (values.length() != LINEEDITSOPTIONSSIZE) {
        emit lineEditsSettingFailed();
        return;
    }
    for (int i = 0; i < values.length(); ++i) {
        lineEdits[i]->setText(values[i]);
    }
}
