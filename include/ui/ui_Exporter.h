/********************************************************************************
** Form generated from reading UI file 'ExporteraQNVAf.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EXPORTERAQNVAF_H
#define EXPORTERAQNVAF_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
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
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QTabWidget *tabWidget;
    QWidget *Preview;
    QVBoxLayout *verticalLayout_3;
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
    QHBoxLayout *horizontalLayout_3;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ExtractButton;
    QProgressBar *ExtractProgressBar;
    QSpacerItem *horizontalSpacer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(649, 562);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        treeWidget = new QTreeWidget(centralwidget);
        treeWidget->setObjectName("treeWidget");

        horizontalLayout->addWidget(treeWidget);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        Preview = new QWidget();
        Preview->setObjectName("Preview");
        verticalLayout_3 = new QVBoxLayout(Preview);
        verticalLayout_3->setObjectName("verticalLayout_3");
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
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NameLabel->sizePolicy().hasHeightForWidth());
        NameLabel->setSizePolicy(sizePolicy);
        NameLabel->setMinimumSize(QSize(80, 30));
        NameLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        NameContainer->addWidget(NameLabel);

        NameData = new QLabel(Details);
        NameData->setObjectName("NameData");
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(NameData->sizePolicy().hasHeightForWidth());
        NameData->setSizePolicy(sizePolicy1);
        NameData->setAlignment(Qt::AlignCenter);

        NameContainer->addWidget(NameData);


        verticalLayout->addLayout(NameContainer);

        FullPathContainer = new QHBoxLayout();
        FullPathContainer->setObjectName("FullPathContainer");
        FullPathLabel = new QLabel(Details);
        FullPathLabel->setObjectName("FullPathLabel");
        sizePolicy.setHeightForWidth(FullPathLabel->sizePolicy().hasHeightForWidth());
        FullPathLabel->setSizePolicy(sizePolicy);
        FullPathLabel->setMinimumSize(QSize(80, 30));
        FullPathLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        FullPathContainer->addWidget(FullPathLabel);

        FullPathData = new QLabel(Details);
        FullPathData->setObjectName("FullPathData");
        sizePolicy1.setHeightForWidth(FullPathData->sizePolicy().hasHeightForWidth());
        FullPathData->setSizePolicy(sizePolicy1);
        FullPathData->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        FullPathContainer->addWidget(FullPathData);


        verticalLayout->addLayout(FullPathContainer);

        SizeContainer = new QHBoxLayout();
        SizeContainer->setObjectName("SizeContainer");
        SizeLabel = new QLabel(Details);
        SizeLabel->setObjectName("SizeLabel");
        sizePolicy.setHeightForWidth(SizeLabel->sizePolicy().hasHeightForWidth());
        SizeLabel->setSizePolicy(sizePolicy);
        SizeLabel->setMinimumSize(QSize(80, 30));
        SizeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        SizeContainer->addWidget(SizeLabel);

        SizeData = new QLabel(Details);
        SizeData->setObjectName("SizeData");
        sizePolicy1.setHeightForWidth(SizeData->sizePolicy().hasHeightForWidth());
        SizeData->setSizePolicy(sizePolicy1);
        SizeData->setAlignment(Qt::AlignCenter);

        SizeContainer->addWidget(SizeData);


        verticalLayout->addLayout(SizeContainer);

        CompressedLengthContainer = new QHBoxLayout();
        CompressedLengthContainer->setObjectName("CompressedLengthContainer");
        CompressedLengthLabel = new QLabel(Details);
        CompressedLengthLabel->setObjectName("CompressedLengthLabel");
        sizePolicy.setHeightForWidth(CompressedLengthLabel->sizePolicy().hasHeightForWidth());
        CompressedLengthLabel->setSizePolicy(sizePolicy);
        CompressedLengthLabel->setMinimumSize(QSize(80, 30));
        CompressedLengthLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        CompressedLengthContainer->addWidget(CompressedLengthLabel);

        CompressedLengthData = new QLabel(Details);
        CompressedLengthData->setObjectName("CompressedLengthData");
        sizePolicy1.setHeightForWidth(CompressedLengthData->sizePolicy().hasHeightForWidth());
        CompressedLengthData->setSizePolicy(sizePolicy1);
        CompressedLengthData->setAlignment(Qt::AlignCenter);

        CompressedLengthContainer->addWidget(CompressedLengthData);


        verticalLayout->addLayout(CompressedLengthContainer);

        TimestampContainer = new QHBoxLayout();
        TimestampContainer->setObjectName("TimestampContainer");
        TimestampLabel = new QLabel(Details);
        TimestampLabel->setObjectName("TimestampLabel");
        sizePolicy.setHeightForWidth(TimestampLabel->sizePolicy().hasHeightForWidth());
        TimestampLabel->setSizePolicy(sizePolicy);
        TimestampLabel->setMinimumSize(QSize(80, 30));
        TimestampLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        TimestampContainer->addWidget(TimestampLabel);

        TimestampData = new QLabel(Details);
        TimestampData->setObjectName("TimestampData");
        sizePolicy1.setHeightForWidth(TimestampData->sizePolicy().hasHeightForWidth());
        TimestampData->setSizePolicy(sizePolicy1);
        TimestampData->setAlignment(Qt::AlignCenter);

        TimestampContainer->addWidget(TimestampData);


        verticalLayout->addLayout(TimestampContainer);

        PackageContainer = new QHBoxLayout();
        PackageContainer->setObjectName("PackageContainer");
        PackageLabel = new QLabel(Details);
        PackageLabel->setObjectName("PackageLabel");
        sizePolicy.setHeightForWidth(PackageLabel->sizePolicy().hasHeightForWidth());
        PackageLabel->setSizePolicy(sizePolicy);
        PackageLabel->setMinimumSize(QSize(80, 30));
        PackageLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        PackageContainer->addWidget(PackageLabel);

        PackageData = new QLabel(Details);
        PackageData->setObjectName("PackageData");
        sizePolicy1.setHeightForWidth(PackageData->sizePolicy().hasHeightForWidth());
        PackageData->setSizePolicy(sizePolicy1);
        PackageData->setAlignment(Qt::AlignCenter);

        PackageContainer->addWidget(PackageData);


        verticalLayout->addLayout(PackageContainer);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(Details, QString());

        horizontalLayout->addWidget(tabWidget);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, -1, 0, -1);
        ExtractButton = new QPushButton(frame);
        ExtractButton->setObjectName("ExtractButton");
        sizePolicy.setHeightForWidth(ExtractButton->sizePolicy().hasHeightForWidth());
        ExtractButton->setSizePolicy(sizePolicy);
        ExtractButton->setCheckable(false);
        ExtractButton->setChecked(false);
        ExtractButton->setFlat(false);

        horizontalLayout_2->addWidget(ExtractButton);

        ExtractProgressBar = new QProgressBar(frame);
        ExtractProgressBar->setObjectName("ExtractProgressBar");
        ExtractProgressBar->setValue(0);

        horizontalLayout_2->addWidget(ExtractProgressBar);


        horizontalLayout_3->addWidget(frame);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_3);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        ExtractButton->setDefault(false);


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
        ExtractButton->setText(QCoreApplication::translate("MainWindow", "Extract", nullptr));
#if QT_CONFIG(shortcut)
        ExtractButton->setShortcut(QString());
#endif // QT_CONFIG(shortcut)
        ExtractProgressBar->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EXPORTERAQNVAF_H
