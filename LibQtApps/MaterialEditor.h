//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//    .--------------------------------------------------.
//    |  This file is part of NTGraphics                 |
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

#include <LibQtApps/ColorPicker.h>
#include <LibQtApps/QtAppUtils.h>

#include <QtWidgets>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class MaterialEditor : public QDialog
{
    Q_OBJECT

public:
    MaterialEditor(const Material::MaterialData& material = Material::MT_Emerald, QWidget* parent = nullptr);

    void setMaterial(const Material::MaterialData& material);
    const auto& getMaterial() const { return m_CurrentMaterial; }

signals:
    void materialChanged(const Material::MaterialData material);

private:
    void setupGUI();

    ColorPicker*           m_AmbientColorPicker;
    ColorPicker*           m_DiffuseColorPicker;
    ColorPicker*           m_SpecularColorPicker;
    QLineEdit*             m_txtShininess;
    Material::MaterialData m_CurrentMaterial;
};

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

class MaterialColorPicker : public QWidget
{
    Q_OBJECT

public:
    MaterialColorPicker(QWidget* parent = nullptr);

    void setMaterial(const Material::MaterialData& material);
    void setWidgetColor(const Material::MaterialData& material);
    void paintEvent(QPaintEvent* e);
    QSize sizeHint() const { return QSize(30, 25); }

signals:
    void materialChanged(const Material::MaterialData& material);

protected:
    void mousePressEvent(QMouseEvent*) { m_MaterialEditor->show(); }
    void enterEvent(QEvent*) { QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor)); }
    void leaveEvent(QEvent*) { QApplication::restoreOverrideCursor(); }

private:
    MaterialEditor* m_MaterialEditor;
};
