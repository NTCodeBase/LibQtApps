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

#include <LibQtApps/EnhancedComboBox.h>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
namespace NTCodeBase {
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
EnhancedComboBox::EnhancedComboBox(int comboBoxSpan /*= 3*/, QWidget* parent /*= nullptr*/) : QWidget(parent) {
    m_ComboBox = new QComboBox;
    connect(m_ComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),           [&](int idx) { emit currentIndexChanged(idx); });
    connect(m_ComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), [&](const QString& str) { emit currentTextChanged(str); });

    m_btnPrev = new QToolButton;
    m_btnPrev->setArrowType(Qt::LeftArrow);
    m_btnNext = new QToolButton;
    m_btnNext->setArrowType(Qt::RightArrow);

    connect(m_btnPrev, SIGNAL(clicked()), this, SLOT(prevItem()));
    connect(m_btnNext, SIGNAL(clicked()), this, SLOT(nextItem()));

    m_Layout = new QGridLayout;
    m_Layout->addWidget(m_ComboBox, 0, 0,                1, comboBoxSpan);
    m_Layout->addWidget(m_btnPrev,  0, comboBoxSpan,     1, 1);
    m_Layout->addWidget(m_btnNext,  0, comboBoxSpan + 1, 1, 1);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
QLayout* EnhancedComboBox::getLayoutWithLabel(const QString& label, int comboStretch /*= 5*/) {
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(new QLabel(label), 1);
    layout->addLayout(m_Layout, comboStretch);
    return layout;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
QGroupBox* EnhancedComboBox::getGroupBox(const QString& title) {
    if(m_GroupBox == nullptr) {
        m_GroupBox = new QGroupBox(title);
        m_GroupBox->setLayout(m_Layout);
    }
    return m_GroupBox;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void EnhancedComboBox::setEnabled(bool bEnable) {
    m_ComboBox->setEnabled(bEnable);
    m_btnPrev->setEnabled(bEnable);
    m_btnNext->setEnabled(bEnable);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void EnhancedComboBox::setDisabled(bool bDisable) {
    m_ComboBox->setDisabled(bDisable);
    m_btnPrev->setDisabled(bDisable);
    m_btnNext->setDisabled(bDisable);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void EnhancedComboBox::prevItem() {
    int currentIndex = m_ComboBox->currentIndex();

    if(currentIndex > 0) {
        m_ComboBox->setCurrentIndex(currentIndex - 1);
    } else if(m_enabledCycling) {
        m_ComboBox->setCurrentIndex(m_ComboBox->count() - 1);
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void EnhancedComboBox::nextItem() {
    int currentIndex = m_ComboBox->currentIndex();
    if(currentIndex < m_ComboBox->count() - 1) {
        m_ComboBox->setCurrentIndex(currentIndex + 1);
    } else if(m_enabledCycling) {
        m_ComboBox->setCurrentIndex(0);
    }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
} // end namespace NTCodeBase
