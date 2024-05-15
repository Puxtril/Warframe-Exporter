/********************************************************************************
** Form generated from reading UI file 'ExporterAOWjlh.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EXPORTERAOWJLH_H
#define EXPORTERAOWJLH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
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
    QWidget *layoutWidget;
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
    QWidget *PreviewButtonsArea;
    QHBoxLayout *horizontalLayout_5;
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
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName("layoutWidget");
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        treeWidget = new QTreeWidget(layoutWidget);
        treeWidget->setObjectName("treeWidget");

        verticalLayout_2->addWidget(treeWidget);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        frame = new QFrame(layoutWidget);
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

        splitter->addWidget(layoutWidget);
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

        verticalLayout_4->addWidget(tabWidget);

        PreviewButtonsArea = new QWidget(horizontalLayoutWidget);
        PreviewButtonsArea->setObjectName("PreviewButtonsArea");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(PreviewButtonsArea->sizePolicy().hasHeightForWidth());
        PreviewButtonsArea->setSizePolicy(sizePolicy2);
        PreviewButtonsArea->setMinimumSize(QSize(0, 47));
        horizontalLayout_5 = new QHBoxLayout(PreviewButtonsArea);
        horizontalLayout_5->setObjectName("horizontalLayout_5");

        verticalLayout_4->addWidget(PreviewButtonsArea);


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
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EXPORTERAOWJLH_H
