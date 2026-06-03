#include "CTextWatermark.h"
#include <QApplication>
#include <QWindow>
#include <QDebug>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QScreen>
#else
#include <QDesktopWidget>
#endif
#include <QFontDatabase>
#include <QPaintDevice>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QStringList>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <cmath>

CTextWatermark::CTextWatermark(QWidget *parent)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QRect rect = QGuiApplication::primaryScreen()->availableGeometry();
#else
    QRect rect = QApplication::desktop()->availableGeometry();
#endif
    this->setFixedSize(rect.size());
    this->setWindowTitle(QString::fromLocal8Bit("mywatermark"));
    this->setAttribute(Qt::WA_ShowWithoutActivating);       // 展示时不激活
    this->setAttribute(Qt::WA_TranslucentBackground, true); // 透明窗口
    this->setFocusPolicy(Qt::NoFocus);                      // 无焦点
    this->setWindowModality(Qt::NonModal);                  // 非模态
    this->setVisible(false);                                // 默认不显示
    this->setWindowFlags(windowFlags() 
        | Qt::X11BypassWindowManagerHint 
        | Qt::WindowStaysOnTopHint);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    qInfo() << "QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)";
    this->setWindowFlags(windowFlags() 
        | Qt::FramelessWindowHint 
        | Qt::WindowDoesNotAcceptFocus 
        | Qt::Tool 
        | Qt::WindowTransparentForInput);

#else
    qInfo() << "QT_VERSION < QT_VERSION_CHECK(5, 0, 0)";
    this->setWindowFlags(windowFlags() 
        | Qt::FramelessWindowHint 
        | Qt::Tool);
#endif

// loongarch? qt5 5.11.3
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    qInfo() << "QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)";
    if (this->windowHandle())
        this->windowHandle()->setWindowStates(Qt::WindowNoState | Qt::WindowFullScreen);
#endif
}

CTextWatermark::~CTextWatermark()
{
}

bool CTextWatermark::InitWaterMark(const QString &strContent)
{
    // 检查输入参数有效性
    if (strContent.isEmpty())
    {
        qWarning() << "Watermark content is empty!";
        return false;
    }

    // 当前窗口的宽度和高度, 单位：像素
    float iDeviceWidth = rect().width();
    float iDeviceHeight = rect().height();

    // 创建 QPixmap 对象
    QPixmap pixMap = QPixmap(iDeviceWidth, iDeviceHeight);

    // 创建 QPainter 对象
    QPainter painter(&pixMap);
    if (!painter.isActive())
    {
        qWarning() << "Failed to activate painter!";
        return false;
    }

    // 设置绘制参数
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    painter.setPen(WM_TEXT_COLOR);
    painter.setFont(WM_TEXT_FONT);

    // 计算文本尺寸和位置（居中显示）
    QFontMetrics fm(painter.font());
    int textWidth = fm.width(strContent);
    int textHeight = fm.height();

    // 计算单个水印+水印间距占用的像素大小（像素点）
    float DataRectWidth = textWidth + WM_TEXT_COL_SPACE;
    float DataRectHeight = textHeight + WM_TEXT_ROW_SPACE;

    // 计算每个水印矩形的离散排列
    int iCountRow = static_cast<int>(std::ceil(iDeviceHeight / DataRectHeight));
    int iCountCol = static_cast<int>(std::ceil(iDeviceWidth / DataRectWidth));

    // 边界偏移
    int iOffsetX = static_cast<int>(WM_TEXT_COL_SPACE / 4);
    int iOffsetY = static_cast<int>(WM_TEXT_ROW_SPACE / 4);
    for (int i = 0; i < iCountRow; i++)
    {
        for (int j = 0; j < iCountCol; j++)
        {
            QPoint point(j * DataRectWidth + iOffsetX, i * DataRectHeight + iOffsetY);
            painter.drawText(point.x(), point.y(), strContent);
        }
    }

    // 结束绘制并验证
    painter.end();

    // 更新水印图案
    m_pixMap.swap(pixMap);

    return true;
}

void CTextWatermark::SetTransparencyMode(TransparencyMode mode, float transparency)
{
    m_transparencyMode = mode;
    m_transparency = transparency;
    qInfo() << "SetTransparencyMode: mode=" << static_cast<int>(mode) << " transparency=" << transparency;
}

void CTextWatermark::paintEvent(QPaintEvent *event)
{
    // 根据模式设置透明度
    switch (m_transparencyMode)
    {
    case TransparencyMode::WindowOpacity:
        // 方式1: 使用 setWindowOpacity - 这会影响系统合成器的透明度处理
        setWindowOpacity(m_transparency);
        break;
    case TransparencyMode::PainterOpacity:
        // 方式2: 使用 QPainter::setOpacity - 仅影响绘制内容，不影响窗口属性
        // setWindowOpacity(1.0);  // 确保窗口本身不透明
        break;
    case TransparencyMode::NoOpacity:
        // 方式3: 不设置透明度
        // setWindowOpacity(1.0);
        break;
    }

    QPainter paint(this);
    paint.setCompositionMode(QPainter::CompositionMode_Source);

    // 如果使用 Painter 模式，在绘制时设置透明度
    if (m_transparencyMode == TransparencyMode::PainterOpacity)
    {
        paint.setOpacity(m_transparency);
    }

    paint.drawPixmap(rect(), m_pixMap);
    QWidget::paintEvent(event);
}