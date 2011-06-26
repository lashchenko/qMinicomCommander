#include "widget.h"

#include "colorparser.h"
#include "commandparser.h"
#include "custombutton.h"

#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *button = new QPushButton("run command");
    button->setPalette(QColor("#CD853F"));;
    connect(button, SIGNAL(clicked()), this, SLOT(runCommand()));

    QVBoxLayout *vl = new QVBoxLayout;

    // set buttons-commands layout
    buttonLayout = new QHBoxLayout();
    vl->addLayout(buttonLayout);


    // set custom command layout
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(&lineEdit);
    hl->addWidget(button);
    vl->addLayout(hl);


    // set text browser
    vl->addWidget(&browser);


    // create configuration layout
    hl = new QHBoxLayout();
    hl->addWidget(new QLabel(tr("CONFIGURATION: ")));
    updateOn.setText("update [ON]");
    updateOn.setChecked(false);
    connect(&updateOn, SIGNAL(clicked(bool)), this, SLOT(updateOnOff(bool)));
    hl->addWidget(&updateOn);

    hl->addWidget(new QLabel(tr("period (in millisec)")));
    updatePeriod.setMinimum(100);
    updatePeriod.setMaximum(60000);
    updatePeriod.setSingleStep(100);
    updatePeriod.setValue(3000);
    connect(&updatePeriod, SIGNAL(valueChanged(int)), this, SLOT(updatePeriodChange(int)));
    hl->addWidget(&updatePeriod);

    QPushButton *sb = new QPushButton();
    sb->setIcon(QIcon(":/img/heart.svg"));
    connect(sb, SIGNAL(clicked()), &settings, SLOT(exec()));
    hl->addWidget(sb);

    vl->addLayout(hl);

    setLayout(vl);

    settings.setModal(true);
    connect(&settings, SIGNAL(save()), this, SLOT(updateSettings()));

    fileSize = 0;
    filePosition = 0;
    lineNumber = 0;

    createActions();
    createTrayIcon();

    trayIcon->setIcon(QIcon(":/img/q.png"));
//    trayIcon->setIcon(QIcon(":/img/heart.svg"));
    trayIcon->setVisible(true);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activated(QSystemTrayIcon::ActivationReason)));

    updateCommands();
    updateColors();


    connect(&timer, SIGNAL(timeout()), this, SLOT(updateText()));
    timer.start(updatePeriod.value());
}

Widget::~Widget()
{

}

void Widget::updateOnOff(bool checked)
{
    if( checked ) {
        updateOn.setText("Update [ON]");
        updateOn.setPalette(Qt::green);
    } else {
        updateOn.setText("Update [OFF]");
        updateOn.setPalette(Qt::red);
    }
}

void Widget::updatePeriodChange(int period)
{
    timer.stop();
    timer.start(period);
}

void Widget::activated(QSystemTrayIcon::ActivationReason reason)
{
    if( reason == QSystemTrayIcon::DoubleClick ) {
        if( isVisible() ) {
            hide();
        } else {
            showMaximized();
        }
    }
}

void Widget::showDebug(QString debug)
{
    trayIcon->showMessage("notification from Qt Proecss Commander", debug);
    std::cerr << debug.toStdString() << std::endl;
}

void Widget::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
//        QMessageBox::information(this, tr("Systray"),
//                                 tr("The program will keep running in the "
//                                    "system tray. To terminate the program, "
//                                    "choose <b>Quit</b> in the context menu "
//                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void Widget::updateColors()
{
//    colorParser = new ColorParser("/home/alashchenko/colors.cfg");
//    colorParser = new ColorParser(QDir::homePath() + "/colors.cfg");
    colorParser = new ColorParser(settings.getValue(Settings::colors));

    filePosition = 0;
    fileSize = 0;
    lineNumber = 0;

    updateText();

    showDebug("colors update successful");
}

void Widget::updateCommands()
{
    QLayoutItem *child;
    while ((child = buttonLayout->takeAt(0)) != 0) {
        buttonLayout->removeWidget(child->widget());
        delete child->widget();
    }

//    for( QLayoutItem *item=buttonLayout->takeAt(0); item != 0; item = buttonLayout->takeAt(0)) {
//        buttonLayout->removeWidget(item->widget());
//        delete item->widget();
//    }

    buttons.clear();

    commandParser = new CommandParser(this);
//    commandParser->parse(QDir::homePath() + "/commands.cfg");
    commandParser->parse(settings.getValue(Settings::commands));
    QList<QString> keys = commandParser->handlers.keys();


    foreach( QString key, keys ) {

        QTextDocument Text;
        Text.setHtml(trUtf8("%1").arg(key));

        QPixmap pixmap(Text.size().width(), Text.size().height());
        pixmap.fill( Qt::transparent );
        QPainter painter( &pixmap );
        Text.drawContents(&painter, pixmap.rect());

        QIcon ButtonIcon(pixmap);

        CustomButton *button = new CustomButton();
        button->setkey(trUtf8("%1").arg(key));
        button->setIcon(ButtonIcon);
        button->setIconSize(pixmap.rect().size());
        button->setPalette(QColor("lightslategray"));
        connect(button, SIGNAL(clicked(QString)), this, SLOT(runCommand(QString)));

        button->setToolTip(commandParser->tips.value(key));

        buttonLayout->addWidget(button);
        buttons.append(button);
    }

    showDebug("commands update successful");
}

void Widget::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(updateColorsAction);
    trayIconMenu->addAction(updateCommandsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void Widget::createActions()
{
    updateColorsAction = new QAction(tr("update co&lors"), this);
    connect(updateColorsAction, SIGNAL(triggered()), this, SLOT(updateColors()));

    updateCommandsAction = new QAction(tr("update co&mmands"), this);
    connect(updateCommandsAction, SIGNAL(triggered()), this, SLOT(updateCommands()));

    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Widget::runCommand(QString commandKey)
{
    foreach( CustomButton *b, buttons ) {
        if( b->getKey() == commandKey ) {
            b->setPalette(QColor("#4682B4"));
        } else {
            b->setPalette(QColor("lightslategray"));
        }
    }

    QList<CommandHandler*> commands = commandParser->handlers.value(commandKey);
    commands.first()->start();
}

void Widget::updateSettings()
{
    updateColors();
    updateCommands();

    filePosition = 0;
    fileSize = 0;
    lineNumber = 0;
    updateText(true);
}

void Widget::updateText(bool force)
{



    if( !updateOn.isChecked() && !force ) {
        return;
    }

    showDebug("Debug");

    if( !isVisible() ) {
        return;
    }

    QFile file(settings.getValue(Settings::out));
//    QFile file("miniout.txt");
//    QFile file(QDir::homePath()+"/testout.txt");

    if( fileSize == file.size() ) {
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

//    if( pos ) {
//        --pos;
//    }

    if( !file.seek(filePosition) ) {
        return;
    }

    QTextStream in(&file);
    QString text;

    qint64 currentNumer = lineNumber + 1;
    while (!in.atEnd()) {
        QString line = in.readLine();

        line = line.replace(QRegExp("\\[\\d+\\;\\d+\\w"), "")
               .replace(QRegExp("\\[\\K"), "")
               .replace(QRegExp("\\(\\B\\[\\d\\m"), "")
               .replace(QRegExp("\\[\\d\\m\\(\\B"), "")
               .replace(QRegExp("\\[\\d\\m"), "")
               .replace(QRegExp("\\(\\B"), "")
               .replace("", "")
               .replace("(B", "");

        line.insert(0, tr("%1: ").arg(++currentNumer));
        text.append(colorParser->processString(line));
    }


    // lineNumber
    if( currentNumer - lineNumber > 10 ) {
//    if( file.pos() - filePosition > 1024 ) {
        browser.setText(text);
    } else {
        browser.append(text);
    }


    QScrollBar *s = browser.verticalScrollBar();
    s->setValue(s->maximum());

    filePosition = file.pos();
    fileSize = file.size();
    file.close();

    lineNumber = currentNumer;

//    if( text.startsWith("</div><div>") ) {
//        text.replace(0,11,"");
//    }
}

void Widget::runCommand()
{
    std::cout << lineEdit.text().toStdString() << std::endl;
}
