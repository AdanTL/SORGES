#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QGraphicsScene>
#include <set>
#include "../classes/station.h"
#include "../classes/origin.h"

namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = 0);
    ~MapWidget();

public slots:
    void paintOrigin(const Origin& origin);
    void paintStations(const std::set<Station>& stationsList);
    void changeStationsColors(const std::set<Station>& changedStations);
void testing(); //slot de prueba de cambio de origen

private slots:
    void paintCircles();
    void showInformation();

private:
    Ui::MapWidget *ui;
    QImage mapImage;
    QGraphicsScene mapScene;
    std::set<Station> stations;
    Origin currentOrigin;
    Origin firstOrigin;
    QTimer *circlesTimer;
    
    /**PRIVATE FUNCTIONS**/		
		
    /*EXPANSION CIRCLES RADIUS*/
    long double calculateRadius();

	/*CONVERSIONS OF COORDINATES*/
    void coordinatesToPixels(long double &pixelX, long double &pixelY,
                             long double degreesTargetLat, long double minutesTargetLat,
                             long double secondsTargetLat, long double degreesTargetLon,
                             long double minutesTargetLon, long double secondsTargetLon);

    void coordinatesToPixels(long double &pixelX, long double &pixelY,
                             long double targetLat, long double targetLon);

    long double convertToDecimalDegrees(long double degrees,
                                        long double minutes,
                                        long double seconds);

	/*STATION FUNCTIONS*/
    void drawStation(const Station &station);
    void clearStation(const std::string& stationID);
    void clearStations();

    /*ORIGIN FUNCTIONS*/
    void clearOrigin();
    void updateEventStations();
    
    /*TESTS*/
    void testPixelPrecision();
    void testOrigen();
    void testStation();
};

#endif // MAPWIDGET_H
