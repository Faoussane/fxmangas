#include "manga.h"
#include "ui_manga.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QRandomGenerator>
#include <QCheckBox>

Manga::Manga(Db *db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Manga)
    ,m_db(db)
    ,manga(nullptr)
    ,modeModification(false)
{
    ui->setupUi(this);
    url = nullptr;
}

void Manga::ChargerPourModifier(Manga_metier *m)
{
    if(!m) {
        return;
    }
    manga=m;
    modeModification=true;
    ui->lineEdiTitre->setText(m->getNom());
    ui->textEditDescription->setText(m->getDescription());
    if(m->getPhotoUrl().isEmpty()){
        m->setPhotoUrl("Ajouter une Photo");
    }
    ui->pushButtonPhoto->setText(m->getPhotoUrl());
    ui->checkBoxRegarder->setChecked(m->getRegarder());
}

Manga::~Manga()
{
    delete ui;
    delete manga;
}

Manga::Manga()
{

}

QString Manga::choisirEtCopierImageUtilisateur()
{
    // 1. Demander à l'utilisateur de choisir une image
    QString fichierSource = QFileDialog::getOpenFileName(
        this,
        "Choisir une image de couverture",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "Images (*.png *.jpg *.jpeg *.bmp)"
        );

    if (fichierSource.isEmpty()) {
        return QString(); // Annulé
    }

    // 2. Vérifier l'image
    QPixmap imageTest(fichierSource);
    if (imageTest.isNull()) {
        QMessageBox::warning(this, "Erreur", "Image invalide");
        return QString();
    }

    // 3. DÉTERMINER le dossier cible (à côté de l'exécutable)
    QString dossierApp = QCoreApplication::applicationDirPath();
    QString dossierImages = dossierApp + "/data/images/mangas/";

    // 4. Créer le dossier s'il n'existe pas
    QDir dir(dossierApp);
    if (!dir.exists("data/images/mangas")) {
        dir.mkpath("data/images/mangas");
    }

    // 5. Générer un nom unique
    QFileInfo sourceInfo(fichierSource);
    QString extension = sourceInfo.suffix().toLower();

    // Utiliser un timestamp + random pour garantir l'unicité
    QString nomFichier = QString("manga_%1_%2.%3")
                             .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"))
                             .arg(QRandomGenerator::global()->bounded(10000))
                             .arg(extension);

    QString cheminComplet = dossierImages + nomFichier;

    // 6. Copier l'image
    if (!QFile::copy(fichierSource, cheminComplet)) {
        QMessageBox::warning(this, "Erreur",
                             QString("Impossible de copier:\n%1\nvers:\n%2")
                                 .arg(fichierSource)
                                 .arg(cheminComplet));
        return QString();
    }

    // 7. Redimensionner si nécessaire
    /*if (imageTest.width() > 600 || imageTest.height() > 800) {
        imageTest = imageTest.scaled(600, 800, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageTest.save(cheminComplet);
    }*/

    // 8. IMPORTANT : Retourner le chemin RELATIF (pas absolu)
    // Pour que ça marche après déploiement
    QMessageBox::information(this, "succès","Le chemin : data/images/mangas/" + nomFichier);
    return "data/images/mangas/" + nomFichier;
}




void Manga::on_pushButtonValider_clicked()
{
    if(!manga){
        manga = new Manga_metier();
    }
    manga->setNom(ui->lineEdiTitre->text());
    manga->setDescription(ui->textEditDescription->toPlainText());

    manga->setRegarder(ui->checkBoxRegarder->isChecked());
    if(manga->getNom().isEmpty()){
        QMessageBox::warning(this,"Titre Manga", "La champ titre est obligation..");
        return;
    }
    if(manga->getDescription().isEmpty()){
        manga->setDescription("Aucune description");
    }
    if(manga->getPhotoUrl()=="Choisir une photo"){
        manga->setPhotoUrl("data/images/mangas/manga.jpg");
    }
    manga->setPhotoUrl(url);
    if(modeModification){
        emit mangaModifie(manga);
        close();
        return;
    }
    if(!modeModification && m_db->existe(manga->getNom())){
        QMessageBox::information(this, "Existe", "Ce manga existe déjà.");
        return;
    }
    if(m_db->ajouter(manga)){
        QMessageBox::information(this, "Succès", "Manga ajouté avec succès.");
        emit mangaAjoute();
        close();
    }else{
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout");
    }

}


void Manga::on_pushButtonPhoto_clicked()
{
    url = QString(choisirEtCopierImageUtilisateur());
    ui->pushButtonPhoto->setText(url);
}

