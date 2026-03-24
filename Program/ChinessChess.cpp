#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "ChinessChess.h"
#include <algorithm>
#include "OLED.h"
#include "string.h"
#include "Delay.h"
using namespace std;

static int table[11][10];   //从1开始，0位置的不用


typedef enum {
    RED_CHE = 0,
    RED_MA = 1,
    RED_XIANG = 2,
    RED_SHI   = 3,
    RED_SHUAI  = 4,
    RED_BING = 5,
    RED_PAO = 6,
    BLACK_CHE = 7,
    BLACK_MA = 8,
    BLACK_XIANG = 9,
    BLACK_SHI   = 10,
    BLACK_JIANG  = 11,
    BLACK_ZU = 12,
    BLACK_PAO = 13,
    DIAN_WEI = 15,
    SHANG_CHU_HE = 16,
    XIA_CHU_HE = 17,
}ChessIndex;


typedef enum{
	CHESS_ZHENYING_NONE = 0,
	CHESS_ZHENYING_RED = 1,
	CHESS_ZHENYING_BLACK = 2,
}ChessZhenYing;


ChineseIndex chess_to_chinese_map[18] = {
    CHESS_RED_CHE,    // 0
    CHESS_RED_MA,     // 1
    CHESS_RED_XIANG,  // 2
    CHESS_RED_SHI,    // 3
    CHESS_RED_SHUAI,  // 4
    CHESS_RED_BING,   // 5
    CHESS_RED_PAO,    // 6
    CHESS_BLACK_CHE,  // 7
    CHESS_BLACK_MA,   // 8
    CHESS_BLACK_XIANG,// 9
    CHESS_BLACK_SHI,  // 10
    CHESS_BLACK_SHUAI,// 11
    CHESS_BLACK_BING, // 12
    CHESS_BLACK_PAO,  // 13
    SELECTION_BOX,  // 14
    CHESS_DIAN_WEI,   // 15
    CHESS_SHANG_CHU_HE,//16
    CHESS_XIA_CHU_HE   //17
};
ChineseIndex chessToChineseIndex(int idx)
{
    if (idx > 17||idx==14) {
        return SELECTION_BOX; // 或者其他默认值
    }
    return chess_to_chinese_map[idx];
}

static bool isRedTurn;  //是否到红方行动
static bool isTakingChess;  //是否拿起了旗子

static bool gameEnd;
static ChessZhenYing whoWin;

struct Pointer {
	int x;
	int y;
};
static Pointer curPointer; //当前指针
static Pointer beginChess;
static Pointer cameraPointer;

int myAbs(int a){
	return a>0?a:-a;
}

int mySquare(int a){
	return a*a;
}

void updateCamera() {
	// 如果光标跑到摄像机左边
	if(curPointer.x < cameraPointer.x) {
		cameraPointer.x = curPointer.x;
	}
	// 如果光标跑到摄像机右边
	else if(curPointer.x > cameraPointer.x + 7) {
		cameraPointer.x = curPointer.x - 7;
	}
	// 如果光标跑到摄像机上边
	if(curPointer.y < cameraPointer.y) {
		cameraPointer.y = curPointer.y;
	}
	// 如果光标跑到摄像机下边
	else if(curPointer.y > cameraPointer.y + 3) {
		cameraPointer.y = curPointer.y - 3;
	}
	// 边界修正
	if(cameraPointer.x < 1) cameraPointer.x = 1;
	if(cameraPointer.y < 1) cameraPointer.y = 1;
	if(cameraPointer.x > 2) cameraPointer.x = 2;
	if(cameraPointer.y > 7) cameraPointer.y = 7;
}


void init() { //初始化棋盘
	whoWin = CHESS_ZHENYING_NONE;
	gameEnd = false;
	curPointer.x=1;
	curPointer.y=1;
	beginChess.x=5;
	beginChess.y=2;
	cameraPointer.x=1;
	cameraPointer.y=1;
	isRedTurn = true;
	isTakingChess = false;
	for(int i=1; i<=10; i++) {
		for(int j=1; j<=9; j++) {
			table[i][j]=DIAN_WEI;  //把所有位置初始化为点位（1-10 1-9）
		}
	}

	for(int i=1; i<=5; i++) {
		table[10][i]=i-1;   //红方 车 马 象 士 帅
	}
	int ij=3;
	for(int i=6; i<=9; i++) {  //红方 士 象 马 车
		table[10][i]=ij;
		ij--;
	}
	for(int i=1; i<=5; i++) {
		table[1][i]=i+6;   //黑方 车 马 象 士 帅
	}

	ij=10;
	for(int i=6; i<=9; i++) {  //黑方 士 象 马 车
		table[1][i]=ij;
		ij--;
	}
	for(int i=1; i<=9; i++) { //1-9都变为楚河
		table[5][i]=SHANG_CHU_HE;
		table[6][i]=XIA_CHU_HE;
	}
	//初始化兵位置
	for(int i=1; i<=9;) {
		table[4][i]=BLACK_ZU;
		table[7][i]=RED_BING;
		i+=2;
	}
	//初始化炮位置
	for(int i=2; i<=9;) {
		table[3][i]=BLACK_PAO;
		table[8][i]=RED_PAO;
		i+=6;
	}
}

void myPrint(){
	OLED_Clear();

	for(int y = cameraPointer.y; y < cameraPointer.y + 4; y++) {
		for(int x = cameraPointer.x; x < cameraPointer.x + 8; x++) {
			if(x > 9 || y > 10) continue;

			int chess=table[y][x];
			ChineseIndex chineseIndex= chessToChineseIndex(chess);
			uint8_t chessStr[32];
				memcpy(chessStr,OLED_F16x16[chineseIndex],32);
			if (isTakingChess&&beginChess.x == x && beginChess.y == y)
			{
				for (int i = 0; i < 32; i++)
				{
					chessStr[i] = chessStr[i] ^ 0xFF;
				}
				
			}
			else if(curPointer.x == x && curPointer.y == y){
				for (int i = 0; i < 32; i++)
				{
					chessStr[i] = chessStr[i] ^ OLED_F16x16[SELECTION_BOX][i];
				}
				
			}
			OLED_ShowChineseCh(y+1-cameraPointer.y,x+1-cameraPointer.x,chessStr);
		}
	}
}

// void myprint() { //打印
// 	string buffer;
	
// 	for(int i=1; i<=10; i++) {
// 		buffer="";
// 		for(int j=1; j<=9; j++) {
// //			cout<<ptr[i][j];
			
// 			buffer+=ptr[i][j];
// 			if(table[i][j]>=0 and table[i][j]<=13) {
// 				// buffer+=allChess[table[i][j]];
// 			} else {
// 				switch(table[i][j]) {
// 					case 15:
// //						cout<<dianWei;
// 						buffer+=dianWei;
// 						break;
// 					case 16:
// //						cout<<shangChuHe;
// 						buffer+=shangChuHe;
// 						break;
// 					case 17:
// //						cout<<xiaChuHe;
// 						buffer+=xiaChuHe;
// 						break;
// 				}
// 				buffer += " ";
// 			}
			
// 		}
// //		cout<<ptr[i][10];
// 		buffer+=ptr[i][10];
// /* 		cout<<buffer;
// 		cout<<endl;
// 		cout<<endl; */
// 	}
// 	// buffer="上个事件：";
// 	buffer+=lastShiJian;
// /* 	cout<<buffer;
// 	cout<<endl; */
// 	// buffer="当前："+shiJian;
// /* 	cout<<buffer;
// 	cout<<endl; */
// 	if(redTurn) {
		
// 		// buffer="现在到红方";
// 	} else {
// 		// buffer="现在到黑方";
// 	}
// /* 	cout<<buffer;
// 	cout<<endl; */
// }


void changePointer(int direction) {
	Pointer afterPointer=curPointer;
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
	if(afterPointer.x>0 and afterPointer.y>0 and afterPointer.x<10 and afterPointer.y<11) {   //判断指针是否超出边界
		curPointer=afterPointer;
	}

	updateCamera();
}
void playError() {
	// shiJian="你不能这么做！";
	//shiJian+=beginChess.x+'0';
	//shiJian+=",";
	//
	//shiJian+=beginChess.y+'0';
	//shiJian+=";;";
	//shiJian+=curPointer.x+'0';
	//shiJian+=",";
	//shiJian+=curPointer.y+'0';
	//shiJian+=",";


	isTakingChess=false;
}
extern void externGameOver(void);
void gameOver() {
	OLED_Clear();
	ChineseIndex tempStr1[]={FANG,SHENG,LI,GANTANHAO,AN,REN,YI};
	ChineseIndex tempStr2[]={JIAN,CHONG,XIN,KAI,SHI,YOU,XI,JUHAO};
	if (whoWin==CHESS_ZHENYING_RED)
	{
		OLED_ShowChinese(1,1,HONG);
	}
	else{
		OLED_ShowChinese(1,1,HEI);
	}
	


	OLED_ShowChineseStr(1,2,tempStr1,7);
	OLED_ShowChineseStr(2,1,tempStr2,8);

	while (true)
	{
					/* code */
	}
	
}
void fangZhi() {
	bool laoJiang=0;
	if(table[curPointer.y][curPointer.x]==RED_SHUAI or table[curPointer.y][curPointer.x]==BLACK_JIANG) {
		if (table[curPointer.y][curPointer.x] == RED_SHUAI) {
			whoWin = CHESS_ZHENYING_BLACK;
		}
		else {
			whoWin = CHESS_ZHENYING_RED;
		}
		laoJiang=1;
	}
	table[curPointer.y][curPointer.x]=table[beginChess.y][beginChess.x];
	if(isRedTurn) {
		isRedTurn=0;
	} else {
		isRedTurn=1;
	}

	//点位形态
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
					if(myAbs(curPointer.x-beginChess.x)==1) {
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
					if(myAbs(curPointer.x-beginChess.x)==1) {
						fangZhi();
						return;
					}
				}
			}
			playError();
			break;
		case 0://0和7是车
		case 7:
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
		case 6://炮
		case 13: 
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
						return;


					}
				} else {
					if(table[curPointer.y][curPointer.x]>13) {
						fangZhi();
						return;
					} else {
						playError();
					}

				}
			} else {
				playError();
			}
			break;
		case 4:
		case 11:
			myDistance=mySquare(myAbs(curPointer.x-beginChess.x))+mySquare(myAbs(curPointer.y-beginChess.y));
			if(curPointer.y == beginChess.y or curPointer.x==beginChess.x) {
				if(myDistance==1) {
					if(table[beginChess.y][beginChess.x]==4) {
						red=1;
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

			break;
		case 3:
		case 10: 
			myDistance=mySquare(myAbs(curPointer.x-beginChess.x))+mySquare(myAbs(curPointer.y-beginChess.y));
			if(myDistance==2) {
				if(table[beginChess.y][beginChess.x]==3) {
					red=1;
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
			// break;
		case 2:
		case 9: 
			myDistance=mySquare(myAbs(curPointer.x-beginChess.x))+mySquare(myAbs(curPointer.y-beginChess.y));
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
		case 1:
		case 8:
			myDistance=mySquare(myAbs(curPointer.x-beginChess.x))+mySquare(myAbs(curPointer.y-beginChess.y));
			if(myDistance!=5) {
				playError();
				return;
			}
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

		default:
			return;
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


bool check() {
	bool hasKeyDown=false;
	int direction;
	direction=0;
	if(keyDown(KEY_DOWN)) {
		hasKeyDown=true;
		direction=1;
	} else if(keyDown(KEY_UP)) {
		hasKeyDown=true;
		direction=2;
	} else if(keyDown(KEY_LEFT)) {
		hasKeyDown=true;
		direction=3;
	} else if(keyDown(KEY_RIGHT)) {
		hasKeyDown=true;
		direction=4;
	} else if(keyDown(KEY_CONFIRM)) {
		hasKeyDown=true;
		if(table[curPointer.y][curPointer.x]<=13) {  //0-13是旗子 14-17是点位
			if(canTakeChess(isRedTurn)) {
				beginChess.y=curPointer.y;
				beginChess.x=curPointer.x;
				isTakingChess=true;
			} else {
				if(isTakingChess) {
					myMove();
					isTakingChess=false;
				} else {
					playError();
				}
			}

		} else {
			if(isTakingChess) {
				myMove();
				isTakingChess=false;
			}
		}
	}
	if(direction!=0) {
		changePointer(direction);
	}
	return hasKeyDown;
}

void game(){

	init();
	myPrint();
	while(true) {
		
		if (check())
		{
			if (gameEnd) {
				gameOver();
			}
			myPrint();
		}
		
		
		
	}
}

