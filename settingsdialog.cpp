#include "settingsdialog.h"
#include <QtGui>

#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout  = new QVBoxLayout();
    QHBoxLayout *hl;

    hl = new QHBoxLayout;
    buttonColors.setText(tr("colors.cfg"));
    connect(&buttonColors, SIGNAL(clicked()), this, SLOT(selectColors()));
    hl->addWidget(&pathToColors);
    hl->addWidget(&buttonColors);
    layout->addLayout(hl);

    hl = new QHBoxLayout;
    buttonCommands.setText(tr("commands.cfg"));
    connect(&buttonCommands, SIGNAL(clicked()), this, SLOT(selectCommands()));
    hl->addWidget(&pathToCommands);
    hl->addWidget(&buttonCommands);
    layout->addLayout(hl);

    hl = new QHBoxLayout;
    buttonOut.setText(tr("out"));
    connect(&buttonOut, SIGNAL(clicked()), this, SLOT(selectOut()));
    hl->addWidget(&pathToOut);
    hl->addWidget(&buttonOut);
    layout->addLayout(hl);

    hl = new QHBoxLayout;
    QPushButton *save = new QPushButton(tr("save settings"));
    connect(save, SIGNAL(clicked()), this, SLOT(saveSettings()));
    QPushButton *cancel = new QPushButton(tr("cancel"));
    connect(cancel, SIGNAL(clicked()), this, SLOT(hide()));
    hl->addStretch();
    hl->addWidget(save);
    hl->addWidget(cancel);
    hl->addStretch();
    layout->addLayout(hl);

    setLayout(layout);

    readSettings();
}

QString SettingsDialog::getValue(QString key) const
{
//    qDebug() << configs.value(key) << ":::";
    return configs.value(key);
}

void SettingsDialog::saveSettings()
{
    QSettings settings(tr("lashchenko.blogspot.com"), tr("qMinicomCommander"));

    configs.insert(Settings::colors, pathToColors.text());
    settings.setValue(Settings::colors, configs.value(Settings::colors));

    configs.insert(Settings::commands, pathToCommands.text());
    settings.setValue(Settings::commands, configs.value(Settings::commands));

    configs.insert(Settings::out, pathToOut.text());
    settings.setValue(Settings::out, configs.value(Settings::out));

    hide();

    emit save();
}

void SettingsDialog::readSettings()
{
    QSettings settings(tr("lashchenko.blogspot.com"), tr("qMinicomCommander"));

    QString value;

    value = settings.value(Settings::colors, QDir::homePath()+"/colors.cfg").toString();
    if( value.isEmpty() ) {
        value = QDir::homePath()+"/colors.cfg";
    }
    configs.insert(Settings::colors, value);

    value = settings.value(Settings::commands, QDir::homePath()+"/commands.cfg").toString();
    if( value.isEmpty() ) {
        value = QDir::homePath()+"/commands.cfg";
    }
    configs.insert(Settings::commands, value);

    value = settings.value(Settings::out, QDir::homePath()+"/out.txt").toString();
    if( value.isEmpty() ) {
        value = QDir::homePath()+"/out.txt";
    }
    configs.insert(Settings::out, value);

    pathToColors.setText(configs.value(Settings::colors));
    pathToCommands.setText(configs.value(Settings::commands));
    pathToOut.setText(configs.value(Settings::out));
}

void SettingsDialog::selectColors()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open colors"),QDir::homePath(), tr("Config files (*.cfg)"));
    if( !fileName.isEmpty() ) {
        pathToColors.setText(fileName);
    }
}

void SettingsDialog::selectCommands()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open commands"),QDir::homePath(), tr("Config files (*.cfg)"));
    if( !fileName.isEmpty() ) {
        pathToCommands.setText(fileName);
    }
}

void SettingsDialog::selectOut()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open out"),QDir::homePath(), tr("Text files (*)"));
    if( !fileName.isEmpty() ) {
        pathToOut.setText(fileName);
    }
}





