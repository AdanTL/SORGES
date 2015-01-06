#include "origindatawidget.h"
#include "ui_origindatawidget.h"

origindatawidget::origindatawidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::origindatawidget)
{
    ui->setupUi(this);
}

origindatawidget::~origindatawidget()
{
    delete ui;
}
