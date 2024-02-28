/********************************************************************************
** Form generated from reading UI file 'PickerPbdMZD.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PICKERPBDMZD_H
#define PICKERPBDMZD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WindowPicker
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *CacheWindowsFrame;
    QHBoxLayout *horizontalLayout;
    QLineEdit *CacheWindowsInput;
    QPushButton *CacheWindowsBrowse;
    QSpacerItem *TopSpacer;
    QGroupBox *PackagesSelectFrame;
    QGridLayout *gridLayout;
    QCheckBox *CheckboxTextures;
    QCheckBox *CheckboxLevels;
    QCheckBox *CheckboxAudio;
    QCheckBox *CheckboxMaterials;
    QCheckBox *CheckboxModels;
    QSpacerItem *BottomSpacer;
    QFrame *LoadButtonFrame;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *LoadButton;

    void setupUi(QDialog *WindowPicker)
    {
        if (WindowPicker->objectName().isEmpty())
            WindowPicker->setObjectName("WindowPicker");
        WindowPicker->resize(397, 322);
        verticalLayout = new QVBoxLayout(WindowPicker);
        verticalLayout->setObjectName("verticalLayout");
        CacheWindowsFrame = new QGroupBox(WindowPicker);
        CacheWindowsFrame->setObjectName("CacheWindowsFrame");
        CacheWindowsFrame->setMaximumSize(QSize(16777215, 70));
        horizontalLayout = new QHBoxLayout(CacheWindowsFrame);
        horizontalLayout->setObjectName("horizontalLayout");
        CacheWindowsInput = new QLineEdit(CacheWindowsFrame);
        CacheWindowsInput->setObjectName("CacheWindowsInput");

        horizontalLayout->addWidget(CacheWindowsInput);

        CacheWindowsBrowse = new QPushButton(CacheWindowsFrame);
        CacheWindowsBrowse->setObjectName("CacheWindowsBrowse");

        horizontalLayout->addWidget(CacheWindowsBrowse);


        verticalLayout->addWidget(CacheWindowsFrame);

        TopSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(TopSpacer);

        PackagesSelectFrame = new QGroupBox(WindowPicker);
        PackagesSelectFrame->setObjectName("PackagesSelectFrame");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(40);
        sizePolicy.setHeightForWidth(PackagesSelectFrame->sizePolicy().hasHeightForWidth());
        PackagesSelectFrame->setSizePolicy(sizePolicy);
        PackagesSelectFrame->setMinimumSize(QSize(0, 100));
        PackagesSelectFrame->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        PackagesSelectFrame->setCheckable(false);
        gridLayout = new QGridLayout(PackagesSelectFrame);
        gridLayout->setObjectName("gridLayout");
        CheckboxTextures = new QCheckBox(PackagesSelectFrame);
        CheckboxTextures->setObjectName("CheckboxTextures");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(CheckboxTextures->sizePolicy().hasHeightForWidth());
        CheckboxTextures->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(CheckboxTextures, 0, 0, 1, 1);

        CheckboxLevels = new QCheckBox(PackagesSelectFrame);
        CheckboxLevels->setObjectName("CheckboxLevels");

        gridLayout->addWidget(CheckboxLevels, 2, 0, 1, 1);

        CheckboxAudio = new QCheckBox(PackagesSelectFrame);
        CheckboxAudio->setObjectName("CheckboxAudio");

        gridLayout->addWidget(CheckboxAudio, 3, 0, 1, 1);

        CheckboxMaterials = new QCheckBox(PackagesSelectFrame);
        CheckboxMaterials->setObjectName("CheckboxMaterials");
        sizePolicy1.setHeightForWidth(CheckboxMaterials->sizePolicy().hasHeightForWidth());
        CheckboxMaterials->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(CheckboxMaterials, 2, 1, 1, 1);

        CheckboxModels = new QCheckBox(PackagesSelectFrame);
        CheckboxModels->setObjectName("CheckboxModels");
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Ignored);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(CheckboxModels->sizePolicy().hasHeightForWidth());
        CheckboxModels->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(CheckboxModels, 0, 1, 1, 1);


        verticalLayout->addWidget(PackagesSelectFrame);

        BottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(BottomSpacer);

        LoadButtonFrame = new QFrame(WindowPicker);
        LoadButtonFrame->setObjectName("LoadButtonFrame");
        LoadButtonFrame->setMaximumSize(QSize(16777215, 60));
        LoadButtonFrame->setFrameShape(QFrame::StyledPanel);
        LoadButtonFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(LoadButtonFrame);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        LoadButton = new QPushButton(LoadButtonFrame);
        LoadButton->setObjectName("LoadButton");
        LoadButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_3->addWidget(LoadButton);


        verticalLayout->addWidget(LoadButtonFrame);


        retranslateUi(WindowPicker);

        QMetaObject::connectSlotsByName(WindowPicker);
    } // setupUi

    void retranslateUi(QDialog *WindowPicker)
    {
        WindowPicker->setWindowTitle(QCoreApplication::translate("WindowPicker", "Dialog", nullptr));
        CacheWindowsFrame->setTitle(QCoreApplication::translate("WindowPicker", "Cache.Windows Path", nullptr));
        CacheWindowsBrowse->setText(QCoreApplication::translate("WindowPicker", "Browse", nullptr));
        PackagesSelectFrame->setTitle(QCoreApplication::translate("WindowPicker", "Packages", nullptr));
        CheckboxTextures->setText(QCoreApplication::translate("WindowPicker", "Textures", nullptr));
        CheckboxLevels->setText(QCoreApplication::translate("WindowPicker", "Levels", nullptr));
        CheckboxAudio->setText(QCoreApplication::translate("WindowPicker", "Audio", nullptr));
        CheckboxMaterials->setText(QCoreApplication::translate("WindowPicker", "Materials", nullptr));
        CheckboxModels->setText(QCoreApplication::translate("WindowPicker", "Models", nullptr));
        LoadButton->setText(QCoreApplication::translate("WindowPicker", "Load", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WindowPicker: public Ui_WindowPicker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PICKERPBDMZD_H
