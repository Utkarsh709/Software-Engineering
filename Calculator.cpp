#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>

class Calculator : public QWidget {
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);

private slots:
    void onDigitPressed();
    void onOperationPressed();
    void onEqualsPressed();
    void onClearPressed();

private:
    QLineEdit *display;
    double currentValue;
    QString pendingOperation;
    bool waitingForOperand;
};

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), currentValue(0.0), waitingForOperand(true) {
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    QString digitButtons[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for (int i = 0; i < 10; ++i) {
        QPushButton *button = new QPushButton(digitButtons[i]);
        mainLayout->addWidget(button, i / 3, i % 3);
        connect(button, &QPushButton::clicked, this, &Calculator::onDigitPressed);
    }

    QString operationButtons[4] = {"+", "-", "*", "/"};
    for (int i = 0; i < 4; ++i) {
        QPushButton *button = new QPushButton(operationButtons[i]);
        mainLayout->addWidget(button, i / 2, 3 + i % 2);
        connect(button, &QPushButton::clicked, this, &Calculator::onOperationPressed);
    }

    QPushButton *equalsButton = new QPushButton("=");
    mainLayout->addWidget(equalsButton, 4, 3);
    connect(equalsButton, &QPushButton::clicked, this, &Calculator::onEqualsPressed);

    QPushButton *clearButton = new QPushButton("C");
    mainLayout->addWidget(clearButton, 4, 2);
    connect(clearButton, &QPushButton::clicked, this, &Calculator::onClearPressed);

    mainLayout->addWidget(display, 0, 0, 1, 4);
    setLayout(mainLayout);
}

void Calculator::onDigitPressed() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString digitValue = clickedButton->text();

    if (display->text() == "0" && digitValue == "0")
        return;

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }

    display->setText(display->text() + digitValue);
}

void Calculator::onOperationPressed() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString clickedOperator = clickedButton->text();

    if (!waitingForOperand) {
        double operand = display->text().toDouble();
        if (pendingOperation == "+") {
            currentValue += operand;
        } else if (pendingOperation == "-") {
            currentValue -= operand;
        } else if (pendingOperation == "*") {
            currentValue *= operand;
        } else if (pendingOperation == "/") {
            if (operand == 0.0) {
                display->setText("Error");
                return;
            }
            currentValue /= operand;
        } else {
            currentValue = operand;
        }
        display->setText(QString::number(currentValue));
        waitingForOperand = true;
    }
    pendingOperation = clickedOperator;
}

void Calculator::onEqualsPressed() {
    onOperationPressed();
    pendingOperation.clear();
}

void Calculator::onClearPressed() {
    display->setText("0");
    currentValue = 0.0;
    pendingOperation.clear();
    waitingForOperand = true;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Calculator calc;
    calc.setWindowTitle("Calculator");
    calc.show();
    return app.exec();
}
