#include <QtCore/QCoreApplication>
#include "../libunhv3/unhv3/unhv3.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Unhv3 unhv3;

    unhv3.open("a.hv3");
    unhv3.extractAllTo(".");

    return 0;
}
