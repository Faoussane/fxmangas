#ifndef MANGA_METIER_H
#define MANGA_METIER_H

#include <QString>

class Manga_metier
{
public:

    Manga_metier();
    Manga_metier(QString nom, QString description, bool regader, QString url);

    int getId(){ return m_id;}
    bool getRegarder(){ return m_regarder;}
    QString getNom(){ return m_nom;}
    QString getDescription(){ return m_description;}
    QString getPhotoUrl(){ return m_photoUrl;}
    QString getCreer(){ return m_creer;}
    QString getModifier(){ return m_modifier;}

    void setId(int id){m_id=id;}
    void setNom(QString nom){m_nom=nom;}
    void setDescription(QString description){m_description=description;}
    void setPhotoUrl(QString url){m_photoUrl=url;}
    void setCreer(QString date){m_creer=date;}
    void setModifier(QString date){m_modifier=date;}
    void setRegarder(bool a){m_regarder=a;}

private:
    QString m_nom;
    QString m_description;
    QString m_photoUrl;
    QString m_creer;
    QString m_modifier;
    int m_id;
    bool m_regarder;
};

#endif // MANGA_METIER_H
