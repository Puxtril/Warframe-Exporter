/********************************************************************************
** Form generated from reading UI file 'PickerDLuUOi.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PICKERDLUUOI_H
#define PICKERDLUUOI_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WindowPicker
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *CacheWindowsFrame;
    QHBoxLayout *horizontalLayout;
    QLineEdit *CacheWindowsInput;
    QPushButton *CacheWindowsBrowse;
    QGroupBox *ExportPathFrame;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *ExportPathInput;
    QPushButton *ExportPathBrowse;
    QSpacerItem *TopSpacer;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *TitleSection;
    QLabel *TitleType;
    QLabel *TitleExtract;
    QLabel *TitleOutputFormat;
    QFrame *TitleLine;
    QHBoxLayout *AudioSection;
    QLabel *AudioLabel;
    QCheckBox *AudioCheckbox;
    QLabel *AudioFormatLabel;
    QHBoxLayout *LandscapeSection;
    QLabel *LandscapeLabel;
    QCheckBox *LandscapeCheckbox;
    QLabel *LandscapeFormatLabel;
    QHBoxLayout *LevelSection;
    QLabel *LevelLabel;
    QCheckBox *LevelCheckbox;
    QLabel *LevelFormatLabel;
    QHBoxLayout *MaterialSection;
    QLabel *MaterialLabel;
    QCheckBox *MaterialCheckbox;
    QLabel *MaterialFormatLabel;
    QHBoxLayout *ModelSection;
    QLabel *ModelLabel;
    QCheckBox *ModelCheckbox;
    QLabel *ModelFormatLabel;
    QHBoxLayout *ShaderSecion;
    QLabel *ShaderLabel;
    QCheckBox *ShaderCheckbox;
    QComboBox *ShaderFormatCombo;
    QHBoxLayout *TextureSection;
    QLabel *TextureLabel;
    QCheckBox *TextureCheckbox;
    QComboBox *TextureFormatCombo;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *BottomSpacer;
    QFrame *LoadButtonFrame;
    QHBoxLayout *horizontalLayout_3;
    QLabel *VersionLabel;
    QPushButton *LoadButton;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *AdditionalSettingsButton;

    void setupUi(QDialog *WindowPicker)
    {
        if (WindowPicker->objectName().isEmpty())
            WindowPicker->setObjectName("WindowPicker");
        WindowPicker->resize(549, 507);
        verticalLayout_2 = new QVBoxLayout(WindowPicker);
        verticalLayout_2->setObjectName("verticalLayout_2");
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


        verticalLayout_2->addWidget(CacheWindowsFrame);

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


        verticalLayout_2->addWidget(ExportPathFrame);

        TopSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::MinimumExpanding);

        verticalLayout_2->addItem(TopSpacer);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_4 = new QSpacerItem(70, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        TitleSection = new QHBoxLayout();
        TitleSection->setObjectName("TitleSection");
        TitleSection->setContentsMargins(-1, 10, -1, 10);
        TitleType = new QLabel(WindowPicker);
        TitleType->setObjectName("TitleType");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TitleType->sizePolicy().hasHeightForWidth());
        TitleType->setSizePolicy(sizePolicy);
        TitleType->setMinimumSize(QSize(70, 0));
        QFont font;
        font.setUnderline(false);
        TitleType->setFont(font);
        TitleType->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        TitleSection->addWidget(TitleType);

        TitleExtract = new QLabel(WindowPicker);
        TitleExtract->setObjectName("TitleExtract");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(TitleExtract->sizePolicy().hasHeightForWidth());
        TitleExtract->setSizePolicy(sizePolicy1);
        TitleExtract->setFont(font);
        TitleExtract->setAlignment(Qt::AlignmentFlag::AlignCenter);

        TitleSection->addWidget(TitleExtract);

        TitleOutputFormat = new QLabel(WindowPicker);
        TitleOutputFormat->setObjectName("TitleOutputFormat");
        sizePolicy1.setHeightForWidth(TitleOutputFormat->sizePolicy().hasHeightForWidth());
        TitleOutputFormat->setSizePolicy(sizePolicy1);
        TitleOutputFormat->setFont(font);
        TitleOutputFormat->setAlignment(Qt::AlignmentFlag::AlignCenter);

        TitleSection->addWidget(TitleOutputFormat);


        verticalLayout->addLayout(TitleSection);

        TitleLine = new QFrame(WindowPicker);
        TitleLine->setObjectName("TitleLine");
        TitleLine->setFrameShape(QFrame::Shape::HLine);
        TitleLine->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout->addWidget(TitleLine);

        AudioSection = new QHBoxLayout();
        AudioSection->setObjectName("AudioSection");
        AudioLabel = new QLabel(WindowPicker);
        AudioLabel->setObjectName("AudioLabel");
        sizePolicy.setHeightForWidth(AudioLabel->sizePolicy().hasHeightForWidth());
        AudioLabel->setSizePolicy(sizePolicy);
        AudioLabel->setMinimumSize(QSize(70, 0));
        QFont font1;
        font1.setPointSize(10);
        AudioLabel->setFont(font1);

        AudioSection->addWidget(AudioLabel);

        AudioCheckbox = new QCheckBox(WindowPicker);
        AudioCheckbox->setObjectName("AudioCheckbox");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(AudioCheckbox->sizePolicy().hasHeightForWidth());
        AudioCheckbox->setSizePolicy(sizePolicy2);
        AudioCheckbox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        AudioCheckbox->setStyleSheet(QString::fromUtf8("margin-left:50%; margin-right:50%;"));
        AudioCheckbox->setTristate(false);

        AudioSection->addWidget(AudioCheckbox);

        AudioFormatLabel = new QLabel(WindowPicker);
        AudioFormatLabel->setObjectName("AudioFormatLabel");
        sizePolicy1.setHeightForWidth(AudioFormatLabel->sizePolicy().hasHeightForWidth());
        AudioFormatLabel->setSizePolicy(sizePolicy1);
        AudioFormatLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        AudioSection->addWidget(AudioFormatLabel);


        verticalLayout->addLayout(AudioSection);

        LandscapeSection = new QHBoxLayout();
        LandscapeSection->setObjectName("LandscapeSection");
        LandscapeLabel = new QLabel(WindowPicker);
        LandscapeLabel->setObjectName("LandscapeLabel");
        sizePolicy.setHeightForWidth(LandscapeLabel->sizePolicy().hasHeightForWidth());
        LandscapeLabel->setSizePolicy(sizePolicy);
        LandscapeLabel->setMinimumSize(QSize(70, 0));
        LandscapeLabel->setFont(font1);

        LandscapeSection->addWidget(LandscapeLabel);

        LandscapeCheckbox = new QCheckBox(WindowPicker);
        LandscapeCheckbox->setObjectName("LandscapeCheckbox");
        sizePolicy2.setHeightForWidth(LandscapeCheckbox->sizePolicy().hasHeightForWidth());
        LandscapeCheckbox->setSizePolicy(sizePolicy2);
        LandscapeCheckbox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        LandscapeCheckbox->setStyleSheet(QString::fromUtf8("margin-left:50%; margin-right:50%;"));
        LandscapeCheckbox->setTristate(false);

        LandscapeSection->addWidget(LandscapeCheckbox);

        LandscapeFormatLabel = new QLabel(WindowPicker);
        LandscapeFormatLabel->setObjectName("LandscapeFormatLabel");
        sizePolicy1.setHeightForWidth(LandscapeFormatLabel->sizePolicy().hasHeightForWidth());
        LandscapeFormatLabel->setSizePolicy(sizePolicy1);
        LandscapeFormatLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        LandscapeSection->addWidget(LandscapeFormatLabel);


        verticalLayout->addLayout(LandscapeSection);

        LevelSection = new QHBoxLayout();
        LevelSection->setObjectName("LevelSection");
        LevelLabel = new QLabel(WindowPicker);
        LevelLabel->setObjectName("LevelLabel");
        sizePolicy.setHeightForWidth(LevelLabel->sizePolicy().hasHeightForWidth());
        LevelLabel->setSizePolicy(sizePolicy);
        LevelLabel->setMinimumSize(QSize(70, 0));
        LevelLabel->setFont(font1);

        LevelSection->addWidget(LevelLabel);

        LevelCheckbox = new QCheckBox(WindowPicker);
        LevelCheckbox->setObjectName("LevelCheckbox");
        sizePolicy2.setHeightForWidth(LevelCheckbox->sizePolicy().hasHeightForWidth());
        LevelCheckbox->setSizePolicy(sizePolicy2);
        LevelCheckbox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        LevelCheckbox->setStyleSheet(QString::fromUtf8("margin-left:50%; margin-right:50%;"));
        LevelCheckbox->setTristate(false);

        LevelSection->addWidget(LevelCheckbox);

        LevelFormatLabel = new QLabel(WindowPicker);
        LevelFormatLabel->setObjectName("LevelFormatLabel");
        LevelFormatLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        LevelSection->addWidget(LevelFormatLabel);


        verticalLayout->addLayout(LevelSection);

        MaterialSection = new QHBoxLayout();
        MaterialSection->setObjectName("MaterialSection");
        MaterialLabel = new QLabel(WindowPicker);
        MaterialLabel->setObjectName("MaterialLabel");
        sizePolicy.setHeightForWidth(MaterialLabel->sizePolicy().hasHeightForWidth());
        MaterialLabel->setSizePolicy(sizePolicy);
        MaterialLabel->setFont(font1);

        MaterialSection->addWidget(MaterialLabel);

        MaterialCheckbox = new QCheckBox(WindowPicker);
        MaterialCheckbox->setObjectName("MaterialCheckbox");
        sizePolicy2.setHeightForWidth(MaterialCheckbox->sizePolicy().hasHeightForWidth());
        MaterialCheckbox->setSizePolicy(sizePolicy2);
        MaterialCheckbox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        MaterialCheckbox->setStyleSheet(QString::fromUtf8("margin-left:50%; margin-right:50%;"));
        MaterialCheckbox->setTristate(false);

        MaterialSection->addWidget(MaterialCheckbox);

        MaterialFormatLabel = new QLabel(WindowPicker);
        MaterialFormatLabel->setObjectName("MaterialFormatLabel");
        MaterialFormatLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        MaterialSection->addWidget(MaterialFormatLabel);


        verticalLayout->addLayout(MaterialSection);

        ModelSection = new QHBoxLayout();
        ModelSection->setObjectName("ModelSection");
        ModelLabel = new QLabel(WindowPicker);
        ModelLabel->setObjectName("ModelLabel");
        ModelLabel->setFont(font1);

        ModelSection->addWidget(ModelLabel);

        ModelCheckbox = new QCheckBox(WindowPicker);
        ModelCheckbox->setObjectName("ModelCheckbox");
        sizePolicy2.setHeightForWidth(ModelCheckbox->sizePolicy().hasHeightForWidth());
        ModelCheckbox->setSizePolicy(sizePolicy2);
        ModelCheckbox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        ModelCheckbox->setStyleSheet(QString::fromUtf8("margin-left:50%; margin-right:50%;"));
        ModelCheckbox->setTristate(false);

        ModelSection->addWidget(ModelCheckbox);

        ModelFormatLabel = new QLabel(WindowPicker);
        ModelFormatLabel->setObjectName("ModelFormatLabel");
        ModelFormatLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        ModelSection->addWidget(ModelFormatLabel);


        verticalLayout->addLayout(ModelSection);

        ShaderSecion = new QHBoxLayout();
        ShaderSecion->setObjectName("ShaderSecion");
        ShaderLabel = new QLabel(WindowPicker);
        ShaderLabel->setObjectName("ShaderLabel");
        ShaderLabel->setFont(font1);

        ShaderSecion->addWidget(ShaderLabel);

        ShaderCheckbox = new QCheckBox(WindowPicker);
        ShaderCheckbox->setObjectName("ShaderCheckbox");
        sizePolicy2.setHeightForWidth(ShaderCheckbox->sizePolicy().hasHeightForWidth());
        ShaderCheckbox->setSizePolicy(sizePolicy2);
        ShaderCheckbox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        ShaderCheckbox->setStyleSheet(QString::fromUtf8("margin-left:50%; margin-right:50%;"));
        ShaderCheckbox->setTristate(false);

        ShaderSecion->addWidget(ShaderCheckbox);

        ShaderFormatCombo = new QComboBox(WindowPicker);
        ShaderFormatCombo->setObjectName("ShaderFormatCombo");

        ShaderSecion->addWidget(ShaderFormatCombo);


        verticalLayout->addLayout(ShaderSecion);

        TextureSection = new QHBoxLayout();
        TextureSection->setObjectName("TextureSection");
        TextureLabel = new QLabel(WindowPicker);
        TextureLabel->setObjectName("TextureLabel");
        TextureLabel->setFont(font1);

        TextureSection->addWidget(TextureLabel);

        TextureCheckbox = new QCheckBox(WindowPicker);
        TextureCheckbox->setObjectName("TextureCheckbox");
        sizePolicy2.setHeightForWidth(TextureCheckbox->sizePolicy().hasHeightForWidth());
        TextureCheckbox->setSizePolicy(sizePolicy2);
        TextureCheckbox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        TextureCheckbox->setStyleSheet(QString::fromUtf8("margin-left:50%; margin-right:50%;"));
        TextureCheckbox->setTristate(false);

        TextureSection->addWidget(TextureCheckbox);

        TextureFormatCombo = new QComboBox(WindowPicker);
        TextureFormatCombo->setObjectName("TextureFormatCombo");

        TextureSection->addWidget(TextureFormatCombo);


        verticalLayout->addLayout(TextureSection);


        horizontalLayout_5->addLayout(verticalLayout);

        horizontalSpacer_3 = new QSpacerItem(70, 13, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_5);

        BottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(BottomSpacer);

        LoadButtonFrame = new QFrame(WindowPicker);
        LoadButtonFrame->setObjectName("LoadButtonFrame");
        LoadButtonFrame->setMaximumSize(QSize(16777215, 60));
        LoadButtonFrame->setFrameShape(QFrame::Shape::NoFrame);
        LoadButtonFrame->setFrameShadow(QFrame::Shadow::Raised);
        LoadButtonFrame->setLineWidth(0);
        horizontalLayout_3 = new QHBoxLayout(LoadButtonFrame);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        VersionLabel = new QLabel(LoadButtonFrame);
        VersionLabel->setObjectName("VersionLabel");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Ignored);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(VersionLabel->sizePolicy().hasHeightForWidth());
        VersionLabel->setSizePolicy(sizePolicy3);
        QFont font2;
        font2.setPointSize(8);
        VersionLabel->setFont(font2);
        VersionLabel->setTextFormat(Qt::TextFormat::PlainText);

        horizontalLayout_3->addWidget(VersionLabel);

        LoadButton = new QPushButton(LoadButtonFrame);
        LoadButton->setObjectName("LoadButton");
        LoadButton->setMinimumSize(QSize(100, 0));
        LoadButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_3->addWidget(LoadButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        AdditionalSettingsButton = new QToolButton(LoadButtonFrame);
        AdditionalSettingsButton->setObjectName("AdditionalSettingsButton");
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("preferences-other")));
        AdditionalSettingsButton->setIcon(icon);

        horizontalLayout_2->addWidget(AdditionalSettingsButton);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(LoadButtonFrame);


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
        TitleType->setText(QCoreApplication::translate("WindowPicker", "Type", nullptr));
        TitleExtract->setText(QCoreApplication::translate("WindowPicker", "Extract", nullptr));
        TitleOutputFormat->setText(QCoreApplication::translate("WindowPicker", "Output Format", nullptr));
#if QT_CONFIG(whatsthis)
        AudioLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Audio/Music files</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        AudioLabel->setText(QCoreApplication::translate("WindowPicker", "Audio", nullptr));
        AudioCheckbox->setText(QString());
#if QT_CONFIG(whatsthis)
        AudioFormatLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Audio files are stored as both of these formats in-game, which determins the export format.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        AudioFormatLabel->setText(QCoreApplication::translate("WindowPicker", "Wav/Opus", nullptr));
#if QT_CONFIG(whatsthis)
        LandscapeLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Landscape meshes for Levels. These are massive models.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        LandscapeLabel->setText(QCoreApplication::translate("WindowPicker", "Landscape", nullptr));
        LandscapeCheckbox->setText(QString());
#if QT_CONFIG(whatsthis)
        LandscapeFormatLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Audio files are stored as both of these formats in-game, which determins the export format.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        LandscapeFormatLabel->setText(QCoreApplication::translate("WindowPicker", "glTF", nullptr));
#if QT_CONFIG(whatsthis)
        LevelLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Rooms and open areas. <span style=\" font-weight:700;\">Exported files are large.</span></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        LevelLabel->setText(QCoreApplication::translate("WindowPicker", "Level", nullptr));
        LevelCheckbox->setText(QString());
#if QT_CONFIG(whatsthis)
        LevelFormatLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>A 3D model format supported by most programs.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        LevelFormatLabel->setText(QCoreApplication::translate("WindowPicker", "glTF", nullptr));
#if QT_CONFIG(whatsthis)
        MaterialLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Material parameters like textures used and default colors.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        MaterialLabel->setText(QCoreApplication::translate("WindowPicker", "Material", nullptr));
        MaterialCheckbox->setText(QString());
#if QT_CONFIG(whatsthis)
        MaterialFormatLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Regular text data</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        MaterialFormatLabel->setText(QCoreApplication::translate("WindowPicker", "Txt", nullptr));
#if QT_CONFIG(whatsthis)
        ModelLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Static and rigged models.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        ModelLabel->setText(QCoreApplication::translate("WindowPicker", "Model", nullptr));
        ModelCheckbox->setText(QString());
#if QT_CONFIG(whatsthis)
        ModelFormatLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>A 3D model format supported by most programs.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        ModelFormatLabel->setText(QCoreApplication::translate("WindowPicker", "glTF", nullptr));
#if QT_CONFIG(whatsthis)
        ShaderLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>High Level Shading Language shaders. These should only be extracted if you understand how to interpret decompiled HLSL code.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        ShaderLabel->setText(QCoreApplication::translate("WindowPicker", "Shader", nullptr));
        ShaderCheckbox->setText(QString());
#if QT_CONFIG(whatsthis)
        ShaderFormatCombo->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p><span style=\" font-weight:700;\">Decompiled:</span> Uses the function D3DDecompile to produce a text-based output.</p><p><span style=\" font-weight:700;\">Bytecode:</span> Just the raw binary data. You can decompile this yourself.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(whatsthis)
        TextureLabel->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Textures used by models</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        TextureLabel->setText(QCoreApplication::translate("WindowPicker", "Texture", nullptr));
        TextureCheckbox->setText(QString());
#if QT_CONFIG(whatsthis)
        TextureFormatCombo->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p><span style=\" font-weight:700;\">DDS:</span> The raw texture. Some compressed formats (like BC7) may not be supported by all programs. This is the fastest option.</p><p><span style=\" font-weight:700;\">PNG:</span> Uses a PNG library to encode the texture. This will take more time but exported textures will be universally compatable.</p><p><span style=\" font-weight:700;\">TGA:</span> Faster than PNG but much larger file sizes.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(whatsthis)
        LoadButtonFrame->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        VersionLabel->setText(QString());
#if QT_CONFIG(whatsthis)
        LoadButton->setWhatsThis(QCoreApplication::translate("WindowPicker", "<html><head/><body><p>Load the main window and begin browsing the internal files.</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        LoadButton->setText(QCoreApplication::translate("WindowPicker", "Load", nullptr));
        AdditionalSettingsButton->setText(QCoreApplication::translate("WindowPicker", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WindowPicker: public Ui_WindowPicker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PICKERDLUUOI_H
