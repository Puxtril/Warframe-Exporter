/********************************************************************************
** Form generated from reading UI file 'PickerXNRkFX.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PICKERXNRKFX_H
#define PICKERXNRKFX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
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
    QGroupBox *ExportPathFrame;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *ExportPathInput;
    QPushButton *ExportPathBrowse;
    QSpacerItem *TopSpacer;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *PackagesViewFrame;
    QGridLayout *gridLayout;
    QCheckBox *CheckboxViewLevels;
    QCheckBox *CheckboxViewModels;
    QCheckBox *CheckboxViewTextures;
    QCheckBox *CheckboxViewMaterials;
    QCheckBox *CheckboxViewAudio;
    QCheckBox *CheckboxViewShaders;
    QSpacerItem *horizontalSpacer;
    QGroupBox *PackagesExportFrame;
    QGridLayout *gridLayout_3;
    QCheckBox *CheckboxExportLevels;
    QCheckBox *CheckboxExportModels;
    QCheckBox *CheckboxExportTextures;
    QCheckBox *CheckboxExportMaterials;
    QCheckBox *CheckboxExportAudio;
    QCheckBox *CheckboxExportShaders;
    QSpacerItem *BottomSpacer;
    QFrame *LoadButtonFrame;
    QHBoxLayout *horizontalLayout_3;
    QLabel *VersionLabel;
    QPushButton *LoadButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *WindowPicker)
    {
        if (WindowPicker->objectName().isEmpty())
            WindowPicker->setObjectName("WindowPicker");
        WindowPicker->resize(465, 394);
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

        ExportPathFrame = new QGroupBox(WindowPicker);
        ExportPathFrame->setObjectName("ExportPathFrame");
        ExportPathFrame->setMaximumSize(QSize(16777215, 70));
        horizontalLayout_4 = new QHBoxLayout(ExportPathFrame);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        ExportPathInput = new QLineEdit(ExportPathFrame);
        ExportPathInput->setObjectName("ExportPathInput");

        horizontalLayout_4->addWidget(ExportPathInput);

        ExportPathBrowse = new QPushButton(ExportPathFrame);
        ExportPathBrowse->setObjectName("ExportPathBrowse");

        horizontalLayout_4->addWidget(ExportPathBrowse);


        verticalLayout->addWidget(ExportPathFrame);

        TopSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(TopSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        PackagesViewFrame = new QGroupBox(WindowPicker);
        PackagesViewFrame->setObjectName("PackagesViewFrame");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(40);
        sizePolicy.setHeightForWidth(PackagesViewFrame->sizePolicy().hasHeightForWidth());
        PackagesViewFrame->setSizePolicy(sizePolicy);
        PackagesViewFrame->setMinimumSize(QSize(0, 100));
        PackagesViewFrame->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        PackagesViewFrame->setCheckable(false);
        gridLayout = new QGridLayout(PackagesViewFrame);
        gridLayout->setObjectName("gridLayout");
        CheckboxViewLevels = new QCheckBox(PackagesViewFrame);
        CheckboxViewLevels->setObjectName("CheckboxViewLevels");

        gridLayout->addWidget(CheckboxViewLevels, 2, 0, 1, 1);

        CheckboxViewModels = new QCheckBox(PackagesViewFrame);
        CheckboxViewModels->setObjectName("CheckboxViewModels");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(CheckboxViewModels->sizePolicy().hasHeightForWidth());
        CheckboxViewModels->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(CheckboxViewModels, 0, 1, 1, 1);

        CheckboxViewTextures = new QCheckBox(PackagesViewFrame);
        CheckboxViewTextures->setObjectName("CheckboxViewTextures");
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(CheckboxViewTextures->sizePolicy().hasHeightForWidth());
        CheckboxViewTextures->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(CheckboxViewTextures, 0, 0, 1, 1);

        CheckboxViewMaterials = new QCheckBox(PackagesViewFrame);
        CheckboxViewMaterials->setObjectName("CheckboxViewMaterials");
        sizePolicy2.setHeightForWidth(CheckboxViewMaterials->sizePolicy().hasHeightForWidth());
        CheckboxViewMaterials->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(CheckboxViewMaterials, 2, 1, 1, 1);

        CheckboxViewAudio = new QCheckBox(PackagesViewFrame);
        CheckboxViewAudio->setObjectName("CheckboxViewAudio");

        gridLayout->addWidget(CheckboxViewAudio, 3, 0, 1, 1);

        CheckboxViewShaders = new QCheckBox(PackagesViewFrame);
        CheckboxViewShaders->setObjectName("CheckboxViewShaders");

        gridLayout->addWidget(CheckboxViewShaders, 3, 1, 1, 1);


        horizontalLayout_2->addWidget(PackagesViewFrame);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        PackagesExportFrame = new QGroupBox(WindowPicker);
        PackagesExportFrame->setObjectName("PackagesExportFrame");
        sizePolicy.setHeightForWidth(PackagesExportFrame->sizePolicy().hasHeightForWidth());
        PackagesExportFrame->setSizePolicy(sizePolicy);
        PackagesExportFrame->setMinimumSize(QSize(0, 100));
        PackagesExportFrame->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        PackagesExportFrame->setCheckable(false);
        gridLayout_3 = new QGridLayout(PackagesExportFrame);
        gridLayout_3->setObjectName("gridLayout_3");
        CheckboxExportLevels = new QCheckBox(PackagesExportFrame);
        CheckboxExportLevels->setObjectName("CheckboxExportLevels");

        gridLayout_3->addWidget(CheckboxExportLevels, 2, 0, 1, 1);

        CheckboxExportModels = new QCheckBox(PackagesExportFrame);
        CheckboxExportModels->setObjectName("CheckboxExportModels");
        sizePolicy1.setHeightForWidth(CheckboxExportModels->sizePolicy().hasHeightForWidth());
        CheckboxExportModels->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(CheckboxExportModels, 0, 1, 1, 1);

        CheckboxExportTextures = new QCheckBox(PackagesExportFrame);
        CheckboxExportTextures->setObjectName("CheckboxExportTextures");
        sizePolicy2.setHeightForWidth(CheckboxExportTextures->sizePolicy().hasHeightForWidth());
        CheckboxExportTextures->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(CheckboxExportTextures, 0, 0, 1, 1);

        CheckboxExportMaterials = new QCheckBox(PackagesExportFrame);
        CheckboxExportMaterials->setObjectName("CheckboxExportMaterials");
        sizePolicy2.setHeightForWidth(CheckboxExportMaterials->sizePolicy().hasHeightForWidth());
        CheckboxExportMaterials->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(CheckboxExportMaterials, 2, 1, 1, 1);

        CheckboxExportAudio = new QCheckBox(PackagesExportFrame);
        CheckboxExportAudio->setObjectName("CheckboxExportAudio");

        gridLayout_3->addWidget(CheckboxExportAudio, 3, 0, 1, 1);

        CheckboxExportShaders = new QCheckBox(PackagesExportFrame);
        CheckboxExportShaders->setObjectName("CheckboxExportShaders");

        gridLayout_3->addWidget(CheckboxExportShaders, 3, 1, 1, 1);


        horizontalLayout_2->addWidget(PackagesExportFrame);


        verticalLayout->addLayout(horizontalLayout_2);

        BottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(BottomSpacer);

        LoadButtonFrame = new QFrame(WindowPicker);
        LoadButtonFrame->setObjectName("LoadButtonFrame");
        LoadButtonFrame->setMaximumSize(QSize(16777215, 60));
        LoadButtonFrame->setFrameShape(QFrame::NoFrame);
        LoadButtonFrame->setFrameShadow(QFrame::Raised);
        LoadButtonFrame->setLineWidth(0);
        horizontalLayout_3 = new QHBoxLayout(LoadButtonFrame);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        VersionLabel = new QLabel(LoadButtonFrame);
        VersionLabel->setObjectName("VersionLabel");
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(VersionLabel->sizePolicy().hasHeightForWidth());
        VersionLabel->setSizePolicy(sizePolicy3);
        QFont font;
        font.setPointSize(8);
        VersionLabel->setFont(font);
        VersionLabel->setTextFormat(Qt::PlainText);

        horizontalLayout_3->addWidget(VersionLabel);

        LoadButton = new QPushButton(LoadButtonFrame);
        LoadButton->setObjectName("LoadButton");
        LoadButton->setMinimumSize(QSize(100, 0));
        LoadButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_3->addWidget(LoadButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(LoadButtonFrame);


        retranslateUi(WindowPicker);

        QMetaObject::connectSlotsByName(WindowPicker);
    } // setupUi

    void retranslateUi(QDialog *WindowPicker)
    {
        WindowPicker->setWindowTitle(QCoreApplication::translate("WindowPicker", "Exporter Options", nullptr));
#if QT_CONFIG(whatsthis)
        CacheWindowsFrame->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Inside the Warframe installation directory, there is a folder <span style=\" font-style:italic;\">Cache.Windows</span></p><p>Typically Warframe is installed in <span style=\" font-weight:700;\">C:\\Steam\\steamapps\\common\\Warframe</span></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        CacheWindowsFrame->setTitle(QCoreApplication::translate("WindowPicker", "Cache.Windows Path", nullptr));
        CacheWindowsBrowse->setText(QCoreApplication::translate("WindowPicker", "Browse", nullptr));
#if QT_CONFIG(whatsthis)
        ExportPathFrame->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Exported files will be saved to the folder specified here</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        ExportPathFrame->setTitle(QCoreApplication::translate("WindowPicker", "Export Path", nullptr));
        ExportPathBrowse->setText(QCoreApplication::translate("WindowPicker", "Browse", nullptr));
#if QT_CONFIG(whatsthis)
        PackagesViewFrame->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Once &quot;Load&quot; is pressed, you will be able to browse the internal files. This determines which packages are loaded and presented. There may be some overlap, so picking Models, Materials, or Audio will all load the &quot;Misc&quot; package. In esence, those 3 checkboxes do the same thing... but don't worry about that, just pick what you want to see!</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        PackagesViewFrame->setTitle(QCoreApplication::translate("WindowPicker", "View", nullptr));
        CheckboxViewLevels->setText(QCoreApplication::translate("WindowPicker", "Levels", nullptr));
        CheckboxViewModels->setText(QCoreApplication::translate("WindowPicker", "Models", nullptr));
        CheckboxViewTextures->setText(QCoreApplication::translate("WindowPicker", "Textures", nullptr));
        CheckboxViewMaterials->setText(QCoreApplication::translate("WindowPicker", "Materials", nullptr));
        CheckboxViewAudio->setText(QCoreApplication::translate("WindowPicker", "Audio", nullptr));
        CheckboxViewShaders->setText(QCoreApplication::translate("WindowPicker", "Shaders", nullptr));
#if QT_CONFIG(whatsthis)
        PackagesExportFrame->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>When a folder is selected for export, this determines which file types are exported to your computer. The general order from smallest -&gt; largest:</p><p>Materials -&gt; Models -&gt; Audio -&gt; Textures -&gt; Levels (very big)</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        PackagesExportFrame->setTitle(QCoreApplication::translate("WindowPicker", "Export", nullptr));
        CheckboxExportLevels->setText(QCoreApplication::translate("WindowPicker", "Levels", nullptr));
        CheckboxExportModels->setText(QCoreApplication::translate("WindowPicker", "Models", nullptr));
        CheckboxExportTextures->setText(QCoreApplication::translate("WindowPicker", "Textures", nullptr));
        CheckboxExportMaterials->setText(QCoreApplication::translate("WindowPicker", "Materials", nullptr));
        CheckboxExportAudio->setText(QCoreApplication::translate("WindowPicker", "Audio", nullptr));
        CheckboxExportShaders->setText(QCoreApplication::translate("WindowPicker", "Shaders", nullptr));
#if QT_CONFIG(whatsthis)
        LoadButtonFrame->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Load the main window and begin browsing the internal files.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        VersionLabel->setText(QString());
        LoadButton->setText(QCoreApplication::translate("WindowPicker", "Load", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WindowPicker: public Ui_WindowPicker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PICKERXNRKFX_H
