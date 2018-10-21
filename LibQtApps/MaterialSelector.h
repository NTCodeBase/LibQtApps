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
#include <LibQtApps/MaterialEditor.h>
#include <LibQtApps/EnhancedComboBox.h>
#include <QtWidgets>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class MaterialSelector : public QWidget
{
    Q_OBJECT

public:
    MaterialSelector(const Material::MaterialData& material = Material::MT_Emerald, bool defaultCustomMaterial = false,
                     int comboBoxSpan                       = 4, QWidget* parent = nullptr);

    void setDefaultCustomMaterial(bool defaultCustomMaterial);
    auto getComboBox() const { return m_ComboBox; }
    auto getLayout() const { return m_Layout; }
    auto getGroupBox(QString title = QString(""));
    auto getNumMaterials() const { return m_ComboBox->count(); }

public slots:
    void setEnabled(bool enabled) { m_ComboBox->setEnabled(enabled); }
    void setMaterial(int materialID);
    void setMaterial(const Material::MaterialData& material);
    void setCustomMaterial(const Material::MaterialData& material);
    void setDebug(bool bDebug) { m_bDebug = bDebug; }

signals:
    void materialChanged(const Material::MaterialData& material);

private:
    QColor floatToQColor(float r, float g, float b) { return QColor(static_cast<int>(255 * r), static_cast<int>(255 * g), static_cast<int>(255 * b)); }
    QColor floatToQColor(const Vec3f& color) { return floatToQColor(color.x, color.y, color.z); }
    Vec3f  QColorToFloat(const QColor& color) { return Vec3f(color.redF(), color.greenF(), color.blueF()); }

    bool                                m_bDebug;
    EnhancedComboBox*                   m_ComboBox;
    QGroupBox*                          m_GroupBox;
    QGridLayout*                        m_Layout;
    MaterialColorPicker*                m_MaterialColorPicker;
    Material::MaterialData              m_CurrentMaterial;
    Material::MaterialData              m_CustomMaterial;
    std::vector<Material::MaterialData> m_Materials;
};
