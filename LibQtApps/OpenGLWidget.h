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

#include <LibCommon/CommonSetup.h>
#include <LibOpenGL/Forward.h>
#include <LibOpenGL/LightAndMaterialData.h>

#include <LibQtApps/Forward.h>
#include <LibQtApps/QtAppMacros.h>
#include <LibQtApps/QtAppUtils.h>

#include <QtGui>
#include <QtWidgets>
#include <QSurfaceFormat>
#include <QDir>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
namespace NTCodeBase {
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class OpenGLWidget : public QOpenGLWidget, public OpenGLFunctions {
    Q_OBJECT
    friend class OpenGLMainWindow;

public:
    OpenGLWidget(QWidget* parent);

    const auto& getClearColor() const { return m_ClearColor; }
    auto& getCamera() const { return m_Camera; }

    ////////////////////////////////////////////////////////////////////////////////
    // => QWidget interface
public:
    virtual QSize sizeHint() const override { return m_DefaultSize; }
    virtual QSize minimumSizeHint() const override { return QSize(10, 10); }
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // => mouse and key handling
protected:
    enum class MouseButton { NoButton, LeftButton, MiddleButton, RightButton };
    virtual void mousePressEvent(QMouseEvent* ev) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override { m_MouseButtonPressed = MouseButton::NoButton; }
    virtual void mouseMoveEvent(QMouseEvent* ev) override;
    virtual void wheelEvent(QWheelEvent* ev) override;
    virtual void showEvent(QShowEvent*) override { setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); }

public:
    enum class SpecialKey { NoKey, ShiftKey, CtrlKey, AltKey };
    virtual void keyPressEvent(QKeyEvent* ev) override;
    virtual void keyReleaseEvent(QKeyEvent*) override { m_SpecialKeyPressed = SpecialKey::NoKey; }
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // => QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    ////////////////////////////////////////////////////////////////////////////////

    // => protected members of class OpenGLWidget
protected:
    virtual void initOpenGL() = 0;
    virtual void resizeOpenGLWindow(int w, int h) = 0;
    virtual void renderOpenGL() = 0;

    void uploadCameraData();
    void checkGLErrors();
    void checkGLVersion();
    void checkGLExtensions(const QVector<QString>& extensions);

    ////////////////////////////////////////////////////////////////////////////////
#ifdef __NT_DEBUG__
    bool m_bPrintDebug = true;
#else
    bool m_bPrintDebug = false;
#endif
    QSize       m_DefaultSize        = QtAppUtils::getDefaultWindowSize();
    Vec3f       m_ClearColor         = QtAppUtils::getDefaultClearColor();
    SpecialKey  m_SpecialKeyPressed  = SpecialKey::NoKey;
    MouseButton m_MouseButtonPressed = MouseButton::NoButton;
    QString     m_CapturePath        = QtAppUtils::getDefaultCapturePath();

    SharedPtr<FPSCounter> m_FPSCounter = nullptr;

    bool                    m_bLockCamera    = false;
    SharedPtr<QTimer>       m_UpdateTimer    = nullptr;
    SharedPtr<OpenGLBuffer> m_UBufferCamData = nullptr;
    SharedPtr<Camera>       m_Camera         = nullptr;

signals:
    void emitDebugString(const QString& str);
    void cameraPositionInfoChanged(const Vec3f& cameraPosition, const Vec3f& cameraFocus);

public slots:
    void printDebugString(const QString& str) { if(m_bPrintDebug) { qDebug() << str; } }
    void setCapturePath(const QString& path);
    void setPrintDebug(bool pdebug) { m_bPrintDebug = pdebug; }
    void setDefaultSize(QSize size) { m_DefaultSize = size; }
    void setClearColor(const Vec3f& color);
    void resetClearColor() { glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], 1.0f); }
    void setViewFrustum(float fov, float nearZ, float farZ);
    void setCamera(const Vec3f& cameraPosition, const Vec3f& cameraFocus);
    void setCamera(const std::pair<Vec3f, Vec3f>& cameraInfo);
    void resetCameraPosition();
    void lockCamera(bool bLocked) { m_bLockCamera = bLocked; }
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // capture image
    void setCaptureImageExtension(const QString& ext) { m_CaptureImageExt = ext; }
    bool exportScreenToImage(int frame);
protected:
    QString           m_CaptureImageExt = QString("png");
    SharedPtr<QImage> m_CaptureImage    = nullptr;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // clip plane
public:
    bool isClipPlaneEnabled() const { return m_bEnabledClipPlane; }
signals:
    void clipPlaneEnabled(bool bEnable);
public slots:
    void enableClipPlane(bool bEnable);
    void setClipPlane(const Vec4f& clipPlane) { m_ClipPlane = clipPlane; }
    void showClipPlaneEditor();

protected:
    bool             m_bEnabledClipPlane = false;
    ClipPlaneEditor* m_ClipPlaneEditor   = nullptr;
    Vec4f            m_ClipPlane         = Vec4f(1.0f, 0.0f, 0.0f, 0.0f);
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // light
signals:
    void lightsObjChanged(const SharedPtr<PointLights>& lights);
public slots:
    void setLights(const StdVT<PointLightData>& lightData);
    void updateLights();
protected:
    void initRDataLight();
    void renderLight();
    SharedPtr<PointLights>      m_Lights      = nullptr;
    SharedPtr<PointLightRender> m_LightRender = nullptr;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // sky box and checkerboard/grid background =>
public:
    enum BackgroundMode {
        SkyBox = 0,
        Color,
        Checkerboard,
        Grid,
        NumBackgroundMode
    };
public slots:
    void setBackgroundMode(int backgroundMode) { m_BackgroundMode = static_cast<BackgroundMode>(backgroundMode); }
    void setSkyBoxTextureIndex(int texIndex);
    void setCheckerboarrdColor1(const Vec3f& color);
    void setCheckerboarrdColor2(const Vec3f& color);
    void setCheckerboarrdScales(const Vec2i& scales);
    void setGridBackgroundColor(const Vec3f& color);
    void setGridLineColor(const Vec3f& color);
    void setGridScales(const Vec2i& scales);
    void reloadSkyboxTextures();
protected:
    void initRDataSkyBox();
    void initRDataCheckerboardBackground();
    void initRDataGridBackground();
    void renderSkyBox();
    void renderCheckerboardBackground();
    void renderGridBackground();
    SharedPtr<SkyBoxRender>                 m_SkyBoxRender       = nullptr;
    SharedPtr<CheckerboardBackgroundRender> m_CheckerboardRender = nullptr;
    SharedPtr<GridBackgroundRender>         m_GridRender         = nullptr;
    BackgroundMode m_BackgroundMode = BackgroundMode::SkyBox;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // floor =>
public slots:
    void setFloorTextureIndex(int texIndex);
    void setFloorExposure(int percentage);
    void setFloorSize(int size);
    void setFloorTexScales(int scale);
    void setFloorHeightX100(int height);
    void setFloorHeight(float height);
    void reloadFloorTextures();
protected:
    void initRDataFloor();
    void renderFloor();
    SharedPtr<PlaneRender> m_FloorRender = nullptr;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // box =>
public slots:
    void enableRenderBox(bool bRender) { m_bRenderBox = bRender; }
    void setBoxColor(const Vec3f& color);
    void setBox(const Vec3f& boxMin, const Vec3f& boxMax);
protected:
    void initRDataBox();
    void renderBox();
    SharedPtr<WireFrameBoxRender> m_DomainBoxRender = nullptr;
    bool m_bRenderBox = true;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // gizmo =>
public slots:
    void enableRenderGizmo(bool bRender) { m_RDataGizmo.bRenderGizmo = bRender; }
    void setGizmoWidgetSize(int widgetSize) { m_RDataGizmo.widgetSize = widgetSize; }
    void setGizmoWidthRatio(float widthRatio) { m_RDataGizmo.widthRatio = widthRatio; }
protected:
    struct RDataGizmo {
        SharedPtr<ShaderProgram> shader = nullptr;
        GLuint                   VAO;
        GLuint                   ub_CamData;
        int                      widgetSize   = 100;
        float                    widthRatio   = 0.1f;
        bool                     bRenderGizmo = true;
        bool                     bInitialized = false;
    } m_RDataGizmo;
    void initRDataGizmo();
    void renderGizmo();
    ////////////////////////////////////////////////////////////////////////////////
};
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
} // end namespace NTCodeBase
