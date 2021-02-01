#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

class Database : public QObject
{
    Q_OBJECT
public:
    QSqlDatabase conn;

public:
    explicit Database(QObject *parent = nullptr);

    bool makeConnection();
    bool openDB();
    void closeDB();
    bool execute(const char *query);

private:
};

#endif // DATABASE_H
