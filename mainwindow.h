#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QToolTip>
#include <QGraphicsEllipseItem>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void drawRuler(); 
    void onAddCharge(double x, double y, double charge); 

private slots:
    void onCalculateButtonClicked();
    void onClearButtonClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override; 

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QLineEdit *xInput;
    QLineEdit *yInput;
    QLineEdit *chargeInput;
    QPushButton *calculateButton;
    QPushButton *clearButton;
    QList<QPointF> charges; // Список введённых координат зарядов
    QList<double> chargesValues; // Список значений зарядов
};

#endif 
