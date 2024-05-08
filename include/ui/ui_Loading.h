/********************************************************************************
** Form generated from reading UI file 'LoadingXQXVzh.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LOADINGXQXVZH_H
#define LOADINGXQXVZH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Loading
{
public:
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;

    void setupUi(QDialog *Loading)
    {
        if (Loading->objectName().isEmpty())
            Loading->setObjectName("Loading");
        Loading->resize(427, 60);
        verticalLayout = new QVBoxLayout(Loading);
        verticalLayout->setObjectName("verticalLayout");
        progressBar = new QProgressBar(Loading);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);


        retranslateUi(Loading);

        QMetaObject::connectSlotsByName(Loading);
    } // setupUi

    void retranslateUi(QDialog *Loading)
    {
        Loading->setWindowTitle(QCoreApplication::translate("Loading", "Loading", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Loading: public Ui_Loading {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LOADINGXQXVZH_H
