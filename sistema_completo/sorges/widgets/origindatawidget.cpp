#include<sstream>
#include "origindatawidget.h"
#include "ui_origindatawidget.h"

OriginDataWidget::OriginDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OriginDataWidget)
{
    ui->setupUi(this);
    /*Test, borrar de aqui antes de entregar
    std::set<Station> stations;
    stations.insert(Station("#Sta1","netId1",10,10,1));
    stations.insert(Station("#Sta2","netId2",20,20,2));
    Origin origin("origin#111111",QDate(),QTime(),36,-8,1.5,stations);
    showOriginData (origin);*/
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

void OriginDataWidget::showOriginData(const Origin& origin){
    currentOrigin = origin;

    ui->textEdit_originid->setPlainText(QString(origin.getOriginID().c_str()));

    ui->textEdit_timestamp->setPlainText(QString(origin.getOriginDate()
                                                 .toString("dd-MM-yyyy"))
                                         +" "+QString(origin.getOriginTime()
                                                      .toString("hh:mm:ss.zzz t")));
    ui->textEdit_latitude->setPlainText(
                           QString(ldoubleToString(origin.getLatitude()).c_str()));
    ui->textEdit_longitude->setPlainText(
                           QString(ldoubleToString(origin.getLongitude ()).c_str()));
    ui->textEdit_magnitude->setPlainText(
                           QString(ldoubleToString(origin.getMagnitude ()).c_str()));

    std::set<Station> stations = origin.getStations();
    for(std::set<Station>::iterator it=stations.begin();it!=stations.end(); ++it){
        //ui->textEdit_stations->setTextBackgroundColor((*it).getCurrentOnSiteAlert());
        ui->textEdit_stations->append((*it).stationToString().c_str());
    }
    //dumpOriginXml();

}


/*void OriginDataWidget::dumpOriginXml(){
    QFile file("/home/felipe/origins.xml");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        std::cerr << "Problem to create Log file: " <<  "xml" << std::endl;
        return;
    }
    QTextStream out(&file);
    out << QString::fromStdString(currentOrigin.toStringXml());
    file.close();

}*/
