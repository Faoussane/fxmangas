#ifndef FXMANGASLIST_H
#define FXMANGASLIST_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include "db.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class FxMangasList;
}
QT_END_NAMESPACE

class FxMangasList : public QMainWindow
{
    Q_OBJECT

public:
    FxMangasList(QWidget *parent = nullptr);
    ~FxMangasList();
    //void mangaContent();
    //void manga(Manga_metier *m);

    //void infoManga(Manga_metier *m, QLabel *titleLabel, QLabel *descLabel, QLabel *dateLabel, QLabel *idLabel);
    QWidget *mangaGUI(Manga_metier *m);
    void rafraichir();


public slots:
    void mangaCourant();
    void mangaChecker(bool check);
    void apresAjou();
    void supprimer();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


    void on_pushButton_3_clicked();

private:
    Ui::FxMangasList *ui;
    Db *m_db;

    QLabel *m_descriptionLabel;
    QList<Manga_metier*> m_listeManga;
    QLabel *m_dueDateLabel;
    QLabel *m_idLabel;
    Manga_metier *m_b;
};
#endif // FXMANGASLIST_H
