/********************************************************************************
** Form generated from reading UI file 'ExporteryhojPv.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EXPORTERYHOJPV_H
#define EXPORTERYHOJPV_H

#include <QtCore/QVariant>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QTabWidget *tabWidget;
    QWidget *Preview;
    QOpenGLWidget *openGLWidget;
    QWidget *Details;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *NameContainer;
    QLabel *NameLabel;
    QLabel *NameData;
    QHBoxLayout *FullPathContainer;
    QLabel *FullPathLabel;
    QLabel *FullPathData;
    QHBoxLayout *SizeContainer;
    QLabel *SizeLabel;
    QLabel *SizeData;
    QHBoxLayout *CompressedLengthContainer;
    QLabel *CompressedLengthLabel;
    QLabel *CompressedLengthData;
    QHBoxLayout *TimestampContainer;
    QLabel *TimestampLabel;
    QLabel *TimestampData;
    QHBoxLayout *PackageContainer;
    QLabel *PackageLabel;
    QLabel *PackageData;
    QSpacerItem *verticalSpacer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(720, 534);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        treeWidget = new QTreeWidget(centralwidget);
        treeWidget->setObjectName("treeWidget");

        horizontalLayout->addWidget(treeWidget);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        Preview = new QWidget();
        Preview->setObjectName("Preview");
        openGLWidget = new QOpenGLWidget(Preview);
        openGLWidget->setObjectName("openGLWidget");
        openGLWidget->setGeometry(QRect(0, 70, 300, 200));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);
        tabWidget->addTab(Preview, QString());
        Details = new QWidget();
        Details->setObjectName("Details");
        verticalLayout = new QVBoxLayout(Details);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        NameContainer = new QHBoxLayout();
        NameContainer->setObjectName("NameContainer");
        NameLabel = new QLabel(Details);
        NameLabel->setObjectName("NameLabel");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(NameLabel->sizePolicy().hasHeightForWidth());
        NameLabel->setSizePolicy(sizePolicy1);
        NameLabel->setMinimumSize(QSize(80, 30));
        NameLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        NameContainer->addWidget(NameLabel);

        NameData = new QLabel(Details);
        NameData->setObjectName("NameData");
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(NameData->sizePolicy().hasHeightForWidth());
        NameData->setSizePolicy(sizePolicy2);
        NameData->setAlignment(Qt::AlignCenter);

        NameContainer->addWidget(NameData);


        verticalLayout->addLayout(NameContainer);

        FullPathContainer = new QHBoxLayout();
        FullPathContainer->setObjectName("FullPathContainer");
        FullPathLabel = new QLabel(Details);
        FullPathLabel->setObjectName("FullPathLabel");
        sizePolicy1.setHeightForWidth(FullPathLabel->sizePolicy().hasHeightForWidth());
        FullPathLabel->setSizePolicy(sizePolicy1);
        FullPathLabel->setMinimumSize(QSize(80, 30));
        FullPathLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        FullPathContainer->addWidget(FullPathLabel);

        FullPathData = new QLabel(Details);
        FullPathData->setObjectName("FullPathData");
        sizePolicy2.setHeightForWidth(FullPathData->sizePolicy().hasHeightForWidth());
        FullPathData->setSizePolicy(sizePolicy2);
        FullPathData->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        FullPathContainer->addWidget(FullPathData);


        verticalLayout->addLayout(FullPathContainer);

        SizeContainer = new QHBoxLayout();
        SizeContainer->setObjectName("SizeContainer");
        SizeLabel = new QLabel(Details);
        SizeLabel->setObjectName("SizeLabel");
        sizePolicy1.setHeightForWidth(SizeLabel->sizePolicy().hasHeightForWidth());
        SizeLabel->setSizePolicy(sizePolicy1);
        SizeLabel->setMinimumSize(QSize(80, 30));
        SizeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        SizeContainer->addWidget(SizeLabel);

        SizeData = new QLabel(Details);
        SizeData->setObjectName("SizeData");
        sizePolicy2.setHeightForWidth(SizeData->sizePolicy().hasHeightForWidth());
        SizeData->setSizePolicy(sizePolicy2);
        SizeData->setAlignment(Qt::AlignCenter);

        SizeContainer->addWidget(SizeData);


        verticalLayout->addLayout(SizeContainer);

        CompressedLengthContainer = new QHBoxLayout();
        CompressedLengthContainer->setObjectName("CompressedLengthContainer");
        CompressedLengthLabel = new QLabel(Details);
        CompressedLengthLabel->setObjectName("CompressedLengthLabel");
        sizePolicy1.setHeightForWidth(CompressedLengthLabel->sizePolicy().hasHeightForWidth());
        CompressedLengthLabel->setSizePolicy(sizePolicy1);
        CompressedLengthLabel->setMinimumSize(QSize(80, 30));
        CompressedLengthLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        CompressedLengthContainer->addWidget(CompressedLengthLabel);

        CompressedLengthData = new QLabel(Details);
        CompressedLengthData->setObjectName("CompressedLengthData");
        sizePolicy2.setHeightForWidth(CompressedLengthData->sizePolicy().hasHeightForWidth());
        CompressedLengthData->setSizePolicy(sizePolicy2);
        CompressedLengthData->setAlignment(Qt::AlignCenter);

        CompressedLengthContainer->addWidget(CompressedLengthData);


        verticalLayout->addLayout(CompressedLengthContainer);

        TimestampContainer = new QHBoxLayout();
        TimestampContainer->setObjectName("TimestampContainer");
        TimestampLabel = new QLabel(Details);
        TimestampLabel->setObjectName("TimestampLabel");
        sizePolicy1.setHeightForWidth(TimestampLabel->sizePolicy().hasHeightForWidth());
        TimestampLabel->setSizePolicy(sizePolicy1);
        TimestampLabel->setMinimumSize(QSize(80, 30));
        TimestampLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        TimestampContainer->addWidget(TimestampLabel);

        TimestampData = new QLabel(Details);
        TimestampData->setObjectName("TimestampData");
        sizePolicy2.setHeightForWidth(TimestampData->sizePolicy().hasHeightForWidth());
        TimestampData->setSizePolicy(sizePolicy2);
        TimestampData->setAlignment(Qt::AlignCenter);

        TimestampContainer->addWidget(TimestampData);


        verticalLayout->addLayout(TimestampContainer);

        PackageContainer = new QHBoxLayout();
        PackageContainer->setObjectName("PackageContainer");
        PackageLabel = new QLabel(Details);
        PackageLabel->setObjectName("PackageLabel");
        sizePolicy1.setHeightForWidth(PackageLabel->sizePolicy().hasHeightForWidth());
        PackageLabel->setSizePolicy(sizePolicy1);
        PackageLabel->setMinimumSize(QSize(80, 30));
        PackageLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        PackageContainer->addWidget(PackageLabel);

        PackageData = new QLabel(Details);
        PackageData->setObjectName("PackageData");
        sizePolicy2.setHeightForWidth(PackageData->sizePolicy().hasHeightForWidth());
        PackageData->setSizePolicy(sizePolicy2);
        PackageData->setAlignment(Qt::AlignCenter);

        PackageContainer->addWidget(PackageData);


        verticalLayout->addLayout(PackageContainer);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(Details, QString());

        horizontalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Warframe Extractor", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "Name", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Preview), QCoreApplication::translate("MainWindow", "Preview", nullptr));
        NameLabel->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        NameData->setText(QString());
        FullPathLabel->setText(QCoreApplication::translate("MainWindow", "Full Path", nullptr));
        FullPathData->setText(QString());
        SizeLabel->setText(QCoreApplication::translate("MainWindow", "Size", nullptr));
        SizeData->setText(QString());
        CompressedLengthLabel->setText(QCoreApplication::translate("MainWindow", "Compressed", nullptr));
        CompressedLengthData->setText(QString());
        TimestampLabel->setText(QCoreApplication::translate("MainWindow", "Timestamp", nullptr));
        TimestampData->setText(QString());
        PackageLabel->setText(QCoreApplication::translate("MainWindow", "Package", nullptr));
        PackageData->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(Details), QCoreApplication::translate("MainWindow", "Metadata", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EXPORTERYHOJPV_H
