#include "fxmangaslist.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FxMangasList w;
    w.show();
    return a.exec();
}
