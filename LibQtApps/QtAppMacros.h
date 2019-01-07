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

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define __NT_QT__

#define OPENGL_MAJOR 4
#define OPENGL_MINOR 1
#include <QFileInfo>
#include <QDir>
#include <QOpenGLFunctions_4_1_Core>
using OpenGLFunctions = QOpenGLFunctions_4_1_Core;

#include <QDebug>
#include <QMessageBox>
#ifdef __NT_ERROR
#   undef __NT_ERROR
#endif
#define __NT_ERROR(err)                                        \
    {                                                          \
        QMessageBox::critical(nullptr, QString("Error"), err); \
    }

#ifdef __NT_INFO
#   undef __NT_INFO
#endif
#define __NT_INFO(info)                                           \
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
#include <LibCommon/Logger/Logger.h>
#define __NTQT_RunMainWindow(MainWindowClass, argc, argv, vsync)                                                              \
    {                                                                                                                         \
        signal(SIGINT,  Logger::signalHandler);                                                                               \
        signal(SIGSEGV, Logger::signalHandler);                                                                               \
        signal(SIGTERM, Logger::signalHandler);                                                                               \
        signal(SIGABRT, Logger::signalHandler);                                                                               \
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
#define __NTQT_PRINT_LINE                                         \
    {                                                             \
        qDebug() << "Line:" << __LINE__ << ", file:" << __FILE__; \
    }

#define __NTQT_PRINT_EXP(x)                                                            \
    {                                                                                  \
        qDebug() << "Printing at line: " << __LINE__ << ", file: " << __FILE__ << ":"; \
        qDebug() << "    " << #x << ": " << x;                                         \
    }

#define __NTQT_PRINT_LOCATION                                       \
    {                                                               \
        qDebug() << "Function: " << __func__;                       \
        qDebug() << "Line: " << __LINE__ << ", file: " << __FILE__; \
    }

#define __NTQT_DIE(errMsg)                                                         \
    {                                                                              \
        qDebug() << "Error occured at line:" << __LINE__ << ", file:" << __FILE__; \
        qDebug() << "Error message:" << errMsg;                                    \
        exit(EXIT_FAILURE);                                                        \
    }

#define __NTQT_ASSERT_MSG(condition, errMsg)                                                 \
    {                                                                                        \
        if(!(condition))                                                                     \
        {                                                                                    \
            qDebug() << "Fatal error occured at line:" << __LINE__ << ", file:" << __FILE__; \
            qDebug() << "Error message:" << errMsg;                                          \
            exit(EXIT_FAILURE);                                                              \
        }                                                                                    \
    }

#define __NTQT_CHECK(condition, errStr)                      \
    {                                                        \
        if(!(condition))                                     \
        {                                                    \
            QMessageBox::information(this, "Error", errStr); \
            return false;                                    \
        }                                                    \
    }
