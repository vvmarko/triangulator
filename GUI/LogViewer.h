#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QWidget>
#include <QFileSystemWatcher>
#include "ui_LogViewer.h"

class MainWindow;

class LogViewer : public QWidget
{
    Q_OBJECT

public:
    LogViewer(MainWindow *mainWnd, QString displayStr, QWidget *parent = Q_NULLPTR);
    ~LogViewer();

private:
    MainWindow *mainWnd;

    const std::string logFilePath = "logfajl.log";
    static int lastFileSize;
    static int initialLogFileReadInProgress;

    QFileSystemWatcher *watcher;

    Ui::LogViewer ui;

    void logFileChanged(const QString &path);

    void closeEvent(QCloseEvent *e) override;
};

#endif // LOGVIEWER_H
