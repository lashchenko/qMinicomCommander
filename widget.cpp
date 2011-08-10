#include "widget.h"

#include "colorparser.h"
#include "commandparser.h"
#include "custombutton.h"

#include "paletteparser.h"

#include <iostream>

#include <QtGui>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *vl = new QVBoxLayout;

    // *******************************************************************
    // set buttons-commands layout
    // *******************************************************************
    buttonLayout = new QHBoxLayout();
    vl->addLayout(buttonLayout);


    // *******************************************************************
    // set text browser
    // *******************************************************************
    vl->addWidget(&browser);


    // *******************************************************************
    // set custom command layout
    // *******************************************************************
    QHBoxLayout *hl = new QHBoxLayout;
    connect(&lineEdit, SIGNAL(enterPressed()), this, SLOT(runCommand()));
    hl->addWidget(new QLabel(trUtf8("run command # ")));
    hl->addWidget(&lineEdit);
    vl->addLayout(hl);



    QWidget *configurationWidget = createConfiturationWidget();
    QCheckBox *configurationCheck = new QCheckBox(trUtf8("show configuration"));
    connect(configurationCheck, SIGNAL(clicked(bool)), configurationWidget, SLOT(setShown(bool)));
    configurationWidget->setVisible(false);
    vl->addWidget(configurationWidget);


    QWidget *findWidget = createFindWidget();
    findCheck = new QCheckBox(trUtf8("show find"));
    connect(findCheck, SIGNAL(toggled(bool)), findWidget, SLOT(setShown(bool)));
    findWidget->setVisible(false);
    vl->addWidget(findWidget);



    // configuration
    hl = new QHBoxLayout;
    hl->addWidget(configurationCheck);
    hl->addWidget(findCheck);

    hl->addStretch();

    QPushButton *clearButton = new QPushButton(tr("clear"));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
    hl->addWidget(clearButton);


    vl->addLayout(hl);

    setLayout(vl);


    connect(&settings, SIGNAL(save()), this, SLOT(updateSettings()));

    fileSize = 0;
    lineNumber = 0;

    createActions();
    createTrayIcon();

//    trayIcon->setIcon(QIcon(":/img/q.png"));
//    trayIcon->setIcon(QIcon(":/img/heart.svg"));
//    trayIcon->setIcon(QIcon(":/img/rabbit.png"));
    trayIcon->setIcon(QIcon(":/img/pig.png"));
    trayIcon->setVisible(true);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activated(QSystemTrayIcon::ActivationReason)));

    lastLine.clear();

    updateSettings();


    connect(&timer, SIGNAL(timeout()), this, SLOT(updateText()));
    timer.start(updatePeriod.value());
}

Widget::~Widget()
{
    if( file.isOpen() ) {
        file.close();
    }

    BashHanndler *minicomKiller = new BashHanndler("killall -KILL minicom");
    minicomKiller->setEnabled(true);
    minicomKiller->start();
//    minicomKiller->wait();
}


QWidget* Widget::createConfiturationWidget()
{
    // create configuration layout
    QHBoxLayout *hl = new QHBoxLayout;
    updateOn.setText("update [ON]");
    updateOn.setChecked(true);
    connect(&updateOn, SIGNAL(clicked(bool)), this, SLOT(updateOnOff(bool)));
    hl->addWidget(&updateOn);

    hl->addWidget(new QLabel(tr("period (in millisec)")));
    updatePeriod.setMinimum(1);
    updatePeriod.setMaximum(60000);
    updatePeriod.setSingleStep(1000);
    updatePeriod.setValue(100);
    connect(&updatePeriod, SIGNAL(valueChanged(int)), this, SLOT(updatePeriodChange(int)));
    hl->addWidget(&updatePeriod);


    showLines.setText(" show line nubmer ");
    showLines.setChecked(false);
    connect(&showLines, SIGNAL(clicked()), this, SLOT(updateShowLines()));
    hl->addWidget(&showLines);

    hl->addWidget(new QLabel(tr(" display maximum lines")));

    updateLines.setMinimum(1);
    updateLines.setMaximum(999999);
    updateLines.setSingleStep(100);
    updateLines.setValue(48);
    connect(&updateLines, SIGNAL(valueChanged(int)), this, SLOT(updateLinesChange(int)));
    hl->addWidget(&updateLines);

    hl->addStretch();

    QPushButton *showSettingDialog = new QPushButton();
    showSettingDialog->setIcon(QIcon(":/img/settings.png"));
    connect(showSettingDialog, SIGNAL(clicked()), &settings, SLOT(exec()));
    hl->addStretch();
    hl->addWidget(showSettingDialog);


    QWidget *widget = new QWidget;
    widget->setLayout(hl);
    return widget;
}

QWidget* Widget::createFindWidget()
{
    // create find layout
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addStretch();

    QPushButton *prev = new QPushButton(trUtf8("prev"));
    connect(prev, SIGNAL(clicked()), this, SLOT(findPrev()));

    QPushButton *next = new QPushButton(trUtf8("next"));
    connect(next, SIGNAL(clicked()), this, SLOT(findNext()));

    hl->addWidget(new QLabel(trUtf8("find: ")));
    hl->addWidget(prev);
    hl->addWidget(&findLine);
    hl->addWidget(next);

    hl->addStretch();

    QWidget *widget = new QWidget;
    widget->setLayout(hl);
    return widget;
}

void Widget::updateShowLines()
{
//    updateLines.setEnabled(showLines.isChecked());
    updateText(true);
}

void Widget::updateOnOff(bool checked)
{
    if( checked ) {
        updateOn.setText("update [ON]");
        updateText();
    } else {
        updateOn.setText("update [OFF]");
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
//                                 tr("The program will keep running in the ..."));
        hide();
        event->ignore();
    }
}

void Widget::processFile()
{
    if( file.isOpen() ) {
        file.close();
    }
    file.setFileName(settings.getValue(OUT));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    in.setDevice(&file);
}

void Widget::updateColors()
{
    colorParser = new ColorParser(settings.getValue(COLORS));
    colorParser->setBgColor(palette().color(QPalette::Window));
    colorParser->setTextColor(palette().color(QPalette::Text));

//    fileSize = 0;
//    lineNumber = 0;

    updateText(true);

    showDebug("colors update successful");
}

void Widget::updateRegexp()
{
    regexpParser = new RegexpParser(settings.getValue(REGEXP));

//    fileSize = 0;
//    lineNumber = 0;

    updateText(true);

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


    QList< QList<CommandHandler*> > handlers = commandParser->handlers.values();
    for( int i=0; i<handlers.size(); ++i ){
        CommandHandler *handler = handlers.at(i).first();
        handler->setEnabled(false);
    }


    commands = commandParser->handlers.value(commandKey);
    if( commands.isEmpty() ) {
        showDebug(trUtf8("Пустая комманда! %1. Проверьте конфиг!").arg(commandKey), 2);
        return;
    }
    commands.first()->setEnabled(true);
    commands.first()->start();
}

void Widget::updateSettings()
{

    updateColors();
    updateCommands();
    updateRegexp();
    updatePalette();

    fileSize = 0;
    lineNumber = 0;

    processFile();

    updateText(true);
}

void Widget::clear()
{
    browser.clear();
    buffer.clear();
    lineNumber = 0;
}

void Widget::updateText(bool force)
{
    if( !updateOn.isChecked() && !force ) {
        return;
    }

    if( !isVisible() ) {
        return;
    }

    if( fileSize == file.size() && !force ) {
        return;
    }

    QStringList temp;

    while (!in.atEnd()) {
        QString line = in.readLine();

        if( !lastLine.isEmpty() ) {
            line.insert(0, lastLine);
            lastLine.clear();
        }

        if( line.trimmed().isEmpty() ) {
            continue;
        }

        temp.append(line);

        if( temp.size() >= updateLines.value() ) {
            buffer = temp;
            temp.clear();
        }
    }

    if( !temp.isEmpty() ) {
        lastLine = temp.last();
    }

    if( !lastLine.isEmpty() && lastLine.indexOf('\n') == -1 ) {
        temp.removeAt(temp.size()-1);
    } else {
        lastLine.clear();
    }

    buffer.append(temp);

    int offset = buffer.size() - updateLines.value();
    if( offset < 0 ) {
        offset = 0;
    }

    bool browserCleaned = false;
    if( lineNumber >= updateLines.value() ) {
        browserCleaned = true;
        lineNumber = 0;
    }

    QString text = "<pre>";
//    QStringList alternate;
    for( int i=offset; i<buffer.size(); ++i ) {
        QString line = (buffer.at(i));
//        alternate.append(line);

        line = regexpParser->processString(line);
//        line.insert(0, tr("%1: ").arg(++lineNumber));

        ++lineNumber;
        if( showLines.isChecked() ) {
            line.insert(0, tr("%1: ").arg(lineNumber));
        }

        line = colorParser->processString(line);

        text += line;
    }
    text += "</pre>";

//    if( buffer.size() > updateLines.value() ) {
        buffer.clear();
//        buffer = alternate;
//    }

//        browser.document()->t
    QScrollBar *s = browser.verticalScrollBar();

    if( browserCleaned ) {
        browser.setText(text);
    } else {
        browser.append(text);
    }

    if( browser.textCursor().hasSelection() || s->value() != s->maximum()) {
        s->setValue(s->value());
    } else {
        s->setValue(s->maximum());
    }

    fileSize = file.size();
}

void Widget::findPrev()
{
    find(QTextDocument::FindCaseSensitively | QTextDocument::FindBackward);
}

void Widget::findNext()
{
    find(QTextDocument::FindCaseSensitively);
}

void Widget::find(QTextDocument::FindFlags options)
{
//    qDebug() << "[FIND ( " << text << " ) ";
    if( browser.find(findLine.text(), options) ) {
        qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
//    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return ) {
//        runCommand();
//    }
    if (event->matches(QKeySequence::Find)) {
        findCheck->setChecked(!findCheck->isChecked());
        return;
    }
    qDebug() << "widget delegate key : " << event->key();
    lineEdit.keyPressEvent(event);
}

void Widget::runCommand()
{
    std::cout << lineEdit.text().toStdString() << std::endl;
    lineEdit.clear();
}
