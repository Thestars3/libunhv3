#include <QtCore/QCoreApplication>
#include "../libunhv3/unhv3/unhv3.hpp"
#include <QImageReader>
#include <QDebug>
#include <QTextStream>
#include <QPluginLoader>
#include <QLibrary>
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::addLibraryPath("../../hdp_image_plugin/Debug");

    Unhv3 unhv3;

    qDebug() << "unhv3.open(\"a.hv3\")" << unhv3.open("a.hv3");
    qDebug() << "unhv3.extractAllTo(\".\")" << unhv3.extractAllTo(".");

    return 0;
}
