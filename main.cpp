#include "CalculatorWidget.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("任意大有理数计算器");
    app.setOrganizationName("Tencent");

    // 设置风格
    app.setStyle(QStyleFactory::create("Fusion"));

    // 创建并显示主窗口
    CalculatorWidget calculator;
    calculator.show();

    return app.exec();
}
