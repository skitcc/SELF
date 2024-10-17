#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>  // Для показа сообщений об ошибках
#include <QVBoxLayout>  // Для компоновки виджетов, если требуется
#include <QWidget>      // Для использования QWidget
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsEllipseItem>
#include <QToolTip>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddPointButtonClicked(); // Слот для клика по сцене
    void onAddManualChargeButtonClicked(); // Слот для ручного добавления заряда
    void onCalculateButtonClicked(); // Добавлено
    void onClearButtonClicked(); // Добавлено

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QLineEdit *xInput;
    QLineEdit *yInput;
    QLineEdit *chargeInput;
    QPushButton *addPointButton;  // Кнопка для добавления заряда по клику
    QPushButton *addManualChargeButton;  // Кнопка для ручного добавления заряда
    QPushButton *calculateButton;
    QPushButton *clearButton;
    QList<QPointF> charges; // Список для хранения координат зарядов
    QList<double> chargesValues; // Список для хранения значений зарядов

    void drawRuler();
    void onAddCharge(double x, double y, double charge); // Добавляем заряд на сцену
};

#endif // MAINWINDOW_H
