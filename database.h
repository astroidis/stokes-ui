#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

class Database : public QObject
{
    Q_OBJECT

public:
    static Database * instance();

    void makeConnection();
    bool openDB();
    void closeDB();
    bool addNewExperiment(QString name, QString comment);
    bool updateExperiment(QString id, QString name, QString comment);
    QSqlQuery getAllExperiments();

private:
    static Database *pdb;
    QSqlDatabase conn;

    explicit Database(QObject *parent = nullptr);
};

#endif // DATABASE_H
