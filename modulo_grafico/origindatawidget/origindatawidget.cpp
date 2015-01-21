#include <iostream>
#include <sstream>
#include "origindatawidget.h"
#include "ui_origindatawidget.h"
#define PATH_OUTPUT_XML "laputamadredeadan.xml"
#include <QDir>
#include <QFile>

OriginDataWidget::OriginDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OriginDataWidget)
{
    ui->setupUi(this);

    std::set<Station> stations;
    stations.insert(Station("#Sta1","netId1",10,10,1));
    stations.insert(Station("#Sta2","netId2",20,20,2));
    Origin origin("origin#111111",std::tm(),36,-8,1.5,stations);
    showOriginData (origin);
}

OriginDataWidget::~OriginDataWidget()
{
    delete ui;
}



std::string ldoubleToString(const long double& number){
    std::ostringstream convert;
    convert << number;
    return convert.str();
}

//llevarse este metodo a la clase station mejor
std::string stationToString(const Station& station){
    std::ostringstream convert;
    convert << station;
    return convert.str();
}

void OriginDataWidget::showOriginData(const Origin& origin){
    currentOrigin = origin;

    ui->textEdit_originid->setPlainText(QString(origin.getOriginID().c_str()));
    //ui->textEdit_timestamp->setPlainText(QString(origin.getOriginTime()));
    ui->textEdit_latitude->setPlainText(
                           QString(ldoubleToString(origin.getLatitude()).c_str()));
    ui->textEdit_longitude->setPlainText(
                           QString(ldoubleToString(origin.getLongitude ()).c_str()));
    ui->textEdit_magnitude->setPlainText(
                           QString(ldoubleToString(origin.getMagnitude ()).c_str()));

    std::set<Station> stations = origin.getStations();
    for(std::set<Station>::iterator it=stations.begin();it!=stations.end(); ++it){
        ui->textEdit_stations->append(stationToString(*it).c_str());
    }

    dumpOriginXml();
}

void OriginDataWidget::dumpOriginXml(){

    QFile file(QDir::currentPath()+"/logFiles/currentTime().xml");
    file.open(QIODevice::WriteOnly || QIODevice::Text);
    QTextStream out(&file);
    out << currentOrigin.toStdString();





}
