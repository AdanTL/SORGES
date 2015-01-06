#ifndef ORIGINDATAWIDGET_H
#define ORIGINDATAWIDGET_H

#include <QWidget>

namespace Ui {
class origindatawidget;
}

class origindatawidget : public QWidget
{
    Q_OBJECT

public:
    explicit origindatawidget(QWidget *parent = 0);
    ~origindatawidget();

private:
    Ui::origindatawidget *ui;
};

#endif // ORIGINDATAWIDGET_H
