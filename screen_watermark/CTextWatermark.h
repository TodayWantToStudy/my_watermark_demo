#ifndef TEXT_WATERMARK_HPP
#define TEXT_WATERMARK_HPP

#include <QWidget>
class CTextWatermark : public QWidget
{
    Q_OBJECT
public:
    explicit CTextWatermark(QWidget *parent = nullptr);
    ~CTextWatermark();
    bool InitWaterMark(const QString &strContent);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pixMap;
    QRect m_rectDest;
};

const float WM_TRANSPARENCY = 0.6f;
const QColor WM_TEXT_COLOR(0, 0, 0, 255);
const QFont WM_TEXT_FONT("Arial", 12, QFont::Bold);
const int WM_TEXT_ROW_SPACE = 100;
const int WM_TEXT_COL_SPACE = 100;

#endif