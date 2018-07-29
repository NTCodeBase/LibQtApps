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

#include <QtWidgets>
#include <QtGui>
#include <QtAppHelpers/ArthurStyle/arthurstyle.h>
#include <QtAppHelpers/OpenGLWidget.h>


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class OpenGLMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    OpenGLMainWindow(QWidget* parent, bool bShowFPS = true, bool bShowCamPosition = true);

    virtual bool processKeyPressEvent(QKeyEvent* ev);
    virtual bool processKeyReleaseEvent(QKeyEvent*) { return false; }

    bool vSync() const { return m_VSync; }
    bool eventFilter(QObject* obj, QEvent* ev);
    void setArthurStyle();
    void showFPS(bool bShowFPS);
    void showCameraPosition(bool bShowCamPosition);

public slots:
    void updateStatusFrameRate(double fps);
    void updateStatusCameraInfo(const Vec3f& camPosition, const Vec3f& camFocus);

protected:
    void setupOpenglWidget(OpenGLWidget* glWidget);

    ////////////////////////////////////////////////////////////////////////////////
    QLabel*       m_lblStatusFPS        = nullptr;
    QLabel*       m_lblStatusCameraInfo = nullptr;
    OpenGLWidget* m_GLWidget            = nullptr;
    bool          m_VSync               = true;
};

