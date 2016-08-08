#pragma once

#include<string.h>
#include<stack>
#include<vector>
#include<algorithm>

using namespace std;


const int m_n = 8;// ��ִ�С
const int m_size = 10;// ���̴�С  �������̴�С��ֹԽ��,����Խ���ж�
// ���ӱ���Ǵ� ��1��1�� �� ��m_n,m_n);��ά����


const int m_direction = 8;// 8 direction


const int m_chess_num = 10;// ������


const int m_success_chess_num = 6;//�ɹ�����������������



//������ͨ���� ��y->(1,m_n) ������ͨ����  x->(1,m_n)





class Manager {
	
public:
	const int m_inf = 0x3f3f3f3f;


	int m_situation[m_size][m_size];// ==0 ��ʾû������,%2=0��ʾ���� %2=1��ʾ����,==m_inf����

	int m_limit[2][m_size][m_size];// m_limit[0][][] ��ʾ����������� m_limit[1][][] ��ʾ�����������
	
	int next_step[m_direction][2] = { {-1,1},{0,1},{1,1},{1,0},{-1,0},{-1,-1},{0,-1},{1,-1}};//�ٽ���



	int m_step = 0;//���µĲ���






	struct node {
		node * m_pioneer = 0;
		node * m_next = 0;
		int m_index=0;//��ʾ������ĵڼ������
		int m_pos = 0;// ��ʾ�������������е�λ��
		node(int index) :m_index(index),m_next(0),m_pioneer(0) {};

		node():m_pioneer(0),m_next(0),m_index(0) {};

		void setZero() {
			m_pioneer = 0;
			m_next = 0;
		}

	};

	node m_doubly_link[m_size][m_size][4];//ÿ��������һ���ڵ�

	node * m_head[m_size][m_size][4];// ����ĸ������������ɵ�˫�������ͷָ��
	




	

	int m_chess[m_chess_num*2+1];// ���� ˳����������   
	int m_chess_num = 0;

//	int m_white[m_chess_num][2];// ��������˳����������

	stack<int> m_before[2];// m_before[0] �洢��������У�m_before[1] : 0 ����  >=1 ����
						   //p1����������;p2=0���� !=0Ϊ����ǰ����





	// �ɹ����磺��
	stack<int> m_success;//�洢�ɹ�����

	
	bool m_visited[m_size][m_size];// �����жϳɹ�����ʱ ����¼�������ĵ�

	bool network_dfs(int pos, int & total, int color);


	//vector<int> m_begin[2];//�洢��ʼ�㣬���ڹ����ɹ�����

	


	//node m_link_head[m_size][m_size];


	void init_link();
	void init_line_head(int x, int y, int direction);






	void init();


	void lay_limit(int color,int x,int y);//color Ϊ0�������°��壬1 �º��壬������� m_limit����Χ8���㣨�����Լ���λ�ã�+1
	void retract_limit(int color,int x,int y);// ���壬��m_limit ����Χ8���㣨�����Լ���λ�ã�-1


	bool judge_border(int x, int y) {//�ж��Ƿ�Խ��
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
	


	void restart();
	
	void restart_v2();
	
	
	bool feasible(int color ,int x,int y);// 0��ʾ���壬1��ʾ����               //�ж��Ƿ���У����������ǽ�����û������ ��û��3��ͨ| 0 ��ʾ �����ߣ�1 ��ʾ����



	bool add(int x, int y);//����

	bool move(int before_x,int before_y,int after_x,int after_y);



	bool back(int & p1,int  &p2,bool & color);//p1����������;p2=Ϊǰ����;0���壬1����

	bool lay(int color,int x, int y);


	void retract(int color,int x, int y);

	
	
	

	//void list_init();


	bool judge_network(int color);// color =0 ����  1 ����



	int  get_scores();// �򵥼���ÿ������Χ�Ŀɼ�����Ŀ

	double get_scores_v2();//

};