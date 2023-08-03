#include "game.h"
#include "ui_game.h"

Cless clessArray[15][15];
int able =0;

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    this->setMouseTracking ( true);
   x=-1000;
   y=-1000;
   cless = new Cless;
   Pix = QPixmap(600, 600);
   clearPix = QPixmap(600, 600);
   Pix.fill(Qt::white);
   clearPix.fill(Qt::white);
   Dchange=1;
   this->ui->label_3->hide();
   first =true;
   AiMode =false;
   conPlay =true;
   VSmode =false;
   setWindowTitle("五子棋界面");
}

Game::~Game()
{
    delete cless;
    delete ui;
}

void Game::draw23(int changeD){
        QPainter painter(&Pix);//在图片上面修改

        if(first){
        for(int i=0;i<=15;i++)
            painter.drawLine(0,40*i,600,40*i);
        for(int i=0;i<=15;i++)
            painter.drawLine(40*i,0,40*i,600);

            first =false;
            this->update();
        }

        int i = x/40;
        int j = y/40;

        if(i>=0&&j>=0&&i<15&&j<15){

        if(clessArray[i][j].color==0&&changeD==1&&Dchange==changeD){
            painter.setBrush(QBrush(Qt::white));
            clessArray[i][j].color =1;

            painter.drawEllipse((i*40)+1,(j*40)+1,37,37);

                    if(cless->change==Dchange) //点击者才发送消息
                    emit  on_sendCless_msg(i,j);

                    if(cless->CheakWin(clessArray[i][j].color)){
                    //如果自己不是白棋，说明对方胜利  注意 仅仅房主发送/邀请方
                    //使用是否白棋来判断是否房主发送/邀请方

                    msgBox.setText("提示");
                    msgBox.setInformativeText("白棋胜利，确认退出?");
                    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    msgBox.setDefaultButton(QMessageBox::Ok);

                    int ret = msgBox.exec();

                    if(ret == QMessageBox::Ok){

                        first =true;
                        x=-1000;
                        y=-1000;
                        Dchange=1;
                        cless->ClearAll();
                        draw23(cless->change);
                        emit on_sendask_Cheak("Quit");
                        this->close();
                        cout<<"Send Ok"<<endl;

                    }else{
                        if(conPlay)
                        emit on_sendask_Cheak("Ok");

                        else {

                            msgBox.close();
                            return;
                        }
                        }

                }

           Dchange=0;
        }
        else if(clessArray[i][j].color==0&&changeD==0&&Dchange==changeD){
            painter.setBrush(QBrush(Qt::black));
            clessArray[i][j].color =2;
            painter.drawEllipse((i*40)+1,(j*40)+1,37,37);

            if(cless->change==Dchange)
            emit  on_sendCless_msg(i,j);

            if(cless->CheakWin(clessArray[i][j].color)){

                //如果自己不是白棋，说明对方胜利  注意 仅仅房主发送/邀请方  而且是非AI模式

                                msgBox.setText("提示");
                                msgBox.setInformativeText("黑棋棋胜利，确认退出?");
                                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                                msgBox.setDefaultButton(QMessageBox::Ok);

                                int ret = msgBox.exec();

                                if(ret == QMessageBox::Ok){

                                    first =true;
                                    x=-1000;
                                    y=-1000;
                                    Dchange=1;
                                    cless->ClearAll();
                                    draw23(cless->change);
                                    emit on_sendask_Cheak("Quit");
                                    this->close();
                                }else{
                                    if(conPlay)
                                    emit on_sendask_Cheak("Ok");
                                    else {

                                        msgBox.close();
                                        return;
                                    }
                                    }


                    }

           Dchange=1;

         }
        }else{
            return;
        }


        this->update();
}

void Game::paintEvent(QPaintEvent * a){

    QPainter painter(this);
        painter.drawPixmap(0, 0,600, 600,Pix);

        if(!AiMode){
        if(Dchange==0){
            painter.setBrush(QBrush(Qt::black));
            painter.drawEllipse(670,350,37,37);
            cout<<"black color"<<endl;

        }else if(Dchange==1){

            painter.setBrush(QBrush(Qt::white));
            painter.drawEllipse(670,350,37,37);
            cout<<"white color"<<endl;

        }

        }else{
                //人机模式显示棋子颜色
                painter.setBrush(QBrush(Qt::white));
                painter.drawEllipse(670,350,37,37);
                cout<<"white color"<<endl;
        }
    }

void Game::mousePressEvent(QMouseEvent *event){

    if(event->button() == Qt::LeftButton&&Dchange==cless->change)
    {
        this->x=event->pos().x();
        this->y=event->pos().y();

        int i = x/40;
        int j = y/40;
        if(x>0&&y>0&&y<600&&x<600){
        if(!AiMode){

        if(Dchange ==cless->change)
        draw23(cless->change);//自己的回合，画自己颜色的

        }else{
            //玩家为白色  change =1; 为1 为白棋

            if(Dchange==1&&clessArray[i][j].color==0){
            QPainter painter(&Pix);
            painter.setBrush(QBrush(Qt::white));
            clessArray[i][j].color =1;
            painter.drawEllipse((i*40)+1,(j*40)+1,37,37);


            AIandPlayDraw(1);

            Dchange=0;
           cless->change =0;//AI 行动
           }

            if(cless->CheakWin(1)){

                QMessageBox msgBox;
                msgBox.setText("提示");
                msgBox.setInformativeText("白棋胜利，确认退出?");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Ok);

                int ret = msgBox.exec();

                if(ret == QMessageBox::Ok){
                    Pix=QPixmap(600, 600);
                    Pix.fill(Qt::white);
                    first =true;
                    this->x=-1000;
                    this->x=-1000;
                    Dchange=1;
                    cless->change=1;
                    cless->ClearAll();
                    AIandPlayDraw(1);
                    this->close();
                    AiMode =false;
                }else{
                    Pix=QPixmap(600, 600);
                    Pix.fill(Qt::white);
                    first =true;
                    this->x=-1000;
                    this->x=-1000;
                    Dchange=1;
                    cless->change=1;
                    cless->ClearAll();
                    AIandPlayDraw(1);

                }
            }

            if(Dchange==0){

            AI_ClessDown();

            if(cless->CheakWin(2)){
                QMessageBox msgBox;
                msgBox.setText("提示");
            msgBox.setInformativeText("黑棋胜利，确认退出?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();

            if(ret == QMessageBox::Ok){
                Pix=QPixmap(600, 600);
                Pix.fill(Qt::white);
                first =true;
                this->x=-1000;
                this->x=-1000;
                Dchange=1;
                cless->change=1;
                cless->ClearAll();
                AIandPlayDraw(1);
                this->close();
                AiMode =false;
            }else{
                Pix=QPixmap(600, 600);
                Pix.fill(Qt::white);
                first =true;
                this->x=-1000;
                this->x=-1000;
                Dchange=1;
                cless->change=1;
                cless->ClearAll();
                AIandPlayDraw(1);

            }

            }

            }

        }

        }
    }
}

void Game::AIandPlayDraw(int a){

    QPainter painter(&Pix);//在图片上面修改

    if(first){
    for(int i=0;i<=15;i++)
        painter.drawLine(0,40*i,600,40*i);

    for(int i=0;i<=15;i++)
        painter.drawLine(40*i,0,40*i,600);
        first =false;

    }
    cout<<"Dchange   "<<Dchange<<endl;
    this->update();

}

void Game::on_pushButton_2_clicked()
{
    //认输
    QMessageBox msgBox;
    msgBox.setText("您将认输");
    msgBox.setInformativeText("是否确认认输?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();
    if(cless->change==0){

    if(ret == QMessageBox::Ok){
        //发送自己失败
        Pix=QPixmap(600, 600);
        Pix.fill(Qt::white);
        first =true;
        x=-1000;
        y=-1000;
        Dchange=1;
        cless->ClearAll();
        if(AiMode ==true){
            this->close();
        draw23(cless->change);
        return;
        }
         draw23(cless->change);
        emit on_sendFaile();

        //发送报文 还要确认对方是否继续
    }

    }else if(cless->change==1){

        if(ret == QMessageBox::Ok){
            Pix=QPixmap(600, 600);
            Pix.fill(Qt::white);
            first =true;
            x=-1000;
            y=-1000;
            Dchange=1;
            cless->ClearAll();
            if(AiMode ==true){
                this->close();
            draw23(cless->change);
            return;
            }
             draw23(cless->change);
            emit on_sendFaile();
            //发送报文 还要确认对方是否继续
            emit on_EndVsgame(againestName,againestName,"b");
        }
    //
    }
}

void Game::on_QuitPB_clicked()
{
    //退出
    QMessageBox msgBox;
    msgBox.setText("是否退出");
    msgBox.setInformativeText("视为您输掉了本局对局");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();


    if(ret == QMessageBox::Ok){
        //发送自己失败
        Pix=QPixmap(600, 600);
        Pix.fill(Qt::white);
        first =true;
        x=-1000;
        y=-1000;

        if(AiMode ==true){
        Dchange=1;
        cless->ClearAll();
        draw23(cless->change);

        //告诉对面我已经退出
        this->close();
        }
        emit on_EndVsgame(againestName,"Myname","b");
        Dchange=1;
        cless->ClearAll();
        draw23(cless->change);
         emit on_sendask_Cheak("Quit");

        this->close();
    }
}

void Game::SetName(string name){
    this->ui->label->clear();
    this->ui->label->setText(QString::fromStdString(name));
}

void Game::AttackAnalyse(){
    // 右下,一个棋 为  1 越近越大
    int x = this->x /40;
    int y= this->y/40;
        int l, k;
        int j = y - 4;
        int temp = 0;
        //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
        for (int i = x - 4; i < x && j < y; i += 1, j += 1) {
            if (i < 0 || j < 0)continue;
            l = i, k = j;
            if (clessArray[i][j].color == 0) {
                while (1) {
                    if (l > i + 4 || k > j + 4) break;
                    if (l > 14 || k > 14)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l++, k++;
                }
                l = i, k = j;
                while (1) {
                    if (l < i - 4 || k < i - 4) break;
                    if (l < 0 || k < 0)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l--, k--;
                }
            }
            temp = 0;
        }
        j = y + 4;
        temp = 0;
        for (int i = x + 4; i > x && j > y; i -= 1, j -= 1) {
            if (i > 15 || j > 15)continue;
            l = i, k = j;
            if (clessArray[i][j].color == 0) {
                while (1) {
                    if (l < i - 4 || k < i - 4) break;
                    if (l < 0 || k < 0)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l--, k--;
                }
                l = i, k = j;
                while (1) {
                    if (l > i + 4 || k > j + 4) break;
                    if (l > 14 || k > 14)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l++, k++;
                }
            }
            temp = 0;
        }
        //水平
        j = y - 4;
        temp = 0;
        //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
        for (int i = x; j < y; j += 1) {
            if (j < 0)continue;
            l = i, k = j;
            if (clessArray[i][j].color == 0) {
                while (1) {
                    if (k > j + 4) break;
                    if (k > 14)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    k++;
                }
                //===================================
                l = i, k = j;
                while (1) {
                    if (k < j - 4)break;
                    if (k < 0)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    k--;
                }
                //===========================
            }
            temp = 0;
        }
        j = y + 4;
        temp = 0;
        //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
        for (int i = x; j > y; j -= 1) {
            if (j > 15)continue;
            l = i, k = j;
            if (clessArray[i][j].color == 0) {
                while (1) {
                    if (k < j - 4) break;
                    if (k < 0) break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    k--;
                }
                l = i, k = j;
                while (1) {
                    if (k > j + 4) break;
                    if (k > 14)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    k++;
                }
            }
            temp = 0;
        }
        //垂直
        j = y;
        temp = 0;
        //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
        for (int i = x - 4; i < x; i += 1) {
            if (i < 0)continue;
            l = i, k = j;
            if (clessArray[i][j].color == 0) {
                while (1) {
                    if (l > i + 4) break;
                    if (l > 14)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l++;
                }
                l = i, k = j;
                while (1) {
                    if (l < i - 4) break;
                    if (l < 0) break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l--;
                }
            }
            temp = 0;
        }
        j = y;
        temp = 0;
        //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
        for (int i = x + 4; i > x; i -= 1) {
            if (i > 15)continue;
            l = i, k = j;
            if (clessArray[i][j].color == 0) {
                while (1) {
                    if (l < i - 4) break;
                    if (l < 0) break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l--;
                }
                l = i; k = j;
                while (1) {
                    if (l > i + 4) break;
                    if (l > 14)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l++;
                }
            }
            temp = 0;
        }
        j = y + 4;
        temp = 0;
        //右上
        for (int i = x - 4; i < x && j >y; i += 1, j -= 1) {
            if (i < 0 || j>15)continue;
            l = i, k = j;
            if (clessArray[i][j].color == 0) {
                while (1) {
                    if (l > i + 4 || k < j - 4) break;
                    if (l > 14 || k < 0)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l++, k--;
                }
                l = i, k = j;
                while (1) {
                    if (l < i - 4 || k > j + 4) break;
                    if (l > 14 || k > 14)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l--, k++;
                }
            }
            temp = 0;
        }
        j = y - 4;
        temp = 0;
        //右上
        for (int i = x + 4; i > x && j < y; i -= 1, j += 1) {
            if (i > 15 || j < 0)continue;
            l = i, k = j;
            if (clessArray[i][j].color == 0) {
                while (1) {
                    if (l < i - 4 || k > j + 4) break;
                    if (l > 14 || k > 14)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l--, k++;
                }
                l = i, k = j;
                while (1) {
                    if (l > i + 4 || k < j - 4) break;
                    if (l > 14 || k < 0)break;
                    if (clessArray[l][k].color == 2) {
                        temp++;
                        clessArray[i][j].AValues += temp;
                    }
                    else {
                        if (clessArray[l][k].color == 1) {
                            clessArray[i][j].AValues--;
                            temp = 0;
                        }
                    }
                    l++, k--;
                }
            }
            temp = 0;
        }
}

void Game::DefendAnalyse(){
    // 右下,一个棋 为  1 越近越大
    int x = this->x /40;
    int y= this->y/40;
    int l, k;
    int j = y - 4;
    int temp = 0;
    //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
    for (int i = x - 4; i < x && j < y; i += 1, j += 1) {
        if (i < 0 || j < 0)continue;
        l = i, k = j;
        if (clessArray[i][j].color == 0) {
            while (1) {
                if (l > i + 4 || k > j + 4) break;
                if (l > 14 || k > 14)break;
                if (clessArray[l][k].color == 1) {
                    temp++;
                    clessArray[i][j].DValues += temp;
                }
                else {
                    if (clessArray[l][k].color == 2) {

                        temp = 0;
                    }
                }
                l++, k++;
            }

        }
    }
    j = y + 4;
    temp = 0;
    for (int i = x + 4; i > x && j > y; i -= 1, j -= 1) {
        if (i > 15 || j > 15)continue;
        l = i, k = j;
        if (clessArray[i][j].color == 0) {
            while (1) {
                if (l < i - 4 || k < i - 4) break;
                if (l < 0 || k < 0)break;
                if (clessArray[l][k].color == 1) {
                    temp++;
                    clessArray[i][j].DValues += temp;
                }
                else {
                    if (clessArray[l][k].color == 2) {

                        temp = 0;
                    }
                }
                l--, k--;
            }

        }
    }
    //水平
    j = y - 4;
    temp = 0;
    //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
    for (int i = x; j < y; j += 1) {
        if (j < 0)continue;
        l = i, k = j;
        if (clessArray[i][j].color == 0) {
            while (1) {
                if (k > j + 4) break;
                if (k > 14)break;
                if (clessArray[l][k].color == 1) {
                    temp++;
                    clessArray[i][j].DValues += temp;
                }
                else {
                    if (clessArray[l][k].color == 2) {

                        temp = 0;
                    }
                }
                k++;
            }

        }
    }
    j = y + 4;
    temp = 0;
    //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
    for (int i = x; j > y; j -= 1) {
        if (j > 15)continue;
        l = i, k = j;
        if (clessArray[i][j].color == 0) {
            while (1) {
                if (k < j - 4) break;
                if (k < 0) break;
                if (clessArray[l][k].color == 1) {
                    temp++;
                    clessArray[i][j].DValues += temp;
                }
                else {
                    if (clessArray[l][k].color == 2) {

                        temp = 0;
                    }
                }
                k--;
            }

        }
    }
    //垂直
    j = y;
    temp = 0;
    //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
    for (int i = x - 4; i < x; i += 1) {
        if (i < 0)continue;
        l = i, k = j;
        if (clessArray[i][j].color == 0) {
            while (1) {
                if (l > i + 4) break;
                if (l > 14)break;
                if (clessArray[l][k].color == 1) {
                    temp++;
                    clessArray[i][j].DValues += temp;
                }
                else {
                    if (clessArray[l][k].color == 2) {

                        temp = 0;
                    }
                }
                l++;
            }

        }
    }
    j = y;
    temp = 0;
    //寻找当前x y点处  4*4 大小内的对面的棋和空位里面，有没有棋
    for (int i = x + 4; i > x; i -= 1) {
        if (i > 15)continue;
        l = i, k = j;
        if (clessArray[i][j].color == 0) {
            while (1) {
                if (l < i - 4) break;
                if (l < 0) break;
                if (clessArray[l][k].color == 1) {
                    temp++;
                    clessArray[i][j].DValues += temp;
                }
                else {
                    if (clessArray[l][k].color == 2) {

                        temp = 0;
                    }
                }
                l--;
            }

        }
    }
    j = y + 4;
    temp = 0;
    //右上
    for (int i = x - 4; i < x && j >y; i += 1, j -= 1) {
        if (i < 0 || j>15)continue;
        l = i, k = j;
        if (clessArray[i][j].color == 0) {
            while (1) {
                if (l > i + 4 || k < j - 4) break;
                if (l > 14 || k < 0)break;
                if (clessArray[l][k].color == 1) {
                    temp++;
                    clessArray[i][j].DValues += temp;
                }
                else {
                    if (clessArray[l][k].color == 2) {

                        temp = 0;
                    }
                }
                l++, k--;
            }

        }
    }
    j = y - 4;
    temp = 0;
    //右上
    for (int i = x + 4; i > x && j < y; i -= 1, j += 1) {
        if (i > 15 || j < 0)continue;
        l = i, k = j;
        if (clessArray[i][j].color == 0) {
            while (1) {
                if (l < i - 4 || k > j + 4) break;
                if (l > 14 || k > 14)break;
                if (clessArray[l][k].color == 1) {
                    temp++;
                    clessArray[i][j].DValues += temp;
                }
                else {
                    if (clessArray[l][k].color == 2) {

                        temp = 0;
                    }
                }
                l--, k++;
            }

        }
    }
}

bool Game::AI_ClessDown(){
    DefendAnalyse();
    AttackAnalyse();

    int maxx,maxy;
        deque<pair<int, int>> v;
        int temp = 1, D = 1;

        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j += 1) {
                if (clessArray[i][j].AValues > temp) {
                    v.clear();
                    temp = clessArray[i][j].AValues;
                    v.push_back(make_pair(i, j));
                }
                else if (clessArray[i][j].AValues == temp) {
                    v.push_back(make_pair(i, j));
                }
            }
        }

        deque<pair<int, int>> x;
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j += 1) {
                if (clessArray[i][j].DValues > D) {
                    x.clear();
                    D = clessArray[i][j].DValues;
                    x.push_back(make_pair(i, j));
                }
                else if (clessArray[i][j].DValues == D) {
                    x.push_back(make_pair(i, j));
                }
            }
        }

        if (D-1 > temp) {
        if (x.size() != 0) {
            int i;
            i = rand() % x.size();
            maxx = x[i].first;
            maxy = x[i].second;
        }
        }else if (temp > D-1 ) {
            if (x.size() != 0) {
                int i;
                i = rand() % v.size();
                maxx = v[i].first;
                maxy = v[i].second;
            }
        }

        if (D-1 == temp) {//多次测试发现D-1,AI的智力会提升一些
            if (x.size() != 0) {
                int i;
                i = rand() % v.size();
                maxx = v[i].first;
                maxy = v[i].second;
            }
            for (int j = 0; j < x.size(); j++) {
                for (int k = 0; k < v.size(); k++) {
                    if (x[j] == v[k]) {
                        maxx = x[j].first;
                        maxy = x[j].second;
                    }
                }
            }
        }

        if (clessArray[maxx][maxy].color == 0) {
            //遍历棋盘，得到最大权值的位置
            QPainter painter(&Pix);
            painter.setBrush(QBrush(Qt::black));

            clessArray[maxx][maxy].color =2;
            painter.drawEllipse((maxx*40)+1,(maxy*40)+1,37,37);
            //下棋  绘图
            this->update();
            Dchange=1;
            cless->change =1;
        }

        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if(clessArray[i][j].color !=0)
                clessArray[i][j].AValues = 0;
                clessArray[i][j].DValues = 0;
            }
        }
}

void  Game::on_Deal_AIgame(){
    cout<<"on_Deal_AIgame"<<endl;
    this->show();
    this->ui->label->setText(QString::fromStdString("AI"));
    this->ui->label_3->show();
    AiMode =true;
    first =true;
    //this->ui->GameChat->hide();
    Dchange =1;
    Pix=QPixmap(600, 600);
    Pix.fill(Qt::white);
    AIandPlayDraw(1);
}


