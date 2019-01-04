﻿//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
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
namespace NTCodeBase {
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class EnhancedComboBox : public QWidget {
    Q_OBJECT

public:
    EnhancedComboBox(int comboBoxSpan = 3, QWidget* parent = nullptr);

    void enableCycling() { m_enabledCycling = true; }
    void disableCycling() { m_enabledCycling = false; }

    QComboBox* getComboBox() const { return m_ComboBox; }
    QLayout* getLayout() const { return m_Layout; }
    QLayout*   getLayoutWithLabel(const QString& label, int comboStretch = 5);
    QGroupBox* getGroupBox(const QString& title = QString(""));

    int count() const { return m_ComboBox->count(); }
    void setCurrentIndex(int index) { m_ComboBox->setCurrentIndex(index); }
    int  currentIndex() const { return m_ComboBox->currentIndex(); }
    QString currentText() const { return m_ComboBox->currentText(); }
    int currentTextAsInt() const { return std::stoi(currentText().toStdString()); }
    unsigned int currentTextAsUInt() const { return static_cast<unsigned int>(std::stoi(currentText().toStdString())); }
    float currentTextAsFloat() const { return std::stof(currentText().toStdString()); }

    void addItem(const QString& item) { m_ComboBox->addItem(item); }
    void addItems(const QStringList& items) { m_ComboBox->addItems(items); }

    void clear() { m_ComboBox->clear(); }

public slots:
    void setEnabled(bool bEnable);
    void setDisabled(bool bDisable);
    void prevItem();
    void nextItem();

signals:
    void currentIndexChanged(int);
    void currentTextChanged(const QString&);

private:
    bool         m_enabledCycling = true;
    QComboBox*   m_ComboBox       = nullptr;
    QGroupBox*   m_GroupBox       = nullptr;
    QGridLayout* m_Layout         = nullptr;
    QToolButton* m_btnPrev        = nullptr;
    QToolButton* m_btnNext        = nullptr;
};
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
} // end namespace NTCodeBase
