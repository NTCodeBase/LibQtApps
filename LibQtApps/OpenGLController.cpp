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

#include <LibOpenGL/RenderObjects.h>
#include <LibQtApps/ColorPicker.h>
#include <LibQtApps/EnhancedComboBox.h>
#include <LibQtApps/EnhancedSlider.h>
#include <LibQtApps/OpenGLWidget.h>
#include <LibQtApps/OpenGLController.h>
#include <LibQtApps/PointLightEditor.h>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
namespace NTCodeBase {
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
OpenGLController::OpenGLController(OpenGLWidget* renderWidget, QWidget* parent, int width /*= 350*/, bool bShowBackgroundControllers /*= true*/,
                                   bool bShowFloorControllers /*= true*/, bool bShowBoxControllers /*= true*/) :
    QWidget(parent), m_GLWidget(renderWidget),
    m_bShowBackgroundControllers(bShowBackgroundControllers),
    m_bShowFloorControllers(bShowFloorControllers),
    m_bShowBoxControllers(bShowBoxControllers) {
    NT_REQUIRE(m_GLWidget != nullptr);
    setupBasicGUI(width);
    connectBasicWidgets();
    if(!m_bShowBackgroundControllers) { m_grBackgroundCtrl->setVisible(false); }
    if(!m_bShowFloorControllers) { m_grFloorCtrl->setVisible(false); }
    if(!m_bShowBoxControllers) { m_grBoxCtrl->setVisible(false); }
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLController::setupBasicGUI(int width) {
    setupBackgroundControllers();
    setupFloorControllers();
    setupBoxControllers();
    ////////////////////////////////////////////////////////////////////////////////
    m_LayoutTextureControllers->addStretch();
    m_RenderControllers->setLayout(m_LayoutMainControllers);
    m_TextureControllers->setLayout(m_LayoutTextureControllers);
    m_LightEditor = new PointLightEditor(nullptr, this);
    ////////////////////////////////////////////////////////////////////////////////
    m_MainTab->setTabPosition(QTabWidget::South);
    m_MainTab->setTabShape(QTabWidget::Triangular);
    m_MainTab->addTab(m_RenderControllers,  "Main Controllers");
    m_MainTab->addTab(m_LightEditor,        "Lights");
    m_MainTab->addTab(m_TextureControllers, "Textures");
    ////////////////////////////////////////////////////////////////////////////////
    m_MainLayout->addWidget(m_MainTab);
    setupButtons();
    setLayout(m_MainLayout);
    ////////////////////////////////////////////////////////////////////////////////
    setFixedWidth(width);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLController::connectBasicWidgets() {
    ////////////////////////////////////////////////////////////////////////////////
    // lights
    connect(m_LightEditor, &PointLightEditor::lightsChanged, m_GLWidget,    &OpenGLWidget::updateLights);
    connect(m_GLWidget,    &OpenGLWidget::lightsObjChanged,  m_LightEditor, &PointLightEditor::setLightObject);
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // background mode
    connect(m_smBackgroundMode, SIGNAL(mapped(int)),                                               m_GLWidget, SLOT(setBackgroundMode(int)));
    connect(m_smBackgroundMode, static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped), [&](int backgroundMode) {
                m_chkRenderBox->setChecked(backgroundMode == OpenGLWidget::BackgroundMode::SkyBox || backgroundMode == OpenGLWidget::BackgroundMode::Color);
            });
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // sky box
    connect(m_cbSkyTexture, &EnhancedComboBox::currentIndexChanged, m_GLWidget, &OpenGLWidget::setSkyBoxTextureIndex);
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // background color
    connect(m_pkrBackgroundColor, &ColorPicker::colorChanged, [&](float r, float g, float b) { m_GLWidget->setClearColor(Vec3f(r, g, b)); });
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // checkerboard background
    connect(m_pkrCheckerColor1,                  &ColorPicker::colorChanged, [&](float r, float g, float b) { m_GLWidget->setCheckerboarrdColor1(Vec3f(r, g, b)); });
    connect(m_pkrCheckerColor2,                  &ColorPicker::colorChanged, [&](float r, float g, float b) { m_GLWidget->setCheckerboarrdColor2(Vec3f(r, g, b)); });
    connect(m_sldCheckerboardScale->getSlider(), &QSlider::valueChanged,     [&](Int value) { m_GLWidget->setCheckerboarrdScales(Vec2i(value)); });
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // grid background
    connect(m_pkrGridBackgroundColor,    &ColorPicker::colorChanged, [&](float r, float g, float b) { m_GLWidget->setGridBackgroundColor(Vec3f(r, g, b)); });
    connect(m_pkrGridLineColor,          &ColorPicker::colorChanged, [&](float r, float g, float b) { m_GLWidget->setGridLineColor(Vec3f(r, g, b)); });
    connect(m_sldGridScale->getSlider(), &QSlider::valueChanged,     [&](Int value) { m_GLWidget->setGridScales(Vec2i(value)); });
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // floor
    connect(m_cbFloorTexture,                &EnhancedComboBox::currentIndexChanged, m_GLWidget, &OpenGLWidget::setFloorTextureIndex);
    connect(m_sldFloorSize->getSlider(),     &QSlider::valueChanged,                 m_GLWidget, &OpenGLWidget::setFloorSize);
    connect(m_sldFloorSize->getSlider(),     &QSlider::valueChanged,                 m_GLWidget, &OpenGLWidget::setFloorTexScales);
    connect(m_sldFloorY->getSlider(),        &QSlider::valueChanged,                 m_GLWidget, &OpenGLWidget::setFloorHeightX100);
    connect(m_sldFloorExposure->getSlider(), &QSlider::valueChanged,                 m_GLWidget, &OpenGLWidget::setFloorExposure);
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // box
    connect(m_chkRenderBox, &QCheckBox::toggled,        m_GLWidget, &OpenGLWidget::enableRenderBox);
    connect(m_pkrBoxColor,  &ColorPicker::colorChanged, [&](float r, float g, float b) { m_GLWidget->setBoxColor(Vec3f(r, g, b)); });
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // buttons
    connect(m_btnResetCamera,   &QPushButton::clicked,           m_GLWidget,         &OpenGLWidget::resetCameraPosition);
    connect(m_btnLockCamera,    &QPushButton::clicked,           m_GLWidget,         &OpenGLWidget::lockCamera);
    connect(m_GLWidget,         &OpenGLWidget::clipPlaneEnabled, m_btnClipViewPlane, &QPushButton::setChecked);
    connect(m_btnEditClipPlane, &QPushButton::clicked,           m_GLWidget,         &OpenGLWidget::showClipPlaneEditor);
    connect(m_btnClipViewPlane, &QPushButton::clicked,           m_GLWidget,         &OpenGLWidget::enableClipPlane);
    connect(m_GLWidget,         &OpenGLWidget::clipPlaneEnabled, [&](bool bEnabled) {
                m_GLWidget->blockSignals(true);
                m_btnClipViewPlane->setChecked(bEnabled);
                m_GLWidget->blockSignals(false);
            });
    ////////////////////////////////////////////////////////////////////////////////
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLController::setupBackgroundControllers() {
    QRadioButton* rdbBackgroundSkyBox       = new QRadioButton("SkyBox");
    QRadioButton* rdbBackgroundColor        = new QRadioButton("Color");
    QRadioButton* rdbBackgroundCheckerboard = new QRadioButton("Checkerboard");
    QRadioButton* rdbBackgroundGrid         = new QRadioButton("Grid");
    rdbBackgroundSkyBox->setChecked(true);

    QGridLayout* layoutBackgroundType = new QGridLayout;
    layoutBackgroundType->addWidget(rdbBackgroundSkyBox,       0, 0, 1, 1);
    layoutBackgroundType->addWidget(rdbBackgroundColor,        0, 1, 1, 1);
    layoutBackgroundType->addWidget(rdbBackgroundCheckerboard, 1, 0, 1, 1);
    layoutBackgroundType->addWidget(rdbBackgroundGrid,         1, 1, 1, 1);
    ////////////////////////////////////////////////////////////////////////////////
    m_cbSkyTexture = new EnhancedComboBox;
    m_cbSkyTexture->getLayout()->setContentsMargins(0, 0, 0, 0);
    QWidget* wSkyTex = new QWidget;
    wSkyTex->setLayout(m_cbSkyTexture->getLayout());
    wSkyTex->setMinimumHeight(25);
    ////////////////////////////////////////////////////////////////////////////////
    m_pkrBackgroundColor = new ColorPicker;
    m_pkrBackgroundColor->setMaximumHeight(25);
    m_pkrBackgroundColor->setVisible(false);
    m_pkrBackgroundColor->setColor(m_GLWidget->getClearColor());
    ////////////////////////////////////////////////////////////////////////////////
    m_sldCheckerboardScale = new EnhancedSlider;
    m_sldCheckerboardScale->setRange(2, 100);
    m_sldCheckerboardScale->setValue(CheckerboardBackgroundRender::getDefaultScales().x);
    m_pkrCheckerColor1 = new ColorPicker;
    m_pkrCheckerColor2 = new ColorPicker;
    m_pkrCheckerColor1->setColor(CheckerboardBackgroundRender::getDefaultColor1());
    m_pkrCheckerColor2->setColor(CheckerboardBackgroundRender::getDefaultColor2());

    QHBoxLayout* layoutCheckerboard = new QHBoxLayout;
    layoutCheckerboard->setContentsMargins(0, 0, 0, 0);
    layoutCheckerboard->addWidget(m_pkrCheckerColor1, 10);
    layoutCheckerboard->addStretch(1);
    layoutCheckerboard->addWidget(m_pkrCheckerColor2, 10);
    layoutCheckerboard->addStretch(1);
    layoutCheckerboard->addLayout(m_sldCheckerboardScale->getLayout(), 40);

    QWidget* wCheckerboard = new QWidget;
    wCheckerboard->setMinimumHeight(25);
    wCheckerboard->setLayout(layoutCheckerboard);
    wCheckerboard->setVisible(false);
    ////////////////////////////////////////////////////////////////////////////////
    m_pkrGridBackgroundColor = new ColorPicker;
    m_pkrGridLineColor       = new ColorPicker;
    m_pkrGridBackgroundColor->setColor(GridBackgroundRender::getDefaultBackgroundColor());
    m_pkrGridLineColor->setColor(GridBackgroundRender::getDefaultLineColor());
    m_sldGridScale = new EnhancedSlider;
    m_sldGridScale->setRange(2, 100);
    m_sldGridScale->setValue(GridBackgroundRender::getDefaultScales().x);

    QHBoxLayout* layoutGrid = new QHBoxLayout;
    layoutGrid->setContentsMargins(0, 0, 0, 0);
    layoutGrid->addWidget(m_pkrGridBackgroundColor, 10);
    layoutGrid->addStretch(1);
    layoutGrid->addWidget(m_pkrGridLineColor, 10);
    layoutGrid->addStretch(1);
    layoutGrid->addLayout(m_sldGridScale->getLayout(), 40);

    QWidget* wGrid = new QWidget;
    wGrid->setLayout(layoutGrid);
    wGrid->setMinimumHeight(25);
    wGrid->setVisible(false);
    ////////////////////////////////////////////////////////////////////////////////
    QVBoxLayout* layoutBackground = new QVBoxLayout;
    layoutBackground->addLayout(layoutBackgroundType);
    layoutBackground->addSpacing(5);
    layoutBackground->addWidget(QtAppUtils::getLineSeparator());
    layoutBackground->addSpacing(5);
    layoutBackground->addWidget(wSkyTex);
    layoutBackground->addWidget(m_pkrBackgroundColor);
    layoutBackground->addWidget(wCheckerboard);
    layoutBackground->addWidget(wGrid);

    connect(rdbBackgroundSkyBox,       &QRadioButton::toggled, wSkyTex,              &QWidget::setVisible);
    connect(rdbBackgroundColor,        &QRadioButton::toggled, m_pkrBackgroundColor, &ColorPicker::setVisible);
    connect(rdbBackgroundCheckerboard, &QRadioButton::toggled, wCheckerboard,        &QWidget::setVisible);
    connect(rdbBackgroundGrid,         &QRadioButton::toggled, wGrid,                &QWidget::setVisible);
    ////////////////////////////////////////////////////////////////////////////////
    m_grBackgroundCtrl = new QGroupBox("Background");
    m_grBackgroundCtrl->setLayout(layoutBackground);
    m_LayoutTextureControllers->addWidget(m_grBackgroundCtrl);
    ////////////////////////////////////////////////////////////////////////////////
    m_smBackgroundMode = new QSignalMapper;
    connect(rdbBackgroundSkyBox,       SIGNAL(clicked()), m_smBackgroundMode, SLOT(map()));
    connect(rdbBackgroundColor,        SIGNAL(clicked()), m_smBackgroundMode, SLOT(map()));
    connect(rdbBackgroundCheckerboard, SIGNAL(clicked()), m_smBackgroundMode, SLOT(map()));
    connect(rdbBackgroundGrid,         SIGNAL(clicked()), m_smBackgroundMode, SLOT(map()));

    m_smBackgroundMode->setMapping(rdbBackgroundSkyBox,       static_cast<int>(OpenGLWidget::BackgroundMode::SkyBox));
    m_smBackgroundMode->setMapping(rdbBackgroundColor,        static_cast<int>(OpenGLWidget::BackgroundMode::Color));
    m_smBackgroundMode->setMapping(rdbBackgroundCheckerboard, static_cast<int>(OpenGLWidget::BackgroundMode::Checkerboard));
    m_smBackgroundMode->setMapping(rdbBackgroundGrid,         static_cast<int>(OpenGLWidget::BackgroundMode::Grid));
    ////////////////////////////////////////////////////////////////////////////////
    loadSkyBoxTextures();
}

void OpenGLController::loadSkyBoxTextures() {
    Int currentSkyTexID = m_cbSkyTexture->currentIndex();
    m_cbSkyTexture->clear();
    m_cbSkyTexture->addItem("None");
    ////////////////////////////////////////////////////////////////////////////////
    auto skyTexFolders = QtAppUtils::getTextureFolders("Sky");
    foreach(const auto& tex, skyTexFolders)
    {
        m_cbSkyTexture->addItem(tex);
    }
    m_cbSkyTexture->setCurrentIndex(currentSkyTexID > 0 ? currentSkyTexID : 0);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLController::setupFloorControllers() {
    m_sldFloorSize = new EnhancedSlider;
    m_sldFloorSize->setRange(1, 100);
    m_sldFloorSize->getSlider()->setValue(10);
    auto layoutFloorSize = new QHBoxLayout;
    layoutFloorSize->addWidget(new QLabel("Size:"), 1);
    layoutFloorSize->addLayout(m_sldFloorSize->getLayout(), 5);
    ////////////////////////////////////////////////////////////////////////////////
    m_sldFloorY = new EnhancedSlider;
    m_sldFloorY->setRange(-100, 100);
    m_sldFloorY->getSlider()->setValue(0);
    auto layoutFloorY = new QHBoxLayout;
    layoutFloorY->addWidget(new QLabel("Y (x100):"), 1);
    layoutFloorY->addLayout(m_sldFloorY->getLayout(), 5);
    ////////////////////////////////////////////////////////////////////////////////
    m_sldFloorExposure = new EnhancedSlider;
    m_sldFloorExposure->setRange(0, 100);
    m_sldFloorExposure->getSlider()->setValue(50);
    auto layoutFloorExposure = new QHBoxLayout;
    layoutFloorExposure->addWidget(new QLabel("Exposure:"), 1);
    layoutFloorExposure->addLayout(m_sldFloorExposure->getLayout(), 5);
    ////////////////////////////////////////////////////////////////////////////////
    auto floorLayout = new QVBoxLayout;
    m_cbFloorTexture = new EnhancedComboBox;
    floorLayout->addLayout(m_cbFloorTexture->getLayout());
    floorLayout->addSpacing(10);
    floorLayout->addWidget(QtAppUtils::getLineSeparator());
    floorLayout->addLayout(layoutFloorSize);
    floorLayout->addLayout(layoutFloorY);
    floorLayout->addLayout(layoutFloorExposure);
    ////////////////////////////////////////////////////////////////////////////////
    m_grFloorCtrl = new QGroupBox("Floor Texture");
    m_grFloorCtrl->setLayout(floorLayout);
    m_LayoutTextureControllers->addWidget(m_grFloorCtrl);
    ////////////////////////////////////////////////////////////////////////////////
    loadFloorTextures();
}

void OpenGLController::loadFloorTextures() {
    Int currentFloorTexID = m_cbFloorTexture->currentIndex();
    m_cbFloorTexture->clear();
    m_cbFloorTexture->addItem("None");
    ////////////////////////////////////////////////////////////////////////////////
    auto floorTexFolders = QtAppUtils::getTextureFiles("Floor");
    foreach(const auto& tex, floorTexFolders)
    {
        m_cbFloorTexture->addItem(tex);
    }
    m_cbFloorTexture->setCurrentIndex(currentFloorTexID > 0 ? currentFloorTexID : 0);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLController::setupBoxControllers() {
    m_chkRenderBox = new QCheckBox("Render box");
    m_pkrBoxColor  = new ColorPicker;
    m_chkRenderBox->setChecked(true);
    m_pkrBoxColor->setMinimumHeight(20);
    m_pkrBoxColor->setColor(WireFrameBoxRender::getDefaultBoxColor());
    connect(m_chkRenderBox, &QCheckBox::toggled, m_pkrBoxColor, &ColorPicker::setEnabled);
    ////////////////////////////////////////////////////////////////////////////////
    QHBoxLayout* layoutBoxCtrl = new QHBoxLayout;
    layoutBoxCtrl->addWidget(m_chkRenderBox);
    layoutBoxCtrl->addWidget(m_pkrBoxColor);
    m_grBoxCtrl = new QGroupBox("Domain Box");
    m_grBoxCtrl->setLayout(layoutBoxCtrl);
    m_LayoutMainControllers->addWidget(m_grBoxCtrl);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void OpenGLController::setupButtons() {
    ////////////////////////////////////////////////////////////////////////////////
    m_btnResetCamera   = new QPushButton("Reset Camera");
    m_btnLockCamera    = new QPushButton("Lock Camera");
    m_btnClipViewPlane = new QPushButton("Clip View");
    m_btnEditClipPlane = new QPushButton("Edit Clip Plane");
    m_btnLockCamera->setCheckable(true);
    m_btnClipViewPlane->setCheckable(true);
    ////////////////////////////////////////////////////////////////////////////////
    m_LayoutButtons = new QGridLayout;
    m_nButtonRows   = 0;
    m_LayoutButtons->addWidget(m_btnResetCamera,   m_nButtonRows,   0, 1, 1);
    m_LayoutButtons->addWidget(m_btnLockCamera,    m_nButtonRows++, 1, 1, 1);
    m_LayoutButtons->addWidget(m_btnClipViewPlane, m_nButtonRows,   0, 1, 1);
    m_LayoutButtons->addWidget(m_btnEditClipPlane, m_nButtonRows++, 1, 1, 1);
    ////////////////////////////////////////////////////////////////////////////////
    m_MainLayout->addStretch();
    m_MainLayout->addLayout(m_LayoutButtons);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
} // end namespace NTCodeBase
