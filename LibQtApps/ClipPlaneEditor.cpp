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

#include <LibQtApps/ClipPlaneEditor.h>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
namespace NTCodeBase {
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
ClipPlaneEditor::ClipPlaneEditor(QWidget* parent) : QWidget(parent) {
    setupGUI();
    setWindowTitle("Clip Plane Editor");
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void ClipPlaneEditor::setClipPlane(const Vec4f& clipPlane) {
    m_ClipPlane = clipPlane;
    for(int i = 0; i < 4; ++i) {
        m_txtCoeffs[i]->setText(QString("%1").arg(m_ClipPlane[i]));
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void ClipPlaneEditor::resetPlane() {
    setClipPlane(Vec4f(1, 0, 0, 0));
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void ClipPlaneEditor::setupGUI() {
    QVBoxLayout* loSliders = new QVBoxLayout;
    ////////////////////////////////////////////////////////////////////////////////
    m_txtMaxValue = new QLineEdit(QString("%1").arg(m_MaxValue));
    m_sldMaxValue = new QSlider(Qt::Horizontal);
    m_sldMaxValue->setMinimum(1);
    m_sldMaxValue->setMaximum(100);
    m_sldMaxValue->setValue(10);

    connect(m_txtMaxValue, &QLineEdit::textChanged, this, [&](const QString& value) {
                float fval = std::stof(value.toStdString());
                m_MaxValue = fval;
                m_sldMaxValue->setValue(static_cast<int>(fval * 10.0f));
            });

    connect(m_sldMaxValue, &QSlider::valueChanged, this, [&](int value) {
                m_MaxValue = static_cast<float>(value) / 10.0f;
                m_txtMaxValue->setText(QString("%1").arg(m_MaxValue));
                for(int i = 0; i < 4; ++i) {
                    m_sldCoeffs[i]->blockSignals(true);
                    m_sldCoeffs[i]->setValue(static_cast<int>((m_ClipPlane[i] / m_MaxValue + 1.0f) * 100.0f));
                    m_sldCoeffs[i]->blockSignals(false);
                }
            });

    auto layoutMaxValue = new QHBoxLayout;
    layoutMaxValue->addWidget(new QLabel("Max. value:"), 1);
    layoutMaxValue->addWidget(m_sldMaxValue,             10);
    layoutMaxValue->addWidget(m_txtMaxValue,             1);
    loSliders->addLayout(layoutMaxValue);
    ////////////////////////////////////////////////////////////////////////////////
    QLabel* lblPlaneIntros[4] = { new QLabel("A:"),
                                  new QLabel("B:"),
                                  new QLabel("C:"),
                                  new QLabel("D:") };
    for(int i = 0; i < 4; ++i) {
        m_txtCoeffs[i] = new QLineEdit(QString("%1").arg(m_ClipPlane[i]));
        m_sldCoeffs[i] = new QSlider(Qt::Horizontal);
        m_sldCoeffs[i]->setMaximum(200);
        m_sldCoeffs[i]->setValue((m_ClipPlane[i] / m_MaxValue + 1.0f) * 100.0f);

        connect(m_txtCoeffs[i], &QLineEdit::textChanged, this, [&, i](const QString& value) {
                    float fval     = std::stof(value.toStdString());
                    m_ClipPlane[i] = fval;
                    m_sldCoeffs[i]->blockSignals(true);
                    m_sldCoeffs[i]->setValue(static_cast<int>((fval / m_MaxValue + 1.0f) * 100.0f));
                    m_sldCoeffs[i]->blockSignals(false);
                    emit clipPlaneChanged(m_ClipPlane);
                });

        connect(m_sldCoeffs[i], &QSlider::valueChanged, this, [&, i](int value) {
                    m_ClipPlane[i] = (static_cast<float>(value) / 100.0f - 1.0f) * m_MaxValue;
                    m_txtCoeffs[i]->blockSignals(true);
                    m_txtCoeffs[i]->setText(QString("%1").arg(m_ClipPlane[i]));
                    m_txtCoeffs[i]->blockSignals(false);
                    emit clipPlaneChanged(m_ClipPlane);
                });

        auto loCoeff = new QHBoxLayout;
        loCoeff->addWidget(lblPlaneIntros[i], 1);
        loCoeff->addWidget(m_sldCoeffs[i],    10);
        loCoeff->addWidget(m_txtCoeffs[i],    1);
        loSliders->addLayout(loCoeff);
    }

    QGroupBox* sldGroup = new QGroupBox;
    sldGroup->setLayout(loSliders);
    QPushButton* btnReset = new QPushButton("Reset");
    QPushButton* btnClose = new QPushButton("Close");
    connect(btnReset, &QPushButton::clicked, this, [&]() { resetPlane(); });
    connect(btnClose, &QPushButton::clicked, this, [&]() { hide(); });
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnReset);
    btnLayout->addStretch(1);
    btnLayout->addWidget(btnClose);
    QGroupBox* btnGroup = new QGroupBox;
    btnGroup->setLayout(btnLayout);
    ////////////////////////////////////////////////////////////////////////////////
    QLabel* lblIntro = new QLabel("Set clip plane Ax + By + Cz + D = 0");
    lblIntro->setAlignment(Qt::AlignCenter);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lblIntro);
    mainLayout->addWidget(sldGroup);
    mainLayout->addWidget(btnGroup);
    setLayout(mainLayout);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
} // end namespace NTCodeBase
