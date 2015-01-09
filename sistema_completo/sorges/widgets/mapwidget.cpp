#include <math.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "mapwidget.h"
#include "config/mapdefinition.h"
#include "ui_mapwidget.h"

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MapWidget), mapScene(new QGraphicsScene(this))
{
    //Setup for the user interface.
    ui->setupUi(this);

    //Image load from resource
    mapImage.load(MAP_IMAGE_PATH);
    //add image to scene
    QPixmap mapPixmap = QPixmap::fromImage(mapImage);
    mapScene.addPixmap(mapPixmap);
    //and fit rectangle to image limits
    mapScene.setSceneRect(mapPixmap.rect());

    //User interface contains the GraphicsView named mapView (see the ui form)
    ui->mapView->setScene(&mapScene);


    /******tests******/
    /***borrar de aqui antes de entrega de codigo**/

    //pintado ejemplo circulo
    //paintCircles();

    //prueba de precision de coordenadas-pixel
    testPixelPrecision();

    //prueba de colocación staciones en el mapa.
    //testStation();

    //prueba de colocación origen en el mapa.
    //testOrigen();

    /*****tests********/
}

MapWidget::~MapWidget()
{
    delete ui;
}

/**CONVERTER FUNCTIONS FROM COORDINATES TO PIXELS**/

/**Converter from degrees-minutes-seconds to decimal degrees*/
long double MapWidget::convertToDecimalDegrees(long double degrees,
                                               long double minutes,
                                               long double seconds)
{

    return degrees + (minutes/60) + (seconds/3600);
}

/*
 * Interpolation function which transform received coordinates to map image pixels
 * (version receiving degrees-minutes-seconds
*/
void MapWidget::coordinatesToPixels(long double &pixelX,long double &pixelY,
                                    long double degreesTargetLat,
                                    long double minutesTargetLat,
                                    long double secondsTargetLat,
                                    long double degreesTargetLon,
                                    long double minutesTargetLon,
                                    long double secondsTargetLon)
{
    long double minLon = MAP_MIN_LONGITUDE;
    long double minLat = MAP_MIN_LATITUDE;
    long double maxLon = MAP_MAX_LONGITUDE;
    long double maxLat = MAP_MAX_LATITUDE;
    long double targetLon = convertToDecimalDegrees(degreesTargetLon,
                                                    minutesTargetLon,
                                                    secondsTargetLon);
    long double targetLat = convertToDecimalDegrees(degreesTargetLat,
                                                    minutesTargetLat,
                                                    secondsTargetLat);
    long double maxXpixel = mapScene.width();
    long double maxYpixel = mapScene.height();
    long double minXpixel = 0;
    long double minYpixel = 0;

    pixelX = ((targetLon - minLon) / (maxLon - minLon)) * (maxXpixel - minXpixel);
    pixelY = ((targetLat - minLat) / (maxLat - minLat)) * (maxYpixel - minYpixel);
}

/*
 * Interpolation function which transform received coordinates to map image pixels
 * (version receiving decimal degrees
*/
void MapWidget::coordinatesToPixels(long double &pixelX,long double &pixelY,
                                    long double targetLat,long double targetLon)
{
    long double minLon = MAP_MIN_LONGITUDE;
    long double minLat = MAP_MIN_LATITUDE;
    long double maxLon = MAP_MAX_LONGITUDE;
    long double maxLat = MAP_MAX_LATITUDE;
    long double maxXpixel = mapScene.width();
    long double maxYpixel = mapScene.height();
    long double minXpixel = 0;
    long double minYpixel = 0;

    pixelX = ((targetLon - minLon) / (maxLon - minLon)) * (maxXpixel - minXpixel);
    pixelY = ((targetLat - minLat) / (maxLat - minLat)) * (maxYpixel - minYpixel);
}



/**STATION FUNCTIONS**/

void MapWidget::paintStations(const std::set<Station>& stationsList)
{
    this->stations = stationsList;
    for(std::set<Station>::iterator it=stations.begin(); it!=stations.end(); ++it)
        drawStation(*it);
}

void MapWidget::drawStation(const Station& station)
{
    long double coordX, coordY;
    coordinatesToPixels(coordX,coordY,station.getLatitude(),station.getLongitude());
    QPolygonF Triangle;
    Triangle.append(QPoint(coordX,coordY));
    Triangle.append(QPoint(coordX+STATION_SIZE_X,coordY-STATION_SIZE_Y));
    Triangle.append(QPoint(coordX-STATION_SIZE_X,coordY-STATION_SIZE_Y));
    mapScene.addPolygon(Triangle,
                        QPen(),
                        QBrush(station.getCurrentOnSiteAlert()));
}

void MapWidget::changeStationsColors(const std::set<Station> &changedStations)
{
    for(std::set<Station>::iterator it=changedStations.begin();
                                    it!=changedStations.end(); ++it)
    {
        stations.erase(stations.find(*it));
        stations.insert(*it);
        drawStation(*it);
    }
}



/**ORIGIN FUNCTIONS**/

void MapWidget::paintOrigin(const Origin &origin){
    this->currentOrigin = origin;

    long double coordX, coordY;
    long double radius = calculateRadius();

    coordinatesToPixels(coordX,coordY,currentOrigin.getLatitude(),
                                      currentOrigin.getLongitude());
    QPoint center(coordX, coordY);

    //change the color of related stations
    changeStationsColors(currentOrigin.getStations());

    //First circle
    QRect rect(0,0,2*radius,2*radius);
    rect.moveCenter(center);
    mapScene.addEllipse(rect,QPen(),
                             QBrush(QColor(R_EPICENTER_FIRST_CIRCLE,
                                           G_EPICENTER_FIRST_CIRCLE,
                                           B_EPICENTER_FIRST_CIRCLE,
                                           T_EPICENTER_FIRST_CIRCLE)));

    //Epicenter mark (to be on top of the first circle)
    QRect rect2(0,0,2*RADIUS_EPICENTER,2*RADIUS_EPICENTER);
    rect2.moveCenter(center);
    mapScene.addEllipse (rect2,QPen(),QBrush(QColor( R_EPICENTER,
                                                     G_EPICENTER,
                                                     B_EPICENTER,
                                                     T_EPICENTER)));
}



/**CONCENTRIC CIRCLES**/

/**
 * Function for calculating the expansion circle radius every 5 seconds
 */
float MapWidget::calculateRadius()
{
    long double radius;
    long int difSeconds=0;
    time_t rawtime;
    struct std::tm * timeinfo;

    // Getting the system time and the origin time diference (only h/m/s).
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::cout <<  currentOrigin.getOriginTime().tm_hour << " -- " <<
                  timeinfo->tm_min - currentOrigin.getOriginTime().tm_min <<
                  " -- " <<  timeinfo->tm_sec - currentOrigin.getOriginTime().tm_sec
                  << " -- " << std::endl;
    difSeconds = timeinfo->tm_sec - currentOrigin.getOriginTime().tm_sec;
    //difSeconds += (timeinfo->tm_min - currentOrigin.getOriginTime().tm_min)*60;
    //difSeconds += (timeinfo->tm_hour - currentOrigin.getOriginTime().tm_hour)*3600;

    // getting the radius in meters.
    radius = difSeconds * PROPAGATION_SPEED;

    // Calculate the numbers of pixels to "Radius meters".
    std::cout << currentOrigin.getOriginTime().tm_sec << std::endl;
    return (radius/MAP_METRES_LONGITUDE)*mapScene.width();

}


/**funcion que pinta un circulo de expansion
 * centro en epicentro
 * radio calculado con la funcion calculateRadius()
 * FALTA HACERLA AUTOMATICA CADA 5s
 */
void MapWidget::paintCircles(){

    //definir el centro del círculo: coordenadas del origen/evento
    //pasar las coordenadas almacenadas en el atributo currentOrigin a pixeles
    //y despues pasarselas al constructor de center
    /*
    long double x,y;
    coordinatesToPixels(x,y,currentOrigin.getLatitude(),currentOrigin.getLongitude());
    QPoint center(x,y);
    */
    //--temporalmente definido el centro en el centro de la imagen
    QPoint center(mapScene.width()/2, mapScene.height ()/2);

    //definir radio del círculo
    float radius = 10;

    //rectágulo que va a contener la elipse,
    //coordenada superior 0,0 y tamaño 2*radio tanto alto como ancho
    QRect rect(0,0,2*radius,2*radius);

    //ESTO ES CLAVE: mover el rectágulo contenedor de manera que el centro del circulo sea el deseado
    rect.moveCenter(center);

    //pintar el circulo sobre la escena que contiene el mapa
    mapScene.addEllipse (rect);
}


/***************************************tests**********************************/

/*
 * Funcion privada para probar la precision de la conversion de coordenadas a
 * pixeles de la imagen
 */
void MapWidget::testPixelPrecision(){
    //prueba coordenadas salida por consola
    //esquinas
    std::cout << convertToDecimalDegrees(38,0,0)<< std::endl;
    std::cout << convertToDecimalDegrees(-14,0,0)<< std::endl;
    std::cout << convertToDecimalDegrees(34,0,0)<< std::endl;
    std::cout << convertToDecimalDegrees(-3,0,0)<< std::endl;

    //cabo san vicente 36°59'39.61" (36.994336) y -8°-56'-9.60" (-8.936)
    std::cout << convertToDecimalDegrees(36,59,39.61)<< std::endl;
    std::cout << convertToDecimalDegrees(-8,-56,-9.6)<< std::endl;
    long double x,y;
    coordinatesToPixels(x,y,36.994336,-8.936);
    std::cout << x << ' ' << y << std::endl;
    //pintado ejemplo coordenadas (linea desde cabo san vicente
    //hasta esquina izquierda arriba)
    mapScene.addLine (x,y,0,0);

    //punta gibraltar 36° 6'34.01" (36.109447) -5°-20'-43.59" (-5.345442)
    std::cout << convertToDecimalDegrees(36,6,34.01)<< std::endl;
    std::cout << convertToDecimalDegrees(-5,-20,-43.59)<< std::endl;
    long double x2,y2;
    coordinatesToPixels(x2,y2,36.109447,-5.345442);
    std::cout << x2 << ' ' << y2 << std::endl;
    //pintado ejemplo coordenadas (linea desde gibraltar hasta derecha abajo
    mapScene.addLine (x2,y2,mapScene.width (),mapScene.height ());

    //Punta san felipe Cádiz 36°32'16.12"  -6°-18'-1.20"
    std::cout << convertToDecimalDegrees(36,32,16.12)<< std::endl;
    std::cout << convertToDecimalDegrees(-6,-18,-1.20)<< std::endl;
    long double x3,y3;
    coordinatesToPixels(x3,y3,36,32,16.12,-6,-18,-1.20);
    std::cout << x3 << ' ' << y3 << std::endl;
    //pintado ejemplo coordenadas (linea desde cadiz hasta izquierda abajo
    mapScene.addLine (x3,y3,0,mapScene.height ());

    //Costa de Lisboa (referencia Plaza del Comercio) 38°42'22.00" -9°-8'-10"
    std::cout << convertToDecimalDegrees(38,42,22)<< std::endl;
    std::cout << convertToDecimalDegrees(-9,-8,-10)<< std::endl;
    long double x4,y4;
    coordinatesToPixels(x4,y4,38,42,22,-9,-8,-10);
    std::cout << x4 << ' ' << y4 << std::endl;
    //pintado ejemplo coordenadas (linea desde el punto hasta derecha arriba
    mapScene.addLine (x4,y4,mapScene.width(),0);

    //Cabo de Peniche (por encima de Lisboa) 39°21'30.87" -9°-24'-24.36"
    std::cout << convertToDecimalDegrees(39,21,30.87)<< std::endl;
    std::cout << convertToDecimalDegrees(-9,-24,-24.36)<< std::endl;
    long double x5,y5;
    coordinatesToPixels(x5,y5,39,21,30.87,-9,-24,-24.36);
    std::cout << x5 << ' ' << y5 << std::endl;
    //pintado ejemplo coordenadas (linea desde mitad del eje x al punto
    mapScene.addLine (x5,y5,mapScene.width()/2,0);
}

/*
 * Funcion privada para probar la colocacion de un Origen en la imagen
 */
void MapWidget::testOrigen(){
    std::set<Station> mystations;
    mystations.insert(
                Station("0x0000", "0x0001", 37.00204023875479, -10.2456402219765,0));
    mystations.insert(
                Station("0x0001", "0x0001", 34.00204023875479, -6.2456402219765,1));
    mystations.insert(
                Station("0x0002", "0x0002", 36.00204023875479, -4.2456402219765,2));
    mystations.insert(
                Station("0x0003", "0x0003", 35.00204023875479, -8.2456402219765,3));

    // getting time from system (best to test it):
    time_t rawtime;
    struct std::tm timeinfo;
    time (&rawtime);
    timeinfo = std::tm(*localtime (&rawtime));

    //System time - 3.
    timeinfo.tm_sec -= 13;

    //Punta san felipe Cádiz 36°32'16.12"  -6°-18'-1.20" -> por google maps
    Origin myOrigin("0x0001b",timeinfo, convertToDecimalDegrees(36,32,16.12),
                    convertToDecimalDegrees(-6,-18,-1.20), 3.54, mystations);
    paintOrigin(myOrigin);
}

/*
 * Funcion privada para probar la colocacion de estaciones en la imagen
 */
void MapWidget::testStation(){
    std::set<Station> mystations;
    mystations.insert(Station("0x0000", "0x0001",
                              convertToDecimalDegrees(36,59,39.61),
                              convertToDecimalDegrees(-8,-56,-9.6), -1));
    mystations.insert(Station("0x0001", "0x0002",
                              convertToDecimalDegrees(36,32,16.12),
                              convertToDecimalDegrees(-6,-18,-1.20), 0));
    mystations.insert(Station("0x0002", "0x0003",
                              convertToDecimalDegrees(36,32,16.12),
                              convertToDecimalDegrees(-7,-18,-1.20), 1));
    mystations.insert(Station("0x0003", "0x0004",
                              convertToDecimalDegrees(36,40,17.19),
                              convertToDecimalDegrees(-4,-13,-1.10), 2));
    mystations.insert(Station("0x0004", "0x0005",
                              convertToDecimalDegrees(35,36,12.12),
                              convertToDecimalDegrees(-7,-4,-1.22), 3));
    paintStations(mystations);

    //changing one colour:
    std::set<Station> testChangeStationsColor;
    testChangeStationsColor.insert(Station("0x0000", "0x0001",
                                           convertToDecimalDegrees(36,59,39.61),
                                           convertToDecimalDegrees(-8,-56,-9.6), 0));
    changeStationsColors(testChangeStationsColor);
}

/***************************************tests**********************************/
