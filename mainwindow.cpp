#include "mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsEllipseItem>
#include <QToolTip>

// Класс для работы с зарядами
class ChargeItem : public QGraphicsEllipseItem {
public:
    ChargeItem(double x, double y, double charge, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(x - 5, y - 5, 10, 10, parent), charge(charge) {
        setBrush(QBrush(Qt::green));
        setFlag(QGraphicsItem::ItemIsSelectable, true);  
    }

protected:
    // Обрабатываем нажатие на точку заряда
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
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


    drawRuler();

    
    QVBoxLayout *inputLayout = new QVBoxLayout();
    xInput = new QLineEdit();
    yInput = new QLineEdit();
    chargeInput = new QLineEdit();
    calculateButton = new QPushButton("Рассчитать");
    clearButton = new QPushButton("Очистить");

    xInput->setPlaceholderText("Введите X");
    yInput->setPlaceholderText("Введите Y");
    chargeInput->setPlaceholderText("Введите заряд");
    inputLayout->addWidget(xInput);
    inputLayout->addWidget(yInput);
    inputLayout->addWidget(chargeInput);
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
    
}

// Обработка нажатия на кнопку "Очистить"
void MainWindow::onClearButtonClicked() {
    scene->clear();
    drawRuler();  
    charges.clear();
    chargesValues.clear();
}

// Обработка нажатий на сцену для добавления зарядов
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPointF scenePos = view->mapToScene(event->pos());

        
        if (scenePos.x() < 0 || scenePos.y() < 0 || scenePos.x() > 800 || scenePos.y() > 600) {
            return;
        }

        
        bool ok;
        double charge = QInputDialog::getDouble(this, "Введите заряд", "Заряд точки (не может быть 0):", 0, -10000, 10000, 2, &ok);

        if (ok && charge != 0) {
        
            onAddCharge(scenePos.x(), scenePos.y(), charge);
        } else {
            QMessageBox::warning(this, "Ошибка", "Заряд не может быть равен нулю. Попробуйте снова.");
        }
    }
}

void MainWindow::onAddCharge(double x, double y, double charge) {
    charges.append(QPointF(x, y));       
    chargesValues.append(charge);        

    ChargeItem *chargeItem = new ChargeItem(x, y, charge);
    scene->addItem(chargeItem);
}

