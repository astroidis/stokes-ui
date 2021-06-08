#include "logger.h"

#include <QDateTime>
#include <QDir>

Logger::Logger(QString filename, QObject *parent) :
    QObject(parent),
    logfile(filename)
{
}

void Logger::logInfo(QString message)
{
    QFile file = QFile(logfile);
    file.open(QFile::WriteOnly | QFile::Append);
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
        << " [INF] " << message << "\n";
    out.flush();
    file.close();
}

void Logger::logDebug(QString message)
{
    QFile file = QFile(logfile);
    file.open(QFile::WriteOnly | QFile::Append);
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
        << " [DBG] " << message << "\n";
    out.flush();
    file.close();
}

void Logger::logWarning(QString message)
{
    QFile file = QFile(logfile);
    file.open(QFile::WriteOnly | QFile::Append);
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
        << " [WRN] " << message << "\n";
    out.flush();
    file.close();
}

void Logger::logError(QString message)
{
    QFile file = QFile(logfile);
    file.open(QFile::WriteOnly | QFile::Append);
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
        << " [ERR] " << message << "\n";
    out.flush();
    file.close();
}
