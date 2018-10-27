//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//    .--------------------------------------------------.
//    |  This file is part of NTCodeBase                 |
//    |  Created 2018 by NT (https://ttnghia.github.io)  |
//    '--------------------------------------------------'
//                            \o/
//                             |
//                            / |
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#pragma once

#include <LibOpenGL/Lights.h>
#include <LibQtApps/Forward.h>

#include <QtWidgets>
#include <vector>
#include <memory>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define MAX_POINT_LIGHT 4

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class PointLightEditor : public QWidget {
    Q_OBJECT

public:
    PointLightEditor(SharedPtr<PointLights> lights = nullptr, QWidget* parent = nullptr);

public slots:
    void changeLights(const StdVT<PointLights::PointLightData>& lightData);
    void setLightObject(const SharedPtr<PointLights>& lights);
    void lightToGUI();

signals:
    void lightsChanged();

private:
    void connectComponents();
    void applyLights();

    QCheckBox*   m_CheckBoxes[MAX_POINT_LIGHT];
    QLineEdit*   m_LightAmbients[MAX_POINT_LIGHT][3];
    QLineEdit*   m_LightDiffuses[MAX_POINT_LIGHT][3];
    QLineEdit*   m_LightSpeculars[MAX_POINT_LIGHT][3];
    QLineEdit*   m_LightPositions[MAX_POINT_LIGHT][3];
    ColorPicker* m_ColorSelectors[MAX_POINT_LIGHT][3];

    SharedPtr<PointLights> m_Lights = nullptr;
};
