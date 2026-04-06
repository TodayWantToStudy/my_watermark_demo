#include "CTextWatermark.h"
#include <QApplication>
#include <QWindow>
#include <QDebug>
#include <QDesktopWidget>
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
    QRect rect = QApplication::desktop()->availableGeometry();
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

void CTextWatermark::paintEvent(QPaintEvent *event)
{
    setWindowOpacity(WM_TRANSPARENCY);

    QPainter paint(this);
    paint.setCompositionMode(QPainter::CompositionMode_Source);
    paint.drawPixmap(rect(), m_pixMap);
    QWidget::paintEvent(event);
}