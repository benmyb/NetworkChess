

#include"Manager.h"


//
//void Manager::list_init() {
//
//}

void Manager::init_link()
{




	//初始化棋盘每个棋子的结点

	memset(m_doubly_link, 0, sizeof(m_doubly_link));



	// 初始化每个结点对应在棋盘的位置

	for (int x = 1; x <= m_n; x++)
	{
		for (int y = 1; y <= m_n; y++)
		{
			for (int j = 0; j < m_direction / 2; j++)
			{
				m_doubly_link[x][y][j].m_pos = x + y*m_size;

			}
		}
	}





	// 初始化棋盘每个位置的4个方向链表的头结点，时间复杂度O(m_size*m_size*4)

	// direction 表示链表的增加方向 x ,y 表示链表起始位置，也就是空头结点的位置，增加一个空的头结点，方便插入和删除结点


	// direction 具体如下 	int next_step[m_direction][2] = { {-1,1},{0,1},{1,1},{1,0},{-1,0},{-1,-1},{0,-1},{1,-1}};


	int x, y, direction;

	x = 0;
	for (int i = 0; i <m_size; i++)
	{
		// init   y= i  0<x<=m_n
		y = i;
		direction = 3;
		init_line_head(x, y, direction);

		// init  　y-x=i  (1<=y<=m_n,1<=x<=m_n)
		direction = 2;
		init_line_head(x, y, direction);


	}

	y = 0;

	for (int i = 0; i < m_size; i++)
	{
		// init x+y=i; （1<=y<=m_n,1<=x<=m_n）
		x = i;
		direction = 0;
		init_line_head(x, y, direction);


		// init x=i; （1<=y<=m_n）
		direction = 1;
		init_line_head(x, y, direction);

		// init x-y=i; （1<=y<=m_n,1<=x<=m_n）
		direction = 2;
		init_line_head(x, y, direction);

	}



	x = m_n + 1;

	direction = 0;
	for (int i = 0; i <m_size; i++)
	{

		// init x+y=m_n+1+i   （1<=y<=m_n,1<=x<=m_n）
		y = i;
		init_line_head(x, y, direction);


	}



}

void Manager::init_line_head(int x, int y, int direction)// O(m_n/2) =O(4)
{


	int count = 0;// 记录是第几个结点

				  // 由于棋盘扩大了，把周边没有用的结点作为一些链表的空头结点，增加结点的利用率 ， 不用NEW  DELETE 结点，方便初始化


	node* head = &m_doubly_link[x][y][direction];

	while (1) {
		//根据方向数组，计算链表的下一个结点的位置，
		x += next_step[direction][0];
		y += next_step[direction][1];
		if (judge_border(x, y)) {
			m_head[x][y][direction] = head;
			m_doubly_link[x][y][direction].m_index = ++count;
		}

		else break;
	}

}

void Manager::init()
{




	memset(m_situation, 0, sizeof(m_situation));
	memset(m_limit, 0, sizeof(m_limit));

	m_chess_num = 0;
	// 设置特区
	for (int i = 1; i <= m_n; i++)
	{

		//白棋连通上下 即y->(1,m_n) 黑棋连通左右  x->(1,m_n)

		// 所以白棋不能走 y=1 (1<=x<=m_n) 和 y=m_n(1<=x<=m_n)
		m_limit[1][1][i] = m_limit[1][m_n][i] = m_limit[0][i][1] = m_limit[0][i][m_n] = m_inf;

		//黑棋不能走上下两行，为白棋的特区

	}

	m_situation[1][1] = m_situation[1][m_n] = m_situation[m_n][1] = m_situation[m_n][m_n] = m_inf;//设置禁区


																								  //// by dyy at 8 7 9:38

																								  //m_limit[0][1][1] = m_limit[0][1][m_n] = m_limit[0][m_n][1] = m_limit[0][m_n][m_n]
																								  //	= m_limit[1][1][1] = m_limit[1][1][m_n] = m_limit[1][m_n][1] = m_limit[1][m_n][m_n] = m_inf;



}


void Manager::lay_limit(int color,int x,int y)
{
	// 便于群的判断， 放下一个棋子，就在它的周围limit 数组+1 ，它自己的位置加上一个特殊值 m_inf2

	int near_x, near_y;
	for (int i = 0; i < m_direction; i++)
	{
		near_x = x + next_step[i][0];
		near_y = y + next_step[i][1];
		// 无须判断越界，因为扩大了棋盘，节省了判断的时间
		m_limit[color][near_x][near_y]++;
	}


	
   // m_limit[color][x][y]++;

	m_limit[color][x][y] += m_inf2;
	
}

void Manager::retract_limit(int color,int x,int y)
{


	// 便于群的判断， 收回一个棋子，就在它的周围limit 数组-1 它自己的位置减去一个特殊值 m_inf2

	int near_x, near_y;
	for (int i = 0; i < m_direction; i++)
	{
		near_x = x + next_step[i][0];
		near_y = y + next_step[i][1];
		// 无须判断越界，因为扩大了棋盘，节省了判断的时间
		m_limit[color][near_x][near_y]--;
	}

	//m_limit[color][x][y]--;
	m_limit[color][x][y] -= m_inf2;
}


void Manager::restart()
{

	init();
	m_step = 0;


	// 重新开始，清空m_before
	while (!m_before[0].empty())m_before[0].pop();

	while (!m_before[1].empty())m_before[1].pop();




	// 初始化 m_doubly_link  m_head
	for (int x = 1; x <= m_n; x++)
	{
		for (int y = 1; y <= m_n; y++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_doubly_link[x][y][j].setZero();

				m_head[x][y][j]->setZero();

			}
		}
	}


}

void Manager::restart_v2()
{

	int total = min(m_n * 2 - 1, m_step);
	
	int x, y;

	
	for (int i = 1; i <= total; i++) {
		x = m_chess[i] % m_size;
		y = m_chess[i] / m_size;

		for (int j = 0; j < 4; j++) {
			m_head[x][y][j]->setZero();

	//		m_doubly_link[x][y][j].m_pioneer->m_next = 0;
			m_doubly_link[x][y][j].setZero();
		}
	}

	init();
	m_step = 0;

	while (!m_before[0].empty())m_before[0].pop();

	while (!m_before[1].empty())m_before[1].pop();


}

bool Manager::feasible(int color,int x,int y)
{

	// judge_border 表示是否越界
	// m_situation 不为0 ，表示有棋子，或为禁区

	if (!judge_border(x, y) || m_situation[x][y] || m_limit[color][x][y]>=m_inf)return 0;
	
	//附近有2个棋子
	if (m_limit[color][x][y]>= 2)return 0;

	int count = 0;//记录周围有几个同色棋子

	int near_x, near_y;
	for (int i = 0; i < m_direction; i++)
	{
		near_x = x + next_step[i][0];
		near_y = y + next_step[i][1];
		//if (m_situation[near_x][near_y] > 0 && m_situation[near_x][near_y] < m_inf&&m_situation[near_x][near_y] % 2 == color&&m_limit[color][near_x][near_y] == 2)return 0;
		
		// change by dyy at 8 7 10:36
		if (m_limit[color][near_x][near_y] ==m_inf2+1)return 0;

	}
	return 1;

}

bool Manager::add(int x, int y)
{

	m_step++;



	
	

	if (lay((m_chess_num+1) % 2, x, y)) {
		



		m_chess[++m_chess_num] = x+m_size*y;
		//m_chess[m_step][1] = y;
		m_before[0].push(x+m_size*y);
		m_before[1].push(0);
		m_situation[x][y] = m_chess_num;


		// test by dyy 8 7 14:33
		int color = m_step % 2;
		cout << "add" << "(" << x << "," << y << ")" << endl;
		cout << "color" << color << endl;


		return 1;
	}
	else {
		m_step--;
		return 0;
	}


	//// 黑棋先行
	//if (feasible((m_step+1) % 2, x, y)) {
	//	lay_limit((m_step+1) % 2, x, y);


	//	


	//	m_step++;

	//	m_chess[m_step][0] = x;
	//	m_chess[m_step][1] = y;
	//	m_before[0].push(m_step);
	//	m_before[1].push(0);
	//	m_situation[x][y] = m_step;

	//}


	//return false;
}

bool Manager::move(int before_x, int before_y, int after_x, int after_y)
{

	

	// 新的地方没有棋子才可以下
	if (m_situation[after_x][after_y])return false;
	
	else {


		int pos = m_situation[before_x][before_y];
		m_step++;
		m_situation[before_x][before_y] = 0;


		retract(pos % 2, before_x, before_y);






		if (lay(pos % 2, after_x, after_y)) {
			m_chess[pos]= after_x+after_y*m_size;
		//	m_chess[pos][1] = after_y;
			m_before[0].push(after_x+ m_size*after_y);
			m_before[1].push(before_x+m_size*before_y);
			m_situation[after_x][after_y] = pos;
			// test by dyy
			int color = pos % 2;

			cout << "(" << before_x << "," << before_y << ")" << "move" << "(" << after_x << "," << after_y << ")" << endl;
			cout << "color" << color << endl;

			return 1;
		}
		else {
			lay(pos % 2, before_x, before_y);
			m_situation[before_x][before_y] = pos;
			m_step--;

			return 0;

		}
	}

	return false;
}




bool Manager::back(int & p1,int & p2,bool & color)
{	//p1：现在坐标;p2=为前坐标

	
	//m_step++;

	if (m_before[0].empty()) {
		p1 = -1;
		p2 = -1;
		return 0;
	}


	int chesspos = m_before[1].top();
	
	int current_x = m_before[0].top() %m_size, current_y(m_before[0].top() / m_size);
	int pos = m_situation[current_x][current_y];
	retract(pos%2, current_x, current_y);
	

	color = pos % 2;
	cout << "color" << color << endl;

	if (chesspos) {
		int before_x(m_before[1].top() % m_size), before_y(m_before[1].top() / m_size);
		m_situation[current_x][current_y] = 0;
		lay(pos%2, before_x, before_y);
		m_situation[before_x][before_y] = pos;
		m_chess[pos] = before_x + before_y*m_size;
		m_step--;
	//	m_chess_num--;
		p1 = m_before[0].top();
		p2 = m_before[1].top();
		m_before[0].pop();
		m_before[1].pop();



		//test
		cout << "back" << "(" << current_x << "," << current_y << ")" <<"  "<< "(" <<before_x<< ","<<before_y<< ")" << endl;
		//cout << "back" << p1 << "  " << p2 << endl;
	}

	else {

		
		p1 = m_before[0].top();
		p2 = m_before[1].top();
		m_before[0].pop();
		m_before[1].pop();
		//test
		cout << "back" <<"("<<current_x<<","<<current_y<<")" <<"("<<","<<")"<< endl;


		if(!m_before[0].empty())
		p2 = m_before[0].top();


		m_situation[current_x][current_y] = 0;

		m_step--;
		m_chess_num--;

	}





	return chesspos;
}

bool Manager::lay(int color,int x, int y)
{


	if (feasible(color, x, y)) {


		lay_limit(color, x, y);
		
		// 插入棋局链表中

		node * temp;
		for (int i = 0; i < 4; i++) {
			temp = m_head[x][y][i];

			while (!(temp->m_next==NULL) && (temp->m_next)->m_index < m_doubly_link[x][y][i].m_index)
				temp = temp->m_next;
			
			m_doubly_link[x][y][i].m_pioneer = temp;
			m_doubly_link[x][y][i].m_next = temp->m_next;
			if (temp->m_next != NULL) {
				temp->m_next->m_pioneer = &m_doubly_link[x][y][i];
			}
			temp->m_next = &m_doubly_link[x][y][i];
		}


		return true;
	}
	

	return false;
}

void Manager::retract(int color,int x, int y)
{
	retract_limit(color, x, y);


	// 在棋局链表中删除结点

	for (int i = 0; i < 4; i++)
	{
		m_doubly_link[x][y][i].m_pioneer->m_next = m_doubly_link[x][y][i].m_next;


		if(!(m_doubly_link[x][y][i].m_next==NULL))
		m_doubly_link[x][y][i].m_next->m_pioneer = m_doubly_link[x][y][i].m_pioneer;

		m_doubly_link[x][y][i].setZero();

	}

	
}

bool Manager::judge_network(int color)
{

	//棋子到达数目才可能形成网络
	if (m_step >= 2*m_success_chess_num-1) {
		
		//白棋连通上下 即y->(1,m_n) 黑棋连通左右  x->(1,m_n)

		// 黑棋起始位置为 x=1
		// 白棋起始位置为 Y=1 

	
		
		
		int total_chess(0);//成功网络上的棋子数

		node * temp_head = 0;


		if (color) {//黑棋，连通
			temp_head = m_head[1][1][3];


		}

		else {
			temp_head = m_head[1][1][1];
		}

		bool isfind = 0;
		while (temp_head->m_next) {
			total_chess = 0;
			memset(m_visited, 0, sizeof(m_visited));
			isfind = 0;
			while (!m_success.empty())m_success.pop();
			isfind=network_dfs(temp_head->m_next->m_pos, total_chess,color);

			if (isfind) {
				return 1;
			}


			temp_head = temp_head->m_next;

		}

	}

	return false;
}

int Manager::get_scores()
{

	int scores[2];
	int total_chess(min(m_step, m_success_chess_num * 2));
	int temp,x,y;
	int total_next;
	for (int i = 1; i <= total_chess; i++) {
		temp = m_chess[i];
		x = temp%m_size;
		y = temp/m_size;
		temp = 0;
		total_next = 0;
		int x1, y1;
		for (int i = 0; i < 4; i++)
		{
			temp = m_doubly_link[x][y][i].m_pioneer->m_pos;
			x1 = temp%m_size;
			y1 = temp/m_size;
			if (m_situation[x1][y1] < m_inf&&m_situation[x1][y1] % 2 == m_situation[x][y] % 2) {
				total_next++;
			}
		}

		scores[m_situation[x][y] % 2] += total_next;
	}

	return scores[m_step % 2] - scores[(m_step % 2 + 1) % 2];

	return false;
}

double Manager::get_scores_v2()
{

	

	return 0.0;
}



bool Manager::network_dfs(int pos, int & total,int color)
{
	if (pos) {// pos >0 表示有棋子
		int x = pos%m_size;
		int y = pos / m_size;

		if (!judge_border(x, y)||m_visited[x][y])return 0;
	//	if (m_visited[x][y])return 0;


		bool isfind(0);
		if (color) {// 黑棋

			// x==1 有重复的棋子在特区，


			if (total&&y == 1)return 0;

			if ((m_situation[x][y] % 2)==0)return 0;


		
			if (y == m_n) {
				if (total >= m_success_chess_num-1) {
					isfind = 1;
				}
				else return 0;
			}
		
		}

		else {

			if (total&&x == 1)return 0;

			if ((m_situation[x][y] % 2))return 0;



			if (x == m_n) {
				if (total >= m_success_chess_num-1) {
					isfind = 1;
				}
				else return 0;
			}


	/*		if (!(m_situation[x][y] % 2) && !m_visited[x][y] && y != 1)
			{
				if (total >= m_success_chess_num&&y == m_n) {
					isfind = 1;

				}
			}*/
		}
		m_visited[x][y] = 1;
		total++;
		m_success.push(pos);
		if (isfind)return 1;

		for (int i = 0; i < 4; i++) {



			// 注意判断指针非空，
			if(m_doubly_link[x][y][i].m_pioneer!=NULL)
			isfind = network_dfs(m_doubly_link[x][y][i].m_pioneer->m_pos, total, color);
			if (isfind)return 1;
			if(m_doubly_link[x][y][i].m_next!=NULL)
			isfind = network_dfs(m_doubly_link[x][y][i].m_next->m_pos, total, color);
			if (isfind)return 1;
		}
		m_visited[x][y] = 0;
		total--;
		m_success.pop();


		return 0;
	}



	else return 0;
}




//void Manager::list_init()
//{
//	
//	
//
//
//
//
//	for (int i = 1; i <= m_n; i++) {
//		for (int j = 1; j <= m_n; j++) {
//		    
//		}
//	}
//
//
//}
