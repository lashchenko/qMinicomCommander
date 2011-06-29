#include "settingsdialog.h"
#include <QtGui>

#include <QDebug>


SettingsItem::SettingsItem(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout  = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout;

    hl->addWidget(&path);
    hl->addWidget(&selector);
    layout->addLayout(hl);

    setLayout(layout);

    connect(&selector, SIGNAL(clicked()), this, SLOT(selectPath()));
}

void SettingsItem::selectPath()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"),QDir::homePath(), tr("Text files (*)"));
    if( !fileName.isEmpty() ) {
        path.setText(fileName);
    }
}

void SettingsItem::setDefault(QString fileName)
{
    defaultFileName = fileName;
    selector.setText(fileName);
}

QString SettingsItem::getDefault() const
{
    return defaultFileName;
}

void SettingsItem::setPath(QString text)
{
    path.setText(text);
}

QString SettingsItem::getPath() const
{
    return path.text();
}

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    items.insert(COLORS, new SettingsItem());
    items.value(COLORS)->setDefault("colors.cfg");

    items.insert(COMMANDS, new SettingsItem());
    items.value(COMMANDS)->setDefault("commands.cfg");

    items.insert(REGEXP, new SettingsItem());
    items.value(REGEXP)->setDefault("regexp.cfg");

    items.insert(PALETTE, new SettingsItem());
    items.value(PALETTE)->setDefault("palette.cfg");

    items.insert(OUT, new SettingsItem());
    items.value(OUT)->setDefault("out.txt");


    QVBoxLayout *layout  = new QVBoxLayout();
    QHBoxLayout *hl;

    const QList<int> keys = items.keys();
    foreach (int key, keys) {
        hl = new QHBoxLayout();
        hl->addWidget(items.value(key));
        layout->addLayout(hl);
    }

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

QString SettingsDialog::getValue(int key) const
{
    return items.value(key)->getPath();
}

void SettingsDialog::saveSettings()
{
    QSettings settings(tr("lashchenko.blogspot.com"), tr("qMinicomCommander"));

    const QList<int> keys = items.keys();
    foreach (int key, keys) {
        settings.setValue(tr("%1").arg(key), items.value(key)->getPath());
    }

    hide();

    emit save();
}

void SettingsDialog::readSettings()
{
    QSettings settings(tr("lashchenko.blogspot.com"), tr("qMinicomCommander"));

    const QList<int> keys = items.keys();
    foreach (int key, keys) {
        QString value = settings.value(tr("%1").arg(key), QDir::homePath() + "/" + items.value(key)->getDefault()).toString();
        if( value.isEmpty() ) {
            value = QDir::homePath() + "/" + items.value(key)->getDefault();
        }
        items.value(key)->setPath(value);
    }
}

