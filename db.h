#ifndef DB_H
#define DB_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "manga.h"
#include "manga_metier.h"

class Manga;
class Db
{

private:
    QSqlDatabase m_db;
public:
    Db();
    ~Db();
    void createTab();

    bool ajouter(Manga_metier *m);
    QList<Manga_metier*> mangas();
    Manga selectioner(int id);
    Manga_metier *manga(QString nom);
    bool update(QString urlP, int id);
    bool updateRegarder(int id, bool check);
    bool supprimer(int id);
    bool updateAll(Manga_metier *m);
    bool existe(QString nom);
};

#endif // DB_H
