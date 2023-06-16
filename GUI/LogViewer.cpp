#include "LogViewer.h"
#include <QFile>
#include <QScrollBar>
#include "MainWindow.h"

//using namespace std;

int LogViewer::lastFileSize = 0;
int LogViewer::initialLogFileReadInProgress = 0;

void LogViewer::logFileChanged(const QString &path) {
    static int fileAccessed = 0;

    if (fileAccessed == 1) {
           //ui.textEdit->moveCursor(QTextCursor::End);
           //ui.textEdit->insertPlainText("<asynchronous log file read attempted, waiting...>\n");
           ui.textEdit->verticalScrollBar()->setValue(ui.textEdit->verticalScrollBar()->maximum());
           while (fileAccessed == 1) { }
       }

    if (initialLogFileReadInProgress == 1) {
        //ui.textEdit->moveCursor(QTextCursor::End);
        //ui.textEdit->insertPlainText("<before insert, waiting for initial log file read...>\n");
        while (initialLogFileReadInProgress == 1) { }
    }

   fileAccessed = 1; // lock, should not be neccessary
                     // qt slots are by default synchronous, if are in same thread
   QFile *f = new QFile(QString::fromStdString(logFilePath));
   f->open(QIODeviceBase::ReadOnly);
   QByteArray data;
   f->seek(lastFileSize);
   data = f->read(f->size() - lastFileSize);
   lastFileSize = f->size();
   //ui.textEdit->moveCursor()   
   f->close();
   //ui.textEdit->moveCursor(QTextCursor::End);
   ui.textEdit->setHtml(QString::fromUtf8(data));
   ui.textEdit->verticalScrollBar()->setValue(ui.textEdit->verticalScrollBar()->maximum());
   fileAccessed = 0;
}

LogViewer::LogViewer(MainWindow *mainWnd, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->mainWnd = mainWnd;

    watcher = new QFileSystemWatcher();

    connect(watcher, &QFileSystemWatcher::fileChanged, this, &LogViewer::logFileChanged);

    QFile *f = new QFile(QString::fromStdString(logFilePath));
    if (!f->exists()) {
        f->open(QIODeviceBase::NewOnly);
        lastFileSize = 0;
        f->close();
        watcher->addPath(QString::fromStdString(logFilePath));
    } else {
        watcher->addPath(QString::fromStdString(logFilePath));
        initialLogFileReadInProgress = true;
        f->open(QIODeviceBase::ReadOnly);
        lastFileSize = f->size();
        QByteArray data;
        data = f->read(f->size());
        //ui.textEdit->moveCursor()
        f->close();
        ui.textEdit->setHtml(QString::fromUtf8(data));
        ui.textEdit->verticalScrollBar()->setValue(ui.textEdit->verticalScrollBar()->maximum());
        initialLogFileReadInProgress = false;
    }

}

void LogViewer::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    if (watcher) {
        delete watcher;
        watcher = 0;
    }

    mainWnd->viewLogFileClosed();
}

LogViewer::~LogViewer()
{
    if (watcher) {
        delete watcher;
        watcher = 0;
    }
}
