#include "database.h"
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent)
{
}

void Database::makeConnection()
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

bool Database::addNewExperiment(QString name, QString comment)
{
    QSqlQuery query = QSqlQuery(conn);
    query.prepare("insert into experiments (obj_name, obj_comment) values (:name, :comment)");
    qDebug() << query.lastQuery();
    query.bindValue(":name", name);
    query.bindValue(":comment", comment);
    return query.exec();
}

bool Database::updateExperiment(QString id, QString name, QString comment)
{
    QSqlQuery query = QSqlQuery(conn);
    query.prepare("update experiments set obj_name = :name, obj_comment = :comment"
                  " where obj_id = :id");
    query.bindValue(":name", name);
    query.bindValue(":comment", comment);
    query.bindValue(":id", id.toInt());
    return query.exec();
}

Database* Database::pdb = nullptr;
