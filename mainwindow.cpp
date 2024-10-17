#include "mainwindow.h"

// Класс для работы с зарядами
class ChargeItem : public QGraphicsEllipseItem {
public:
    ChargeItem(double x, double y, double charge, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(x - 5, y - 5, 10, 10, parent), charge(charge) {
        setBrush(QBrush(Qt::green));
        setFlag(QGraphicsItem::ItemIsSelectable, true);  
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override{
        if (event->button() == Qt::LeftButton) {
            QToolTip::showText(event->screenPos(), QString("Заряд: %1").arg(charge));
        }
        QGraphicsEllipseItem::mousePressEvent(event);  
    } 
private:
    double charge;  
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Электростатическое поле");
    setFixedSize(1200, 800);

    // Главное меню
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

    // Создаём сцену и вид
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    view->setFixedSize(800, 600);
    view->setSceneRect(0, 0, 800, 600);  // Устанавливаем границы сцены

    drawRuler();

    // Поля ввода и кнопки
    QVBoxLayout *inputLayout = new QVBoxLayout();
    xInput = new QLineEdit();
    yInput = new QLineEdit();
    chargeInput = new QLineEdit();
    calculateButton = new QPushButton("Рассчитать");
    clearButton = new QPushButton("Очистить");
    addPointButton = new QPushButton("Добавить заряд по клику");  // Кнопка для добавления заряда кликом
    addManualChargeButton = new QPushButton("Добавить заряд вручную");  // Кнопка для ручного добавления

    xInput->setPlaceholderText("Введите X");
    yInput->setPlaceholderText("Введите Y");
    chargeInput->setPlaceholderText("Введите заряд");
    inputLayout->addWidget(xInput);
    inputLayout->addWidget(yInput);
    inputLayout->addWidget(chargeInput);
    inputLayout->addWidget(addPointButton);  // Добавляем кнопку для ввода заряда
    inputLayout->addWidget(addManualChargeButton);  // Добавляем кнопку для ручного добавления заряда
    inputLayout->addWidget(calculateButton);
    inputLayout->addWidget(clearButton);

    // Основной layout
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(view);
    mainLayout->addLayout(inputLayout);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Подключаем кнопки к обработчикам
    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::onCalculateButtonClicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(addPointButton, &QPushButton::clicked, this, &MainWindow::onAddPointButtonClicked);
    connect(addManualChargeButton, &QPushButton::clicked, this, &MainWindow::onAddManualChargeButtonClicked);  // Подключаем новую кнопку
}

// Функция для отрисовки линейки
void MainWindow::drawRuler() {
    int N = 100;
    for (int i = 0; i <= 800; i += N) {
        scene->addLine(i, 0, i, 10);
        scene->addLine(0, i, 10, i);
    }
}

// Обработка нажатия на кнопку "Рассчитать"
void MainWindow::onCalculateButtonClicked() {
    if (charges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Необходимо ввести хотя бы один заряд!");
        return;
    }
    // Тут можно добавить код для расчета поля или потенциалов
}

// Обработка нажатия на кнопку "Очистить"
void MainWindow::onClearButtonClicked() {
    scene->clear();
    drawRuler();  
    charges.clear();
    chargesValues.clear();
}

// Обработка нажатий на сцену для добавления зарядов по клику
void MainWindow::onAddPointButtonClicked() {
    // Подключаем событие мыши для добавления заряда по клику
    // Запрашиваем значение заряда
    bool ok;
    double charge = QInputDialog::getDouble(this, "Введите заряд", "Заряд точки (не может быть 0):", 0, -10000, 10000, 2, &ok);

    if (!ok || charge == 0) {
        QMessageBox::warning(this, "Ошибка", "Заряд не может быть равен нулю. Попробуйте снова.");
        return;
    }

    // Если значение корректное, активируем режим добавления точки по клику
    connect(view, &QGraphicsView::mousePressEvent, this, [this, charge](QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            // Преобразуем координаты относительно виджета в координаты сцены
            QPointF scenePos = view->mapToScene(event->pos());

            // Проверяем, что нажали внутри рабочей области
            if (scenePos.x() < 0 || scenePos.y() < 0 || scenePos.x() > 800 || scenePos.y() > 600) {
                return;
            }

            // Добавляем заряд на сцену
            onAddCharge(scenePos.x(), scenePos.y(), charge);
        }
    });
}

// Обработка нажатия на кнопку "Добавить заряд вручную"
void MainWindow::onAddManualChargeButtonClicked() {
    bool xOk, yOk, chargeOk;
    
    // Чтение значений X, Y и заряда
    double x = xInput->text().toDouble(&xOk);
    double y = yInput->text().toDouble(&yOk);
    double charge = chargeInput->text().toDouble(&chargeOk);

    if (!xOk || !yOk || !chargeOk || charge == 0) {
        QMessageBox::warning(this, "Ошибка", "Проверьте правильность ввода координат и заряда.");
        return;
    }

    // Проверяем, что точка находится в пределах сцены
    if (x < 0 || x > 800 || y < 0 || y > 600) {
        QMessageBox::warning(this, "Ошибка", "Координаты должны быть в пределах рабочей области (0, 0) до (800, 600).");
        return;
    }

    // Добавляем заряд по указанным координатам
    onAddCharge(x, y, charge);
}

// Добавляем заряд на сцену
void MainWindow::onAddCharge(double x, double y, double charge) {
    charges.append(QPointF(x, y));  // Добавляем координаты в список
    chargesValues.append(charge);  // Добавляем заряд в список

    ChargeItem *chargeItem = new ChargeItem(x, y, charge);
    scene->addItem(chargeItem);  // Добавляем объект на сцену
}
