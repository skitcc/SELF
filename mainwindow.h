#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./maths/inc/math_module.h"
#include "./maths/inc/definitions_math.h"

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
#include <QInputDialog>
#include <QGraphicsEllipseItem>
#include <QToolTip>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainterPath>
#include <algorithm>
#include <cmath>

struct ChargePoint {
    uint x;
    uint y;
    double charge;
};

class ChargeItem : public QGraphicsEllipseItem {
public:
    ChargeItem(ChargePoint point, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(point.x - 5, point.y - 5, 10, 10, parent), charge(point.charge) {
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
    double calculatePotentialAt(int x, int y);

private slots:
    void onCalculateButtonClicked();
    void onClearButtonClicked();
    void onAddPointButtonClicked();
    void onAddManualChargeButtonClicked();
    void onAddCharge(double x, double y, double charge);

private:
    void drawRuler();
    void logAllCharges();

    QGraphicsScene *scene;
    QGraphicsView *view;
    QLineEdit *xInput;
    QLineEdit *yInput;
    QLineEdit *chargeInput;
    QPushButton *calculateButton;
    QPushButton *clearButton;
    QPushButton *addPointButton;
    QPushButton *addManualChargeButton;
    double **potentialField;

    QList<ChargePoint> chargePoints;
    QPainterPath splinePath;
};

#endif
