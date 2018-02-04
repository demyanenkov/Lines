#ifndef NEXTBALLS_H
#define NEXTBALLS_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

class NextBalls : public QWidget
{
    Q_OBJECT
public:
    explicit NextBalls(QWidget *parent = 0);
signals:
public slots:
    void setNext(int n,int c);
    void timerUpdate(); // действия при срабатывании таймера

protected:
    void paintEvent(QPaintEvent *event); // прорисовка поля
private:
    void showBall(int n, int c, int r);
    int balls[3];
    int oldBalls[3];
    QTimer *timer; // таймер для прорисовки
    int cnt[3];


};

#endif // NEXTBALLS_H
