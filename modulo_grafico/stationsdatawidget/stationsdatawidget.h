#ifndef STATIONSDATAWIDGET_H
#define STATIONSDATAWIDGET_H

#include <QWidget>

namespace Ui {
class StationsDataWidget;
}

class StationsDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StationsDataWidget(QWidget *parent = 0);
    ~StationsDataWidget();

private:
    Ui::StationsDataWidget *ui;
};

#endif // STATIONSDATAWIDGET_H
