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

#include <LibCommon/CommonSetup.h>
#include <LibQtApps/QtAppUtils.h>

#include <QtGui>
#include <QtWidgets>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    ColorPicker(QWidget* parent = 0, QSize size = QSize(50, 25));

    void setColor(QColor color);
    void setColor(float r, float g, float b) { setColor(QtAppUtils::floatToQColor(r, g, b)); }
    void setColor(const Vec3f& color) { setColor(QtAppUtils::floatToQColor(color)); }
    void setColor(const Vec4f& color) { setColor(QtAppUtils::floatToQColor(Vec3f(color.x, color.y, color.z))); }

    auto  getQColor() const { return m_CurrentColor; }
    auto  getColor() const { return Vec3f(m_CurrentColor.redF(), m_CurrentColor.greenF(), m_CurrentColor.blueF()); }
    void  paintEvent(QPaintEvent* e);
    void  setDefaultSize(QSize size);
    QSize sizeHint() const;

    QLayout*   getLayout(const QString& label   = QString(""), int labelStretch = 5);
    QGroupBox* getGroupBox(const QString& title = QString(""), const QString& label = QString(""));

signals:
    void colorChanged(float r, float g, float b);
    void colorRGBChanged(const QColor& color);

protected:
    void mousePressEvent(QMouseEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);

private:
    QSize        m_DefaultSize;
    QColor       m_CurrentColor;
    QGroupBox*   m_GroupBox;
    QGridLayout* m_Layout;
};
