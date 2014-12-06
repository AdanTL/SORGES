#include <QCoreApplication>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "lib/origen.h"
#include "lib/station.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::set<Station> estaciones;
    estaciones.insert(Station("0x0000", "0x0001", 36.00204023875479, 14.2456402219765, 0));
    estaciones.insert(Station("0x0001", "0x0001", 37.00204023875479, 15.2456402219765, 1));
    estaciones.insert(Station("0x0002", "0x0002", 33.00204023875479, 13.2456402219765, 2));
    estaciones.insert(Station("0x0003", "0x0003", 30.00204023875479, 18.2456402219765, 3));

    struct std::tm tm;
    std::istringstream ss("2014-07-02 12:34:20.5");
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    Origen Mi_Origen("0x0001b",tm,36.00204023875479, 14.2456402219765, 3.54, estaciones);

    std::cout << Mi_Origen << std::endl;


    return a.exec();
}
