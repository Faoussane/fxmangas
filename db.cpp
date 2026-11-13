#include "db.h"
#include <QDebug>

Db::Db() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("mangas.db");
    if(!m_db.open()){
        qDebug()<<"Erreur d'ouverture de la base";
    }
    //this->createTab();
    //this->update("data/images/mangas/naruto.jpg", 1);
    //this->update("data/images/mangas/black_clover.jpg", 3);
    //this->update("data/images/mangas/solo.jpeg", 4);
    this->update("data/images/mangas/manga.jpg", 5);

}

Db::~Db()
{
    if(m_db.open()){
        m_db.close();
    }
}

void Db::createTab()
{
    QString sql("CREATE TABLE IF NOT EXISTS manga ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nom VARCHAR(255) NOT NULL,"
                "description TEXT,"
                "photoUrl TEXT,"
                "regarder BOOLEAN DEFAULT 0,"
                "creer DATETIME DEFAULT CURRENT_TIMESTAMP,"
                "modifier DATE NOT NULL DEFAULT CURRENT_TIMESTAMP)");
    QSqlQuery req;
    if(!req.exec(sql)){
        qCritical()<<"Erreur de création de la table manga: "<<req.lastError();
    }
}

bool Db::ajouter(Manga_metier *m)
{
    QString sql="INSERT INTO manga(nom, description, photoUrl, regarder)"
                "VALUES(?, ?, ?, ?)";
    QSqlQuery req;
    req.prepare(sql);
    req.addBindValue(m->getNom());
    req.addBindValue(m->getDescription());
    req.addBindValue(m->getPhotoUrl());
    req.addBindValue(m->getRegarder());
    if(!req.exec()){
        qCritical() << req.lastError();
        return false;
    }
    return true;
}

bool Db::update(QString urlP, int id)
{
    QString sql="UPDATE manga SET photoUrl = ? WHERE id = ?";
    QSqlQuery req;
    req.prepare(sql);
    req.addBindValue(urlP);
    req.addBindValue(id);
    if(!req.exec()){
        qCritical() << req.lastError();
        return false;
    }
    return true;
}
bool Db::updateRegarder(int id, bool check)
{
    QString sql="UPDATE manga SET regarder = ? WHERE id = ?";
    QSqlQuery req;
    req.prepare(sql);
    req.addBindValue(check);
    req.addBindValue(id);
    if(!req.exec()){
        qCritical() << req.lastError();
        return false;
    }
    return true;
}
bool Db::updateAll(Manga_metier *m)
{
    QSqlQuery req;
    QString sql ="UPDATE manga SET nom=?, description=?, photoUrl=?, regarder=?, modifier=CURRENT_TIMESTAMP WHERE id=?";

    req.prepare(sql);
    req.addBindValue(m->getNom());
    req.addBindValue(m->getDescription());
    req.addBindValue(m->getPhotoUrl());
    req.addBindValue(m->getRegarder());
    req.addBindValue(m->getId());

    if (!req.exec()) {
        qCritical() << req.lastError();
        return false;
    }
    return true;
}


bool Db::supprimer(int id)
{
    //DELETE FROM `clients` WHERE `clients`.`id_client` = 5
    QSqlQuery req;
    QString sql="DELETE FROM manga WHERE id=?";
    req.prepare(sql);
    req.addBindValue(id);
    if(!req.exec()){
        qCritical() << req.lastError();
        return false;
    }
    return true;
}

QList<Manga_metier *> Db::mangas()
{
    QSqlQuery req;
    if(!req.exec("SELECT * FROM manga")){
        qCritical()<<"Erreur de création de la table manga: "<<req.lastError();
        return QList<Manga_metier*>();
    }
    QList<Manga_metier*> mangas;
    while(req.next()){
        Manga_metier *manga = new Manga_metier();
        manga->setId(req.value(0).toInt());
        manga->setNom(req.value(1).toString());
        manga->setDescription(req.value(2).toString());
        manga->setPhotoUrl(req.value(3).toString());
        manga->setRegarder(req.value(4).toBool());
        manga->setCreer(req.value(5).toString());
        manga->setModifier(req.value(6).toString());
        mangas.append(manga);
    }
    return mangas;
}

Manga_metier *Db::manga(QString nom)
{
    QSqlQuery req;
    QString sql="SELECT * FROM manga WHERE nom=?";
    if(!req.exec()){
        qCritical()<<"Erreur de création de la table manga: "<<req.lastError();
        return nullptr;
    }
    Manga_metier *manga = new Manga_metier();
    if(req.next()){

        manga->setId(req.value(0).toInt());
        manga->setNom(req.value(1).toString());
        manga->setDescription(req.value(2).toString());
        manga->setPhotoUrl(req.value(3).toString());
        manga->setRegarder(req.value(4).toBool());
        manga->setCreer(req.value(5).toString());
        manga->setModifier(req.value(6).toString());
    }
    return manga;
}
bool Db::existe(QString nom)
{
    QSqlQuery req;
    QString sql="SELECT * FROM manga WHERE nom=?";

    req.prepare(sql);
    req.addBindValue(nom);
    if(!req.exec()){
        qCritical() << req.lastError();
        return false;
    }
    if(req.next()){
        qDebug()<<"Manga existe: "<<req.value(1).toString();
    }

    return req.next();
}



/*


*/
