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
    void timerUpdate(); // �������� ��� ������������ �������
    void restart();
    void save();
signals:
    void scoreChanged(int score);
    void topScoreChanged(int score);
    void setNext(int n, int c);
protected:
    void paintEvent(QPaintEvent *event); // ���������� ����
    void mousePressEvent(QMouseEvent *event);
private:
    void showBalls(int x, int y, int c, int r); // �������� ���� ���
    void addBalls(int cnt); // �������� c ����� �� ����
    int getBalls(QPoint where);
    bool isPath(QPoint end); // ����� ����
    void moveBall(); // ����������� ��� �� ����
    int killBalls(QPoint start);
    bool killLine();
    int findLine(QPoint whereCursor);
    void showPath(QPoint end);
    void newBalls();
    int size; // ������ ����
    int side; // ������ ����� ������
    int maxColor; // ������������ ���� ����
    int balls[9][9]; // ������� ���� �� ����
    int countBalls; // ������� ����� �� ����
    bool block; // ���������� �� ����� ����������
    QTimer *timer; // ������ ��� ����������
    QPoint center; // ���������� ������ ����
    bool isSelect; // ������ �������
    QPoint selectPoint; // ���������� ���������� ��������
    QPoint addPoint;
    QPoint startPoint;
    QPoint endPoint;
    int moveColor;
    int nextColor[81]; // ��������� ������������� �����
    int fields[9][9]; // ���� ���������� ����
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
