#ifndef WIDGETCAMERAPARAMETER_GUI_H
#define WIDGETCAMERAPARAMETER_GUI_H


#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QToolBox *toolBox;
    QWidget *pageInfo;
    QGridLayout *gridLayoutPageInfo;
    QGridLayout *gridLayout;
    QLabel *labelCameraSerial;
    QLabel *labelCameraName;
    QLabel *labelCameraSerialText;
    QLabel *labelCameraNameText;
    QWidget *pageExtInfo;
    QGridLayout *gridLayoutPageExtInfo;
    QListWidget *listWidgetCameraInfo;
    QSpacerItem *verticalSpacer;
    QWidget *pageProperties;
    QGridLayout *gridLayoutPageProperties;
    QLabel *labelExposureText;
    QSlider *horizontalSliderExposure;
    QDoubleSpinBox *doubleSpinBoxExposure;
    QLabel *labelGainText;
    QSlider *horizontalSliderGain;
    QDoubleSpinBox *doubleSpinBoxGain;
    QLabel *labelSensorSizeText;
    QHBoxLayout *horizontalLayoutSensorSize;
    QLabel *labelSensorSizeX;
    QLabel *labelSensorSizeY;
    QLabel *label;
    QGridLayout *gridLayoutROI;
    QLabel *labelTextX;
    QSpinBox *spinBoxdX;
    QSpinBox *spinBoxX;
    QLabel *labelTextdX;
    QLabel *labelTextY;
    QLabel *labelTextdY;
    QSpinBox *spinBoxY;
    QSpinBox *spinBoxdY;
    QSpacerItem *verticalSpacerAquProp;
    QPushButton *pushButtonResetROI;


    void setupUi(QWidget *Form)
    {
        // main widget layout
        verticalLayout = new QVBoxLayout(Form);

        // widget with pages
        toolBox = new QToolBox(Form);
        // first page
        pageInfo = new QWidget();
        // layout for content
        gridLayout = new QGridLayout();

        labelCameraSerial = new QLabel(pageInfo);
        gridLayout->addWidget(labelCameraSerial, 1, 1, 1, 2);

        labelCameraName = new QLabel(pageInfo);
        gridLayout->addWidget(labelCameraName, 0, 1, 1, 2);

        labelCameraNameText = new QLabel(pageInfo);
        gridLayout->addWidget(labelCameraNameText, 0, 0, 1, 1);

        labelCameraSerialText = new QLabel(pageInfo);
        gridLayout->addWidget(labelCameraSerialText, 1, 0, 1, 1);

        gridLayoutPageInfo = new QGridLayout(pageInfo);
        gridLayoutPageInfo->addLayout(gridLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        gridLayoutPageInfo->addItem(verticalSpacer, 1, 0, 1, 1);

        // add to toolbox
        toolBox->addItem(pageInfo, QStringLiteral("Camera Identification"));

        // ### Page 2: Extendet info
        pageExtInfo = new QWidget();
        gridLayoutPageExtInfo = new QGridLayout(pageExtInfo);
        listWidgetCameraInfo = new QListWidget(pageExtInfo);
        gridLayoutPageExtInfo->addWidget(listWidgetCameraInfo, 0, 0, 1, 1);

        // add to toolbox
        toolBox->addItem(pageExtInfo, QStringLiteral("Extended Information"));

        // ### Page 3: Aquisition Properties
        pageProperties = new QWidget();
        gridLayoutPageProperties = new QGridLayout(pageProperties);

        labelExposureText = new QLabel(pageProperties);
        gridLayoutPageProperties->addWidget(labelExposureText, 0, 0, 1, 1);

        horizontalSliderExposure = new QSlider(pageProperties);
        horizontalSliderExposure->setOrientation(Qt::Horizontal);
        gridLayoutPageProperties->addWidget(horizontalSliderExposure, 0, 1, 1, 1);

        doubleSpinBoxExposure = new QDoubleSpinBox(pageProperties);
        gridLayoutPageProperties->addWidget(doubleSpinBoxExposure, 0, 2, 1, 1);

        labelGainText = new QLabel(pageProperties);
        gridLayoutPageProperties->addWidget(labelGainText, 1, 0, 1, 1);

        horizontalSliderGain = new QSlider(pageProperties);
        horizontalSliderGain->setOrientation(Qt::Horizontal);
        gridLayoutPageProperties->addWidget(horizontalSliderGain, 1, 1, 1, 1);

        doubleSpinBoxGain = new QDoubleSpinBox(pageProperties);
            QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(doubleSpinBoxGain->sizePolicy().hasHeightForWidth());
            doubleSpinBoxGain->setSizePolicy(sizePolicy);
            doubleSpinBoxGain->setMinimumSize(QSize(100, 0));
        gridLayoutPageProperties->addWidget(doubleSpinBoxGain, 1, 2, 1, 1);

        labelSensorSizeText = new QLabel(pageProperties);
        gridLayoutPageProperties->addWidget(labelSensorSizeText, 2, 0, 1, 1);

        horizontalLayoutSensorSize = new QHBoxLayout();
        labelSensorSizeX = new QLabel(pageProperties);
        horizontalLayoutSensorSize->addWidget(labelSensorSizeX);

        labelSensorSizeY = new QLabel(pageProperties);
        horizontalLayoutSensorSize->addWidget(labelSensorSizeY);

        gridLayoutPageProperties->addLayout(horizontalLayoutSensorSize, 2, 1, 1, 2);

        label = new QLabel(pageProperties);
        gridLayoutPageProperties->addWidget(label, 3, 0, 1, 1);

        gridLayoutROI = new QGridLayout();
        labelTextX = new QLabel(pageProperties);

        gridLayoutROI->addWidget(labelTextX, 0, 0, 1, 1);

        spinBoxdX = new QSpinBox(pageProperties);
        gridLayoutROI->addWidget(spinBoxdX, 0, 3, 1, 1);

        spinBoxX = new QSpinBox(pageProperties);
        gridLayoutROI->addWidget(spinBoxX, 0, 1, 1, 1);

        labelTextdX = new QLabel(pageProperties);
        gridLayoutROI->addWidget(labelTextdX, 0, 2, 1, 1);

        labelTextY = new QLabel(pageProperties);
        gridLayoutROI->addWidget(labelTextY, 1, 0, 1, 1);

        labelTextdY = new QLabel(pageProperties);
        gridLayoutROI->addWidget(labelTextdY, 1, 2, 1, 1);

        spinBoxY = new QSpinBox(pageProperties);
        gridLayoutROI->addWidget(spinBoxY, 1, 1, 1, 1);

        spinBoxdY = new QSpinBox(pageProperties);
        gridLayoutROI->addWidget(spinBoxdY, 1, 3, 1, 1);

        gridLayoutPageProperties->addLayout(gridLayoutROI, 3, 1, 1, 2);

        pushButtonResetROI = new QPushButton(pageProperties);
        gridLayoutROI->addWidget(pushButtonResetROI, 2, 1, 1, 3);

        verticalSpacerAquProp = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayoutPageProperties->addItem(verticalSpacerAquProp, 4, 1, 1, 1);

        toolBox->addItem(pageProperties, QStringLiteral("Aquisition Properties"));
        toolBox->setCurrentIndex(1);


        // ###  add to Widget

        verticalLayout->addWidget(toolBox);


        retranslateUi(Form);

        toolBox->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", nullptr));
        labelCameraSerial->setText(QString());
        labelCameraName->setText(QString());
        labelCameraSerialText->setText(QApplication::translate("Form", "Serial", nullptr));
        labelCameraNameText->setText(QApplication::translate("Form", "Name:", nullptr));
        toolBox->setItemText(toolBox->indexOf(pageInfo), QApplication::translate("Form", "Camera Identification", nullptr));
        toolBox->setItemText(toolBox->indexOf(pageExtInfo), QApplication::translate("Form", "Extended Information", nullptr));
        labelExposureText->setText(QApplication::translate("Form", "Exposure", nullptr));
        labelGainText->setText(QApplication::translate("Form", "Gain", nullptr));
        labelSensorSizeText->setText(QApplication::translate("Form", "Sensor size / px", nullptr));
        labelSensorSizeX->setText(QString());
        labelSensorSizeY->setText(QString());
        label->setText(QApplication::translate("Form", "ROI", nullptr));
        labelTextX->setText(QApplication::translate("Form", "x / px", nullptr));
        labelTextdX->setText(QApplication::translate("Form", "delta X / px", nullptr));
        labelTextY->setText(QApplication::translate("Form", "y / px", nullptr));
        labelTextdY->setText(QApplication::translate("Form", "delta Y / px", nullptr));
        toolBox->setItemText(toolBox->indexOf(pageProperties), QApplication::translate("Form", "Aquisition Properties", nullptr));
        pushButtonResetROI->setText(QApplication::translate("Form", "reset roi to sensor size", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WidgetCameraParameter: public Ui_Form {};
} // namespace Ui


#endif // WIDGETCAMERAPARAMETER_GUI_H
