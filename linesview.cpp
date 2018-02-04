#include "linesview.h"

LinesView::LinesView(QWidget *parent) :
    QWidget(parent)
{

    QRect geom;
    prnt=parent;
    int tmp;

    size=9;
    maxColor=7;
    minLine=5;
    addBallSkip=false;
    QFile file("lines.dat");
    if(file.open(QIODevice::ReadOnly)){
        QDataStream in(&file);
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                in >> balls[i][j];
        in>>countBalls;
        in>>score;
        in>>topScore;
        for(int i=0;i<3;i++) in >> nextColor[i];
        block = false;
        timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
        cntAdd=0;
        isSelect=false;
        action=ACT_NO;
        //period=100;
        period=30;
        update();
        for(int i=0;i<3;i++) emit setNext(i,nextColor[i]);
        emit topScoreChanged(topScore);
        emit scoreChanged(score);
        action=ACT_NEW;
        timer->start(20);
        in>>tmp; geom.setX(tmp);
        in>>tmp; geom.setY(tmp);
        in>>tmp; geom.setWidth(tmp);
        in>>tmp; geom.setHeight(tmp);

        parent->setGeometry(geom);
    }
    else
        restart();
}

void LinesView::save()
{
    QRect geom;
    QFile file("lines.dat");
    if(file.open(QIODevice::WriteOnly)){
        QDataStream out(&file);
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                out << balls[i][j];
        out<<countBalls;
        out<<score;
        out<<topScore;
        for(int i=0;i<3;i++)
            out<<nextColor[i];
        geom=prnt->geometry();
        out<< geom.x();
        out<< geom.y();
        out<< geom.width();
        out<< geom.height();

        file.close();
    }

}

void LinesView::restart()
{
    size=9;
    maxColor=7;
    minLine=5;
    addBallSkip=false;
    //qsrand(10);
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            balls[i][j] = 0;
    countBalls=0;
    block = false;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    cntAdd=0;
    for(int i=0;i<81;i++)
        nextColor[i]=1+qrand()%maxColor;
    isSelect=false;
    score=0;
    cntAction=1;
    action=ACT_ADD;
    period=30;
    score=0;
    emit scoreChanged(score);
    addBalls(3);
}

void LinesView::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    QPoint current;
    int c = balls[endPoint.x()][endPoint.y()];
    side=(qMin(width(),height()))/size;

    center.setX((width()-side*size)/2+3);
    center.setY((height()-side*size)/2+3);

    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++){
            current.setX(i);
            current.setY(j);
            switch(action){
            case ACT_ADD:
                if( current==addPoint)
                    showBalls(i,j,balls[i][j],cntAction);
                else showBalls(i,j,balls[i][j],-1);
            break;
            case ACT_DEL:
                if(fields[i][j]) showBalls(i,j,balls[i][j],5-cntAction);
                else showBalls(i,j,balls[i][j],-1);
                break;
            case ACT_MOV:
                if(current==endPoint)showBalls(i,j,balls[i][j],cntAction);
                else if(current==startPoint)showBalls(i,j,balls[i][j],5-cntAction);
                else if(fields[i][j]>1) showBalls(i,j,c,1);
                else showBalls(i,j,balls[i][j],-1);
            break;
            default:showBalls(i,j,balls[i][j],-1);
            }
        }
}

void LinesView::mousePressEvent(QMouseEvent *event)
{
    QPoint whereCursor;
    if(event->button()==Qt::LeftButton){
        whereCursor.setX((event->pos().x()-center.x())/side);
        whereCursor.setY((event->pos().y()-center.y())/side);
        if(getBalls(whereCursor)){
            selectPoint=whereCursor;
            isSelect=true;
        }
        else{
            if(isSelect){
                if(isPath(whereCursor)){
                    startPoint=selectPoint;
                    endPoint=whereCursor;
                    moveBall();
                }
                isSelect=false;
            }
        }
        update();
    }
    if(event->button()==Qt::RightButton){
        isSelect=false;
        update();
    }
}

int LinesView::getBalls(QPoint where){
   return  balls[where.x()][where.y()];
}

void LinesView::showBalls(int x, int y, int c, int r){
    QPainter painter(this);
    QColor ballColors[2];
    QRadialGradient radGradient(0,0,20, 0, 0);
    QRadialGradient shadeGradient(0,0,20, 0, 0);
    double sd=side/11.0;
    int xc=center.x();
    int yc=center.y();

    painter.setRenderHint(QPainter::Antialiasing, true);
    if(isSelect && x==selectPoint.x() && y==selectPoint.y())
        painter.setBrush(QBrush(QColor(134,134,134), Qt::SolidPattern));
    else
        painter.setBrush(QBrush(QColor(104,104,104), Qt::SolidPattern));

    painter.setPen(Qt::NoPen);
    painter.drawRoundRect(x*side+xc,y*side+yc,(side*10)/11,(side*10)/11,10,10);
    painter.setPen(QPen(QColor(119,119,119),1,Qt::SolidLine));
    painter.drawLine(x*side+xc+side/22,y*side+yc,x*side+xc+(side*10)/11-side/22,y*side+yc);
    painter.setPen(QPen(QColor(93,93,93),1,Qt::SolidLine));
    painter.drawLine(x*side+xc+3,y*side+yc+(side*10)/11,x*side+xc+(side*10)/11-3,y*side+yc+(side*10)/11);

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
    painter.setPen(Qt::NoPen);
    shadeGradient.setColorAt(0.0,QColor(0,0,0));
    if(isSelect && x==selectPoint.x() && y==selectPoint.y())
        shadeGradient.setColorAt(1.0,QColor(134,134,134));
    else
        shadeGradient.setColorAt(1.0,QColor(104,104,104));
    if(r==-1){
        shadeGradient.setRadius(4.5*side/11);
        shadeGradient.setCenter(x*side+xc + 5*side/11,y*side+yc+5*side/11);
        shadeGradient.setFocalPoint(x*side+xc + 5.5*side/11,y*side+yc+5.5*side/11);
        painter.setBrush(shadeGradient);
        painter.drawEllipse(x*side+xc,y*side+yc,(side*10)/11,(side*10)/11);
    }
    else{
        shadeGradient.setRadius((r*0.9)*sd);
        shadeGradient.setCenter(x*side+xc + 5*sd,y*side+yc+5*sd);
        shadeGradient.setFocalPoint(x*side+xc + (5)*sd,y*side+yc+(5)*sd);
        painter.setBrush(shadeGradient);
        painter.drawEllipse(x*side+xc+(5-r)*sd,y*side+yc+(5-r)*sd,sd*r*2,sd*r*2);
    }

    //painter.setPen(QPen(ballColors[0],0.5,Qt::SolidLine));
    radGradient.setColorAt(0.0,ballColors[0]);
    radGradient.setColorAt(0.9,ballColors[1]);
    if(r==-1){
        radGradient.setRadius(4*side/11);
        radGradient.setCenter(x*side+xc + 5*side/11,y*side+yc+5*side/11);
        radGradient.setFocalPoint(x*side+xc + 4*side/11,y*side+yc+3*side/11);
        painter.setBrush(radGradient);
        painter.drawEllipse(x*side+xc +1.25*sd,y*side+yc+1.25*sd,7.5*sd,7.5*sd);
    }
    else{
        shadeGradient.setRadius(r*sd);
        radGradient.setCenter(x*side+xc + 5*sd,y*side+yc+5*sd);
        radGradient.setFocalPoint(x*side+xc + (5-0.2*r)*sd,y*side+yc+(5-0.4*r)*sd);
        painter.setBrush(radGradient);
        painter.drawEllipse(x*side+xc +(5-r*0.75)*sd,y*side+yc+(5-r*0.75)*sd,sd*(r*1.5),sd*(r*1.5));

    }
}

void LinesView::addBalls(int cnt){
    int x,y;
    cntAdd=cnt;
    action=ACT_ADD;
    cntAdd--;
    if(countBalls<(size*size)){
        do{
            x=qrand()%size;
            y=qrand()%size;
        }while(balls[x][y]>0);
        addPoint.setX(x);
        addPoint.setY(y);
        if(nextColor[cntAdd]>0){
            countBalls++;
            balls[x][y]=nextColor[cntAdd];
        }
        //nextColor[cntAdd]=1+qrand()%maxColor;
        //if((countBalls+2) >= (size*size)) nextColor[cntAdd]=0;
        //emit setNext(cntAdd,nextColor[cntAdd]);
        block=true;
        update();
        timer->start(period); //!!!!
    }
}

void LinesView::timerUpdate()
{
    switch(action){
    case ACT_NO:
        update();
        emit topScoreChanged(topScore);
        emit scoreChanged(score);
        timer->stop();
    break;
    case ACT_ADD:
        if(cntAction<5){
            cntAction++;
            update();
        }
        else{
            cntAction=0;
            if(findLine(addPoint)) action=ACT_DEL;
            else{
                if(countBalls>=(size*size)){
                    action=ACT_NO;
                }
                else if(!cntAdd) action=ACT_NEW;
                else addBalls(cntAdd);
            }
            update();
        }
    break;
    case ACT_DEL:
        timer->setInterval(period*3/2);
        if(cntAction<5){
            cntAction++;
            update();
        }
        else{
            cntAction=0;
            if(killLine()) addBallSkip=true;
            if(cntAdd)addBalls(cntAdd);
            else action=ACT_NO;
            update();
            if(action==ACT_NO) {
                if(!addBallSkip)
                    addBalls(3);
                addBallSkip=false;
            }
        }
        break;
    case ACT_MOV:
        if(cntAction<5){
            cntAction++;
            update();
        }
        else{
            cntAction=0;
            action=ACT_ADD;
            balls[startPoint.x()][startPoint.y()]=0;
            if(findLine(endPoint)){
                action=ACT_DEL;
            }
            else addBalls(3);
            update();

        }
        break;
    case ACT_NEW:
        newBalls();
        if(countBalls< (size*size))
            for(int i=0;i<3;i++) emit setNext(i,nextColor[i]);
        else
            for(int i=0;i<3;i++) emit setNext(i,0);

        action=ACT_NO;
        break;
    default: timer->stop();
    }
}

bool LinesView::isPath(QPoint end)
{
    int i,j,k=0,c,x,y;
    bool find=false;
    bool noPath=true;
    QPoint start=selectPoint;

    for(i=0;i<size;i++)
        for(j=0;j<size;j++)
            fields[i][j]=0;
    fields[start.x()][start.y()]=1;
    do{
        k++;
        noPath=true;
        for(i=0;i<size;i++)
        for(j=0;j<size;j++)
        if(fields[i][j]==k)
        for(c=0;c<4;c++){
            x=i;y=j;
            if(c==0)x--;else if(c==1)y--; else if(c==2)x++; else if(c==3)y++;
            if(x>=0 && y>=0 && x<size && y<size){
                if(x==end.x() && y==end.y()){
                    fields[x][y]=k+1;
                    find=true;
                }else{
                    if(!balls[x][y] && fields[x][y]==0){
                        fields[x][y]=k+1;
                        noPath=false;
                    }
                }
            }
        }
    }while(!find && !noPath);
    if(find) showPath(end);
    return find;
}

void LinesView::moveBall()
{
    balls[endPoint.x()][endPoint.y()]=balls[startPoint.x()][startPoint.y()];
    //balls[startPoint.x()][startPoint.y()]=0;
    action=ACT_MOV;
    cntAction=0;
    block=true;
    update();
    timer->start(period);
}

int LinesView::killBalls(QPoint start)
{
    int v; // вариант прохождения
    int c; // цвет шара
    int x,y; //координаты исследуемого поля
    int cnt; //счетчик подряд идущих
    int score=0; //сколько прибавлять к счету
    int tmpFields[9][9];
    int i,j;
    bool brk; //break
    for(i=0;i<size;i++) for(j=0;j<size;j++) fields[i][j]=0;
    c=balls[start.x()][start.y()];
    fields[start.x()][start.y()]=1;
    for(v=0;v<4;v++){
        x=start.x(); y=start.y();
        cnt=0;
        for(i=0;i<size;i++) for(j=0;j<size;j++) tmpFields[i][j]=0;
        do{
            brk=false;
            switch(v){
            case 0: y--; x--; break;
            case 1: x--; break;
            case 2: x--; y++; break;
            case 3: y++; break;
            }
            if(x>=0 && y>=0 && x<size && y<size){
                if(balls[x][y]==c){
                    cnt++;
                    tmpFields[x][y]=1;
                }
                else brk=true;
            }
            else brk=true;
        }while(!brk);
        x=start.x(); y=start.y();
        brk=false;
        do{
            switch(v){
            case 0: y++; x++; break;
            case 1: x++; break;
            case 2: x++; y--; break;
            case 3: y--; break;
            }
            if(x>=0 && y>=0 && x<size && y<size){
                if(balls[x][y]==c){
                    cnt++;
                    tmpFields[x][y]=1;
                }
                else brk=true;
            }
            else brk=true;
        }while(!brk);
        if(cnt>=(minLine-1)){
            for(i=0;i<size;i++)
                for(j=0;j<size;j++)
                    fields[i][j]+=tmpFields[i][j];
            if(score) score+=cnt*2;
            else score=2+cnt*2;
        }
    }
    return score;
}

bool LinesView::killLine()
{
    bool ret=false;
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            if(fields[i][j]) {
                balls[i][j]=0;
                fields[i][j]=0;
                ret=true;
                countBalls--;
            }
    return ret;
}

int LinesView::findLine(QPoint whereCursor)
{
    int incScore;
    incScore=killBalls(whereCursor);
    if(incScore){
        //killLine();
        score+=incScore;
        if(score>topScore){
            topScore=score;
            emit topScoreChanged(score);
        }
        emit scoreChanged(score);
    }
    return incScore/2;
}

void LinesView::showPath(QPoint end)
{
    int start=fields[end.x()][end.y()];
    int x,y;
    int x1,y1;
    bool skip;
    x=end.x(); y=end.y();
    for(int i= start;i>0;i--){
        for(int j=0;j<size;j++)
            for(int k=0;k<size;k++)
                if((j!=x || k!=y) && (fields[j][k]==i))
                    fields[j][k]=0;
        skip=false;
        for(int v=0;v<4;v++){
            if(!skip){
                x1=x;y1=y;
                switch(v){
                case 0: x1++; break;
                case 1: y1++; break;
                case 2: x1--; break;
                case 3: y1--; break;
                }
                if(x1>=0 && y1>=0 && x1<size && y1<size)
                if(fields[x1][y1]==(i-1)){
                    x=x1;y=y1;

                    skip=true;
                }
            }
        }
    }
}

void LinesView::newBalls()
{
    for(int i=0;i<3;i++)
        if((countBalls+i)>= (size*size))
            nextColor[i]=0;
        else
            nextColor[i]=1+qrand()%maxColor;
}
