#include "stationsdatawidget.h"
#include "ui_stationsdatawidget.h"

StationsDataWidget::StationsDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationsDataWidget)
{
    ui->setupUi(this);

    std::set<Station> stations;
    stations.insert(Station("#Sta1","netId1",10,10,1));
    stations.insert(Station("#Sta2","netId2",20,20,2));
    showStationsData (stations);
}

StationsDataWidget::~StationsDataWidget()
{
    delete ui;
}

void StationsDataWidget::showStationsData(const std::set<Station>& stations){
    currentStations = stations;
    for(std::set<Station>::iterator it=currentStations.begin();
                                    it!=currentStations.end();++it){
        ui->textEdit_stations->append((*it).stationToString().c_str());
    }
}

