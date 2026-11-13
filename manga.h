#ifndef MANGA_H
#define MANGA_H

#include <QWidget>
#include "db.h"
#include "manga_metier.h"

class Db;
namespace Ui {
class Manga;
}

class Manga : public QWidget
{
    Q_OBJECT

public:
    explicit Manga(Db *db, QWidget *parent = nullptr);
    ~Manga();

    Manga();



    QString choisirEtCopierImageUtilisateur();
    void ChargerPourModifier(Manga_metier *m);
private slots:
    void on_pushButtonValider_clicked();

    void on_pushButtonPhoto_clicked();
signals:
    void mangaAjoute();
    void mangaModifie(Manga_metier *m);
private:
    Ui::Manga *ui;
    Manga_metier *manga = nullptr;
    bool modeModification = false;
    Db *m_db;
    QString url;
};

#endif // MANGA_H
