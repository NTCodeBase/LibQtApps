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
#include <LibQtApps/Forward.h>
#include <QtWidgets>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
namespace NTCodeBase {
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class OpenGLController : public QWidget {
    Q_OBJECT
    friend class OpenGLMainWindow;
public:
    explicit OpenGLController(OpenGLWidget* renderWidget,
                              QWidget*      parent,
                              int           width = 350,
                              bool          bShowBackgroundControllers = true,
                              bool          bShowFloorControllers      = true,
                              bool          bShowBoxControllers        = true);

public slots:
    void showBackgroundControllers(bool bShow) { m_grBackgroundCtrl->setVisible(bShow); }
    void showFloorControllers(bool bShow) { m_grFloorCtrl->setVisible(bShow); }
    void showBoxControllers(bool bShow) { m_grBoxCtrl->setVisible(bShow); }
    void reloadTextures() { loadSkyBoxTextures(); loadFloorTextures(); }
    void loadSkyBoxTextures();
    void loadFloorTextures();

protected:
    void setupBasicGUI(int width);
    void connectBasicWidgets();

    ////////////////////////////////////////////////////////////////////////////////
    // background
    void setupBackgroundControllers();
    bool              m_bShowBackgroundControllers = true;
    EnhancedComboBox* m_cbSkyTexture;
    QSignalMapper*    m_smBackgroundMode;
    EnhancedSlider*   m_sldCheckerboardScale;
    ColorPicker*      m_pkrCheckerColor1;
    ColorPicker*      m_pkrCheckerColor2;
    EnhancedSlider*   m_sldGridScale;
    ColorPicker*      m_pkrGridBackgroundColor;
    ColorPicker*      m_pkrGridLineColor;
    QGroupBox*        m_grBackgroundCtrl;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // floor
    void setupFloorControllers();
    bool              m_bShowFloorControllers;
    EnhancedComboBox* m_cbFloorTexture;
    EnhancedSlider*   m_sldFloorSize;
    EnhancedSlider*   m_sldFloorY;
    EnhancedSlider*   m_sldFloorExposure;
    ColorPicker*      m_pkrBackgroundColor;
    QGroupBox*        m_grFloorCtrl;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // box
    void setupBoxControllers();
    bool         m_bShowBoxControllers = true;
    QCheckBox*   m_chkRenderBox;
    ColorPicker* m_pkrBoxColor;
    QGroupBox*   m_grBoxCtrl;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // buttons
    void setupButtons();
    int          m_nButtonRows;
    QGridLayout* m_LayoutButtons;
    QPushButton* m_btnResetCamera;
    QPushButton* m_btnLockCamera;
    QPushButton* m_btnClipViewPlane;
    QPushButton* m_btnEditClipPlane;
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    OpenGLWidget*     m_GLWidget                 = nullptr;
    QTabWidget*       m_MainTab                  = new QTabWidget;
    QVBoxLayout*      m_MainLayout               = new QVBoxLayout;
    PointLightEditor* m_LightEditor              = nullptr;
    QWidget*          m_RenderControllers        = new QWidget;
    QWidget*          m_TextureControllers       = new QWidget;
    QVBoxLayout*      m_LayoutMainControllers    = new QVBoxLayout;
    QVBoxLayout*      m_LayoutTextureControllers = new QVBoxLayout;
};
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
} // end namespace NTCodeBase
