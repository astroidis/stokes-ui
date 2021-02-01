#include "database.h"
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent)
{
}

bool Database::makeConnection()
{
    conn = QSqlDatabase::addDatabase("QPSQL", "stokes_db");
    conn.setDatabaseName("stokes_db");
    conn.setHostName("127.0.0.1");
    conn.setPort(5432);
    conn.setUserName("postgres");
    conn.setPassword("postgres");
}

bool Database::openDB()
{
    if (!conn.isOpen()){
        if (!conn.open()){
            qDebug() << conn.lastError();
            return false;
        }
        return true;
    }
}

void Database::closeDB()
{
    if (conn.isOpen()){
        conn.close();
    }
}
