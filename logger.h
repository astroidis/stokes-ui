#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>


class Logger : public QObject
{
    Q_OBJECT

public:
    explicit Logger(QString filename, QObject *parent = nullptr);

    void logInfo(QString message);
    void logDebug(QString message);
    void logWarning(QString message);
    void logError(QString message);

private:
    QString logfile;
};

#endif // LOGGER_H
