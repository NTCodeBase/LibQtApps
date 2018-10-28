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

#include <LibOpenGL/Camera.h>
#include <LibOpenGL/Lights.h>
#include <LibOpenGL/OpenGLBuffer.h>
#include <LibOpenGL/ShaderProgram.h>
#include <LibOpenGL/RenderObjects.h>

#include <LibQtApps/ClipPlaneEditor.h>
#include <LibQtApps/FPSCounter.h>
#include <LibQtApps/OpenGLWidget.h>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent), m_FPSCounter(std::make_shared<FPSCounter>()), m_Camera(std::make_shared<Camera>()) {
    m_ClipPlaneEditor = new ClipPlaneEditor;
    connect(m_ClipPlaneEditor,   &ClipPlaneEditor::clipPlaneChanged, this, &OpenGLWidget::setClipPlane);
    ////////////////////////////////////////////////////////////////////////////////
    m_UpdateTimer = std::make_shared<QTimer>(this);
    connect(m_UpdateTimer.get(), SIGNAL(timeout()),                  this, SLOT(update()));
    m_UpdateTimer->start(0);
    ////////////////////////////////////////////////////////////////////////////////
    connect(this, &OpenGLWidget::emitDebugString, this, &OpenGLWidget::printDebugString);
    ////////////////////////////////////////////////////////////////////////////////
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::mousePressEvent(QMouseEvent* ev) {
    if(ev->button() == Qt::LeftButton) {
        m_MouseButtonPressed = MouseButton::LeftButton;
    } else if(ev->button() == Qt::RightButton) {
        m_MouseButtonPressed = MouseButton::RightButton;
    } else {
        m_MouseButtonPressed = MouseButton::NoButton;
    }
    ////////////////////////////////////////////////////////////////////////////////
    if(!m_bLockCamera) {
        m_Camera->set_last_mouse_pos(ev->x(), ev->y());
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::mouseMoveEvent(QMouseEvent* ev) {
    if(m_bLockCamera) {
        return;
    }
    if(m_MouseButtonPressed == MouseButton::LeftButton) {
        m_Camera->rotate_by_mouse(ev->x(), ev->y());
    } else if(m_MouseButtonPressed == MouseButton::RightButton) {
        if(m_SpecialKeyPressed == SpecialKey::NoKey) {
            m_Camera->translate_by_mouse(ev->x(), ev->y());
        } else {
            m_Camera->zoom_by_mouse(ev->x(), ev->y());
        }
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::wheelEvent(QWheelEvent* ev) {
    if(m_bLockCamera) {
        return;
    }
    if(ev->angleDelta().isNull()) {
        return;
    }
    float zoomFactor = (ev->angleDelta().x() + ev->angleDelta().y()) / 5000.0f;
    m_Camera->zoom(zoomFactor);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::keyPressEvent(QKeyEvent* ev) {
    switch(ev->key()) {
        case Qt::Key_Shift:
            m_SpecialKeyPressed = SpecialKey::ShiftKey;
            break;

        case Qt::Key_Plus:
            if(!m_bLockCamera) {
                m_Camera->zoom(-0.01f);
            }
            break;

        case Qt::Key_Minus:
            if(!m_bLockCamera) {
                m_Camera->zoom(0.01f);
            }
            break;

        case Qt::Key_Up:
            if(!m_bLockCamera) {
                m_Camera->translate(Vec2f(0.0f, 0.1f));
            }
            break;

        case Qt::Key_Down:
            if(!m_bLockCamera) {
                m_Camera->translate(Vec2f(0.0f, -0.1f));
            }
            break;

        case Qt::Key_Left:
            if(!m_bLockCamera) {
                m_Camera->translate(Vec2f(-0.1f, 0.0f));
            }
            break;

        case Qt::Key_Right:
            if(!m_bLockCamera) {
                m_Camera->translate(Vec2f(0.1f, 0.0f));
            }
            break;

        case Qt::Key_C:
            if(!m_bLockCamera) {
                m_Camera->reset();
            }
            break;

        default:
            ;
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    checkGLVersion();
    checkGLErrors();
    resetClearColor();
    glCall(glEnable(GL_DEPTH_TEST));
    glCall(glEnable(GL_MULTISAMPLE));
    ////////////////////////////////////////////////////////////////////////////////
    m_CaptureImage = std::make_shared<QImage>(width(), height(), QImage::Format_RGB888);
    ////////////////////////////////////////////////////////////////////////////////
    // view matrix, prj matrix, inverse view matrix, inverse proj matrix, shadow matrix, cam position
    m_UBufferCamData = std::make_shared<OpenGLBuffer>();
    m_UBufferCamData->createBuffer(GL_UNIFORM_BUFFER, 5 * sizeof(Mat4x4f) + 2 * sizeof(Vec4f), nullptr, GL_DYNAMIC_DRAW);
    emit cameraPositionInfoChanged(m_Camera->getCameraPosition(), m_Camera->getCameraFocus());
    ////////////////////////////////////////////////////////////////////////////////
    initRDataLight();
    initRDataSkyBox();
    initRDataCheckerboardBackground();
    initRDataGridBackground();
    initRDataFloor();
    initRDataBox();
    initRDataGizmo();
    ////////////////////////////////////////////////////////////////////////////////
    // call init function from derived class
    initOpenGL();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::resizeGL(int w, int h) {
    glCall(glViewport(0, 0, w, h));
    m_Camera->resizeWindow((float)w, (float)h);
    m_CaptureImage.reset(new QImage(w, h, QImage::Format_RGB888));
    ////////////////////////////////////////////////////////////////////////////////
    if(m_CheckerboardRender != nullptr) {
        m_CheckerboardRender->setScreenSize(w, h);
    }
    if(m_GridRender != nullptr) {
        m_GridRender->setScreenSize(w, h);
    }
    ////////////////////////////////////////////////////////////////////////////////
    // call resize function from derived class
    resizeOpenGLWindow(w, h);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::paintGL() {
    m_FPSCounter->countFrame();
    ////////////////////////////////////////////////////////////////////////////////
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    uploadCameraData();
    ////////////////////////////////////////////////////////////////////////////////
    switch(m_BackgroundMode) {
        case BackgroundMode::SkyBox:
            renderSkyBox();
            break;
        case BackgroundMode::Checkerboard:
            renderCheckerboardBackground();
            break;
        case BackgroundMode::Grid:
            renderGridBackground();
            break;
        case BackgroundMode::Color:
        default:
            ;
    }
    renderLight();
    renderFloor();
    renderBox();
    renderGizmo();
    ////////////////////////////////////////////////////////////////////////////////
    // call render function from derived class
    glCall(glViewport(0, 0, width(), height()));
    renderOpenGL();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::uploadCameraData() {
    m_Camera->updateCameraMatrices();
    if(m_Camera->isCameraChanged()) {
        m_UBufferCamData->uploadData(glm::value_ptr(m_Camera->getViewMatrix()),       0,               sizeof(Mat4x4f));
        m_UBufferCamData->uploadData(glm::value_ptr(m_Camera->getProjectionMatrix()), sizeof(Mat4x4f), sizeof(Mat4x4f));
        if(m_Camera->useInverseViewMatrix()) {
            m_UBufferCamData->uploadData(glm::value_ptr(m_Camera->getInverseViewMatrix()), 2 * sizeof(Mat4x4f), sizeof(Mat4x4f));
        }
        if(m_Camera->useInverseProjectionMatrix()) {
            m_UBufferCamData->uploadData(glm::value_ptr(m_Camera->getInverseProjectionMatrix()), 3 * sizeof(Mat4x4f), sizeof(Mat4x4f));
        }
        m_UBufferCamData->uploadData(glm::value_ptr(m_Camera->getCameraPosition()), 5 * sizeof(Mat4x4f),                 sizeof(Vec3f));
        m_UBufferCamData->uploadData(glm::value_ptr(m_Camera->getCameraFocus()),    5 * sizeof(Mat4x4f) + sizeof(Vec4f), sizeof(Vec3f));
        ////////////////////////////////////////////////////////////////////////////////
        emit cameraPositionInfoChanged(m_Camera->getCameraPosition(), m_Camera->getCameraFocus());
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::checkGLErrors() {
    glCall(GLenum glStatus = glGetError());
    if(glStatus == GL_NO_ERROR) {
        return;
    }

    QString glErrStr = "OpenGL error: ";

    switch(glStatus) {
        case GL_INVALID_ENUM:
            glErrStr += "Invalid enum.";
            break;

        case GL_INVALID_VALUE:
            glErrStr += "Invalid value.";
            break;

        case GL_INVALID_OPERATION:
            glErrStr += "Invalid operation.";
            break;

        case GL_STACK_OVERFLOW:
            glErrStr += "Stack overflow.";
            break;

        case GL_STACK_UNDERFLOW:
            glErrStr += "Stack underflow.";
            break;

        case GL_OUT_OF_MEMORY:
            glErrStr += "Out of memory.";
            break;

        default:
            glErrStr += "Unknown error.";
            break;
    }

    emit emitDebugString(glErrStr);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::checkGLVersion() {
    glCall(QString deviceStr = QString((const char*)glGetString(GL_RENDERER)));
    glCall(QString verStr    = QString((const char*)glGetString(GL_VERSION)));
    emit emitDebugString(QString("GPU: ") + deviceStr);
    emit emitDebugString(QString("OpenGL driver: ") + verStr);

    int major = verStr.left(verStr.indexOf(".")).toInt();
    int minor = verStr.mid(verStr.indexOf(".") + 1, 1).toInt();

    if(!(major >= 4 && minor >= 1)) {
        QMessageBox msgBox(QMessageBox::Critical, "Error", QString("Your OpenGL version is %1.%2 (Required: OpenGL >= 4.1).").arg(major).arg(minor));
        msgBox.exec();
        __NT_RAISE_TERMINATION_SIGNAL
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::checkGLExtensions(const QVector<QString>& extensions) {
    glCall(QString extStr = QString((const char*)glGetString(GL_EXTENSIONS)));
    emit emitDebugString(extStr);

    bool check = true;
    for(QString ext : extensions) {
        if(!extStr.contains(ext)) {
            QMessageBox msgBox(QMessageBox::Critical, "Error", QString("Extension %1 is not supported.").arg(ext));
            msgBox.exec();
            check = false;
        }
    }

    if(!check) {
        __NT_RAISE_TERMINATION_SIGNAL
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::setCapturePath(const QString& path) {
    m_CapturePath = path;
    if(!QDir(path).exists()) {
        QDir().mkdir(path);
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::setClearColor(const Vec3f& color) {
    m_ClearColor = color;
    if(isValid()) {
        makeCurrent();
        resetClearColor();
        doneCurrent();
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::setViewFrustum(float fov, float nearZ, float farZ) {
    m_Camera->setFrustum(fov, nearZ, farZ);
}

void OpenGLWidget::setCamera(const Vec3f& cameraPosition, const Vec3f& cameraFocus) {
    m_Camera->setCamera(cameraPosition, cameraFocus, Vec3f(0, 1, 0));
}

void OpenGLWidget::setCamera(const std::pair<Vec3f, Vec3f>& cameraInfo) {
    m_Camera->setCamera(cameraInfo.first, cameraInfo.second, Vec3f(0, 1, 0));
}

void OpenGLWidget::resetCameraPosition() {
    m_Camera->reset();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
bool OpenGLWidget::exportScreenToImage(int frame) {
    if(m_CapturePath.isEmpty()) {
        return false;
    }
    ////////////////////////////////////////////////////////////////////////////////
    makeCurrent();
    glCall(glReadPixels(0, 0, width(), height(), GL_RGB, GL_UNSIGNED_BYTE, m_CaptureImage->bits()));
    doneCurrent();
    m_CaptureImage->mirrored().save(QString(m_CapturePath + "/frame.%1.%2").arg(frame, 4, 10, QChar('0')).arg(m_CaptureImageExt));
    ////////////////////////////////////////////////////////////////////////////////
    return true;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::enableClipPlane(bool bEnable) {
    if(isValid()) {
        m_bEnabledClipPlane = bEnable;
        makeCurrent();
        if(bEnable) {
            glCall(glEnable(GL_CLIP_PLANE0));
        } else {
            glCall(glDisable(GL_CLIP_PLANE0));
        }
        doneCurrent();
        emit clipPlaneEnabled(m_bEnabledClipPlane);
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::showClipPlaneEditor() {
    m_ClipPlaneEditor->show();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::initRDataLight() {
    m_Lights = std::make_shared<PointLights>();
    m_Lights->setNumLights(2);
    m_Lights->setLightPosition(Vec4f(-10.0f, 20.0f, 10.0f, 1.0f),  0);
    m_Lights->setLightPosition(Vec4f(10.0f, -20.0f, -10.0f, 1.0f), 1);
    ////////////////////////////////////////////////////////////////////////////////
    m_Lights->setSceneCenter(Vec3f(0, 0, 0));
    m_Lights->setLightViewPerspective(30);
    m_Lights->uploadDataToGPU();
    ////////////////////////////////////////////////////////////////////////////////
    m_LightRender = std::make_shared<PointLightRender>(m_Camera, m_Lights, m_UBufferCamData);
    emit lightsObjChanged(m_Lights);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::renderLight() {
    Q_ASSERT(m_LightRender != nullptr);
    m_LightRender->render();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::setLights(const StdVT<PointLightData>& lightData) {
    m_Lights->setNumLights(static_cast<int>(lightData.size()));
    for(int i = 0, iend = static_cast<int>(lightData.size()); i < iend; ++i) {
        m_Lights->setLight(lightData[i], i);
    }
    updateLights();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::updateLights() {
    if(isValid()) {
        makeCurrent();
        m_Lights->uploadDataToGPU();
        doneCurrent();
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::initRDataSkyBox() {
    Q_ASSERT(m_UBufferCamData != nullptr);
    m_SkyBoxRender = std::make_shared<SkyBoxRender>(m_Camera, QtAppUtils::getTexturePath() + "/Sky/", m_UBufferCamData);
}

void OpenGLWidget::initRDataCheckerboardBackground() {
    m_CheckerboardRender = std::make_shared<CheckerboardBackgroundRender>();
}

void OpenGLWidget::initRDataGridBackground() {
    m_GridRender = std::make_shared<GridBackgroundRender>();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::setSkyBoxTextureIndex(int texIndex) {
    Q_ASSERT(m_SkyBoxRender != nullptr);
    m_SkyBoxRender->setRenderTextureIndex(texIndex);
}

void OpenGLWidget::setCheckerboarrdColor1(const Vec3f& color) {
    Q_ASSERT(m_CheckerboardRender != nullptr);
    m_CheckerboardRender->setColor1(color);
}

void OpenGLWidget::setCheckerboarrdColor2(const Vec3f& color) {
    Q_ASSERT(m_CheckerboardRender != nullptr);
    m_CheckerboardRender->setColor2(color);
}

void OpenGLWidget::setCheckerboarrdScales(const Vec2i& scales) {
    Q_ASSERT(m_CheckerboardRender != nullptr);
    m_CheckerboardRender->setScales(scales);
}

void OpenGLWidget::setGridBackgroundColor(const Vec3f& color) {
    Q_ASSERT(m_GridRender != nullptr);
    m_GridRender->setBackgroundColor(color);
}

void OpenGLWidget::setGridLineColor(const Vec3f& color) {
    Q_ASSERT(m_GridRender != nullptr);
    m_GridRender->setLineColor(color);
}

void OpenGLWidget::setGridScales(const Vec2i& scales) {
    Q_ASSERT(m_GridRender != nullptr);
    m_GridRender->setScales(scales);
}

void OpenGLWidget::reloadSkyboxTextures() {
    m_SkyBoxRender->clearTextures();
    m_SkyBoxRender->loadTextures(QtAppUtils::getTexturePath() + "/Sky/");
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::renderSkyBox() {
    Q_ASSERT(m_SkyBoxRender != nullptr);
    m_SkyBoxRender->render();
}

void OpenGLWidget::renderCheckerboardBackground() {
    Q_ASSERT(m_CheckerboardRender != nullptr);
    m_CheckerboardRender->render();
}

void OpenGLWidget::renderGridBackground() {
    Q_ASSERT(m_GridRender != nullptr);
    m_GridRender->render();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::initRDataFloor() {
    Q_ASSERT(m_UBufferCamData != nullptr && m_Lights != nullptr);
    m_FloorRender = std::make_shared<PlaneRender>(m_Camera, m_Lights, QtAppUtils::getTexturePath() + "/Floor/", m_UBufferCamData);
    m_FloorRender->setAllowNonTextureRender(false);
    m_FloorRender->setExposure(0.5f);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::setFloorTextureIndex(int texIndex) {
    Q_ASSERT(m_FloorRender != nullptr);
    m_FloorRender->setRenderTextureIndex(texIndex);
}

void OpenGLWidget::setFloorExposure(int percentage) {
    Q_ASSERT(m_FloorRender != nullptr);
    m_FloorRender->setExposure(static_cast<float>(percentage) / 100.0f);
}

void OpenGLWidget::setFloorSize(int size) {
    Q_ASSERT(m_FloorRender != nullptr);
    m_FloorRender->scale(Vec3f(static_cast<float>(size)));
}

void OpenGLWidget::setFloorTexScales(int scale) {
    Q_ASSERT(m_FloorRender != nullptr);
    m_FloorRender->scaleTexCoord(scale, scale);
}

void OpenGLWidget::setFloorHeightX100(int height) {
    Q_ASSERT(m_FloorRender != nullptr);
    m_FloorRender->translate(Vec3f(0, static_cast<float>(height) / 100.0f, 0));
}

void OpenGLWidget::setFloorHeight(float height) {
    Q_ASSERT(m_FloorRender != nullptr);
    m_FloorRender->translate(Vec3f(0, height, 0));
}

void OpenGLWidget::reloadFloorTextures() {
    Q_ASSERT(m_FloorRender != nullptr);
    m_FloorRender->clearTextures();
    m_FloorRender->loadTextures(QtAppUtils::getTexturePath() + "/Floor/");
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::renderFloor() {
    Q_ASSERT(m_FloorRender != nullptr);
    m_FloorRender->render();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::initRDataBox() {
    Q_ASSERT(m_UBufferCamData != nullptr);
    m_DomainBoxRender = std::make_shared<WireFrameBoxRender>(m_Camera, m_UBufferCamData);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::setBoxColor(const Vec3f& color) {
    Q_ASSERT(m_DomainBoxRender != nullptr);
    m_DomainBoxRender->setColor(color);
}

void OpenGLWidget::setBox(const Vec3f& boxMin, const Vec3f& boxMax) {
    Q_ASSERT(m_DomainBoxRender != nullptr);
    if(isValid()) {
        makeCurrent();
        m_DomainBoxRender->setBox(boxMin, boxMax); doneCurrent();
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::renderBox() {
    Q_ASSERT(m_DomainBoxRender != nullptr);
    if(m_bRenderBox) {
        m_DomainBoxRender->render();
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::initRDataGizmo() {
    m_RDataGizmo.shader     = ShaderProgram::getGizmoShader();
    m_RDataGizmo.ub_CamData = m_RDataGizmo.shader->getUniformBlockIndex("CameraData");
    glCall(glGenVertexArrays(1, &m_RDataGizmo.VAO));
    m_RDataGizmo.bInitialized = true;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLWidget::renderGizmo() {
    Q_ASSERT(m_RDataGizmo.bInitialized);
    glCall(glViewport(0, 0, m_RDataGizmo.widgetSize, m_RDataGizmo.widgetSize));
    m_RDataGizmo.shader->bind();
    ////////////////////////////////////////////////////////////////////////////////
    m_UBufferCamData->bindBufferBase();
    m_RDataGizmo.shader->bindUniformBlock(m_RDataGizmo.ub_CamData, m_UBufferCamData->getBindingPoint());
    ////////////////////////////////////////////////////////////////////////////////
    glCall(glBindVertexArray(m_RDataGizmo.VAO));
    glCall(glDrawArrays(GL_TRIANGLES, 0, 108));
    ////////////////////////////////////////////////////////////////////////////////
    glCall(glBindVertexArray(0));
    m_RDataGizmo.shader->release();
}
