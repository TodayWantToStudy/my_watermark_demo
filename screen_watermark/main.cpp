#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QStringList>
#include "CTextWatermark.h"

void printUsage()
{
    qDebug() << "Usage: screen_watermark [options]";
    qDebug() << "Options:";
    qDebug() << "  --mode-opacity     使用 setWindowOpacity() 设置透明度 (默认)";
    qDebug() << "  --mode-painter     使用 QPainter::setOpacity() 设置透明度";
    qDebug() << "  --mode-none        不设置透明度";
    qDebug() << "  --transparency N   设置透明度值 (0.0-1.0, 默认0.6)";
    qDebug() << "  --text TEXT        设置水印文本";
    qDebug() << "  --help             显示帮助信息";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 解析命令行参数
    TransparencyMode mode = TransparencyMode::WindowOpacity;  // 默认使用 setWindowOpacity
    float transparency = 0.6f;
    QString text = QString::fromLocal8Bit("水印文案 水印文案 水印文案 水印文案");

    QStringList args = app.arguments();
    for (int i = 1; i < args.size(); i++)
    {
        if (args[i] == "--mode-opacity")
        {
            mode = TransparencyMode::WindowOpacity;
        }
        else if (args[i] == "--mode-painter")
        {
            mode = TransparencyMode::PainterOpacity;
        }
        else if (args[i] == "--mode-none")
        {
            mode = TransparencyMode::NoOpacity;
        }
        else if (args[i] == "--transparency" && i + 1 < args.size())
        {
            transparency = args[++i].toFloat();
            if (transparency < 0.0f || transparency > 1.0f)
            {
                qDebug() << "Error: transparency must be between 0.0 and 1.0";
                return 1;
            }
        }
        else if (args[i] == "--text" && i + 1 < args.size())
        {
            text = args[++i];
        }
        else if (args[i] == "--help")
        {
            printUsage();
            return 0;
        }
        else
        {
            qDebug() << "Unknown option:" << args[i];
            printUsage();
            return 1;
        }
    }

    qDebug() << "==========================================";
    qDebug() << "Screen Watermark Demo";
    qDebug() << "==========================================";
    qDebug() << "Mode:" << (mode == TransparencyMode::WindowOpacity ? "setWindowOpacity" :
                               mode == TransparencyMode::PainterOpacity ? "QPainter::setOpacity" : "No opacity");
    qDebug() << "Transparency:" << transparency;
    qDebug() << "Text:" << text;
    qDebug() << "==========================================";

    CTextWatermark watermark;
    watermark.SetTransparencyMode(mode, transparency);
    watermark.show();
    watermark.InitWaterMark(text);

    return app.exec();
}
