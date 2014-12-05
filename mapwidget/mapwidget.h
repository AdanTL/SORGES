#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = 0);
    ~MapWidget();
    void paintOrigin();
    void paintStations();
    void paintCircles();

private:
    Ui::MapWidget *ui;
    QImage mapImage;
    QGraphicsScene mapScene;

    float calculateRadius();
    void coordinatesToPixels(long double &pixelX,long double &pixelY,
                             long double degreesMinLat,long double minutesMinLat,long double secondsMinLat,
                             long double degreesMinLon, long double minutesMinLon,long double secondsMinLon,
                             long double degreesMaxLat,long double minutesMaxLat,long double secondsMaxLat,
                             long double degreesMaxLon, long double minutesMaxLon,long double secondsMaxLon,
                             long double degreesTargetLat,long double minutesTargetLat,long double secondsTargetLat,
                             long double degreesTargetLon, long double minutesTargetLon,long double secondsTargetLon);
    long double convertToRadians(long double degrees, long double minutes, long double seconds);
    long double convertToDecimalDegrees(long double degrees, long double minutes, long double seconds);
};

#endif // MAPWIDGET_H
