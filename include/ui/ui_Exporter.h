/********************************************************************************
** Form generated from reading UI file 'ExporterOekCDg.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EXPORTEROEKCDG_H
#define EXPORTEROEKCDG_H

#include <QtCore/QVariant>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QTabWidget *tabWidget;
    QWidget *Preview;
    QOpenGLWidget *openGLWidget;
    QWidget *Details;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *FullPathContainer;
    QLabel *FullPathLabel;
    QLabel *FullPathData;
    QHBoxLayout *TimestampContainer;
    QLabel *TimestampLabel;
    QLabel *TimestampData;
    QHBoxLayout *CompressedLengthContainer;
    QLabel *CompressedLengthLabel;
    QLabel *CompressedLengthData;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(818, 634);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        treeView = new QTreeView(centralwidget);
        treeView->setObjectName("treeView");

        horizontalLayout->addWidget(treeView);

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
        verticalLayoutWidget = new QWidget(Details);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 100, 291, 231));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        FullPathContainer = new QHBoxLayout();
        FullPathContainer->setObjectName("FullPathContainer");
        FullPathLabel = new QLabel(verticalLayoutWidget);
        FullPathLabel->setObjectName("FullPathLabel");

        FullPathContainer->addWidget(FullPathLabel);

        FullPathData = new QLabel(verticalLayoutWidget);
        FullPathData->setObjectName("FullPathData");
        FullPathData->setAlignment(Qt::AlignCenter);

        FullPathContainer->addWidget(FullPathData);


        verticalLayout_2->addLayout(FullPathContainer);

        TimestampContainer = new QHBoxLayout();
        TimestampContainer->setObjectName("TimestampContainer");
        TimestampLabel = new QLabel(verticalLayoutWidget);
        TimestampLabel->setObjectName("TimestampLabel");

        TimestampContainer->addWidget(TimestampLabel);

        TimestampData = new QLabel(verticalLayoutWidget);
        TimestampData->setObjectName("TimestampData");
        TimestampData->setAlignment(Qt::AlignCenter);

        TimestampContainer->addWidget(TimestampData);


        verticalLayout_2->addLayout(TimestampContainer);

        CompressedLengthContainer = new QHBoxLayout();
        CompressedLengthContainer->setObjectName("CompressedLengthContainer");
        CompressedLengthLabel = new QLabel(verticalLayoutWidget);
        CompressedLengthLabel->setObjectName("CompressedLengthLabel");

        CompressedLengthContainer->addWidget(CompressedLengthLabel);

        CompressedLengthData = new QLabel(verticalLayoutWidget);
        CompressedLengthData->setObjectName("CompressedLengthData");
        CompressedLengthData->setAlignment(Qt::AlignCenter);

        CompressedLengthContainer->addWidget(CompressedLengthData);


        verticalLayout_2->addLayout(CompressedLengthContainer);

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
        tabWidget->setTabText(tabWidget->indexOf(Preview), QCoreApplication::translate("MainWindow", "Preview", nullptr));
        FullPathLabel->setText(QCoreApplication::translate("MainWindow", "Full Path", nullptr));
        FullPathData->setText(QString());
        TimestampLabel->setText(QCoreApplication::translate("MainWindow", "Timestamp", nullptr));
        TimestampData->setText(QString());
        CompressedLengthLabel->setText(QCoreApplication::translate("MainWindow", "Compressed Length", nullptr));
        CompressedLengthData->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(Details), QCoreApplication::translate("MainWindow", "Metadata", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EXPORTEROEKCDG_H
