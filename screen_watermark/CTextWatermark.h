#ifndef TEXT_WATERMARK_HPP
#define TEXT_WATERMARK_HPP

#include <QWidget>

// 透明度设置模式
enum class TransparencyMode
{
    WindowOpacity,   // 使用 setWindowOpacity() - 影响系统合成
    PainterOpacity,  // 使用 QPainter::setOpacity() - 仅影响绘制
    NoOpacity        // 不设置透明度
};

class CTextWatermark : public QWidget
{
    Q_OBJECT
public:
    explicit CTextWatermark(QWidget *parent = nullptr);
    ~CTextWatermark();
    bool InitWaterMark(const QString &strContent);

    // 设置透明度模式
    void SetTransparencyMode(TransparencyMode mode, float transparency = 0.6f);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pixMap;
    QRect m_rectDest;
    TransparencyMode m_transparencyMode = TransparencyMode::WindowOpacity;
    float m_transparency = 0.6f;
};

const float WM_TRANSPARENCY = 0.6f;
const QColor WM_TEXT_COLOR(0, 0, 0, 255);
const QFont WM_TEXT_FONT("Arial", 12, QFont::Bold);
const int WM_TEXT_ROW_SPACE = 100;
const int WM_TEXT_COL_SPACE = 100;

#endif