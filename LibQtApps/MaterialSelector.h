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

#include <LibOpenGL/Material.h>
#include <LibQtApps/Forward.h>
#include <QtWidgets>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class MaterialSelector : public QWidget {
    Q_OBJECT

public:
    MaterialSelector(const MaterialData& material = BuildInMaterials::MT_Emerald, bool defaultCustomMaterial = false,
                     int comboBoxSpan = 4, QWidget* parent = nullptr);

    void setDefaultCustomMaterial(bool defaultCustomMaterial);
    auto getComboBox() const { return m_ComboBox; }
    auto getLayout() const { return m_Layout; }
    auto getGroupBox(QString title = QString(""));
    int  getNumMaterials() const;

public slots:
    void setEnabled(bool enabled);
    void setMaterial(int materialID);
    void setMaterial(const MaterialData& material);
    void setCustomMaterial(const MaterialData& material);
    void setDebug(bool bDebug) { m_bDebug = bDebug; }

signals:
    void materialChanged(const MaterialData& material);

private:
    QColor floatToQColor(float r, float g, float b) { return QColor(static_cast<int>(255 * r), static_cast<int>(255 * g), static_cast<int>(255 * b)); }
    QColor floatToQColor(const Vec3f& color) { return floatToQColor(color.x, color.y, color.z); }
    Vec3f  QColorToFloat(const QColor& color) { return Vec3f(color.redF(), color.greenF(), color.blueF()); }

    bool                      m_bDebug   = false;
    EnhancedComboBox*         m_ComboBox = nullptr;
    QGroupBox*                m_GroupBox = nullptr;
    QGridLayout*              m_Layout   = nullptr;
    MaterialColorPicker*      m_MaterialColorPicker;
    MaterialData              m_CurrentMaterial;
    MaterialData              m_CustomMaterial;
    std::vector<MaterialData> m_Materials;
};
