#include "nextballs.h"

NextBalls::NextBalls(QWidget *parent) :
    QWidget(parent)
{
    for(int i=0;i<3;i++)
        balls[i]=0;
    for(int i=0;i<3;i++)
        cnt[i]=0;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));

}

void NextBalls::paintEvent(QPaintEvent *event)
{
    for(int i=0;i<3;i++)
        if(cnt[i]==0 || cnt[i]==9)
            showBall(i,balls[i],5);
    else if(cnt[i]<5) showBall(i,oldBalls[i],5-cnt[i]);
    else showBall(i,oldBalls[i],cnt[i]-5);
}

void NextBalls::setNext(int n, int c)
{
    if(n>2) return;
    balls[2-n]=c;
    cnt[2-n]=1;
    oldBalls[2-n]=balls[2-n];
    balls[2-n]=c;
    timer->start(10);
    update();
}

void NextBalls::showBall(int n, int c, int r)
{
    QPainter painter(this);
    QColor ballColors[2];
    QRadialGradient radGradient(0,0,20, 0, 0);

    int side = 20;
    double rd=(7.5/2.0)*r/5.0;

    side=(qMin(width(),height()*3))/3;
    double sd=side/11.0;

    int xc=(width()-side*3)/2;
    int yc=(height()-side*1)/2;

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.drawRect(n*side+xc,yc,side,side);
    if(c==0)return;
    switch(c){
    case 1: // синий
        ballColors[0]=QColor(88,144,240);
        ballColors[1]=QColor(49,108,212); break;
    case 2: // зеленый
        ballColors[0]=QColor(102,200,65);
        ballColors[1]=QColor(64,141,12); break;
    case 3: // фиолетовый
        ballColors[0]=QColor(191,122,240);
        ballColors[1]=QColor(147,86,191); break;
    case 4: // оранжевый
        ballColors[0]=QColor(254,130,88);
        ballColors[1]=QColor(200,80,40); break;
    case 5: // бордовый
        ballColors[0]=QColor(255,102,166);
        ballColors[1]=QColor(204,61,121); break;
    case 6: // голубой
        ballColors[0]=QColor(78,223,223);
        ballColors[1]=QColor(33,163,163); break;
    case 7: // желтый
        ballColors[0]=QColor(254,225,76);
        ballColors[1]=QColor(191,153,0); break;
    }

    radGradient.setColorAt(0.0,ballColors[0]);
    radGradient.setColorAt(0.9,ballColors[1]);
    radGradient.setRadius(4*sd);
    radGradient.setCenter(n*side+xc + 5*sd,yc+5*sd);
    radGradient.setFocalPoint(n*side+xc + 4*sd,yc+3*sd);
    painter.setBrush(radGradient);
    //painter.drawEllipse(n*side+xc +1.75*sd,yc+1.25*sd,7.5*sd,7.5*sd);
    painter.drawEllipse(n*side+xc +(5.5-rd)*sd,yc+(5.0-rd)*sd,rd*2.0*sd,rd*2.0*sd);

}
void NextBalls::timerUpdate()
{
    bool exit=true;
    for(int i=0;i<3;i++){
        if(cnt[i]<9){
            cnt[i]++;
            exit=false;
        }
    }
    update();
    if (exit) timer->stop();
}
