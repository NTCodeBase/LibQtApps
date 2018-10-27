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

#include <QtWidgets>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class EnhancedMessageBox : public QMessageBox {
    Q_OBJECT

public:
    EnhancedMessageBox(QWidget* parent = nullptr) : QMessageBox(parent) {}
    void         setAutoClose(bool bAutoClose) { m_bAutoClose = bAutoClose; }
    void         setAutoCloseTimeout(int timeOut) { m_TimeOut = timeOut; }
    void         setCountDownTime(int countDownTime) { m_CountDownTime = countDownTime; }
    virtual void showEvent(QShowEvent* ev) {
        QMessageBox::showEvent(ev);
        if(m_bAutoClose) {
            m_ShowedTime = 0;
            connect(&m_Timer, &QTimer::timeout, [&] {
                        m_ShowedTime += m_CountDownTime;
                        if(m_ShowedTime >= m_TimeOut) {
                            close();
                        }
                        setWindowTitle(QString("Closing in %1 sec.....").arg((m_TimeOut - m_ShowedTime) / 1000));
                    });
            setWindowTitle(QString("Closing in %1 sec.....").arg((m_TimeOut - m_ShowedTime) / 1000));
            m_Timer.start(m_CountDownTime);
        }
    }

private:
    bool   m_bAutoClose    = true;
    int    m_TimeOut       = 2000;
    int    m_ShowedTime    = 0;
    int    m_CountDownTime = 1000;
    QTimer m_Timer;
};
