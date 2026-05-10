#include "CalculatorWidget.h"
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>

CalculatorWidget::CalculatorWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    applyStyle();

    // 连接信号和槽
    connect(calculateButton, &QPushButton::clicked, this, &CalculatorWidget::calculate);
    connect(clearButton, &QPushButton::clicked, this, &CalculatorWidget::clearAll);
}

void CalculatorWidget::setupUI()
{
    // 创建输入部分
    num1Edit = new QLineEdit(this);
    num1Edit->setPlaceholderText("输入第一个数 (如: 1/2, 3.14, 12345678901234567890)");
    num1Edit->setMinimumWidth(300);

    num2Edit = new QLineEdit(this);
    num2Edit->setPlaceholderText("输入第二个数 (如: 2/3, 2.5, 98765432109876543210)");
    num2Edit->setMinimumWidth(300);

    // 创建运算符选择
    operationCombo = new QComboBox(this);
    operationCombo->addItem("+ 加法");
    operationCombo->addItem("- 减法");
    operationCombo->addItem("× 乘法");
    operationCombo->addItem("÷ 除法");
    operationCombo->setMinimumWidth(150);

    // 创建按钮
    calculateButton = new QPushButton("计算", this);
    calculateButton->setFixedSize(100, 40);
    calculateButton->setStyleSheet("QPushButton { font-weight: bold; }");

    clearButton = new QPushButton("清空", this);
    clearButton->setFixedSize(100, 40);

    // 创建结果显示
    resultLabel = new QLabel("计算结果:", this);
    resultLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    resultValueLabel = new QLabel("", this);
    resultValueLabel->setStyleSheet("font-size: 16px; color: #2c3e50;");
    resultValueLabel->setWordWrap(true);
    resultValueLabel->setMinimumHeight(50);
    resultValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    resultValueLabel->setAlignment(Qt::AlignCenter);

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 输入区域分组
    QGroupBox *inputGroup = new QGroupBox("输入参数", this);
    QFormLayout *inputLayout = new QFormLayout;
    inputLayout->addRow("第一个数:", num1Edit);
    inputLayout->addRow("运算符:", operationCombo);
    inputLayout->addRow("第二个数:", num2Edit);
    inputGroup->setLayout(inputLayout);

    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();

    // 结果区域分组
    QGroupBox *resultGroup = new QGroupBox("计算结果", this);
    QVBoxLayout *resultLayout = new QVBoxLayout;
    resultLayout->addWidget(resultLabel);
    resultLayout->addWidget(resultValueLabel);
    resultGroup->setLayout(resultLayout);

    // 添加到主布局
    mainLayout->addWidget(inputGroup);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(resultGroup);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setWindowTitle("任意大有理数计算器");
    resize(600, 400);
}

void CalculatorWidget::applyStyle()
{
    QString style = R"(
        QWidget {
            font-family: 'Arial', 'Microsoft YaHei', sans-serif;
            font-size: 14px;
        }

        QGroupBox {
            font-weight: bold;
            border: 2px solid #3498db;
            border-radius: 5px;
            margin-top: 10px;
            padding-top: 10px;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }

        QLineEdit {
            padding: 8px;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            font-size: 14px;
        }

        QLineEdit:focus {
            border: 1px solid #3498db;
        }

        QPushButton {
            padding: 8px 16px;
            border: none;
            border-radius: 4px;
            font-size: 14px;
        }

        QPushButton#calculateButton {
            background-color: #2ecc71;
            color: white;
        }

        QPushButton#calculateButton:hover {
            background-color: #27ae60;
        }

        QPushButton#clearButton {
            background-color: #e74c3c;
            color: white;
        }

        QPushButton#clearButton:hover {
            background-color: #c0392b;
        }
    )";

    setStyleSheet(style);
    calculateButton->setObjectName("calculateButton");
    clearButton->setObjectName("clearButton");
}

void CalculatorWidget::calculate()
{
    // 获取输入
    QString str1 = num1Edit->text().trimmed();
    QString str2 = num2Edit->text().trimmed();
    int opIndex = operationCombo->currentIndex();

    // 验证输入
    if (str1.isEmpty() || str2.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入两个数！");
        return;
    }

    try {
        // 构造Rational对象
        Rational num1(str1.toStdString());
        Rational num2(str2.toStdString());
        Rational result;

        // 执行运算
        switch (opIndex) {
        case 0: // 加法
            result = num1 + num2;
            break;
        case 1: // 减法
            result = num1 - num2;
            break;
        case 2: // 乘法
            result = num1 * num2;
            break;
        case 3: // 除法
            if (num2.getNumerator() == 0) {
                QMessageBox::warning(this, "计算错误", "除数不能为零！");
                return;
            }
            result = num1 / num2;
            break;
        default:
            return;
        }

        // 显示结果
        QString resultStr = QString::fromStdString(result.toString());

        // 如果结果是整数，也显示小数形式
        if (result.isInteger()) {
            QString decimalStr = QString::fromStdString(result.toDecimalString(10));
            resultValueLabel->setText(QString("分数形式: %1\n小数形式: %2")
                                      .arg(resultStr)
                                      .arg(decimalStr));
        } else {
            QString decimalStr = QString::fromStdString(result.toDecimalString(20));
            resultValueLabel->setText(QString("分数形式: %1\n小数形式: %2 (20位)")
                                      .arg(resultStr)
                                      .arg(decimalStr));
        }

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "计算错误",
                             QString("计算过程中发生错误:\n%1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "计算错误", "发生未知错误！");
    }
}

void CalculatorWidget::clearAll()
{
    num1Edit->clear();
    num2Edit->clear();
    operationCombo->setCurrentIndex(0);
    resultValueLabel->clear();
    num1Edit->setFocus();
}
