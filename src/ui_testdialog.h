/********************************************************************************
** Form generated from reading UI file 'testdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTDIALOG_H
#define UI_TESTDIALOG_H

#include <QGCMapWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_TestDialog
{
public:
    QDialogButtonBox *buttonBox;
    QPushButton *pushButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QProgressBar *progressBarLevel;
    QGroupBox *groupBox;
    QTableWidget *tableTranspondersList;
    QPushButton *buttonAddTransponder;
    QPushButton *buttonRemoveTransponder;
    QPushButton *buttonStartLocking;
    QPushButton *buttonStopLocking;
    QCheckBox *checkBoxLocked;
    QLabel *label;
    QLabel *label_2;
    QProgressBar *progressBarQuality;
    QWidget *tab_4;
    QLabel *label_3;
    QLineEdit *lineEditFrom;
    QLineEdit *lineEditTo;
    QComboBox *comboBoxStep;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *pushButtonAnalize;
    QCheckBox *checkBoxNonStop;
    QCustomPlot *plot;
    QPushButton *pushButtonStopAnalize;
    QCheckBox *checkBoxAppend;
    QWidget *tab_2;
    QTableWidget *tablePoints;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QComboBox *comboBox_2;
    QLabel *label_7;
    QGCMapWidget *widget_2;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QWidget *tab_3;
    QGroupBox *groupBox_2;
    QComboBox *comboBoxConstellRefStream;
    QComboBox *comboBoxConstellFreq;
    QPushButton *buttonObtainData;
    QLabel *label_8;
    QCheckBox *checkBoxConstellNonStop;
    QLabel *label_6;
    QGroupBox *groupBox_3;
    QLabel *label_11;
    QComboBox *comboBoxConstallVizMod;
    QCheckBox *checkBoxGroupNearest;
    QCheckBox *checkBoxRecovery;
    QWidget *widgetView;
    QLabel *label_9;
    QLabel *labelCard;

    void setupUi(QDialog *TestDialog)
    {
        if (TestDialog->objectName().isEmpty())
            TestDialog->setObjectName(QString::fromUtf8("TestDialog"));
        TestDialog->resize(787, 665);
        buttonBox = new QDialogButtonBox(TestDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(390, 630, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        pushButton = new QPushButton(TestDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 630, 89, 25));
        tabWidget = new QTabWidget(TestDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 40, 731, 571));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        progressBarLevel = new QProgressBar(tab);
        progressBarLevel->setObjectName(QString::fromUtf8("progressBarLevel"));
        progressBarLevel->setGeometry(QRect(80, 300, 561, 23));
        progressBarLevel->setValue(0);
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(50, 20, 561, 201));
        tableTranspondersList = new QTableWidget(groupBox);
        if (tableTranspondersList->columnCount() < 3)
            tableTranspondersList->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableTranspondersList->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableTranspondersList->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableTranspondersList->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableTranspondersList->rowCount() < 2)
            tableTranspondersList->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableTranspondersList->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableTranspondersList->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableTranspondersList->setItem(0, 0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableTranspondersList->setItem(0, 1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableTranspondersList->setItem(0, 2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableTranspondersList->setItem(1, 0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableTranspondersList->setItem(1, 1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableTranspondersList->setItem(1, 2, __qtablewidgetitem10);
        tableTranspondersList->setObjectName(QString::fromUtf8("tableTranspondersList"));
        tableTranspondersList->setGeometry(QRect(60, 30, 321, 161));
        buttonAddTransponder = new QPushButton(groupBox);
        buttonAddTransponder->setObjectName(QString::fromUtf8("buttonAddTransponder"));
        buttonAddTransponder->setGeometry(QRect(430, 50, 89, 25));
        buttonRemoveTransponder = new QPushButton(groupBox);
        buttonRemoveTransponder->setObjectName(QString::fromUtf8("buttonRemoveTransponder"));
        buttonRemoveTransponder->setGeometry(QRect(430, 90, 89, 25));
        buttonStartLocking = new QPushButton(tab);
        buttonStartLocking->setObjectName(QString::fromUtf8("buttonStartLocking"));
        buttonStartLocking->setGeometry(QRect(200, 230, 111, 25));
        buttonStopLocking = new QPushButton(tab);
        buttonStopLocking->setObjectName(QString::fromUtf8("buttonStopLocking"));
        buttonStopLocking->setGeometry(QRect(360, 230, 111, 25));
        checkBoxLocked = new QCheckBox(tab);
        checkBoxLocked->setObjectName(QString::fromUtf8("checkBoxLocked"));
        checkBoxLocked->setEnabled(true);
        checkBoxLocked->setGeometry(QRect(80, 330, 111, 23));
        checkBoxLocked->setCheckable(false);
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(80, 280, 81, 17));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(80, 360, 111, 17));
        progressBarQuality = new QProgressBar(tab);
        progressBarQuality->setObjectName(QString::fromUtf8("progressBarQuality"));
        progressBarQuality->setGeometry(QRect(80, 380, 561, 23));
        progressBarQuality->setValue(0);
        tabWidget->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        label_3 = new QLabel(tab_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 20, 67, 17));
        lineEditFrom = new QLineEdit(tab_4);
        lineEditFrom->setObjectName(QString::fromUtf8("lineEditFrom"));
        lineEditFrom->setGeometry(QRect(70, 20, 61, 25));
        lineEditTo = new QLineEdit(tab_4);
        lineEditTo->setObjectName(QString::fromUtf8("lineEditTo"));
        lineEditTo->setGeometry(QRect(210, 20, 61, 25));
        comboBoxStep = new QComboBox(tab_4);
        comboBoxStep->addItem(QString());
        comboBoxStep->addItem(QString());
        comboBoxStep->addItem(QString());
        comboBoxStep->setObjectName(QString::fromUtf8("comboBoxStep"));
        comboBoxStep->setGeometry(QRect(375, 20, 71, 25));
        label_4 = new QLabel(tab_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(140, 20, 67, 17));
        label_5 = new QLabel(tab_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(280, 20, 81, 17));
        pushButtonAnalize = new QPushButton(tab_4);
        pushButtonAnalize->setObjectName(QString::fromUtf8("pushButtonAnalize"));
        pushButtonAnalize->setGeometry(QRect(460, 20, 89, 25));
        checkBoxNonStop = new QCheckBox(tab_4);
        checkBoxNonStop->setObjectName(QString::fromUtf8("checkBoxNonStop"));
        checkBoxNonStop->setGeometry(QRect(460, 50, 92, 23));
        plot = new QCustomPlot(tab_4);
        plot->setObjectName(QString::fromUtf8("plot"));
        plot->setGeometry(QRect(20, 90, 671, 421));
        pushButtonStopAnalize = new QPushButton(tab_4);
        pushButtonStopAnalize->setObjectName(QString::fromUtf8("pushButtonStopAnalize"));
        pushButtonStopAnalize->setGeometry(QRect(560, 20, 89, 25));
        checkBoxAppend = new QCheckBox(tab_4);
        checkBoxAppend->setObjectName(QString::fromUtf8("checkBoxAppend"));
        checkBoxAppend->setGeometry(QRect(130, 50, 261, 23));
        checkBoxAppend->setChecked(true);
        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tablePoints = new QTableWidget(tab_2);
        if (tablePoints->columnCount() < 5)
            tablePoints->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tablePoints->setHorizontalHeaderItem(0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tablePoints->setHorizontalHeaderItem(1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tablePoints->setHorizontalHeaderItem(2, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tablePoints->setHorizontalHeaderItem(3, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tablePoints->setHorizontalHeaderItem(4, __qtablewidgetitem15);
        tablePoints->setObjectName(QString::fromUtf8("tablePoints"));
        tablePoints->setGeometry(QRect(20, 50, 591, 101));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(260, 20, 89, 25));
        pushButton_3 = new QPushButton(tab_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(390, 20, 89, 25));
        comboBox_2 = new QComboBox(tab_2);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(140, 20, 86, 25));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(40, 20, 71, 17));
        widget_2 = new QGCMapWidget(tab_2);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(20, 170, 661, 351));
        pushButton_4 = new QPushButton(tab_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(630, 80, 89, 25));
        pushButton_5 = new QPushButton(tab_2);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(630, 110, 89, 25));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        groupBox_2 = new QGroupBox(tab_3);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 20, 241, 201));
        comboBoxConstellRefStream = new QComboBox(groupBox_2);
        comboBoxConstellRefStream->addItem(QString());
        comboBoxConstellRefStream->setObjectName(QString::fromUtf8("comboBoxConstellRefStream"));
        comboBoxConstellRefStream->setGeometry(QRect(10, 120, 211, 25));
        comboBoxConstellFreq = new QComboBox(groupBox_2);
        comboBoxConstellFreq->addItem(QString());
        comboBoxConstellFreq->setObjectName(QString::fromUtf8("comboBoxConstellFreq"));
        comboBoxConstellFreq->setGeometry(QRect(90, 40, 131, 25));
        buttonObtainData = new QPushButton(groupBox_2);
        buttonObtainData->setObjectName(QString::fromUtf8("buttonObtainData"));
        buttonObtainData->setGeometry(QRect(70, 160, 89, 25));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(0, 100, 141, 17));
        checkBoxConstellNonStop = new QCheckBox(groupBox_2);
        checkBoxConstellNonStop->setObjectName(QString::fromUtf8("checkBoxConstellNonStop"));
        checkBoxConstellNonStop->setGeometry(QRect(70, 70, 151, 23));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(0, 40, 91, 17));
        groupBox_3 = new QGroupBox(tab_3);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 240, 241, 151));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 40, 131, 17));
        comboBoxConstallVizMod = new QComboBox(groupBox_3);
        comboBoxConstallVizMod->addItem(QString());
        comboBoxConstallVizMod->addItem(QString());
        comboBoxConstallVizMod->addItem(QString());
        comboBoxConstallVizMod->setObjectName(QString::fromUtf8("comboBoxConstallVizMod"));
        comboBoxConstallVizMod->setGeometry(QRect(150, 40, 86, 25));
        checkBoxGroupNearest = new QCheckBox(groupBox_3);
        checkBoxGroupNearest->setObjectName(QString::fromUtf8("checkBoxGroupNearest"));
        checkBoxGroupNearest->setGeometry(QRect(30, 70, 191, 23));
        checkBoxGroupNearest->setChecked(true);
        checkBoxRecovery = new QCheckBox(groupBox_3);
        checkBoxRecovery->setObjectName(QString::fromUtf8("checkBoxRecovery"));
        checkBoxRecovery->setGeometry(QRect(30, 100, 171, 23));
        checkBoxRecovery->setChecked(true);
        widgetView = new QWidget(tab_3);
        widgetView->setObjectName(QString::fromUtf8("widgetView"));
        widgetView->setGeometry(QRect(260, 40, 451, 351));
        tabWidget->addTab(tab_3, QString());
        label_9 = new QLabel(TestDialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 10, 67, 17));
        labelCard = new QLabel(TestDialog);
        labelCard->setObjectName(QString::fromUtf8("labelCard"));
        labelCard->setGeometry(QRect(100, 10, 431, 17));

        retranslateUi(TestDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), TestDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TestDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(TestDialog);
    } // setupUi

    void retranslateUi(QDialog *TestDialog)
    {
        TestDialog->setWindowTitle(QApplication::translate("TestDialog", "DVB Terrestrial Test", nullptr));
        pushButton->setText(QApplication::translate("TestDialog", "pressme", nullptr));
        groupBox->setTitle(QApplication::translate("TestDialog", "Transponders", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableTranspondersList->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("TestDialog", "Freq", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableTranspondersList->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("TestDialog", "Symbol Rate", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableTranspondersList->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("TestDialog", "Modulation", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableTranspondersList->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("TestDialog", "1", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableTranspondersList->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("TestDialog", "2", nullptr));

        const bool __sortingEnabled = tableTranspondersList->isSortingEnabled();
        tableTranspondersList->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem5 = tableTranspondersList->item(0, 0);
        ___qtablewidgetitem5->setText(QApplication::translate("TestDialog", "522", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableTranspondersList->item(0, 1);
        ___qtablewidgetitem6->setText(QApplication::translate("TestDialog", "AUTO", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableTranspondersList->item(0, 2);
        ___qtablewidgetitem7->setText(QApplication::translate("TestDialog", "QAM256", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableTranspondersList->item(1, 0);
        ___qtablewidgetitem8->setText(QApplication::translate("TestDialog", "770", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableTranspondersList->item(1, 1);
        ___qtablewidgetitem9->setText(QApplication::translate("TestDialog", "AUTO", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableTranspondersList->item(1, 2);
        ___qtablewidgetitem10->setText(QApplication::translate("TestDialog", "QAM256", nullptr));
        tableTranspondersList->setSortingEnabled(__sortingEnabled);

        buttonAddTransponder->setText(QApplication::translate("TestDialog", "Add", nullptr));
        buttonRemoveTransponder->setText(QApplication::translate("TestDialog", "Remove", nullptr));
        buttonStartLocking->setText(QApplication::translate("TestDialog", "Start Locking", nullptr));
        buttonStopLocking->setText(QApplication::translate("TestDialog", "Stop Locking", nullptr));
        checkBoxLocked->setText(QApplication::translate("TestDialog", "Signal Lock", nullptr));
        label->setText(QApplication::translate("TestDialog", "Signal Level", nullptr));
        label_2->setText(QApplication::translate("TestDialog", "Signal Quality", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("TestDialog", "Transponders/Antenna Adjust", nullptr));
        label_3->setText(QApplication::translate("TestDialog", "From ", nullptr));
        lineEditFrom->setText(QApplication::translate("TestDialog", "510", nullptr));
        lineEditTo->setText(QApplication::translate("TestDialog", "799", nullptr));
        comboBoxStep->setItemText(0, QApplication::translate("TestDialog", "1", nullptr));
        comboBoxStep->setItemText(1, QApplication::translate("TestDialog", "4", nullptr));
        comboBoxStep->setItemText(2, QApplication::translate("TestDialog", "8", nullptr));

        label_4->setText(QApplication::translate("TestDialog", "MHz    to", nullptr));
        label_5->setText(QApplication::translate("TestDialog", "MHz    step", nullptr));
        pushButtonAnalize->setText(QApplication::translate("TestDialog", "Analyze", nullptr));
        checkBoxNonStop->setText(QApplication::translate("TestDialog", "Non-Stop", nullptr));
        pushButtonStopAnalize->setText(QApplication::translate("TestDialog", "Stop", nullptr));
        checkBoxAppend->setText(QApplication::translate("TestDialog", "Append results to the same plot", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("TestDialog", "Scan Spectrum", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tablePoints->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QApplication::translate("TestDialog", "time", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tablePoints->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QApplication::translate("TestDialog", "alt", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tablePoints->horizontalHeaderItem(2);
        ___qtablewidgetitem13->setText(QApplication::translate("TestDialog", "lat", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tablePoints->horizontalHeaderItem(3);
        ___qtablewidgetitem14->setText(QApplication::translate("TestDialog", "lock", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tablePoints->horizontalHeaderItem(4);
        ___qtablewidgetitem15->setText(QApplication::translate("TestDialog", "signal", nullptr));
        pushButton_2->setText(QApplication::translate("TestDialog", "Start scan", nullptr));
        pushButton_3->setText(QApplication::translate("TestDialog", "Stop", nullptr));
        comboBox_2->setItemText(0, QApplication::translate("TestDialog", "522", nullptr));
        comboBox_2->setItemText(1, QApplication::translate("TestDialog", "770", nullptr));

        label_7->setText(QApplication::translate("TestDialog", "Frequency", nullptr));
        pushButton_4->setText(QApplication::translate("TestDialog", "Clear data", nullptr));
        pushButton_5->setText(QApplication::translate("TestDialog", "To CSV", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("TestDialog", "GIS", nullptr));
        groupBox_2->setTitle(QApplication::translate("TestDialog", "Input", nullptr));
        comboBoxConstellRefStream->setItemText(0, QApplication::translate("TestDialog", "VV016-256QAM34_TP06_CSP", nullptr));

        comboBoxConstellFreq->setItemText(0, QApplication::translate("TestDialog", "522", nullptr));

        buttonObtainData->setText(QApplication::translate("TestDialog", "Obtain Data", nullptr));
        label_8->setText(QApplication::translate("TestDialog", "Reference Stream", nullptr));
        checkBoxConstellNonStop->setText(QApplication::translate("TestDialog", "non-stop receiving", nullptr));
        label_6->setText(QApplication::translate("TestDialog", "Frequency", nullptr));
        groupBox_3->setTitle(QApplication::translate("TestDialog", "Options", nullptr));
        label_11->setText(QApplication::translate("TestDialog", "Visualization Mode", nullptr));
        comboBoxConstallVizMod->setItemText(0, QApplication::translate("TestDialog", "QAM256", nullptr));
        comboBoxConstallVizMod->setItemText(1, QApplication::translate("TestDialog", "QAM128", nullptr));
        comboBoxConstallVizMod->setItemText(2, QApplication::translate("TestDialog", "QAM64", nullptr));

        checkBoxGroupNearest->setText(QApplication::translate("TestDialog", "Group Nearest Points", nullptr));
        checkBoxRecovery->setText(QApplication::translate("TestDialog", "Carrier Recovery", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("TestDialog", "Constellation", nullptr));
        label_9->setText(QApplication::translate("TestDialog", "DVB card:", nullptr));
        labelCard->setText(QApplication::translate("TestDialog", "(none)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TestDialog: public Ui_TestDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTDIALOG_H
