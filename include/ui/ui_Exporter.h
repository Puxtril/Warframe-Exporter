/********************************************************************************
** Form generated from reading UI file 'ExporterkwnOcf.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EXPORTERKWNOCF_H
#define EXPORTERKWNOCF_H

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
#include <QtWidgets/QSplitter>
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
    QVBoxLayout *verticalLayout_5;
    QSplitter *splitter;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout_3;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ExtractButton;
    QProgressBar *ExtractProgressBar;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_4;
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
    QSpacerItem *verticalSpacer_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(649, 562);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_5 = new QVBoxLayout(centralwidget);
        verticalLayout_5->setObjectName("verticalLayout_5");
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        widget = new QWidget(splitter);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        treeWidget = new QTreeWidget(widget);
        treeWidget->setObjectName("treeWidget");

        verticalLayout_2->addWidget(treeWidget);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        frame = new QFrame(widget);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, -1, 0, -1);
        ExtractButton = new QPushButton(frame);
        ExtractButton->setObjectName("ExtractButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ExtractButton->sizePolicy().hasHeightForWidth());
        ExtractButton->setSizePolicy(sizePolicy1);
        ExtractButton->setCheckable(false);
        ExtractButton->setChecked(false);
        ExtractButton->setFlat(false);

        horizontalLayout_2->addWidget(ExtractButton);

        ExtractProgressBar = new QProgressBar(frame);
        ExtractProgressBar->setObjectName("ExtractProgressBar");
        ExtractProgressBar->setValue(0);

        horizontalLayout_2->addWidget(ExtractProgressBar);


        horizontalLayout_3->addWidget(frame);


        verticalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_2);

        splitter->addWidget(widget);
        horizontalLayoutWidget = new QWidget(splitter);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        tabWidget = new QTabWidget(horizontalLayoutWidget);
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
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        NameContainer = new QHBoxLayout();
        NameContainer->setObjectName("NameContainer");
        NameLabel = new QLabel(Details);
        NameLabel->setObjectName("NameLabel");
        sizePolicy1.setHeightForWidth(NameLabel->sizePolicy().hasHeightForWidth());
        NameLabel->setSizePolicy(sizePolicy1);
        NameLabel->setMinimumSize(QSize(80, 30));
        NameLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        NameContainer->addWidget(NameLabel);

        NameData = new QLabel(Details);
        NameData->setObjectName("NameData");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Preferred);
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

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(Details, QString());

        verticalLayout_4->addWidget(tabWidget);

        verticalSpacer_3 = new QSpacerItem(20, 47, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_3);


        horizontalLayout_4->addLayout(verticalLayout_4);

        splitter->addWidget(horizontalLayoutWidget);

        verticalLayout_5->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        ExtractButton->setDefault(false);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Warframe Extractor", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "Name", nullptr));
        ExtractButton->setText(QCoreApplication::translate("MainWindow", "Extract", nullptr));
#if QT_CONFIG(shortcut)
        ExtractButton->setShortcut(QString());
#endif // QT_CONFIG(shortcut)
        ExtractProgressBar->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
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

#endif // EXPORTERKWNOCF_H
