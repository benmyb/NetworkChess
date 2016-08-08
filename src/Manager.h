#pragma once

#include<string.h>
#include<stack>
#include<vector>
#include<algorithm>
#include<iostream>
using namespace std;


const int m_n = 8;// 棋局大小
const int m_size = 10;// 棋盘大小  扩大棋盘大小防止越界,减少越界判断
					  // 棋子编号是从 （1，1） 到 （m_n,m_n);二维数组


const int m_direction = 8;// 8 direction


const int m_chess_num = 10;// 棋子数


const int m_success_chess_num = 6;//成功网络中所需棋子数



//白棋连通上下 即y->(1,m_n) 黑棋连通左右  x->(1,m_n)





class Manager {

public:
	const int m_inf = 0x3f3f3f3f;// 设置无穷值，便于设置特殊值

	const int m_inf2 = 0x1f1f1f1f;// 设置 第二个特殊值，表示特殊情况


	int m_situation[m_size][m_size];// ==0 表示没有棋子,%2=0表示白棋 %2=1表示黑棋,==m_inf禁区
									// >0 表示这个棋子放置在m_chess的下标

	int m_limit[2][m_size][m_size];// m_limit[0][][] 表示白棋的限制域， m_limit[1][][] 表示黑棋的限制域

	int next_step[m_direction][2] = { { -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ -1,0 },{ -1,-1 },{ 0,-1 },{ 1,-1 } };//临近域



	int m_step = 0;//已下的步数






	struct node {
		node * m_pioneer = 0;
		node * m_next = 0;
		int m_index = 0;//表示是链表的第几个结点
		int m_pos = 0;// 表示这个结点在棋盘中的位置
		node(int index) :m_index(index), m_next(0), m_pioneer(0) {};

		node() :m_pioneer(0), m_next(0), m_index(0) {};

		void setZero() {
			m_pioneer = 0;
			m_next = 0;
		}

	};

	node m_doubly_link[m_size][m_size][4];//每个棋子有一个节点

	node * m_head[m_size][m_size][4];// 存放四个方向的棋子组成的双向链表的头指针





	int m_chess[m_chess_num * 2 + 1];// 储存  已经在下在棋盘上的棋子的位置  0 位置不存储 
	int m_chess_num = 0;// 记录放在棋盘的棋子数





	//stack<int> m_before;
	


	// 有一个压缩存储的方式

	stack<int> m_before[2];// m_before[0] 存储下棋的序列，m_before[1] : 0 下棋  >=1 移棋前的位置坐标
						   //p1：现在坐标;p2=0下棋 !=0为移棋前坐标





						   // 成功网络：：
	stack<int> m_success;//存储成功网络


	bool m_visited[m_size][m_size];// 用于判断成功网络时 ，记录遍历过的点

	bool network_dfs(int pos, int & total, int color);




	void init_link();
	void init_line_head(int x, int y, int direction);






	void init();


	void lay_limit(int color, int x, int y);//color 为0，就是下白棋，1 下黑棋，下棋里，在 m_limit的周围8个点（包括自己的位置）+1
	void retract_limit(int color, int x, int y);// 悔棋，在m_limit 的周围8个点（包括自己的位置）-1


	bool judge_border(int x, int y) {//判断是否越界
		return x >= 1 && x <= m_n&&y >= 1 && y <= m_n;
	}






public:



	Manager() {

		memset(m_head, 0, sizeof(m_head));


		init();
		init_link();


	}

	~Manager() {

	}



	void restart();// 游戏重新开始的初始化

	void restart_v2();


	bool feasible(int color, int x, int y);// 0表示白棋，1表示黑棋     //判断是否可行，非特区，非禁区，没有棋子 ，没有3连通| 0 表示 不能走，1 表示能走



	bool add(int x, int y);//下棋

	bool move(int before_x, int before_y, int after_x, int after_y);// 移棋



	bool back(int & p1, int  &p2, bool & color);//p1：现在坐标;p2=为前坐标;0下棋，1移棋

	bool lay(int color, int x, int y);//放下棋子


	void retract(int color, int x, int y);// 收回棋子




	bool judge_network(int color);// color =0 白棋  1 黑棋



	int  get_scores();// 简单计算每个点周围的可见点数目

	double get_scores_v2();//

};