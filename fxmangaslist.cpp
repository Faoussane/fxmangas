#include "fxmangaslist.h"
#include "ui_fxmangaslist.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QRandomGenerator>
#include <QCheckBox>

FxMangasList::FxMangasList(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FxMangasList)
{
    ui->setupUi(this);
    m_db=new Db();
    m_b = new Manga_metier();
    QPixmap project = QPixmap("data/images/mangas/manga_20260128_200906_950.png");
    //ui->label->setPixmap(project.scaled(256, 486, Qt::KeepAspectRatio));
    m_b->setPhotoUrl("data/images/mangas/manga_20260128_200906_950.png");
    ui->label->setStyleSheet(QString("background-color: rgb(9, 15, 47);"
                                     "image: url(%1);").arg(m_b->getPhotoUrl()));
    rafraichir();
    connect(ui->listWidget, &QListWidget::clicked, this, &FxMangasList::mangaCourant);


}

FxMangasList::~FxMangasList()
{
    delete ui;
    delete m_db;
    delete m_b;
    qDeleteAll(m_listeManga);
    m_listeManga.clear();
}



void FxMangasList::rafraichir()
{
    ui->listWidget->clear();
    qDeleteAll(m_listeManga);
    m_listeManga.clear();

    m_listeManga = m_db->mangas();

    foreach (Manga_metier *m, m_listeManga) {
        QListWidgetItem *item = new QListWidgetItem();
        ui->listWidget->addItem(item);
        QWidget *w = mangaGUI(m);
        item->setData(Qt::UserRole, QVariant::fromValue(m));
        item->setSizeHint(w->sizeHint());
        ui->listWidget->setItemWidget(item, w);

    }
}
void FxMangasList::apresAjou()
{
    rafraichir();

}

void FxMangasList::supprimer()
{
    QList <QListWidgetItem*> items = ui->listWidget->selectedItems();
    QList<Manga_metier*> mangaList;
    foreach (QListWidgetItem *item, items) {
        Manga_metier *m=item->data(Qt::UserRole).value<Manga_metier*>();
        mangaList.append(m);
    }
    QString nomSup;
    foreach (Manga_metier *m, mangaList) {
        nomSup +=m->getNom()+"\n";
    }
    if(QMessageBox::question(this, "Confirmez la suppression : ", nomSup, QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
        foreach (Manga_metier *m, mangaList) {
            m_db->supprimer(m->getId());
        }
    }
    mangaList.clear();
    qDeleteAll(mangaList);
    items.clear();
    qDeleteAll(items);
    rafraichir();

}
void FxMangasList::on_pushButton_clicked()
{
    Manga *z = new Manga(m_db, nullptr);
    z->setAttribute(Qt::WA_DeleteOnClose);
    connect(z, &Manga::mangaAjoute, this, &FxMangasList::rafraichir);
    z->show();

}
void FxMangasList::mangaCourant()
{
    Manga_metier *manga = ui->listWidget->currentItem()->data(Qt::UserRole).value<Manga_metier*>();
    QString url = manga->getPhotoUrl();
    ui->label->setStyleSheet(QString("background-color: rgb(9, 15, 47);"
                                     "image: url(%1);").arg(url));
    qDebug()<<"url : "<<url;

}
void FxMangasList::mangaChecker(bool check)
{
    Manga_metier *manga = ui->listWidget->currentItem()->data(Qt::UserRole).value<Manga_metier*>();
    QString url = manga->getPhotoUrl();
    manga->setRegarder(check);
    m_db->updateRegarder(manga->getId(), check);
    qDebug()<<"Dans le if : "<<manga->getRegarder()<<"Pour : " <<manga->getNom();
       // qDebug()<<"Pas Dans le if : "<<m->getRegarder();

}

QWidget* FxMangasList::mangaGUI(Manga_metier *m)
{
    QWidget *fen = new QWidget();
    QHBoxLayout *fenLay = new QHBoxLayout(fen);
    QLabel *photo = new QLabel();
    if (m->getPhotoUrl()=="Choisir une photo"){
        m->setPhotoUrl("data/images/mangas/manga.jpg");
    }
    photo->setPixmap(QPixmap(m->getPhotoUrl()));
    photo->setFixedSize(64, 64);
    photo->setScaledContents(true);


    QWidget *info = new QWidget();

    QVBoxLayout *fenVLay = new QVBoxLayout(info);

    QLabel *titre = new QLabel(m->getNom());
    QString descriptio = m->getDescription();
    if (descriptio.length() > 100) {
        descriptio = descriptio.left(100) + "...";
    }
    QLabel *description = new QLabel(descriptio);
    titre->setObjectName("mangaTitle");
    titre->setWordWrap(true);
    titre->setStyleSheet(
        "#mangaTitle {"
        "   font-weight: bold;"
        "   font-size: 14px;"
        //"   color: #333333;"
        "}"
        );


    QWidget *dateWidget = new QWidget();
    QHBoxLayout *dateLay = new QHBoxLayout(dateWidget);

    QLabel *date = new QLabel("Créer : "+m->getCreer());
    QLabel *maj = new QLabel("Modifier : "+m->getModifier());
    maj->setAlignment(Qt::AlignRight);
    dateLay->addWidget(date);
    dateLay->addWidget(maj);

    fenVLay->addWidget(titre);
    fenVLay->addWidget(description);
    fenVLay->addWidget(dateWidget);


    QWidget *checkWidget = new QWidget();
    QVBoxLayout *checkLay = new QVBoxLayout(checkWidget);
    checkLay->setAlignment(Qt::AlignCenter);
    QCheckBox *regarder = new QCheckBox("Regardé");
    connect(regarder, &QCheckBox::toggled, this, [this, m](bool checked){
        m->setRegarder(checked);
        m_db->updateRegarder(m->getId(), checked);
    });
    regarder->setChecked(m->getRegarder());
    qDebug()<<m->getRegarder();
    checkLay->addWidget(regarder);
    checkWidget->setFixedWidth(90);



    fenLay->addWidget(photo);
    fenLay->addWidget(info, 1);
    fenLay->addWidget(checkWidget);


    return fen;
}



void FxMangasList::on_pushButton_2_clicked()
{
    auto items = ui->listWidget->selectedItems();
    if(items.size()!=1){
        QMessageBox::information(this, "Attention...", "Sélectionnez un manga.");
        return;
    }
    Manga_metier *manga = ui->listWidget->currentItem()->data(Qt::UserRole).value<Manga_metier*>();

    Manga *z = new Manga(m_db, nullptr);
    z->ChargerPourModifier(manga);

    z->setAttribute(Qt::WA_DeleteOnClose);

    connect(z, &Manga::mangaModifie, this, [this](Manga_metier *m){
        m_db->updateAll(m);
        this->rafraichir();
    });
    z->show();

}



void FxMangasList::on_pushButton_3_clicked()
{
    this->supprimer();
}

