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
    //indicar la interfaz de usuario del widget
    ui->setupUi(this);

    //cargar la imagen en el atributo QImage (imagen ya añadida como recurso)
    mapImage.load(MAP_IMAGE_PATH);

    //convertir a tipo pixmap para poder añadirla a la escena gráfica
    QPixmap mapPixmap = QPixmap::fromImage(mapImage);
    mapScene.addPixmap(mapPixmap);

    //ajustar el rectángulo contenedor de la escena al tamaño exacto de la imagen
    mapScene.setSceneRect(mapPixmap.rect());

    //decirle al graphicsview de la interfaz de usuario que su escena es esta
    ui->mapView->setScene(&mapScene);

    //pintado ejemplo circulo
    //paintCircles();

    //prueba de precision de coordenadas-pixel
    //testPixelPrecision();

    //prueba de colocación staciones en el mapa.
    testStation();

    //prueba de colocación origen en el mapa.
    testOrigen();
}

MapWidget::~MapWidget()
{
    delete ui;
}

/*
 * Funcion privada para probar la precision de la conversion de coordenadas a pixeles de la imagen
 */
void MapWidget::testPixelPrecision(){
    //prueba coordenadas salida por consola
    //esquinas
    std::cout << convertToDecimalDegrees(38,0,0)<< std::endl;
    std::cout << convertToDecimalDegrees(-14,0,0)<< std::endl;
    std::cout << convertToDecimalDegrees(34,0,0)<< std::endl;
    std::cout << convertToDecimalDegrees(-3,0,0)<< std::endl;

    //cabo san vicente 36°59'39.61" (36.994336) y -8°-56'-9.60" (-8.936) -> por google maps
    std::cout << convertToDecimalDegrees(36,59,39.61)<< std::endl;
    std::cout << convertToDecimalDegrees(-8,-56,-9.6)<< std::endl;
    long double x,y;
    coordinatesToPixels(x,y,36.994336,-8.936);
    std::cout << x << ' ' << y << std::endl;
    //pintado ejemplo coordenadas (linea desde cabo san vicente hasta esquina izquierda arriba)
    mapScene.addLine (x,y,0,0);

    //punta gibraltar 36° 6'34.01" (36.109447) -5°-20'-43.59" (-5.345442) -> por google maps
    std::cout << convertToDecimalDegrees(36,6,34.01)<< std::endl;
    std::cout << convertToDecimalDegrees(-5,-20,-43.59)<< std::endl;
    long double x2,y2;
    coordinatesToPixels(x2,y2,36.109447,-5.345442);
    std::cout << x2 << ' ' << y2 << std::endl;
    //pintado ejemplo coordenadas (linea desde gibraltar hasta derecha abajo
    mapScene.addLine (x2,y2,mapScene.width (),mapScene.height ());

    //Punta san felipe Cádiz 36°32'16.12"  -6°-18'-1.20" -> por google maps
    std::cout << convertToDecimalDegrees(36,32,16.12)<< std::endl;
    std::cout << convertToDecimalDegrees(-6,-18,-1.20)<< std::endl;
    long double x3,y3;
    coordinatesToPixels(x3,y3,36,32,16.12,-6,-18,-1.20);
    std::cout << x3 << ' ' << y3 << std::endl;
    //pintado ejemplo coordenadas (linea desde cadiz hasta izquierda abajo
    mapScene.addLine (x3,y3,0,mapScene.height ());
}

/*
 * Funcion privada para probar la colocacion de un Origen en la imagen
 */
void MapWidget::testOrigen(){
    std::set<Station> mystations;
    mystations.insert(Station("0x0000", "0x0001", 37.00204023875479, -10.2456402219765, 0));
    mystations.insert(Station("0x0001", "0x0001", 34.00204023875479, -6.2456402219765, 1));
    mystations.insert(Station("0x0002", "0x0002", 36.00204023875479, -4.2456402219765, 2));
    mystations.insert(Station("0x0003", "0x0003", 35.00204023875479, -8.2456402219765, 3));

    // getting time from system (best to test it):
    time_t rawtime;
    struct std::tm timeinfo;
    time (&rawtime);
    timeinfo = std::tm(*localtime (&rawtime));

    //System time - 3.
    timeinfo.tm_sec -= 13;

    // getting time from string:
    //struct std::tm tm;
    //std::istringstream ss("2014-12-10 13:54:00");
    //ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    //Punta san felipe Cádiz 36°32'16.12"  -6°-18'-1.20" -> por google maps
    Origin myOrigin("0x0001b",timeinfo, convertToDecimalDegrees(36,32,16.12), convertToDecimalDegrees(-6,-18,-1.20), 3.54, mystations);
    paintOrigin(myOrigin);
}

/*
 * Funcion privada para probar la colocacion de estaciones en la imagen
 */
void MapWidget::testStation(){
    std::set<Station> mystations;
    mystations.insert(Station("0x0000", "0x0001", convertToDecimalDegrees(36,59,39.61), convertToDecimalDegrees(-8,-56,-9.6), -1));
    mystations.insert(Station("0x0001", "0x0002", convertToDecimalDegrees(36,32,16.12), convertToDecimalDegrees(-6,-18,-1.20), 0));
    mystations.insert(Station("0x0002", "0x0003", convertToDecimalDegrees(36,32,16.12), convertToDecimalDegrees(-7,-18,-1.20), 1));
    mystations.insert(Station("0x0003", "0x0004", convertToDecimalDegrees(36,40,17.19), convertToDecimalDegrees(-4,-13,-1.10), 2));
    mystations.insert(Station("0x0004", "0x0005", convertToDecimalDegrees(35,36,12.12), convertToDecimalDegrees(-7,-4,-1.22), 3));
    paintStations(mystations);

    //changing one colour:
    std::set<Station> testChangeStationsColor;
    testChangeStationsColor.insert(Station("0x0000", "0x0001", convertToDecimalDegrees(36,59,39.61), convertToDecimalDegrees(-8,-56,-9.6), 0));
    changeStationsColors(testChangeStationsColor);
}


/**COORDENADAS->PIXELES**/

/**funcion que convierte grados minutos y segundos a grados en decima*/
long double MapWidget::convertToDecimalDegrees(long double degrees, long double minutes, long double seconds){
    return degrees + (minutes/60) + (seconds/3600);
}

/*funcion que interpola y transforma las cooordenadas geográficas que se le indiquen a pixeles de la imagen
 *--version recibiendo las coordenadas en grados minutos y segundos
*/
void MapWidget::coordinatesToPixels(long double &pixelX,long double &pixelY,
                                    long double degreesTargetLat,long double minutesTargetLat,long double secondsTargetLat,
                                    long double degreesTargetLon, long double minutesTargetLon,long double secondsTargetLon)
{
    long double minLon = MAP_MIN_LONGITUDE;
    long double minLat = MAP_MIN_LATITUDE;
    long double maxLon = MAP_MAX_LONGITUDE;
    long double maxLat = MAP_MAX_LATITUDE;
    long double targetLon = convertToDecimalDegrees(degreesTargetLon, minutesTargetLon, secondsTargetLon);
    long double targetLat = convertToDecimalDegrees(degreesTargetLat, minutesTargetLat, secondsTargetLat);
    long double maxXpixel = mapScene.width();
    long double maxYpixel = mapScene.height();
    long double minXpixel = 0;
    long double minYpixel = 0;

    pixelX = ((targetLon - minLon) / (maxLon - minLon)) * (maxXpixel - minXpixel);
    pixelY = ((targetLat - minLat) / (maxLat - minLat)) * (maxYpixel - minYpixel);
}

/*funcion que interpola y transforma las cooordenadas geográficas que se le indiquen a pixeles de la imagen
 *--version recibiendo las coordenadas ya en decimal
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





/**ESTACIONES**/
void MapWidget::paintStations(const std::set<Station>& stations){
    this->stations = stations;
    for(std::set<Station>::iterator it=stations.begin(); it!=stations.end(); ++it)
        drawStation(*it);

}


void MapWidget::drawStation(const Station& station){
    long double coordX, coordY;
    coordinatesToPixels(coordX,coordY,station.getLatitude(),station.getLongitude());
    QPolygonF Triangle;
    Triangle.append(QPoint(coordX,coordY));
    Triangle.append(QPoint(coordX+STATION_SIZE_X,coordY-STATION_SIZE_Y));
    Triangle.append(QPoint(coordX-STATION_SIZE_X,coordY-STATION_SIZE_Y));
    QGraphicsPolygonItem* pTriangleItem = mapScene.addPolygon(Triangle,QPen(),QBrush(Station::onSiteAlert[station.getColor()]));
}

void MapWidget::changeStationsColors(const std::set<Station> &c_stations){
    paintStations(c_stations);
    for(std::set<Station>::iterator it=c_stations.begin(); it!=c_stations.end(); ++it){
        stations.erase(stations.find(*it));
        stations.insert(*it);
        //drawStation(*it);
    }
}



/**ORIGEN**/
void MapWidget::paintOrigin(const Origin &origin){
    long double coordX, coordY;
    this->currentOrigin = origin;
    long double radius = calculateRadius();

    coordinatesToPixels(coordX,coordY,currentOrigin.getLatitude(),currentOrigin.getLongitude());

    //pintamos las estaciones con su nuevo color
    changeStationsColors(currentOrigin.getStations());

    QPoint center(coordX, coordY);

    // pintamos el primer circulo de expansion.
    QRect rect(0,0,2*radius,2*radius);
    rect.moveCenter(center);
    mapScene.addEllipse (rect,QPen(),QBrush(QColor(R_EPICENTER_FIRST_CIRCLE,G_EPICENTER_FIRST_CIRCLE,B_EPICENTER_FIRST_CIRCLE,T_EPICENTER_FIRST_CIRCLE)));

    // pintamos el epicentro.
    QRect rect2(0,0,2*RADIUS_EPICENTER,2*RADIUS_EPICENTER);
    rect2.moveCenter(center);
    mapScene.addEllipse (rect2,QPen(),QBrush(QColor( R_EPICENTER, G_EPICENTER, B_EPICENTER, T_EPICENTER)));
}



/**CIRCULOS CONCENTRICOS**/

/**funcion que calcula el radio del círculo de expansion
 * fórmula-> radio = (tiempo actual - tiempo origen) * velocidad de propagacion de la onda
 * la diferencia de tiempo habrá que medirla dependiendo de las unidades de la velocidad
 */
float MapWidget::calculateRadius(){
    long double radius;
    long int difSeconds=0;
    time_t rawtime;
    struct std::tm * timeinfo;

    // Getting the system time and the origin time diference (only h/m/s).
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    std::cout <<  currentOrigin.getOriginTime().tm_hour << " -- " <<  timeinfo->tm_min - currentOrigin.getOriginTime().tm_min << " -- " <<  timeinfo->tm_sec - currentOrigin.getOriginTime().tm_sec << " -- " << std::endl;
    difSeconds = timeinfo->tm_sec - currentOrigin.getOriginTime().tm_sec;
    //difSeconds += (timeinfo->tm_min - currentOrigin.getOriginTime().tm_min)*60;
    //difSeconds += (timeinfo->tm_hour - currentOrigin.getOriginTime().tm_hour)*3600;

    // getting the radius in meters.
    radius = difSeconds * EXPANSION_SPEED;

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
