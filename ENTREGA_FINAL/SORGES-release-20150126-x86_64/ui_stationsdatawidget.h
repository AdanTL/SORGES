/********************************************************************************
** Form generated from reading UI file 'stationsdatawidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONSDATAWIDGET_H
#define UI_STATIONSDATAWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StationsDataWidget
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *stations;
    QTextEdit *textEdit_stations;

    void setupUi(QWidget *StationsDataWidget)
    {
        if (StationsDataWidget->objectName().isEmpty())
            StationsDataWidget->setObjectName(QStringLiteral("StationsDataWidget"));
        StationsDataWidget->resize(340, 342);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Logo_sorges.png"), QSize(), QIcon::Normal, QIcon::Off);
        StationsDataWidget->setWindowIcon(icon);
        layoutWidget = new QWidget(StationsDataWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 11, 321, 321));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        stations = new QLabel(layoutWidget);
        stations->setObjectName(QStringLiteral("stations"));

        verticalLayout->addWidget(stations);

        textEdit_stations = new QTextEdit(layoutWidget);
        textEdit_stations->setObjectName(QStringLiteral("textEdit_stations"));
        textEdit_stations->setReadOnly(true);

        verticalLayout->addWidget(textEdit_stations);


        retranslateUi(StationsDataWidget);

        QMetaObject::connectSlotsByName(StationsDataWidget);
    } // setupUi

    void retranslateUi(QWidget *StationsDataWidget)
    {
        StationsDataWidget->setWindowTitle(QApplication::translate("StationsDataWidget", "SORGES list of stations", 0));
        stations->setText(QApplication::translate("StationsDataWidget", "List of current stations", 0));
    } // retranslateUi

};

namespace Ui {
    class StationsDataWidget: public Ui_StationsDataWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONSDATAWIDGET_H
