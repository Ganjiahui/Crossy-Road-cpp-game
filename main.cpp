//Project "Crossing Road"
//GAN JIA HUI A21EC0177 
//GAN QI YOU A21EC0178
//TAN KAI YUAN A21EC0230
//OH HONG MING B21EC3006

#include <iostream>
#include <graphics.h>
#include <time.h>
#include <fstream>
#include <cctype>
using namespace std;

#define ESC 27
#define UP 72
#define LEFT 75
#define RIGHT 77
#define KEY_F5 63

class Game{
    protected:
        int x1, y1, x2, y2, carX1, carX2, count, score;
		int carY1[8]={600, 525, 450, 375, 300, 225, 150};
		int carY2[8]={630, 555, 480, 405, 330, 255, 180};
		int f[7],t[7];
        char Input;
        string user;
    public:
        Game();
        void border(int, int, int, int);
        void border() const{setlinestyle(SOLID_LINE,1,1); rectangle(x1, y1, x2, y2);};
        void startLine(int, int, int, int) const;
        void endLine(int, int, int, int) const;
        void setUsername();
        void reset(){cleardevice();};
        void gameOverText();
        string getUsername() const{return user;};
};

Game::Game(){
	x1=30;
	y1=30;
	x2=360;
	y2=760;
}

void Game::border(int X1, int Y1, int X2, int Y2){
	x1=X1;
	y1=Y1;
	x2=X2;
	y2=Y2;
}

void Game::startLine(int X1, int Y1, int X2, int Y2) const{
	setlinestyle(DASHED_LINE,1,1);
	line(X1,Y1,X2,Y2);
}

void Game::endLine(int X1, int Y1, int X2, int Y2) const{
	setlinestyle(DASHED_LINE,1,1);
	line(X1,Y1,X2,Y2);
}

void Game::setUsername(){
	cout<<"Please enter your username (5 letters only): ";
	cin>>user;
	for (int i=0; i<5; i++) {
        user[i]=toupper(user[i]);
    }
}


void Game::gameOverText(){
	settextstyle(2,HORIZ_DIR,10);
	outtextxy(50,300,"GAME OVER");
	settextstyle(2,HORIZ_DIR,6);
	outtextxy(95,340,"Press Esc to quit & save");
	outtextxy(125,360,"Press F5 to save");
	outtextxy(95,380,"Press Any key to continue");
}



class Score:public Game{
	protected:
		int num, save[10];
    public:
        Score(){num=0; save[10]={}; count=0; score=0;};
        void setScore(int);
        void setCount(int);
        void setSave();
        void displayScore();
        void leaderboard(int);
        int getScore(int) const;
};

void Score::setScore(int s){
	if(s<=0&&count<=0){
		score=0;
	}else if(s==0&&count>=0){
		score=0;
		score=26*count;
	}else{
		score+=s;
	}
}

void Score::setCount(int c){
	if(c==0){
		count=0;
	}
	count+=c;
}

void Score::setSave(){
	save[num]=score;
	num++;
}

void Score::displayScore(){
	char text_score[5+sizeof(char)];
	sprintf(text_score, "%d", score);
	settextstyle(2,HORIZ_DIR,6);
	outtextxy(30,5,"Score:");
	outtextxy(85,5,text_score);
}

void Score::leaderboard(int i){
	cout<<"Leaderboard: "<<"\n\n"
		<<i+1<<". "<<user<<" "<<score<<"\n";
}

int Score::getScore(int i) const{
	return save[i];
}
 
class Player: public Score{
	protected:
        int player_x, player_y, A;
        char input;
    public:
        Player();
        void setA(int a){A+=a;};
		void move();
		void clear();
		void reset();
		int getPX() const{return player_x;};
		int getPY() const{return player_y;};
		int getA() const{return A;};
		bool gameOver();
};

Player::Player(){
	player_x=200;
	player_y=700;
	A=0;
}

void Player::clear(){
	player_x=200;
	player_y=700;
	setCount(0);
	setScore(0);
}

void Player::reset(){
	player_y=700;
	setCount(1);
	setScore(0);
}

void Player::move(){						// 26 steps to reach end
	settextstyle(10,HORIZ_DIR,5);
	outtextxy(player_x,player_y,"^");
	if (kbhit()){
		input=getch();
		switch (input){
			case 'w':
			case 'W':
			case UP:
				if(player_y>50){
					player_y-=25;
					setScore(1);
				}
				break;
			case 'a':
			case 'A':
			case LEFT:
				if(player_x>45){
					player_x-=15;
				}
				break;
			case 'd':
    		case 'D':
			case RIGHT:
				if(player_x<315){
					player_x+=15;
				}
				break;
			case 'r':
			case 'R':
				player_x=200;
				clear();
				break;
		}
		
		if(score==26&&count>=0){
			reset();
			setScore(1);
		}
	}
}

bool Player::gameOver(){
	gameOverText();
	if(!kbhit()){
		Input=getch();
		switch(Input){
			case ESC:
				return false;
			case KEY_F5:
				setSave();
				setA(1);
				break;
			default:
				break;
		}
	}
	return true;
}

class Obstacle:public Game{
	private:
		int choice;
	public:
		Obstacle();
        void speed();
        void car();
		bool hit(Player &p);
		void reset(int);
};

Obstacle::Obstacle(){
	choice=1;
	carX1=35;
	carX2=100;
	f[7]={};
	t[7]={};
}

void Obstacle::speed(){
	for(int a=0; a<7; a++){
		f[a]=rand()%5+3;
		if(f[a]!=f[a-1]&&f[a]!=f[a-2]){
			t[a]=f[a];
		}else{
			--a;
		}
	}      
}

void Obstacle::car(){
	setlinestyle(SOLID_LINE,1,1);
	for(int i=0; i<7; i++){
		rectangle(carX1+f[i],carY1[i],carX2+f[i],carY2[i]);
		reset(i);
	}
}

bool Obstacle::hit(Player &p){
	for(int i=0; i<7; i++){
		int car_box1=(carX1-20)+f[i];
		int car_box2=carX2+f[i];
		if(((car_box1<p.getPX())&&(car_box2>p.getPX()))&&(((carY1[i]-25)<p.getPY())&&((carY2[i]-25)>p.getPY()))){
			choice=p.gameOver();
			if(choice!=0){
				p.clear();
				Obstacle();
				speed();
			}
		}
	}
	return choice;
}

void Obstacle::reset(int I){
	if(I==6){
		for(int a=0; a<7; a++){
			if(f[a]>250){
				f[a]=0;
			}else{
				f[a]+=t[a];
			}
		}
		delay(20);
	}
}
			
int main(){
	srand(time(NULL));
    initwindow(400,800);
    Game g;
    Player p[10];
    Obstacle o;
    int i=0;
    bool start=true;
	delay(100);
    g.border(30,30,360,760);
    o.speed();
    do{
    	p[i].move();
    	p[i].displayScore();
    	g.border();
    	g.startLine(30,680,360,680);
    	g.endLine(30,105,360,105);
    	o.car();
    	start=o.hit(p[i]);
    	g.reset();
    	i=p[i].getA();
    }while(start!=false);
    closegraph();
    ofstream SaveFile("save.txt");
    for(int x=0; x<i+1; x++){
    	p[x].setUsername();
    	p[x].leaderboard(x);
    	int score=p[x].getScore(x);
    	SaveFile<<x+1<<" "<<p[x].getUsername()<<" "<<score<<endl;
	}
	SaveFile.close();
    return 0;
}

/*
Known issue:
-score in save.txt is broken
*/
