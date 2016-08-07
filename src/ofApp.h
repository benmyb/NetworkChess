#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxUI.h"
#include "Manager.h"
#include "ofxFontStash-master\src\ofxFontStash.h"
#include <fstream>

enum CURR_MODE { STEP, MOVE, FREE, BEGIN };
enum DUEL_MODE { MvsC = 0, MvsM, CvsC };

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofEasyCam mCam;
		vector<ofSpherePrimitive> chess;
		ofBoxPrimitive chess_board;
		ofxAssimpModelLoader human_hand;
		ofxAssimpModelLoader human_hand_sub;
		ofxAssimpModelLoader robot_hand;
		ofxAssimpModelLoader robot_hand_sub;
		ofxAssimpModelLoader chess_bowl1;
		ofxAssimpModelLoader chess_stack1; 
		ofxAssimpModelLoader chess_bowl2;
		ofxAssimpModelLoader chess_stack2;
		ofImage board;
		ofImage wood;
		ofImage wood_bowl;
		ofImage skin;
		ofImage white_chess;
		ofImage black_chess;
		ofImage step_png;
		ofImage move_png;
		ofLight up_light;
		ofLight down_light;
		ofLight on_light;
		ofLight other_light;
		ofxUITabBar* guiTabBar = nullptr;
		ofxUICanvas* startmenu = nullptr;
		ofPolyline success_path;
		ofPolyline poly;
		vector<int> curr_board = vector<int>(100, 0);
		int moving_chess = -1;
		int select_chess = -1;
		int pre_chess = -1;
		int who_step = 0;
		int path_thickness = 20;
		bool white_moving = true;
		bool is_selected = false;
		bool our_first = true;
		CURR_MODE curr_mode = BEGIN;
		DUEL_MODE duel_mode = MvsC;

		Manager chess_man;

		void drawBoard(vector<int>& now);
		void drawBowl();
		void drawMode();
		int xytoi(int x, int y);
		int teni2eighti(int i);
		void screen2board(int & x, int & y);
		void step(int x, int y);
		void move_step(int x, int y);
		int select(int x, int y);
		void drawHand();
		void setguitab();
		void guiEvent(ofxUIEventArgs & e);


		int head_1 = -1;
		int head_2 = -1;
		int head_3 = -1;
		int head_4 = -1;
		int tail_1 = -1;
		int tail_2 = -1;
		int tail_3 = -1;
		int tail_4 = -1;

		ofxFontStash output;
};
