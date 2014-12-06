#include <math.h>
#include <iostream>
#include "mapwidget.h"
#include "ui_mapwidget.h"

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MapWidget), mapScene(new QGraphicsScene(this))
{
    //indicar la interfaz de usuario del widget
    ui->setupUi(this);

    //cargar la imagen en el atributo QImage (imagen ya añadida como recurso)
    mapImage.load(":/mapacortado.png");

    //convertir a tipo pixmap para poder añadirla a la escena gráfica
    QPixmap mapPixmap = QPixmap::fromImage(mapImage);
    mapScene.addPixmap(mapPixmap);

    //ajustar el rectángulo contenedor de la escena al tamaño exacto de la imagen
    mapScene.setSceneRect(mapPixmap.rect());

    //decirle al graphicsview de la interfaz de usuario que su escena es esta
    ui->mapView->setScene(&mapScene);

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
    coordinatesToPixels(x,y,38,0,0,-14,0,0,34,0,0,-3,0,0,36,59,39.61,-8,-56,-9.6);
    std::cout << x << ' ' << y << std::endl;
    //pintado ejemplo coordenadas (linea desde cabo san vicente hasta esquina izquierda arriba)
    mapScene.addLine (x,y,0,0);

    //punta gibraltar 36° 6'34.01" (36.109447) -5°-20'-43.59" (-5.345442) -> por google maps
    std::cout << convertToDecimalDegrees(36,6,34.01)<< std::endl;
    std::cout << convertToDecimalDegrees(-5,-20,-43.59)<< std::endl;
    long double x2,y2;
    coordinatesToPixels(x2,y2,38,0,0,-14,0,0,34,0,0,-3,0,0,36,6,34.01,-5,-20,-43.59);
    std::cout << x2 << ' ' << y2 << std::endl;
    //pintado ejemplo coordenadas (linea desde gibraltar hasta derecha abajo
    mapScene.addLine (x2,y2,mapScene.width (),mapScene.height ());

    //Punta san felipe Cádiz 36°32'16.12"  -6°-18'-1.20" -> por google maps
    std::cout << convertToDecimalDegrees(36,32,16.12)<< std::endl;
    std::cout << convertToDecimalDegrees(-6,-18,-1.20)<< std::endl;
    long double x3,y3;
    coordinatesToPixels(x3,y3,38,0,0,-14,0,0,34,0,0,-3,0,0,36,32,16.12,-6,-18,-1.20);
    std::cout << x3 << ' ' << y3 << std::endl;
    //pintado ejemplo coordenadas (linea desde cadiz hasta izquierda abajo
    mapScene.addLine (x3,y3,0,mapScene.height ());

    //pintado ejemplo circulo
    //paintCircles();
}

MapWidget::~MapWidget()
{
    delete ui;
}



/**COORDENADAS->PIXELES**/

long double MapWidget::convertToDecimalDegrees(long double degrees, long double minutes, long double seconds){
    return degrees + (minutes/60) + (seconds/3600);
}

/*
pixelX: variable por referencia para rellenarla con el pixel del eje x
pixelY: variable por referencia para rellenarla con el pixel del eje y

minLat: minima latitud en el eje Y (correspondencia con pixel Y=0)
minLon: minima longitud en el eje X (correspondencia con pixel X=0)

targetLat: coordenada que queremos pintar en el eje Y.
targetLon: coordenada que queremos pintar en el eje X.

Cálculo de pixeles donde pintar las coordenadas:

pixelY = ((targetLat - minLat) / (maxLat - minLat)) * (maxYPixel - minYPixel)

pixelX = ((targetLon - minLon) / (maxLon - minLon)) * (maxXPixel - minXPixel)
*/

void MapWidget::coordinatesToPixels(long double &pixelX,long double &pixelY,
                                    long double degreesMinLat,long double minutesMinLat,long double secondsMinLat,
                                    long double degreesMinLon, long double minutesMinLon,long double secondsMinLon,
                                    long double degreesMaxLat,long double minutesMaxLat,long double secondsMaxLat,
                                    long double degreesMaxLon, long double minutesMaxLon,long double secondsMaxLon,
                                    long double degreesTargetLat,long double minutesTargetLat,long double secondsTargetLat,
                                    long double degreesTargetLon, long double minutesTargetLon,long double secondsTargetLon)
{
    long double minLon = convertToDecimalDegrees(degreesMinLon, minutesMinLon, secondsMinLon);
    long double minLat = convertToDecimalDegrees(degreesMinLat, minutesMinLat, secondsMinLat);
    long double maxLon = convertToDecimalDegrees(degreesMaxLon, minutesMaxLon, secondsMaxLon);
    long double maxLat = convertToDecimalDegrees(degreesMaxLat, minutesMaxLat, secondsMaxLat);
    long double targetLon = convertToDecimalDegrees(degreesTargetLon, minutesTargetLon, secondsTargetLon);
    long double targetLat = convertToDecimalDegrees(degreesTargetLat, minutesTargetLat, secondsTargetLat);
    long double maxXpixel = mapScene.width();
    long double maxYpixel = mapScene.height();
    long double minXpixel = 0;
    long double minYpixel = 0;

    pixelX = ((targetLon - minLon) / (maxLon - minLon)) * (maxXpixel - minXpixel);
    pixelY = ((targetLat - minLat) / (maxLat - minLat)) * (maxYpixel - minYpixel);
}




/**ESTACIONES**/
void MapWidget::paintStations(/*tiene que recibir las estaciones para pintarlas*/){
}

/**ORIGEN**/
void MapWidget::paintOrigin(/*tiene que recibir el origen para pintarlo*/){}



/**CIRCULOS CONCENTRICOS**/

float MapWidget::calculateRadius(/*tiene que recibir tiempo inicial, tiempo actual y velocidad de propagación*/){
    //hay que calcular la fórmula (Tiempo_origen - Tiempo_actual) * Velocidad_propagacion
    return 200;
}

void MapWidget::paintCircles(){
    //este metodo se debe llamar cada 5 segundos para pintar un nuevo circulo concentrico
    //los datos que necesita este metodo son: tiempo actual, tiempo origen, velocidad de expansión, coordenadas del origen.
    //hay que definir de dónde va a venir cada dato.

    //definir el centro del círculo: coordenadas del origen/evento (temporalmente puesto en el centro de la imagen)
    QPoint center(mapScene.width()/2, mapScene.height ()/2);

    //definir radio del círculo
    float radius = calculateRadius();

    //rectágulo que va a contener la elipse, coordenada superior 0,0 y tamaño radio tanto alto como ancho
    QRect rect(0,0,2*radius,2*radius);

    //ESTO ES CLAVE: mover el rectágulo contenedor de manera que el centro del circulo sea el deseado
    rect.moveCenter(center);

    //pintar el circulo sobre la escena que contiene el mapa
    mapScene.addEllipse (rect);
}
