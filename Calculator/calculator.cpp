#include "calculator.h"
#include "ui_calculator.h"
#include <QPushButton>
#include <QRegularExpression>
#include <QDebug>
#include <QMapIterator>

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
    , mCurrentValue(0.0)
    , mMemoryValue(0.0)
{
    ui->setupUi(this);


    // Set functions to all number push buttons
    for (int i = 0; i < 10; i++)
    {
        QString buttonName = "button_" + QString::number(i);
        QPushButton* button = Calculator::findChild<QPushButton*>(buttonName);
        mNumPushButtons.push_back(std::move(button));
        connect(mNumPushButtons[i], &QPushButton::released, this, &Calculator::numPressed);
    }

    connect(ui->button_Plus, &QPushButton::released, this, &Calculator::mathButtonPressed);
    connect(ui->button_Sub, &QPushButton::released, this, &Calculator::mathButtonPressed);
    connect(ui->button_Mul, &QPushButton::released, this, &Calculator::mathButtonPressed);
    connect(ui->button_Div, &QPushButton::released, this, &Calculator::mathButtonPressed);
    connect(ui->button_Equ, &QPushButton::released, this, &Calculator::equalButtonPressed);
    connect(ui->button_Sign, &QPushButton::released, this, &Calculator::signChange);
    connect(ui->button_AC, &QPushButton::released, this, &Calculator::allClear);
    connect(ui->button_M_Plus, &QPushButton::released, this, &Calculator::addToMemoryVal);
    connect(ui->button_M_Min, &QPushButton::released, this, &Calculator::subtractFromMemoryVal);
    connect(ui->button_MR, &QPushButton::released, this, &Calculator::recallMemoryVal);

    // Initialize Math Triggers
    initMathTriggers();
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::initMathTriggers()
{
    mMathTriggerMap.insert("divide", false);
    mMathTriggerMap.insert("multiply", false);
    mMathTriggerMap.insert("add", false);
    mMathTriggerMap.insert("subtract", false);
}

void Calculator::numPressed()
{
    QPushButton* button = (QPushButton*)sender();
    QString buttonVal = button->text();
    QString displayVal = ui->num_Display->text();

    // If display val is zero, set to button value
    if (displayVal.toDouble() == 0 || displayVal.toDouble() == 0.0)
    {
        ui->num_Display->setText(buttonVal);
    }
    else
    {
        QString newValue = displayVal + buttonVal;
        double dblNewValue = newValue.toDouble();

        // Set the display to new value and set the max characters to 16, before using exponents
        ui->num_Display->setText(QString::number(dblNewValue, 'g', 16));
    }
}

void Calculator::mathButtonPressed()
{
    // Set all math triggers/operators to false
    for(auto& key : mMathTriggerMap)
       key = false;

    QString displayVal = ui->num_Display->text();
    mCurrentValue = displayVal.toDouble();
    // Get the currently pressed math operator
    QPushButton* button = (QPushButton*)sender();
    // Create a QString to compare based on the button's text
    QString buttonVal = button->text();

    // Set the currently pressed math operator to true
    if (QString::compare(buttonVal, "/", Qt::CaseInsensitive) == 0)
        mMathTriggerMap["divide"] = true;
    else if (QString::compare(buttonVal, "*", Qt::CaseInsensitive) == 0)
        mMathTriggerMap["multiply"] = true;
    else if (QString::compare(buttonVal, "+", Qt::CaseInsensitive) == 0)
        mMathTriggerMap["add"] = true;
    else if (QString::compare(buttonVal, "-", Qt::CaseInsensitive) == 0)
        mMathTriggerMap["subtract"] = true;

    // Set the num Display to blank and be ready to accept the next value
    ui->num_Display->setText("");
}

void Calculator::equalButtonPressed()
{
    double sum = 0.0;
    QString displayVal = ui->num_Display->text();
    double dblDisplayVal = displayVal.toDouble();

    if (mMathTriggerMap["add"])
        sum = mCurrentValue + dblDisplayVal;
    else if (mMathTriggerMap["subtract"])
        sum = mCurrentValue - dblDisplayVal;
    else if (mMathTriggerMap["multiply"])
        sum = mCurrentValue * dblDisplayVal;
    else if (mMathTriggerMap["divide"])
    {
        if (dblDisplayVal != 0)
            sum = mCurrentValue / dblDisplayVal;
    }

    ui->num_Display->setText(QString::number(sum));
}

void Calculator::signChange()
{
    QString displayVal = ui->num_Display->text();
    QRegularExpression reg("[-]?[0-9.]*");
    QRegularExpressionMatch match = reg.match(displayVal);

    if (match.hasMatch())
    {
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;
        ui->num_Display->setText((QString::number(dblDisplayValSign)));
    }
}

void Calculator::allClear()
{
    // Reset all math operators
    for (auto& key : mMathTriggerMap)
        key = false;
    // Reset the display back to zero
    ui->num_Display->setText(QString::number(0.0));

    // Reset the current value to zero
    mCurrentValue = 0.0;
}
void Calculator::addToMemoryVal()
{
    QString displayVal = ui->num_Display->text();
    double dblDisplayVal = displayVal.toDouble();

    mMemoryValue += dblDisplayVal;
}

void Calculator::subtractFromMemoryVal()
{
    QString displayVal = ui->num_Display->text();
    double dblDisplayVal = displayVal.toDouble();

    mMemoryValue -= dblDisplayVal;
}

void Calculator::recallMemoryVal()
{
    QString displayVal = ui->num_Display->text();

    // If display val is zero, set to button value
    if (displayVal.toDouble() == 0 || displayVal.toDouble() == 0.0)
    {
        ui->num_Display->setText(QString::number(mMemoryValue));
    }
    else
    {
        QString newValue = displayVal + QString::number(mMemoryValue);
        double dblNewValue = newValue.toDouble();

        // Set the display to new value and set the max characters to 16, before using exponents
        ui->num_Display->setText(QString::number(dblNewValue, 'g', 16));
    }
}
