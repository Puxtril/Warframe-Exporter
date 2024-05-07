/********************************************************************************
** Form generated from reading UI file 'AdditionalSettingsSbpqln.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ADDITIONALSETTINGSSBPQLN_H
#define ADDITIONALSETTINGSSBPQLN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AdditionalSettings
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *MiscSettingsGroup;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *FilterFilesLayout;
    QLabel *FilterFilesLabel;
    QCheckBox *FilterFilesCheckbox;
    QSpacerItem *SettingsSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AdditionalSettings)
    {
        if (AdditionalSettings->objectName().isEmpty())
            AdditionalSettings->setObjectName("AdditionalSettings");
        AdditionalSettings->resize(382, 204);
        verticalLayout = new QVBoxLayout(AdditionalSettings);
        verticalLayout->setObjectName("verticalLayout");
        MiscSettingsGroup = new QGroupBox(AdditionalSettings);
        MiscSettingsGroup->setObjectName("MiscSettingsGroup");
        verticalLayout_2 = new QVBoxLayout(MiscSettingsGroup);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        FilterFilesLayout = new QHBoxLayout();
        FilterFilesLayout->setObjectName("FilterFilesLayout");
        FilterFilesLabel = new QLabel(MiscSettingsGroup);
        FilterFilesLabel->setObjectName("FilterFilesLabel");

        FilterFilesLayout->addWidget(FilterFilesLabel);

        FilterFilesCheckbox = new QCheckBox(MiscSettingsGroup);
        FilterFilesCheckbox->setObjectName("FilterFilesCheckbox");
        FilterFilesCheckbox->setChecked(true);

        FilterFilesLayout->addWidget(FilterFilesCheckbox);


        verticalLayout_2->addLayout(FilterFilesLayout);

        SettingsSpacer = new QSpacerItem(20, 8, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(SettingsSpacer);


        verticalLayout->addWidget(MiscSettingsGroup);

        buttonBox = new QDialogButtonBox(AdditionalSettings);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(AdditionalSettings);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, AdditionalSettings, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, AdditionalSettings, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(AdditionalSettings);
    } // setupUi

    void retranslateUi(QDialog *AdditionalSettings)
    {
        AdditionalSettings->setWindowTitle(QCoreApplication::translate("AdditionalSettings", "Additional Settings", nullptr));
        MiscSettingsGroup->setTitle(QCoreApplication::translate("AdditionalSettings", "Settings", nullptr));
        FilterFilesLabel->setText(QCoreApplication::translate("AdditionalSettings", "Filter files by selected type", nullptr));
        FilterFilesCheckbox->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AdditionalSettings: public Ui_AdditionalSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ADDITIONALSETTINGSSBPQLN_H
