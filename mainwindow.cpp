#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Электростатическое поле");
    setFixedSize(1200, 800);

    QMenuBar *menuBar = new QMenuBar();
    QMenu *menu = new QMenu("Меню");
    QAction *inputAction = new QAction("Ввод", this);
    QAction *fieldAction = new QAction("Напряженность поля", this);
    QAction *potentialAction = new QAction("Потенциалы", this);

    menu->addAction(inputAction);
    menu->addAction(fieldAction);
    menu->addAction(potentialAction);
    menuBar->addMenu(menu);
    setMenuBar(menuBar);

    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    view->setFixedSize(800, 600);
    view->setSceneRect(0, 0, 800, 600);

    drawRuler();
    view->installEventFilter(this);

    QVBoxLayout *inputLayout = new QVBoxLayout();
    xInput = new QLineEdit();
    yInput = new QLineEdit();
    chargeInput = new QLineEdit();
    calculateButton = new QPushButton("Рассчитать");
    clearButton = new QPushButton("Очистить");
    addPointButton = new QPushButton("Добавить заряд по клику");
    addManualChargeButton = new QPushButton("Добавить заряд вручную");

    xInput->setPlaceholderText("Введите X");
    yInput->setPlaceholderText("Введите Y");
    chargeInput->setPlaceholderText("Введите заряд");
    inputLayout->addWidget(xInput);
    inputLayout->addWidget(yInput);
    inputLayout->addWidget(chargeInput);
    inputLayout->addWidget(addPointButton);
    inputLayout->addWidget(addManualChargeButton);
    inputLayout->addWidget(calculateButton);
    inputLayout->addWidget(clearButton);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(view);
    mainLayout->addLayout(inputLayout);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::onCalculateButtonClicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(addPointButton, &QPushButton::clicked, this, &MainWindow::onAddPointButtonClicked);
    connect(addManualChargeButton, &QPushButton::clicked, this, &MainWindow::onAddManualChargeButtonClicked);
}

void MainWindow::drawRuler() {
    int N = 100;
    for (int i = 0; i <= 800; i += N) {
        scene->addLine(i, 0, i, 10);
        scene->addLine(0, i, 10, i);
    }
}

void MainWindow::onCalculateButtonClicked() {
    if (charges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Необходимо ввести хотя бы один заряд!");
        return;
    }
}

void MainWindow::onClearButtonClicked() {
    scene->clear();
    drawRuler();
    charges.clear();
    chargesValues.clear();
}

void MainWindow::onAddPointButtonClicked() {
}

void MainWindow::onAddManualChargeButtonClicked() {
    bool xOk, yOk, chargeOk;

    double x = xInput->text().toDouble(&xOk);
    double y = yInput->text().toDouble(&yOk);
    double charge = chargeInput->text().toDouble(&chargeOk);

    if (!xOk || !yOk || !chargeOk || charge == 0) {
        QMessageBox::warning(this, "Ошибка", "Проверьте правильность ввода координат и заряда.");
        return;
    }

    if (x < 0 || x > 800 || y < 0 || y > 600) {
        QMessageBox::warning(this, "Ошибка", "Координаты должны быть в пределах рабочей области (0, 0) до (800, 600).");
        return;
    }

    onAddCharge(x, y, charge);
}

void MainWindow::onAddCharge(double x, double y, double charge) {
    charges.append(QPointF(x, y));
    chargesValues.append(charge);

    ChargeItem *chargeItem = new ChargeItem(x, y, charge);
    scene->addItem(chargeItem);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == view && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QPointF scenePos = view->mapToScene(mouseEvent->pos());

            if (scenePos.x() >= 0 && scenePos.y() >= 0 && scenePos.x() <= 800 && scenePos.y() <= 600) {
                bool ok;
                double charge = QInputDialog::getDouble(this, "Введите заряд", "Заряд точки (не может быть 0):", 0, -10000, 10000, 2, &ok);

                if (ok && charge != 0) {
                    onAddCharge(scenePos.x(), scenePos.y(), charge);
                }
            }
        }
        return true;
    }

    return QMainWindow::eventFilter(watched, event);
}
