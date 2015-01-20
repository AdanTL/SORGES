#include <math.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <QTimer>
#include <QGraphicsItem>
#include <algorithm>
#include "mapwidget.h"
#include "config/mapdefinition.h"
#include "ui_mapwidget.h"

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MapWidget), mapScene(new QGraphicsScene(this)),
                                            circlesTimer(new QTimer(this))
{    
    //Setup for the user interface.
    ui->setupUi(this);

    //Image load from resource
    mapImage.load(MAP_IMAGE_PATH);

    //add image to scene
    QPixmap mapPixmap = QPixmap::fromImage(mapImage);
    QGraphicsItem *mapItem = mapScene.addPixmap(mapPixmap);
    //and set the name of the item
    mapItem->setData (0,"map");
    mapItem->setFlag (QGraphicsItem::ItemIsSelectable, true);

    //and fit rectangle to image limits
    mapScene.setSceneRect(mapPixmap.rect());

    //User interface contains the GraphicsView named mapView (see the ui form)
    ui->mapView->setScene(&mapScene);

    //connect the timeout of the timer to the event to paint the concentric circles
    connect(circlesTimer, SIGNAL(timeout()), this, SLOT(paintCircles()));

	/******tests******/		
    /***borrar de aqui antes de entrega de codigo**/

    //QTimer::singleShot(15000, this, SLOT(testing()));
    
    //pintado ejemplo circulo
    //paintCircles();

    //prueba de precision de coordenadas-pixel
    //testPixelPrecision();
    
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

    return degrees + (long double)(minutes/60) + (long double)(seconds/3600);
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
    this->stations.clear();
    clearStations();
    this->stations = stationsList;
    for(std::set<Station>::iterator it=stations.begin(); it!=stations.end(); ++it)
        drawStation(*it);
}

void MapWidget::clearStations()
{
    foreach(QGraphicsItem * item, mapScene.items()){
        if (item->data(1).toString () == "station"){
            mapScene.removeItem(item);
        }
    }
}

void MapWidget::clearStation(const std::string &stationID)
{
    foreach(QGraphicsItem * item, mapScene.items()){
        if (item->data(0).toString () == stationID.c_str()){
            mapScene.removeItem(item);
            break;
        }
    }
}

void MapWidget::drawStation(const Station& station)
{
    long double coordX, coordY;
    coordinatesToPixels(coordX,coordY,station.getLatitude(),station.getLongitude());

    QPolygonF triangle;
    triangle.append(QPoint(coordX,coordY));
    triangle.append(QPoint(coordX+STATION_SIZE_X,coordY-STATION_SIZE_Y));
    triangle.append(QPoint(coordX-STATION_SIZE_X,coordY-STATION_SIZE_Y));

    //in case there is a station already there, we delete it
    clearStation(station.getStationID());

    //and we add the new one
    const char * color = station.getCurrentOnSiteAlert();
    QGraphicsItem *stationItem = mapScene.addPolygon(triangle,QPen(),QBrush(color));
    stationItem->setData(0,station.getStationID().c_str());
    stationItem->setData(1,"station");
    stationItem->setFlag (QGraphicsItem::ItemIsSelectable, true);

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

void MapWidget::clearOrigin()
{
    foreach(QGraphicsItem * item, mapScene.items()){
        QString itemName = item->data(0).toString ();
        if ( (itemName == "epicenter") || (itemName == "circle")){
            mapScene.removeItem(item);
        }
    }
}

void MapWidget::updateEventStations()
{
    std::set<Station> relatedStations = this->currentOrigin.getStations();

    //update stations related
    changeStationsColors(relatedStations);

    //turn off to black the non-related
    foreach (Station currentStation, this->stations) {
        if (relatedStations.find(currentStation) == relatedStations.end()){
            currentStation.setColor(-1);
            drawStation(currentStation);
            stations.erase(stations.find(currentStation));
            stations.insert(currentStation);
        }
    }
}

void MapWidget::paintOrigin(const Origin &origin){

    //if timer of circles painting is on, stop it
    if (circlesTimer->isActive ())
        circlesTimer->stop();

    //if there is a current origin, we erase it
    clearOrigin();

    this->currentOrigin = origin;

    long double coordX, coordY;
    long double radius = calculateRadius();

    coordinatesToPixels(coordX,coordY,currentOrigin.getLatitude(),
                                      currentOrigin.getLongitude());
    QPoint center(coordX, coordY);

    //First circle
    QRect rect(0,0,2*radius,2*radius);
    rect.moveCenter(center);
    QGraphicsItem *circleItem = mapScene.addEllipse(rect,
                                                    QPen(),
                                                    QBrush(QColor(R_EPICENTER_FIRST_CIRCLE,
                                                                  G_EPICENTER_FIRST_CIRCLE,
                                                                  B_EPICENTER_FIRST_CIRCLE,
                                                                  T_EPICENTER_FIRST_CIRCLE)));
    circleItem->setData(0,"circle");

    //Epicenter mark (to be on top of the first circle)
    QRect rect2(0,0,2*RADIUS_EPICENTER,2*RADIUS_EPICENTER);
    rect2.moveCenter(center);
    QGraphicsItem *epicenterItem = mapScene.addEllipse(rect2,
                                                       QPen(),
                                                       QBrush(QColor(R_EPICENTER,
                                                                     G_EPICENTER,
                                                                     B_EPICENTER,
                                                                     T_EPICENTER)));
    epicenterItem->setData(0,"epicenter");
    epicenterItem->setFlag (QGraphicsItem::ItemIsSelectable, true);

    //Set the timer (each 5 seconds) for the concentric circles
    circlesTimer->start(5000);

    //only the last origin (event) will have related stations
    if (!currentOrigin.getStations().empty()){
        updateEventStations();
    }

}



/**CONCENTRIC CIRCLES**/

/**
 * Function for calculating the expansion circle radius every 5 seconds
 */
long double MapWidget::calculateRadius()
{
    long double radius;
    long int difMseconds;
    QTime timeinfo = QTime::currentTime();
    QDate dateinfo = QDate::currentDate();

    // Getting the system time and the origin time diference (only h/m/s).
    difMseconds = currentOrigin.getOriginDate().daysTo(dateinfo)*24*3600000;
    difMseconds += currentOrigin.getOriginTime().msecsTo(timeinfo);
    // getting the radius in meters.
    radius = (difMseconds/1000) * PROPAGATION_SPEED;

    // Calculate the number of pixels to "Radius meters".
    //meanMetres is the medium between the referenced distances in metres of
    //the X-axis at the top and and the botton of the image, which are different
    //due to the shape of heart and its projection
    long double meanMetres = (MAP_METRES_LONGITUDE_BOTTOM+MAP_METRES_LONGITUDE_TOP)/2;
    return radius/meanMetres*mapScene.width();

}


/**
 * Painter method for the concentric circles of the origins
 * representing the seismic wave expansion.
 * Authomatically called by timer each 5s when a origin is set
 */
void MapWidget::paintCircles(){

    //center of the circles: origin epicenter
    long double x,y;
    coordinatesToPixels(x,y,currentOrigin.getLatitude(),currentOrigin.getLongitude());
    QPoint center(x,y);

    //radius of the circle at current time
    long double radius = calculateRadius();

    //circle will be fit into a rectangle whose center is moved to epicenter
    QRect rect(0,0,2*radius,2*radius);
    rect.moveCenter(center);

    QGraphicsItem *circleItem = mapScene.addEllipse(rect);
    circleItem->setData(0,"circle");

}


/***************************************tests**********************************/


/*
 * slot de prueba para simular un cambio en la llegada de un nuevo origen
 * (llama con un nuevo objeto a paintOrigin, que ya se encarga de limpiar y pintar)
*/
void MapWidget::testing(){
    Origin myOrigin("0x0001b",QDate().currentDate (),QTime().currentTime ().addSecs (-5),
                    36.533517,
                    -6.300797,
                    3.54);
    paintOrigin(myOrigin);
}
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

    // getting time from system (best to test it)
    //it simulates 9 seconds of delay:
    QTime timeinfo = QTime(QTime::currentTime().addMSecs(-999));
    QDate dateinfo = QDate::currentDate();

    // getting time from string: [TESTED AND WORK]
    //QTime timeinfo2 = QTime::fromString ("13:54:00.5","hh:mm:ss.z");
    //QDate dateinfo2 = QDate::fromString ("2014-12-10","yyyy-MM-dd");
    //std::cout << timeinfo2.hour() << "--" << timeinfo2.minute() 
    //			<< "--" << timeinfo2.second() << std::endl;
    //std::cout << dateinfo2.year() << "--" << dateinfo2.month() << "--" 
    //			<< dateinfo2.day() << std::endl;

    //Punta san felipe Cádiz 36°32'16.12"  -6°-18'-1.20"
    Origin myOrigin("0x0001b",dateinfo,timeinfo, 
    				convertToDecimalDegrees(36,32,16.12),
                    convertToDecimalDegrees(-6,-18,-1.20), 
                    3.54, mystations);
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
