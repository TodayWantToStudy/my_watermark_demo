#include <QApplication>
#include <QWidget>
#include "CTextWatermark.h"

class MyWindow : public QWidget {
public:
    MyWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Hello Qt!");
        resize(400, 300);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // MyWindow window;
    // window.show();

    CTextWatermark watermark;
    watermark.show();
    watermark.InitWaterMark("水印文案 水印文案 水印文案 水印文案");

    return app.exec();
}