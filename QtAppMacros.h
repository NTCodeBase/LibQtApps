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

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define __Banana_Qt__

#define OPENGL_MAJOR 4
#define OPENGL_MINOR 1
#include <QOpenGLFunctions_4_1_Core>
using OpenGLFunctions = QOpenGLFunctions_4_1_Core;

#include <QDebug>
#include <QMessageBox>
#ifdef __BNN_ERROR
#   undef __BNN_ERROR
#endif
#define __BNN_ERROR(err)                                       \
    {                                                          \
        QMessageBox::critical(nullptr, QString("Error"), err); \
    }

#ifdef __BNN_INFO
#   undef __BNN_INFO
#endif
#define __BNN_INFO(info)                                          \
    {                                                             \
        QMessageBox::information(nullptr, QString("Info"), info); \
    }

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#if defined(_WIN32) || defined(_WIN64)
#   ifndef NOMINMAX
#      define NOMINMAX
#   endif
#   define __func__ __FUNCTION__
#endif
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define __BNNQt_RunMainWindow(MainWindowClass, argc, argv, vsync)                                                             \
    {                                                                                                                         \
        QSurfaceFormat format;                                                                                                \
        format.setDepthBufferSize(24);                                                                                        \
        format.setStencilBufferSize(8);                                                                                       \
        format.setVersion(OPENGL_MAJOR, OPENGL_MINOR);                                                                        \
        format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);                                                                 \
        format.setProfile(QSurfaceFormat::CoreProfile);                                                                       \
        format.setSwapInterval(vsync ? 1 : 0);                                                                                \
        QSurfaceFormat::setDefaultFormat(format);                                                                             \
        QApplication a(argc, argv);                                                                                           \
        a.setAttribute(Qt::AA_UseDesktopOpenGL);                                                                              \
        MainWindowClass w;                                                                                                    \
        w.show();                                                                                                             \
        w.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w.size(), qApp->desktop()->availableGeometry())); \
        return a.exec();                                                                                                      \
    }

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define __BNNQt_PrintLine                                         \
    {                                                             \
        qDebug() << "Line:" << __LINE__ << ", file:" << __FILE__; \
    }

#define __BNNQt_PrintExp(x)                                                            \
    {                                                                                  \
        qDebug() << "Printing at line: " << __LINE__ << ", file: " << __FILE__ << ":"; \
        qDebug() << "    " << #x << ": " << x;                                         \
    }

#define __BNNQt_PrintLocation                                       \
    {                                                               \
        qDebug() << "Function: " << __func__;                       \
        qDebug() << "Line: " << __LINE__ << ", file: " << __FILE__; \
    }

#define __BNNQt_Die(errMsg)                                                        \
    {                                                                              \
        qDebug() << "Error occured at line:" << __LINE__ << ", file:" << __FILE__; \
        qDebug() << "Error message:" << errMsg;                                    \
        exit(EXIT_FAILURE);                                                        \
    }

#define __BNNQt_AssertMsg(condition, errMsg)                                                 \
    {                                                                                        \
        if(!(condition))                                                                     \
        {                                                                                    \
            qDebug() << "Fatal error occured at line:" << __LINE__ << ", file:" << __FILE__; \
            qDebug() << "Error message:" << errMsg;                                          \
            exit(EXIT_FAILURE);                                                              \
        }                                                                                    \
    }

#define __BNNQt_Check(condition, errStr)                     \
    {                                                        \
        if(!(condition))                                     \
        {                                                    \
            QMessageBox::information(this, "Error", errStr); \
            return false;                                    \
        }                                                    \
    }

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// => enums

enum class SpecialKey
{
    NoKey,
    ShiftKey,
    CtrlKey,
    AltKey
};

enum class MouseButton
{
    NoButton,
    LeftButton,
    MiddleButton,
    RightButton
};

enum class MeshFileType
{
    OBJFile,
    PLYFile,
    UnsupportedFileType
};

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// => structs
struct Frustum
{
    float fov;
    float neard;
    float fard;

    Frustum(float _fov = 45.0f, float _near = 0.1f, float _far = 1000.0f)
    {
        fov   = _fov;
        neard = _near;
        fard  = _far;
    }
};
