#include <QCoreApplication>
#include "dataprocessing.h"
#include <QDateTime>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DataProcessing processing;
    int pos = 0;
    int pos2 = 0;
    QDateTime datetime = QDateTime::fromString("2015-01-20 11:30:34.1" ,"yyyy-MM-dd hh:mm:ss.z");
    pos = processing.getPositionBegin(datetime, ":/testFiles/scalertes_origenes.log");
    pos2 = processing.getPositionEnd(datetime,  ":/testFiles/scalertes_origenes.log");

    QFile file(":/testFiles/scalertes_origenes.log");
    file.open(QIODevice::ReadOnly);
    file.seek(pos);
    QString fileContent = file.read(pos2-pos);
    std::cerr << fileContent.toStdString() << std::endl;
    return a.exec();
}
