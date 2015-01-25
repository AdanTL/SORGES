/********************************************************************************
** Form generated from reading UI file 'origindatawidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORIGINDATAWIDGET_H
#define UI_ORIGINDATAWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OriginDataWidget
{
public:
    QLabel *timestamp;
    QTextEdit *textEdit_timestamp;
    QLabel *latitude;
    QLabel *epicenter;
    QTextEdit *textEdit_latitude;
    QLabel *stations;
    QTextEdit *textEdit_stations;
    QTextEdit *textEdit_longitude;
    QLabel *longitude;
    QLabel *originid;
    QTextEdit *textEdit_originid;
    QTextEdit *textEdit_magnitude;
    QLabel *magnitude;

    void setupUi(QWidget *OriginDataWidget)
    {
        if (OriginDataWidget->objectName().isEmpty())
            OriginDataWidget->setObjectName(QStringLiteral("OriginDataWidget"));
        OriginDataWidget->resize(400, 370);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Logo_sorges.png"), QSize(), QIcon::Normal, QIcon::Off);
        OriginDataWidget->setWindowIcon(icon);
        timestamp = new QLabel(OriginDataWidget);
        timestamp->setObjectName(QStringLiteral("timestamp"));
        timestamp->setGeometry(QRect(11, 51, 76, 17));
        textEdit_timestamp = new QTextEdit(OriginDataWidget);
        textEdit_timestamp->setObjectName(QStringLiteral("textEdit_timestamp"));
        textEdit_timestamp->setGeometry(QRect(90, 51, 301, 29));
        textEdit_timestamp->setReadOnly(true);
        latitude = new QLabel(OriginDataWidget);
        latitude->setObjectName(QStringLiteral("latitude"));
        latitude->setGeometry(QRect(85, 90, 61, 17));
        epicenter = new QLabel(OriginDataWidget);
        epicenter->setObjectName(QStringLiteral("epicenter"));
        epicenter->setGeometry(QRect(10, 90, 69, 17));
        textEdit_latitude = new QTextEdit(OriginDataWidget);
        textEdit_latitude->setObjectName(QStringLiteral("textEdit_latitude"));
        textEdit_latitude->setGeometry(QRect(160, 90, 231, 29));
        textEdit_latitude->setLineWidth(1);
        textEdit_latitude->setReadOnly(true);
        stations = new QLabel(OriginDataWidget);
        stations->setObjectName(QStringLiteral("stations"));
        stations->setGeometry(QRect(10, 197, 113, 17));
        textEdit_stations = new QTextEdit(OriginDataWidget);
        textEdit_stations->setObjectName(QStringLiteral("textEdit_stations"));
        textEdit_stations->setGeometry(QRect(10, 220, 381, 141));
        textEdit_stations->setReadOnly(true);
        textEdit_longitude = new QTextEdit(OriginDataWidget);
        textEdit_longitude->setObjectName(QStringLiteral("textEdit_longitude"));
        textEdit_longitude->setGeometry(QRect(160, 121, 231, 29));
        textEdit_longitude->setReadOnly(true);
        longitude = new QLabel(OriginDataWidget);
        longitude->setObjectName(QStringLiteral("longitude"));
        longitude->setGeometry(QRect(91, 121, 66, 17));
        originid = new QLabel(OriginDataWidget);
        originid->setObjectName(QStringLiteral("originid"));
        originid->setGeometry(QRect(11, 11, 61, 17));
        textEdit_originid = new QTextEdit(OriginDataWidget);
        textEdit_originid->setObjectName(QStringLiteral("textEdit_originid"));
        textEdit_originid->setGeometry(QRect(80, 11, 311, 29));
        textEdit_originid->setReadOnly(true);
        textEdit_magnitude = new QTextEdit(OriginDataWidget);
        textEdit_magnitude->setObjectName(QStringLiteral("textEdit_magnitude"));
        textEdit_magnitude->setGeometry(QRect(90, 160, 301, 29));
        textEdit_magnitude->setReadOnly(true);
        magnitude = new QLabel(OriginDataWidget);
        magnitude->setObjectName(QStringLiteral("magnitude"));
        magnitude->setGeometry(QRect(10, 160, 74, 17));

        retranslateUi(OriginDataWidget);

        QMetaObject::connectSlotsByName(OriginDataWidget);
    } // setupUi

    void retranslateUi(QWidget *OriginDataWidget)
    {
        OriginDataWidget->setWindowTitle(QApplication::translate("OriginDataWidget", "SORGES origin data", 0));
        timestamp->setText(QApplication::translate("OriginDataWidget", "Timestamp", 0));
        latitude->setText(QApplication::translate("OriginDataWidget", "  latitude", 0));
        epicenter->setText(QApplication::translate("OriginDataWidget", "Epicenter:", 0));
        textEdit_latitude->setHtml(QApplication::translate("OriginDataWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
        stations->setText(QApplication::translate("OriginDataWidget", "Related Stations", 0));
        longitude->setText(QApplication::translate("OriginDataWidget", "longitude", 0));
        originid->setText(QApplication::translate("OriginDataWidget", "Origin ID", 0));
        magnitude->setText(QApplication::translate("OriginDataWidget", "Magnitude", 0));
    } // retranslateUi

};

namespace Ui {
    class OriginDataWidget: public Ui_OriginDataWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORIGINDATAWIDGET_H
