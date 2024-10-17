#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>  
#include <QVBoxLayout>  
#include <QWidget>      
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsEllipseItem>
#include <QToolTip>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

class ChargeItem : public QGraphicsEllipseItem {
public:
    ChargeItem(double x, double y, double charge, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(x - 5, y - 5, 10, 10, parent), charge(charge) {
        setBrush(QBrush(Qt::green));
        setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            QToolTip::showText(event->screenPos(), QString("Заряд: %1").arg(charge));
        }
        QGraphicsEllipseItem::mousePressEvent(event);
    }

private:
    double charge;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onCalculateButtonClicked();
    void onClearButtonClicked();
    void onAddPointButtonClicked();
    void onAddManualChargeButtonClicked();
    void onAddCharge(double x, double y, double charge);

private:
    void drawRuler();

    QGraphicsScene *scene;
    QGraphicsView *view;
    QLineEdit *xInput;
    QLineEdit *yInput;
    QLineEdit *chargeInput;
    QPushButton *calculateButton;
    QPushButton *clearButton;
    QPushButton *addPointButton;
    QPushButton *addManualChargeButton;

    QList<QPointF> charges;
    QList<double> chargesValues;
};

#endif