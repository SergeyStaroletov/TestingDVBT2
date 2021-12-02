/********************************************************************************
** Form generated from reading UI file 'QGCMapTool.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGCMAPTOOL_H
#define UI_QGCMAPTOOL_H

#include <QGCMapWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QGCMapTool
{
public:
    QGridLayout *gridLayout;
    QGCMapWidget *map;
    QLabel *longitudeLabel;
    QLabel *latitudeLabel;
    QLabel *satsLabel;
    QLabel *hdopLabel;
    QLabel *fixLabel;
    QLabel *zoomLabel;
    QSlider *zoomSlider;

    void setupUi(QWidget *QGCMapTool)
    {
        if (QGCMapTool->objectName().isEmpty())
            QGCMapTool->setObjectName(QString::fromUtf8("QGCMapTool"));
        QGCMapTool->resize(739, 205);
        gridLayout = new QGridLayout(QGCMapTool);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(3);
        gridLayout->setVerticalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        map = new QGCMapWidget(QGCMapTool);
        map->setObjectName(QString::fromUtf8("map"));
        longitudeLabel = new QLabel(map);
        longitudeLabel->setObjectName(QString::fromUtf8("longitudeLabel"));
        longitudeLabel->setGeometry(QRect(210, 10, 111, 21));
        QFont font;
        font.setPointSize(17);
        longitudeLabel->setFont(font);
        longitudeLabel->setFrameShape(QFrame::Box);
        latitudeLabel = new QLabel(map);
        latitudeLabel->setObjectName(QString::fromUtf8("latitudeLabel"));
        latitudeLabel->setGeometry(QRect(90, 10, 111, 21));
        latitudeLabel->setFont(font);
        latitudeLabel->setFrameShape(QFrame::Box);
        satsLabel = new QLabel(map);
        satsLabel->setObjectName(QString::fromUtf8("satsLabel"));
        satsLabel->setGeometry(QRect(430, 10, 61, 21));
        satsLabel->setFont(font);
        satsLabel->setFrameShape(QFrame::Box);
        hdopLabel = new QLabel(map);
        hdopLabel->setObjectName(QString::fromUtf8("hdopLabel"));
        hdopLabel->setGeometry(QRect(500, 10, 71, 21));
        hdopLabel->setFont(font);
        hdopLabel->setFrameShape(QFrame::Box);
        fixLabel = new QLabel(map);
        fixLabel->setObjectName(QString::fromUtf8("fixLabel"));
        fixLabel->setGeometry(QRect(330, 10, 91, 21));
        fixLabel->setFont(font);
        fixLabel->setFrameShape(QFrame::Box);
        zoomLabel = new QLabel(map);
        zoomLabel->setObjectName(QString::fromUtf8("zoomLabel"));
        zoomLabel->setGeometry(QRect(580, 10, 71, 21));
        zoomLabel->setFont(font);
        zoomLabel->setFrameShape(QFrame::Box);

        gridLayout->addWidget(map, 0, 0, 1, 1);

        zoomSlider = new QSlider(QGCMapTool);
        zoomSlider->setObjectName(QString::fromUtf8("zoomSlider"));
        zoomSlider->setMaximum(600);
        zoomSlider->setPageStep(5);
        zoomSlider->setOrientation(Qt::Vertical);
        zoomSlider->setTickPosition(QSlider::TicksBelow);
        zoomSlider->setTickInterval(0);

        gridLayout->addWidget(zoomSlider, 0, 1, 2, 1);

        gridLayout->setRowStretch(0, 100);

        retranslateUi(QGCMapTool);

        QMetaObject::connectSlotsByName(QGCMapTool);
    } // setupUi

    void retranslateUi(QWidget *QGCMapTool)
    {
        QGCMapTool->setWindowTitle(QApplication::translate("QGCMapTool", "Form", nullptr));
        longitudeLabel->setText(QApplication::translate("QGCMapTool", "LON:", nullptr));
        latitudeLabel->setText(QApplication::translate("QGCMapTool", "LAT:", nullptr));
        satsLabel->setText(QApplication::translate("QGCMapTool", "SATS:", nullptr));
        hdopLabel->setText(QApplication::translate("QGCMapTool", "HDOP:", nullptr));
        fixLabel->setText(QApplication::translate("QGCMapTool", "FIX:", nullptr));
        zoomLabel->setText(QApplication::translate("QGCMapTool", "ZOOM:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QGCMapTool: public Ui_QGCMapTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGCMAPTOOL_H
