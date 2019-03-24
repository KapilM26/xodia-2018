//============================================================================
// Name        : bot_final.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

int flag,tempmob=0,goflag=0;
int a;
bool whosemove,countP,countM,elimFlag=0,reachhome=0,reachhome2=0,choice;
char X,Y;

char board[5][5] =
{
		   {'_', '_', 'w', '_', 'w'},
		   {'_', '_', 'w', '_', '_'},
		   {'_', 'B', '_', '_', '_'},
		   {'_', '_', '_', '_', 'B'},
		   {'b', 'b', 'b', 'b', '_'},
};

struct Move
{
  int score=0,goflag;
  int p[5];
  vector <vector<int>> movesets[5];
  bool whosemove,elimFlag=0;
};

struct game
{
	int r,c;
};

struct bMove
{
	game fsinput,fminput;
};

void checkMove (Move &);
int mValidation(game ,game ,Move &);
void makeMove(game ,game ,Move &,Move &,char &);
void Elimination(game ,game ,Move &,char &);
void colour_generalize(bool );
int negamax(int ,Move* ,int ,int);
bMove findBestMove();

void displayboard ()
{
    int i, j;
    cout << " \n \n    0 1 2 3 4\n";
    cout << "  -------------\n";
    for (i = 0; i < 5; i++)
    {
        cout << i << "|  ";
        for (j = 0; j < 5; j++)
	    {
	        cout << board[i][j] << ' ';
	    }
        cout << "\n";
    }
}

void colour_generalize (bool x)
{
      	if (x == 0)
      	{
          	X = 'W';			//X initialized to mantri
          	Y = 'w';			//Y initialized to pawn
        }
      	else
      	{
          	X = 'B';
          	Y = 'b';
        }
}

int winner()
{
	int sum1=0,sum2=0;
	for (int j = 0; j < 7; j++)
	{
	        if (board[0][j] == 'W')
		    {
		        sum1++;
		        cout<<"WHITE WINS\n";
		        a=0;
		        return 0;
	        }
	        if (board[4][j] == 'B')
		    {
		        sum2++;
		        cout<<"BLACK WINS\n";
		        a=1;
		        return 1;
	        }
	}
	return 3;
}

int checkElimination (game sinput, game minput,Move &currentobj)
{
    bool x = currentobj.whosemove;
    char t1 = board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];	//target piece  //changing value of X and Y temporarily to detect correct target
    colour_generalize (!x);
    if (t1 == X )
    {
    	currentobj.goflag=3;
        return 2;
    }
    else if( t1 == Y)
    {
    	currentobj.goflag=2;
    	return 2;
    }
    else
    {
        return 0;
    }
}


int mValidation (game sinput, game minput,Move &currentobj)
{
    int d1, d2;
    bool x = currentobj.whosemove;
    d1 = minput.r - sinput.r;	// difference between initial and final row position
    d2 = minput.c - sinput.c;	//difference between initial and final cols position
    colour_generalize (x);
    if (board[minput.r][minput.c] == '_' && minput.c >= 0 && minput.c <= 4 && minput.r <= 4 && minput.c >= 0)	//check if final position is vacant
    {
        	if(board[sinput.r][sinput.c]==X)
	    	{
	        	if ((d1 == (2 * x - 1) || d1 == 0) && (d2 == (2 * x - 1) || d2 == -(2 * x - 1) || d2 == 0))	//mantri moves single space in any allowed direction
	        	{
	        		flag = 1;		//signifies valid non-eliminating move,turn passes to opponent
	        		currentobj.goflag = 1;
	        	}
	        	else if (((d1 == 2 * (2 * x - 1) && d2 == 0) || (d1 == 0 && (d2 == 2 || d2 == -2 ))) && sinput.r!=4 && sinput.r!=0)	//moves two spaces, in an elimination move
	        	{
	            		flag =checkElimination (sinput, minput, currentobj);	//checks if elimination is possible and sets flag accordingly
	        	}
	        	else
	        	{
	            		flag = 0;
	        	}
	    	}
        	else if (board[sinput.r][sinput.c] == Y)	//moveset for pawn
        	{
        		if(minput.r==4*x && sinput.r!=4*x)
        		{
        			currentobj.goflag=0;
        		}
	        	if ((d1 == (2 * x - 1) && d2 == (2 * x - 1)) || (d1 == (2 * x - 1) && d2 == -(2 * x - 1)))	//moves a single space
	        	{
	            		flag = 1;
	            		currentobj.goflag = 1;
	        	}
	        	else if (((d1 == 2 * (2 * x - 1) && d2 == 0) || (d1 == 0 && (d2 == 2 || d2 == -2 ))) && sinput.r!=4 && sinput.r!=0)	//moves two spaces, in an elimination move
	        	{
	            		flag = checkElimination (sinput, minput,currentobj);
	        	}
	        	else
	        	{
	            		flag = 0;
	        	}
	    	}
    }
    else
    {
        flag = 0;
    }
    colour_generalize(x);
    if(minput.r==4*x && sinput.r!=4*x&&board[sinput.r][sinput.c]==X)
    {
        currentobj.goflag=4;
    }
    return flag;
}

void checkMove (Move &currentobj)
{
	bool x=currentobj.whosemove;
	vector<vector<int>> initialize;
	for(int i=0;i<=4;i++)
	{
		currentobj.p[i]=0;
		currentobj.movesets[i]=initialize;
	}
	game gsinput, gminput;
	colour_generalize(x);
	for (int i = 0; i < 5; i++)
    {
	   	for (int j = 0; j < 5; j++)
        {
	       	if (board[i][j] == X || board[i][j] == Y)
	       	{
               	gsinput.r = i;
		       	gsinput.c = j;
		       	for (int k = 0; k <= 2; k++)
		       	{
		           	for (int l = -2; l <= 2; l++)
		           	{
			          	if (x == 1)
			           	{
			           		gminput.r = i + k;
			           	}
			           	else
			            {
			               	gminput.r = i - k;
		            	}
		            	gminput.c = j + l;
		            	if (gminput.c <= 4 && gminput.c >= 0 && gminput.r <= 4 && gminput.r >= 0)
			           	{
			               	flag = mValidation (gsinput, gminput,currentobj);
			               	colour_generalize(x);
			               	if (flag == 1 || flag == 2)
			               	{
			               		vector<int> myrow;
			               		myrow.push_back(gsinput.r);
			               		myrow.push_back(gsinput.c);
			               		myrow.push_back(gminput.r);
			               		myrow.push_back(gminput.c);
			               		myrow.push_back(currentobj.goflag);
			               		currentobj.movesets[currentobj.goflag].push_back(myrow);
				               	currentobj.p[currentobj.goflag]++;
				            }
			         	 }
		           	 }
		       	 }
	       	 }
    	 }
     }
}

void Elimination (game sinput, game minput,Move &currentobj,char &T)
{
	currentobj.elimFlag = 1;
    bool x=!currentobj.whosemove;
    colour_generalize(x);
    char t1 = board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];
    if (t1 == X)
    {
    	countM=1;			//increment count for dead mantri
    }
    else
    {
       	countP=1;			//increment count for dead pawn
    }
    T=board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2];
    board[(sinput.r + minput.r) / 2][(sinput.c + minput.c) / 2]='_';
}

void makeMove (game sinput, game minput ,Move &currentobj,Move &nextobj,char &T)
{
	bool x=currentobj.whosemove;
	colour_generalize(x);
	int d1=minput.r-sinput.r;
	int d2=minput.c-sinput.c;
    if (d1 == (2*x-1)|| d2==1 || d2==-1)
   	{
        board[minput.r][minput.c] = board[sinput.r][sinput.c];
        board[sinput.r][sinput.c] = '_';
    }
    else if (d1 == 2*(2*x-1) || d2==2 || d2==-2)
    {
        board[minput.r][minput.c] = board[sinput.r][sinput.c];
        board[sinput.r][sinput.c] = '_';
        Elimination(sinput, minput, currentobj, T);
    }
    colour_generalize(x);
	if(minput.r==4*x && sinput.r!=4*x && board[minput.r][minput.c]==X)
	{
		reachhome = 1;
	}
	else if(minput.r==4*x && sinput.r!=4*x && board[minput.r][minput.c]==Y)
	{
		reachhome2 = 1;
	}
	nextobj.whosemove=!currentobj.whosemove;
}
void undoMove(game sinput,game minput,char &T,Move &currentobj)
{
	board[sinput.r][sinput.c]=board[minput.r][minput.c];
	board[minput.r][minput.c]='_';
	if(currentobj.elimFlag==1)
	{
		board[(sinput.r+minput.r)/2][(sinput.c+minput.c)/2]=T;
		currentobj.elimFlag=0;
	}
}
void makefinalMove(game sinput, game minput, bool x)
{
		colour_generalize(x);
		int d1=minput.r-sinput.r;
		int d2=minput.c-sinput.c;
	    if (d1 == (2*x-1)|| d2==1 || d2==-1)
	   	{
	        board[minput.r][minput.c] = board[sinput.r][sinput.c];
	        board[sinput.r][sinput.c] = '_';
	    }
	    else if (d1 == 2*(2*x-1) || d2==2 || d2==-2)
	    {
	        board[minput.r][minput.c] = board[sinput.r][sinput.c];
	        board[sinput.r][sinput.c] = '_';
	        board[(sinput.r+minput.r)/2][(sinput.c+minput.c)/2] = '_';
	    }
	    whosemove=!whosemove;
}


int evaluate (game sinput, game minput,Move currentobj,int depth)
{
	bool x=currentobj.whosemove;
	int result=0;
    if (currentobj.elimFlag == 1)
    {
	    if (countM == 1)
	    {
	        	result = (result+500*(2*x-1) + 50*(4*x-(sinput.r+minput.r)/2))*(7-depth);
	    }
	    else if (countP == 1)
	    {
	    	result = result + (100*(2*x-1) + 10*(4*x-(sinput.r+minput.r)/2))*(7-depth);
	    }
    }
    if (reachhome == 1)
    {
    	result =  result + 10000*(2*x-1)*(7-depth);
    }
    if (reachhome2==1)
    {
    	result =  result - 90*(2*x-1)*(7-depth);
    }
    if(x==1)
    {
    	result =  result + 5*(minput.r)*(2*x-1);
    }
    else
    {
    	result =  result + 5*(4-minput.r)*(2*x-1);
    }
    result = result -(2*x-1)*(minput.c-2)*(minput.c-2)*10;
    countM=0,countP=0;
    return result;
}

int negamax (int depth,Move obj[],int alpha,int beta)
{

	int best;
	game sinput,minput;
	Move currentobj = *obj;
	Move nextobj = *(obj+1);
	int score=0;
	best=200000*(1-2*currentobj.whosemove);
	if(reachhome==1)
	{
	    reachhome=0;
	    return currentobj.score;
	}
    if(depth==4)
    {
    	return currentobj.score;
    }
    checkMove (currentobj);
    (*obj)=currentobj;
    for(int i=4;i>=0;i--)
    {
    	for (int j = currentobj.p[i]-1; j >= 0; j--)
    	{
    		sinput.r = currentobj.movesets[i][j][0];
    		sinput.c = currentobj.movesets[i][j][1];
    		minput.r = currentobj.movesets[i][j][2];
    		minput.c = currentobj.movesets[i][j][3];
    		char T=' ';
    		makeMove (sinput, minput,currentobj,nextobj,T);
    		score = evaluate (sinput, minput,currentobj,depth);
    		//cout<<"board at depth "<<depth<<"\n";
    		//displayboard();
    		nextobj.score = currentobj.score+score;
    		*(obj+1)=nextobj;
    		if(currentobj.whosemove)
    		{
    			best = max (best, negamax (depth + 1,obj+1,alpha,beta));
    			alpha = max (alpha,best);
    			if(beta<=alpha)
    			{
    				//cout<<"pruning at depth "<<depth<<"\n";
    				undoMove(sinput,minput,T,currentobj);
    				//displayboard();
    				break;
    			}
    		}
    		else
    		{
    			best = min (best, negamax (depth + 1,obj+1,alpha,beta));
    			beta = min (beta,best);
    			if(beta<=alpha)
    			{
    				//cout<<"pruning at depth "<<depth<<"\n";
    				undoMove(sinput,minput,T,currentobj);
    				//displayboard();
    				break;
    			}
    		}
    		undoMove(sinput,minput,T,currentobj);
    		//cout<<"board reset at depth "<<depth<<"\n";
    		//displayboard();
    	}
    }
    return best;
}


bMove findBestMove ()
{
	Move obj[10];
    bMove bestMove;
    obj[0].whosemove=choice;
    checkMove (obj[0]);
    game sinput,minput;
    int bestVal;
    if(choice==0)
    {
    	bestVal = 250000;
    }
    else
    {
    	bestVal = -250000;
    }

	for (int i = 4; i>=0;i--)
	{
		for(int j=obj[0].p[i]-1;j>=0;j--)
		{
			sinput.r = obj[0].movesets[i][j][0];
			sinput.c = obj[0].movesets[i][j][1];
			minput.r = obj[0].movesets[i][j][2];
			minput.c = obj[0].movesets[i][j][3];
			char T;
			makeMove (sinput, minput,obj[0],obj[1],T);
			obj[1].score = evaluate(sinput,minput,obj[0],-1);
			int moveVal = negamax (0,obj+1,-200000,200000);
			undoMove(sinput,minput,T,obj[0]);
			//cout<<"board reset in fbm\n";
			//displayboard();
			if(choice==1)
			{
				if (moveVal > bestVal && moveVal!=200000)
				{
					bestMove.fsinput.r =sinput.r;
					bestMove.fsinput.c =sinput.c;
					bestMove.fminput.r =minput.r;
					bestMove.fminput.c =minput.c;
					bestVal = moveVal;
				}
			}
			else if(choice==0)
			{
				if (moveVal < bestVal && moveVal!=-200000)
				{
					bestMove.fsinput.r =sinput.r;
					bestMove.fsinput.c =sinput.c;
					bestMove.fminput.r =minput.r;
					bestMove.fminput.c =minput.c;
					bestVal = moveVal;
				}
			}
		}
	}
    return bestMove;
}

void initBoard ()
{
    for (int i = 0; i < 5; i++)
    {
        board[0][i] = 'B';
        board[1][i] = 'b';
        board[2][i] = '_';
        board[3][i] = 'w';
        board[4][i] = 'W';
    }
}
// Driver code
int main ()
{
	initBoard();
	//displayboard();
	game sinput,minput;
	string input,binput;
	int firsttime=0;
	/*fstream fobj;
	fobj.open("logs.txt",ios::out|ios::app);
	fobj<<"\n*************************NEW MATCH BEGINS HERE******************************\ndepth 5 bot\n";
	cout<<"Enter player preference[0/1]\n";*/
	cin>>choice;
	whosemove=choice;
	do
	{
		if(whosemove)
		{
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			bMove bestMove = findBestMove();
			makefinalMove(bestMove.fsinput,bestMove.fminput,choice);
			/*binput = bestMove.fsinput.r+48;
			binput += ' ';
			binput += bestMove.fsinput.c+48;
			binput += ' ';
			binput += bestMove.fminput.r+48;
			binput += ' ';
			binput += bestMove.fminput.c+48;
			cout<<binput<<endl;*/
			cout<<bestMove.fsinput.r<<" "<<bestMove.fsinput.c<<" "<<bestMove.fminput.r<<" "<<bestMove.fminput.c;
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			//fobj<<bestMove.fsinput.r<<" "<<bestMove.fsinput.c<<" "<<bestMove.fminput.r<<" "<<bestMove.fminput.c<<endl;
			auto duration = duration_cast<milliseconds>(t2-t1).count();
			cout<<"\nMove duration:"<<duration<<endl;
			//fobj<<"time "<<duration<<endl;
			//displayboard();
		}
		else
		{
			if(firsttime==0)
			{
				cin.ignore();
				firsttime=1;
			}
			getline(cin,input);
			sinput.r=input[0]-48;
			sinput.c=input[2]-48;
			minput.r=input[4]-48;
			minput.c=input[6]-48;
			makefinalMove(sinput,minput,!choice);
			//displayboard();
		}
	} while(true);
	return 0;
}
