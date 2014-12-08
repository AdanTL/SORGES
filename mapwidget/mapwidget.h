#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QGraphicsScene>
#include <set>
#include "station.h"
#include "origin.h"

namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = 0);
    ~MapWidget();
    void paintOrigin(const Origin& origin);
    void paintStations(const std::set<Station>& stations);
    void paintCircles();

private:
    Ui::MapWidget *ui;
    QImage mapImage;
    QGraphicsScene mapScene;
    std::set<Station> stations;
    Origin currentOrigin;

    float calculateRadius();

    void coordinatesToPixels(long double &pixelX, long double &pixelY,
                             long double degreesTargetLat, long double minutesTargetLat,
                             long double secondsTargetLat, long double degreesTargetLon,
                             long double minutesTargetLon, long double secondsTargetLon);

    void coordinatesToPixels(long double &pixelX, long double &pixelY,
                             long double targetLat, long double targetLon);

    long double convertToDecimalDegrees(long double degrees,
                                        long double minutes,
                                        long double seconds);

    void testPixelPrecision();
};

#endif // MAPWIDGET_H
