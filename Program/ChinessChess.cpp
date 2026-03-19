#include<bits/stdc++.h>
#include<windows.h>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;
int table[11][10];
string ptr[11][11];
//string red[7]={"車","馬","相","仕","帅","兵","炮"};
//string black[7]={"车","马","象","士","将","卒","砲"};
string allChess[14]= {"車","馬","相","仕","帅","兵","炮","车","马","象","士","将","卒","砲"};
string shiJian;
string lastShiJian;
bool redTurn;
bool takeChess;
//const string shangChuHe="┴";
//const string xiaChuHe="┬";
//const string dianWei="┼";

const string shangChuHe = "┻";
const string xiaChuHe = "┳";
const string dianWei = "╋";
static bool gameEnd;
static string whoWin;

struct Pointer {
	int x;
	int y;
} curPointer,afterPointer,beginChess,endChess;


void init() { //初始化棋盘
	whoWin = "无";
	gameEnd = false;
	curPointer.x=1;
	curPointer.y=1;
	redTurn = true;
	shiJian = "";
	lastShiJian = "";
	takeChess = 0;
	for(int i=1; i<=10; i++) {
		for(int j=1; j<=9; j++) {
			table[i][j]=15;
			ptr[i][j]="  ";
		}
	}
	ptr[curPointer.y][curPointer.x]="→";
	ptr[curPointer.y][curPointer.x+1]="←";
	for(int i=0; i<=5; i++) {
		table[10][i+1]=i;
	}
	int ij=6;
	for(int i=3; i>=0; i--) {
		table[10][ij]=i;
		ij++;
	}
	for(int i=0; i<=5; i++) {
		table[1][i+1]=7+i;
	}

	ij=6;
	for(int i=3; i>=0; i--) {
		table[1][ij]=7+i;
		ij++;
	}
	for(int i=1; i<=9; i++) {
		table[5][i]=16;
		table[6][i]=17;
	}
	for(int i=1; i<=9;) {
		table[4][i]=12;
		table[7][i]=5;
		i+=2;
	}
	for(int i=2; i<=9;) {
		table[3][i]=13;
		table[8][i]=6;
		i+=6;
	}
}



void myprint() { //打印
	string buffer;
	
	for(int i=1; i<=10; i++) {
		buffer="";
		for(int j=1; j<=9; j++) {
//			cout<<ptr[i][j];
			
			buffer+=ptr[i][j];
			if(table[i][j]>=0 and table[i][j]<=13) {
//				cout<<allChess[table[i][j]];
				buffer+=allChess[table[i][j]];
			} else {
				switch(table[i][j]) {
					case 15:
//						cout<<dianWei;
						buffer+=dianWei;
						break;
					case 16:
//						cout<<shangChuHe;
						buffer+=shangChuHe;
						break;
					case 17:
//						cout<<xiaChuHe;
						buffer+=xiaChuHe;
						break;
				}
				buffer += " ";
			}
			
		}
//		cout<<ptr[i][10];
		buffer+=ptr[i][10];
		cout<<buffer;
		cout<<endl;
		cout<<endl;
	}
	buffer="上个事件：";
	buffer+=lastShiJian;
	cout<<buffer;
	cout<<endl;
	buffer="当前："+shiJian;
	cout<<buffer;
	cout<<endl;
	if(redTurn) {
		
		buffer="现在到红方";
	} else {
		buffer="现在到黑方";
	}
	cout<<buffer;
	cout<<endl;
}


void changePointer(int direction) {
	afterPointer=curPointer;
	switch(direction) {
		case 1:
			afterPointer.y+=1;
			break;
		case 2:
			afterPointer.y-=1;
			break;
		case 3:
			afterPointer.x-=1;
			break;
		case 4:
			afterPointer.x+=1;
			break;
		default:
			break;
	}
	if(afterPointer.x>0 and afterPointer.y>0 and afterPointer.x<10 and afterPointer.y<11) {
		ptr[curPointer.y][curPointer.x]="  ";
		ptr[curPointer.y][curPointer.x+1]="  ";
		curPointer=afterPointer;
		ptr[curPointer.y][curPointer.x]="→";
		ptr[curPointer.y][curPointer.x+1]="←";
	}
}
void playError() {
	shiJian="你不能这么做！";
	//shiJian+=beginChess.x+'0';
	//shiJian+=",";
	//
	//shiJian+=beginChess.y+'0';
	//shiJian+=";;";
	//shiJian+=curPointer.x+'0';
	//shiJian+=",";
	//shiJian+=curPointer.y+'0';
	//shiJian+=",";


	takeChess=0;
}
void gameOver() {

	shiJian = whoWin+"方胜利！按任意键重新开始游戏。";
	gameEnd = true;
}
void fangZhi() {
	bool laoJiang=0;
	if(table[curPointer.y][curPointer.x]==4 or table[curPointer.y][curPointer.x]==11) {
		if (table[curPointer.y][curPointer.x] == 4) {
			whoWin = "黑";
		}
		else {
			whoWin = "红";
		}
		laoJiang=1;
	}
	table[curPointer.y][curPointer.x]=table[beginChess.y][beginChess.x];
	shiJian="";
	lastShiJian="移动了：";


	if(redTurn) {
		redTurn=0;
	} else {
		redTurn=1;
	}
	lastShiJian+=allChess[table[beginChess.y][beginChess.x]];

	if(beginChess.y==5) {
		table[beginChess.y][beginChess.x]=16;
	} else if(beginChess.y==6) {
		table[beginChess.y][beginChess.x]=17;
	} else {
		table[beginChess.y][beginChess.x]=15;
	}
	if(laoJiang) {
		gameOver();
	}
}

void myMove() {
//	string chess;
	int chess;
	int myStart,myEnd;
	int myDistance;
	bool red;
	int operation1,operation2;
	bool myHorizontal; //是否平移
	bool mountain;//炮是否隔一个子
	chess=table[beginChess.y][beginChess.x];
	switch(chess) {
		case 5://5,12是兵
			if(curPointer.y == beginChess.y or curPointer.x==beginChess.x) {
				if(curPointer.x==beginChess.x) {
					if(beginChess.y-curPointer.y==1) {
						fangZhi();
						return;

					}
				} else if(beginChess.y<=5) {
					if(abs(curPointer.x-beginChess.x)==1) {
						fangZhi();
						return;
					}
				}
			}
			playError();
			break;
		case 12:
			if(curPointer.y == beginChess.y or curPointer.x==beginChess.x) {
				if(curPointer.x==beginChess.x) {
					if(curPointer.y-beginChess.y==1) {
						fangZhi();
						return;

					}
				} else if(beginChess.y>=6) {
					if(abs(curPointer.x-beginChess.x)==1) {
						fangZhi();
						return;
					}
				}
			}
			playError();
			break;
		case 0://0和7是车
CHE:
			if(curPointer.y == beginChess.y or curPointer.x==beginChess.x) {
				if(curPointer.y == beginChess.y) { //平移
					myHorizontal=1;
					if(curPointer.x-beginChess.x>0) {
						myStart=beginChess.x;
						myEnd=curPointer.x;
					} else {
						myStart=curPointer.x;
						myEnd=beginChess.x;
					}
				}

				else { //竖直移动
					myHorizontal=0;
					if(curPointer.y>beginChess.y) {
						myStart=beginChess.y;
						myEnd=curPointer.y;
					} else {
						myEnd=beginChess.y;
						myStart=curPointer.y;
					}
				}
				for(int i=myStart+1; i<myEnd; i++) {
					if(myHorizontal) {
						if(table[curPointer.y][i]<=13) {
							playError();
							return;
						}
					} else {
						if(table[i][curPointer.x]<=13) {
							playError();
							return;
						}
					}

				}

				fangZhi();
				return;
			} else {
				playError();
			}

			break;
		case 7:
			goto CHE;
			break;
		case 6://炮
PAO:
			mountain=0;
			if(curPointer.y == beginChess.y or curPointer.x==beginChess.x) {
				if(curPointer.y == beginChess.y) { //平移
					myHorizontal=1;
					if(curPointer.x-beginChess.x>0) {
						myStart=beginChess.x;
						myEnd=curPointer.x;
					} else {
						myStart=curPointer.x;
						myEnd=beginChess.x;
					}
				}

				else { //竖直移动
					myHorizontal=0;
					if(curPointer.y>beginChess.y) {
						myStart=beginChess.y;
						myEnd=curPointer.y;
					} else {
						myEnd=beginChess.y;
						myStart=curPointer.y;
					}
				}
				for(int i=myStart+1; i<myEnd; i++) {
					if(myHorizontal) {
						if(table[curPointer.y][i]<=13) {
							if(!mountain) {
								mountain=1;
							} else {
								playError();
								return;
							}

						}
					} else {
						if(table[i][curPointer.x]<=13) {
							if(!mountain) {
								mountain=1;
							} else {
								playError();
								return;
							}
						}
					}

				}
				if(mountain) {
					if(table[curPointer.y][curPointer.x]>13) {
						playError();
					} else {
						fangZhi();
//						shiJian="findbug";
						return;


					}
//					fangZhi();
//					return;
				} else {
					if(table[curPointer.y][curPointer.x]>13) {
						fangZhi();
						return;
					} else {
//						shiJian="findbug";
						playError();
					}

				}
			} else {
				playError();
			}
			break;
		case 13:
			goto PAO;
			break;
		case 4:
JIANGJUN:
			myDistance=pow(abs(curPointer.x-beginChess.x),2)+pow(abs(curPointer.y-beginChess.y),2);
//		int bianJie;

			if(curPointer.y == beginChess.y or curPointer.x==beginChess.x) {
				if(myDistance==1) {
//				if(curPointer.)
					if(table[beginChess.y][beginChess.x]==4) {
						red=1;
//bianJie=
					} else {
						red=0;
					}
					if(curPointer.x>=4 and curPointer.x<=6) {
						if(red) {
							if(curPointer.y<=10 and curPointer.y>=8) {
								fangZhi();
								return;
							} else {
								playError();
								return;
							}
						} else {
							if(curPointer.y>=1 and curPointer.y<=3) {
								fangZhi();
								return;
							} else {
								playError();
								return;
							}
						}
					} else {
						playError();
						return;
					}
				} else {
					if(table[curPointer.y][curPointer.x]==4 or table[curPointer.y][curPointer.x]==11) {
						myEnd=max(curPointer.y,beginChess.y);
						myStart=min(curPointer.y,beginChess.y);
						for(int i=myStart+1; i<myEnd; i++) {
							if(table[i][curPointer.x]<=13) {
								playError();
								return;
							}
						}
					} else {
						playError();
						return;
					}
				}
			} else {
				playError();
				return;
			}
//			curPointer.x;

			break;
		case 11:
			goto JIANGJUN;
			break;
		case 3:
SHI:
			myDistance=pow(abs(curPointer.x-beginChess.x),2)+pow(abs(curPointer.y-beginChess.y),2);
			if(myDistance==2) {
				if(table[beginChess.y][beginChess.x]==3) {
					red=1;
//bianJie=
				} else {
					red=0;
				}
				if(curPointer.x>=4 and curPointer.x<=6) {
					if(red) {
						if(curPointer.y<=10 and curPointer.y>=8) {
							fangZhi();
							return;
						} else {
							playError();
							return;
						}
					} else {
						if(curPointer.y>=1 and curPointer.y<=3) {
							fangZhi();
							return;
						} else {
							playError();
							return;
						}
					}
				} else {
					playError();
					return;
				}
			} else {
				playError();
				return;
			}
			break;
		case 10:
			goto SHI;
			break;
		case 2:
XIANG:
			myDistance=pow(abs(curPointer.x-beginChess.x),2)+pow(abs(curPointer.y-beginChess.y),2);
			if(myDistance!=8) {
				playError();
				return;
			}
			if(table[beginChess.y][beginChess.x]==2){
				if(curPointer.y<=5){
					playError();
				return;
				}
			}
			else{
				if(curPointer.y>=6){
					playError();
				return;
				}
			}
			operation1=curPointer.x-beginChess.x;
			operation2=curPointer.y-beginChess.y;
			switch(operation1) {
				case 2:
					switch(operation2) {
						case 2:
							if(table[beginChess.y+1][beginChess.x+1]<=13) {
								playError();
								return;
							}
							break;
						case -2:
							if(table[beginChess.y-1][beginChess.x+1]<=13) {
								playError();
								return;
							}
							break;
					}

					break;
				case -2:
					switch(operation2) {
						case 2:
							if(table[beginChess.y+1][beginChess.x-1]<=13) {
								playError();
								return;
							}
							break;
						case -2:
							if(table[beginChess.y-1][beginChess.x-1]<=13) {
								playError();
								return;
							}
							break;
					}
					break;
			}
			fangZhi();

			break;
		case 9:
			goto XIANG;
			break;
		case 1:
MA:
			myDistance=pow(abs(curPointer.x-beginChess.x),2)+pow(abs(curPointer.y-beginChess.y),2);
			if(myDistance!=5) {
				playError();
				return;
			}
//	bool leftOrRight=0;
//	bool upOrDown=0;
			operation1=curPointer.x-beginChess.x;
			switch(operation1) {
				case 2:
					if(table[beginChess.y][beginChess.x+1]<=13) {
						playError();
						return;
					}
					break;
				case -2:
					if(table[beginChess.y][beginChess.x-1]<=13) {
						playError();
						return;
					}

					break;
			}
			operation1=curPointer.y-beginChess.y;
			switch(operation1) {
				case 2:
					if(table[beginChess.y+1][beginChess.x]<=13) {
						playError();
						return;
					}
					break;
				case -2:
					if(table[beginChess.y-1][beginChess.x]<=13) {
						playError();
						return;
					}
					break;
			}
			fangZhi();
			break;
		case 8:
			goto MA;
			break;

		default:
			cout<<"error!"<<endl;
	}
}


bool canTakeChess(bool isRed) {
	int chess;
	chess=table[curPointer.y][curPointer.x];
	if(isRed) {
		if(chess<=6) {
			return 1;
		}
		return 0;
	} else {
		if(chess<=13 and chess>6) {
			return 1;
		}
		return 0;
	}
}


void check() {
	int direction;
	direction=0;
	if(KEY_DOWN('S') or KEY_DOWN(40)) {
		direction=1;
	} else if(KEY_DOWN('W') or KEY_DOWN(38)) {
		direction=2;
	} else if(KEY_DOWN('A') or KEY_DOWN(37)) {
		direction=3;
	} else if(KEY_DOWN('D') or KEY_DOWN(39)) {
		direction=4;
	} else if(KEY_DOWN(32)) {
//		shiJian="1234";
		if(table[curPointer.y][curPointer.x]<=13) {
			if(canTakeChess(redTurn)) {
				beginChess.y=curPointer.y;
				beginChess.x=curPointer.x;
				shiJian="拿起了：";
				shiJian+=allChess[table[beginChess.y][beginChess.x]];
				takeChess=1;
			} else {
				if(takeChess) {
					myMove();
					takeChess=0;
				} else {
					playError();
				}
			}

		} else {
			if(takeChess) {
				myMove();
				takeChess=0;
			}
		}
	}
	if(direction!=0) {
		changePointer(direction);
	}
}

int main() {
	RESTART:
	init();
	while(true) {
		myprint();
		system("pause");
		if (gameEnd) {
			system("cls");
			goto RESTART;
		}
		check();
		system("cls");
	}

	return 0;
}
