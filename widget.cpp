#include "widget.h"

#include "colorparser.h"
#include "commandparser.h"
#include "custombutton.h"

#include "paletteparser.h"

#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{


//    lineEdit.setPalette(p);

//    p.setColor(QPalette::Text, QColor("#439A00"));

    QPushButton *button = new QPushButton("run command");
//    button->setPalette(QColor("#CD853F"));
//    button->setPalette(QColor("#B95F05"));
//    button->setPalette(p);
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
    updateOn.setChecked(true);
    connect(&updateOn, SIGNAL(clicked(bool)), this, SLOT(updateOnOff(bool)));
    hl->addWidget(&updateOn);

    hl->addWidget(new QLabel(tr("period (in millisec)")));
    updatePeriod.setMinimum(100);
    updatePeriod.setMaximum(60000);
    updatePeriod.setSingleStep(1000);
    updatePeriod.setValue(3000);
    connect(&updatePeriod, SIGNAL(valueChanged(int)), this, SLOT(updatePeriodChange(int)));
    hl->addWidget(&updatePeriod);

    hl->addWidget(new QLabel(tr("show maximum lines")));
    updateLines.setMinimum(1);
    updateLines.setMaximum(999999);
    updateLines.setSingleStep(100);
    updateLines.setValue(48);
//    updateLines.setDisabled(true);
    updateLines.setToolTip("this future be enable in future time :)");
    connect(&updateLines, SIGNAL(valueChanged(int)), this, SLOT(updateLinesChange(int)));
    hl->addWidget(&updateLines);

    QPushButton *clearButton = new QPushButton(tr("clear"));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearOut()));
//    connect(clearButton, SIGNAL(clicked()), &browser, SLOT(clear()));
    hl->addWidget(clearButton);

    QPushButton *sb = new QPushButton();
//    sb->setIcon(QIcon(":/img/heart.svg"));
    sb->setIcon(QIcon(":/img/settings.png"));
    connect(sb, SIGNAL(clicked()), &settings, SLOT(exec()));
    hl->addStretch();
    hl->addWidget(sb);

    vl->addLayout(hl);

    setLayout(vl);

    settings.setModal(true);
    connect(&settings, SIGNAL(save()), this, SLOT(updateSettings()));

    fileSize = 0;
    filePosition = 0;
    lineNumber = 0;

//    commands = 0;

    createActions();
    createTrayIcon();

//    trayIcon->setIcon(QIcon(":/img/q.png"));
//    trayIcon->setIcon(QIcon(":/img/heart.svg"));
//    trayIcon->setIcon(QIcon(":/img/rabbit.png"));
    trayIcon->setIcon(QIcon(":/img/pig.png"));
    trayIcon->setVisible(true);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activated(QSystemTrayIcon::ActivationReason)));

    updateCommands();
    updatePalette();
    updateColors();
    updateRegexp();

    processFile();


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
//        updateOn.setPalette(Qt::green);
        updateText();
    } else {
        updateOn.setText("Update [OFF]");
//        updateOn.setPalette(Qt::red);
    }
}

void Widget::updatePeriodChange(int period)
{
    timer.stop();
    timer.start(period);
}

void Widget::updateLinesChange(int lines)
{
    updateText(true);
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

void Widget::showDebug(QString debug, int icon)
{
    switch (icon) {
    case 2:
        trayIcon->showMessage("notification from Qt Proecss Commander", debug, QSystemTrayIcon::Warning);
        break;
    case 1:
        trayIcon->showMessage("notification from Qt Proecss Commander", debug, QSystemTrayIcon::Information);
        break;
    default:
        trayIcon->showMessage("notification from Qt Proecss Commander", debug, QSystemTrayIcon::NoIcon);
        break;
    }


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

void Widget::processFile()
{
//    file = QFile(settings.getValue(OUT));
    file.setFileName(settings.getValue(OUT));


//    if( fileSize == file.size() ) {
//        return;
//    }

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

//    if( !file.seek(filePosition) ) {
//        return;
//    }

//    in = QTextStream(&file);
    in.setDevice(&file);
}

void Widget::updateColors()
{
    colorParser = new ColorParser(settings.getValue(COLORS));
    colorParser->setBgColor(palette().color(QPalette::Window));
    colorParser->setTextColor(palette().color(QPalette::Text));

    filePosition = 0;
    fileSize = 0;
    lineNumber = 0;

    updateText();

    showDebug("colors update successful");
}

void Widget::updateRegexp()
{
    regexpParser = new RegexpParser(settings.getValue(REGEXP));

    filePosition = 0;
    fileSize = 0;
    lineNumber = 0;

    updateText();

    showDebug("colors update successful");
}

void Widget::updatePalette()
{
    qApp->setPalette(PaletteParser::getPalette(settings.getValue(PALETTE)));
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
    commandParser->parse(settings.getValue(COMMANDS));
    QList<QString> keys = commandParser->handlers.keys();


    foreach( QString key, keys ) {

        QString rich = commandParser->rich.value(key).trimmed();
        if( rich.isEmpty() ) {
            continue;
        }

        QTextDocument Text;
        Text.setHtml(trUtf8("%1").arg(rich));

        QPixmap pixmap(Text.size().width(), Text.size().height());
        pixmap.fill( Qt::transparent );
        QPainter painter( &pixmap );
        Text.drawContents(&painter, pixmap.rect());

        QIcon ButtonIcon(pixmap);

        CustomButton *button = new CustomButton();
        button->setkey(trUtf8("%1").arg(key));
        button->setIcon(ButtonIcon);
        button->setIconSize(pixmap.rect().size());
//        button->setPalette(QColor("lightslategray"));
//        button->setPalette(QColor("#323232"));
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
    trayIconMenu->addAction(updateRegexpAction);
    trayIconMenu->addAction(updatePaletteAction);
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

    updateRegexpAction = new QAction(tr("update RegExp"), this);
    connect(updateRegexpAction, SIGNAL(triggered()), this, SLOT(updateRegexp()));

    updatePaletteAction = new QAction(tr("update palette"), this);
    connect(updatePaletteAction, SIGNAL(triggered()), this, SLOT(updatePalette()));

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
            b->setPalette(QColor("#222222"));
        } else {
            b->setPalette(palette());//QColor("#4d4d4d"));
        }
    }

    commands = commandParser->handlers.value(commandKey);
    if( commands.isEmpty() ) {
        showDebug(trUtf8("Пустая комманда! %1. Проверьте конфиг!").arg(commandKey), 2);
        return;
    }
    commands.first()->setEnabled(true);
    commands.first()->start();
//    QList<CommandHandler*> current = commandParser->handlers.value(commandKey);
//    if( current.isEmpty() ) {
//        showDebug(trUtf8("Пустая комманда! Проверьте конфиг!"), 2);
//        return;
//    }

//    if( commands.isEmpty() ) {
//        current.first()->start();
//    } else {
//        bool anyRunning = false;
//        foreach(CommandHandler *handler, commands) {
//            if( handler->isRunning() ) {
//                anyRunning = true;
//                break;
//            }
//        }
//        if( anyRunning ) {
//            connect(commands.last(), SIGNAL(terminated()), current.first(), SLOT(start()));
//            foreach(CommandHandler *handler, commands) {
//                handler->terminate();
//            }
//        } else {
//            current.first()->start();
//        }

//    }

//    commands = current;
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

    if( !isVisible() ) {
        return;
    }

//    QFile file(settings.getValue(OUT));

//    qDebug() << "1111111111";
    if( fileSize == file.size() ) {
        return;
    }

//    qDebug() << "22222222222";
//    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
//        return;
//    }

//    qDebug() << "3333333333333";
//    if( !file.seek(filePosition) ) {
//        return;
//    }

//    QTextStream in(&file);

    QStringList buffer;
    QStringList temp;

    while (!in.atEnd()) {
        QString line = in.readLine();

//        line = regexpParser->processString(line);
//        line.insert(0, tr("%1: ").arg(++lineNumber));
//        line = colorParser->processString(line);

//        text.append(line);

        temp.append(line);

        if( temp.size() >= updateLines.value() ) {
            buffer = temp;
            temp.clear();
        }
    }
//    file.close();

    QString text;
    buffer.append(temp);

    int offset = buffer.size() - updateLines.value();
    if( offset < 0 ) {
        offset = 0;
    }

    if( lineNumber >= updateLines.value() ) {
        browser.clear();
        lineNumber = 0;
    }

    for( int i=offset; i<buffer.size(); ++i ) {
        QString line = (buffer.at(i));

        line = regexpParser->processString(line);
        line.insert(0, tr("%1: ").arg(++lineNumber));
        line = colorParser->processString(line);

        text += line;
    }

    if( lineNumber < updateLines.value() ) {
        browser.append(text);
    } else {
        browser.setText(text);
//        lineNumber = 0;
    }

    QScrollBar *s = browser.verticalScrollBar();
    s->setValue(s->maximum());


//    filePosition = file.pos();
    fileSize = file.size();
//    file.close();

//    if( lineNumber >= updateLines.value() ) {
//        clearOut();
//    }
}

void Widget::clearOut()
{
    QFile file(settings.getValue(OUT));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    file.resize(0);

    filePosition = file.pos();
    fileSize = file.size();
    lineNumber = 0;

    file.close();

    browser.clear();
}

void Widget::runCommand()
{
    std::cout << lineEdit.text().toStdString() << std::endl;
}
