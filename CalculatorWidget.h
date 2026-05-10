#ifndef CALCULATORWIDGET_H
#define CALCULATORWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include "Rational.h"

class CalculatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalculatorWidget(QWidget *parent = nullptr);

private slots:
    void calculate();
    void clearAll();

private:
    void setupUI();
    void applyStyle();

    QLineEdit *num1Edit;
    QLineEdit *num2Edit;
    QComboBox *operationCombo;
    QPushButton *calculateButton;
    QPushButton *clearButton;
    QLabel *resultLabel;
    QLabel *resultValueLabel;
};

#endif // CALCULATORWIDGET_H
