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

#include <LibOpenGL/LightAndMaterialData.h>
#include <LibQtApps/Forward.h>
#include <QtWidgets>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class MaterialEditor : public QDialog {
    Q_OBJECT

public:
    MaterialEditor(const MaterialData& material = BuildInMaterials::MT_Emerald, QWidget* parent = nullptr);
    void setMaterial(const MaterialData& material);
    const auto& getMaterial() const { return m_CurrentMaterial; }

signals:
    void materialChanged(const MaterialData material);

private:
    void setupGUI();
    ////////////////////////////////////////////////////////////////////////////////
    ColorPicker* m_AmbientColorPicker;
    ColorPicker* m_DiffuseColorPicker;
    ColorPicker* m_SpecularColorPicker;
    QLineEdit*   m_txtShininess;
    MaterialData m_CurrentMaterial;
};

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class MaterialColorPicker : public QWidget {
    Q_OBJECT

public:
    MaterialColorPicker(QWidget* parent = nullptr);
    void setMaterial(const MaterialData& material);
    void setWidgetColor(const MaterialData& material);
    void paintEvent(QPaintEvent* e);
    QSize sizeHint() const { return QSize(30, 25); }

signals:
    void materialChanged(const MaterialData& material);

protected:
    void mousePressEvent(QMouseEvent*) { m_MaterialEditor->show(); }
    void enterEvent(QEvent*) { QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor)); }
    void leaveEvent(QEvent*) { QApplication::restoreOverrideCursor(); }

private:
    MaterialEditor* m_MaterialEditor;
};
