/********************************************************************************
** Form generated from reading UI file 'mapripform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPRIPFORM_H
#define UI_MAPRIPFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapRipForm
{
public:
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_rippingInfo;
    QVBoxLayout *verticalLayout;
    QLabel *lblProvider;
    QLabel *lblLevel;
    QLabel *statuslabel;
    QProgressBar *progressBar;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *rdoBtn_singleLayer;
    QHBoxLayout *horizontalLayout;
    QRadioButton *rdoBtn_multiLlayer;
    QSpinBox *spnBoxLevelLimit;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pshBtnBeginRip;
    QPushButton *cancelButton;

    void setupUi(QWidget *MapRipForm)
    {
        if (MapRipForm->objectName().isEmpty())
            MapRipForm->setObjectName(QString::fromUtf8("MapRipForm"));
        MapRipForm->resize(615, 250);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MapRipForm->sizePolicy().hasHeightForWidth());
        MapRipForm->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(MapRipForm);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame_rippingInfo = new QFrame(MapRipForm);
        frame_rippingInfo->setObjectName(QString::fromUtf8("frame_rippingInfo"));
        frame_rippingInfo->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_rippingInfo->sizePolicy().hasHeightForWidth());
        frame_rippingInfo->setSizePolicy(sizePolicy1);
        frame_rippingInfo->setFrameShape(QFrame::StyledPanel);
        frame_rippingInfo->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_rippingInfo);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lblProvider = new QLabel(frame_rippingInfo);
        lblProvider->setObjectName(QString::fromUtf8("lblProvider"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lblProvider->sizePolicy().hasHeightForWidth());
        lblProvider->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(lblProvider);

        lblLevel = new QLabel(frame_rippingInfo);
        lblLevel->setObjectName(QString::fromUtf8("lblLevel"));
        sizePolicy2.setHeightForWidth(lblLevel->sizePolicy().hasHeightForWidth());
        lblLevel->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(lblLevel);

        statuslabel = new QLabel(frame_rippingInfo);
        statuslabel->setObjectName(QString::fromUtf8("statuslabel"));
        sizePolicy2.setHeightForWidth(statuslabel->sizePolicy().hasHeightForWidth());
        statuslabel->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(statuslabel);

        progressBar = new QProgressBar(frame_rippingInfo);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy3);
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);


        verticalLayout_3->addWidget(frame_rippingInfo);

        frame = new QFrame(MapRipForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy4);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        rdoBtn_singleLayer = new QRadioButton(frame);
        rdoBtn_singleLayer->setObjectName(QString::fromUtf8("rdoBtn_singleLayer"));
        sizePolicy2.setHeightForWidth(rdoBtn_singleLayer->sizePolicy().hasHeightForWidth());
        rdoBtn_singleLayer->setSizePolicy(sizePolicy2);
        rdoBtn_singleLayer->setMinimumSize(QSize(300, 0));
        rdoBtn_singleLayer->setChecked(false);

        verticalLayout_2->addWidget(rdoBtn_singleLayer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rdoBtn_multiLlayer = new QRadioButton(frame);
        rdoBtn_multiLlayer->setObjectName(QString::fromUtf8("rdoBtn_multiLlayer"));
        sizePolicy1.setHeightForWidth(rdoBtn_multiLlayer->sizePolicy().hasHeightForWidth());
        rdoBtn_multiLlayer->setSizePolicy(sizePolicy1);
        rdoBtn_multiLlayer->setMinimumSize(QSize(320, 0));
        rdoBtn_multiLlayer->setChecked(false);

        horizontalLayout->addWidget(rdoBtn_multiLlayer);

        spnBoxLevelLimit = new QSpinBox(frame);
        spnBoxLevelLimit->setObjectName(QString::fromUtf8("spnBoxLevelLimit"));
        sizePolicy2.setHeightForWidth(spnBoxLevelLimit->sizePolicy().hasHeightForWidth());
        spnBoxLevelLimit->setSizePolicy(sizePolicy2);
        spnBoxLevelLimit->setMinimum(1);
        spnBoxLevelLimit->setMaximum(21);
        spnBoxLevelLimit->setValue(19);

        horizontalLayout->addWidget(spnBoxLevelLimit);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy5);

        horizontalLayout->addWidget(label_2);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(frame);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pshBtnBeginRip = new QPushButton(MapRipForm);
        pshBtnBeginRip->setObjectName(QString::fromUtf8("pshBtnBeginRip"));
        sizePolicy3.setHeightForWidth(pshBtnBeginRip->sizePolicy().hasHeightForWidth());
        pshBtnBeginRip->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(pshBtnBeginRip);

        cancelButton = new QPushButton(MapRipForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        sizePolicy3.setHeightForWidth(cancelButton->sizePolicy().hasHeightForWidth());
        cancelButton->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(cancelButton);


        verticalLayout_3->addLayout(horizontalLayout_2);


        retranslateUi(MapRipForm);

        QMetaObject::connectSlotsByName(MapRipForm);
    } // setupUi

    void retranslateUi(QWidget *MapRipForm)
    {
        MapRipForm->setWindowTitle(QApplication::translate("MapRipForm", "MapRipper", nullptr));
        lblProvider->setText(QApplication::translate("MapRipForm", "Currently ripping from:", nullptr));
        lblLevel->setText(QApplication::translate("MapRipForm", "Zoom level:", nullptr));
        statuslabel->setText(QApplication::translate("MapRipForm", "Downloading tile:", nullptr));
        rdoBtn_singleLayer->setText(QApplication::translate("MapRipForm", "Single Layer Cache -- Current Level		", nullptr));
        rdoBtn_multiLlayer->setText(QApplication::translate("MapRipForm", "Auto Muli-Layer Cache -- Current Level -> Stopping Level: ", nullptr));
        label_2->setText(QApplication::translate("MapRipForm", "(Default 19)", nullptr));
        pshBtnBeginRip->setText(QApplication::translate("MapRipForm", "Begin", nullptr));
        cancelButton->setText(QApplication::translate("MapRipForm", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapRipForm: public Ui_MapRipForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPRIPFORM_H
