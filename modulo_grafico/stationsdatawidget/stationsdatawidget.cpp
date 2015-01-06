#include "stationsdatawidget.h"
#include "ui_stationsdatawidget.h"

StationsDataWidget::StationsDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StationsDataWidget)
{
    ui->setupUi(this);
}

StationsDataWidget::~StationsDataWidget()
{
    delete ui;
}
