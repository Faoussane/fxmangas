#include "manga_metier.h"

Manga_metier::Manga_metier() {}

Manga_metier::Manga_metier(QString nom, QString description, bool regader, QString url)
{
    m_nom=nom;
    m_description=description;
    m_regarder=regader;
    m_photoUrl=url;

}
//Les getters et les setters sont dans le .h
