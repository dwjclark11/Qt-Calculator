#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QMap>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;
    double mCurrentValue;
    double mMemoryValue;
    QMap<QString, bool> mMathTriggerMap;
    QVector<class QPushButton*> mNumPushButtons;

private:
    void initUI();
    void initMathTriggers();

private slots:
    void numPressed();
    void mathButtonPressed();
    void equalButtonPressed();
    void signChange();
    void allClear();
    void addToMemoryVal();
    void subtractFromMemoryVal();
    void recallMemoryVal();

};

#endif // CALCULATOR_H
