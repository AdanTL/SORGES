/********************************************************************************
** Form generated from reading UI file 'mapwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPWIDGET_H
#define UI_MAPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QGraphicsView *mapView;

    void setupUi(QWidget *MapWidget)
    {
        if (MapWidget->objectName().isEmpty())
            MapWidget->setObjectName(QStringLiteral("MapWidget"));
        MapWidget->resize(1027, 577);
        MapWidget->setCursor(QCursor(Qt::ArrowCursor));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Logo_sorges.png"), QSize(), QIcon::Normal, QIcon::Off);
        MapWidget->setWindowIcon(icon);
        horizontalLayout = new QHBoxLayout(MapWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mapView = new QGraphicsView(MapWidget);
        mapView->setObjectName(QStringLiteral("mapView"));
        mapView->setEnabled(true);

        horizontalLayout->addWidget(mapView);


        retranslateUi(MapWidget);

        QMetaObject::connectSlotsByName(MapWidget);
    } // setupUi

    void retranslateUi(QWidget *MapWidget)
    {
        MapWidget->setWindowTitle(QApplication::translate("MapWidget", "SORGES map view", 0));
    } // retranslateUi

};

namespace Ui {
    class MapWidget: public Ui_MapWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPWIDGET_H
