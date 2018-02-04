#ifndef LINESVIEW_H
#define LINESVIEW_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QFile>
#include <QDataStream>


class LinesView : public QWidget
{
    Q_OBJECT
public:
    explicit LinesView(QWidget *parent = 0);
public slots:
    void timerUpdate(); // действия при срабатывании таймера
    void restart();
    void save();
signals:
    void scoreChanged(int score);
    void topScoreChanged(int score);
    void setNext(int n, int c);
protected:
    void paintEvent(QPaintEvent *event); // прорисовка поля
    void mousePressEvent(QMouseEvent *event);
private:
    void showBalls(int x, int y, int c, int r); // показать один шар
    void addBalls(int cnt); // добавить c шаров на поле
    int getBalls(QPoint where);
    bool isPath(QPoint end); // поиск пути
    void moveBall(); // передвинуть шар по пути
    int killBalls(QPoint start);
    bool killLine();
    int findLine(QPoint whereCursor);
    void showPath(QPoint end);
    void newBalls();
    int size; // размер поля
    int side; // размер грани плитки
    int maxColor; // максимальный цвет шара
    int balls[9][9]; // текущие шары на поле
    int countBalls; // счетчик шаров на поле
    bool block; // блокировка на время прорисовки
    QTimer *timer; // таймер для прорисовки
    QPoint center; // координаты центра поля
    bool isSelect; // выбран квадрат
    QPoint selectPoint; // координаты выбранного квадрата
    QPoint addPoint;
    QPoint startPoint;
    QPoint endPoint;
    int moveColor;
    int nextColor[81]; // следующие выбрасываемые цвета
    int fields[9][9]; // поле найденного пути
    int minLine;
    int score;
    int topScore;
    int action;
    enum {ACT_NO, ACT_ADD, ACT_DEL, ACT_MOV, ACT_NEW};
    int cntAdd;
    int cntAction;
    int period;
    QWidget *prnt;
    bool addBallSkip;
};

#endif // LINESVIEW_H
