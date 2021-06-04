#include "database.h"
#include <QMessageBox>

Database::Database(QObject *parent) : QObject(parent)
{
}

void Database::makeConnection()
{
    conn = QSqlDatabase::addDatabase("QPSQL", "stokes_db");
    if (!conn.isValid()){
        QMessageBox msgb;
        msgb.setText(conn.lastError().text());
        msgb.setStandardButtons(QMessageBox::Cancel);
        msgb.exec();
    }

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
            QMessageBox msgb;
            msgb.setText(conn.lastError().text());
            msgb.setStandardButtons(QMessageBox::Cancel);
            msgb.exec();
        }
    }
    return true;
}

void Database::closeDB()
{
    if (conn.isOpen()){
        conn.close();
    }
}

Database * Database::instance()
{
    if (!pdb){
        pdb = new Database();
    }
    return pdb;
}

Database* Database::pdb = nullptr;
