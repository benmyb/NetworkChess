#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofDisableArbTex();
	srand((unsigned)time(NULL));
	mCam.begin();
	up_light.setPosition(1000, 1000, 2000);
	up_light.lookAt(ofVec3f(0, 0, 0));
	down_light.setPosition(-1000, -1000, -2000);
	down_light.lookAt(ofVec3f(0, 0, 0));
	on_light.setPosition(0, 0, 1000);
	on_light.lookAt(ofVec3f(0, 0, 0));
	other_light.setPosition(750, 750, 1000);
	other_light.lookAt(ofVec3f(400, 400, 0));
	mCam.end();
	black_chess.loadImage("black.jpg");
	white_chess.loadImage("white.jpg");
	board.load("chessboard.jpg");
	wood.loadImage("wood.jpg");
	wood_bowl.loadImage("wood2.jpg");
	skin.loadImage("skin.jpg");
	step_png.loadImage("step.png");
	move_png.loadImage("move.png");
	step_png.rotate90(1);
	move_png.rotate90(1);
	human_hand.loadModel("human_hand.stl");
	human_hand.setScale(1.8, 1.6, 1.6);
	human_hand_sub.loadModel("human_hand.stl");
	human_hand_sub.setScale(1.8, 1.6, 1.6);
	human_hand_sub.setRotation(0, 180, 0, 0, 1);
	robot_hand.loadModel("robot_hand.obj");
	robot_hand.setScale(1.6, 1.6, 1.6);
	robot_hand_sub.loadModel("robot_hand.obj");
	robot_hand_sub.setScale(1.6, 1.6, 1.6);
	robot_hand_sub.setRotation(0, 180, 0, 0, 1);
	chess_bowl1.loadModel("bowl.obj");
	chess_bowl1.setScale(0.5, 0.5, 0.5);
	chess_stack1.loadModel("chess.obj");
	chess_stack1.setScale(0.45, 0.45, 0.45);
	chess_bowl1.setRotation(0, 180, 0, 0, 1);
	chess_bowl1.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8.85 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 0.8 * board.getHeight() / 8, -40);
	chess_stack1.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8.85 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 0.8 * board.getHeight() / 8, -40);
	chess_bowl2.loadModel("bowl.obj");
	chess_bowl2.setScale(0.5, 0.5, 0.5);
	chess_stack2.loadModel("chess.obj");
	chess_stack2.setScale(0.45, 0.45, 0.45);
	chess_bowl2.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8.85 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 6.2 * board.getHeight() / 8, -40);
	chess_stack2.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8.85 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 6.2 * board.getHeight() / 8, -40);

	ofSpherePrimitive c;
	c.setRadius(35);
	c.setScale(1, 1, 0.3);
	c.setResolution(30);
	c.rotate(180, 1, 0, 0);
	for (int y(0); y < 8; ++y) {
		for (int x(0); x < 8; ++x) {
			c.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + x * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - y * board.getHeight() / 8, 12);
			chess.push_back(c);
		}
	}
	mCam.setAutoDistance(true);
	setguitab();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (curr_mode == BEGIN) {
		mCam.begin();
		mCam.rotateAround(0.1, ofVec3f(ofNoise(ofGetElapsedTimef()*0.5), ofNoise(ofGetElapsedTimef()*0.2), ofNoise(ofGetElapsedTimef()*0.5)), ofVec3f(0, 0, 0));
		mCam.lookAt(ofVec3f(0, 0, 0));
		mCam.end();
	}
	else if (who_step >= 11)keyPressed('b');
	else if (who_step >= 20)curr_mode = MOVE;
}

//--------------------------------------------------------------
void ofApp::draw() {
	mCam.begin();
	ofEnableDepthTest();
	up_light.draw();
	down_light.draw();
	if (curr_mode == BEGIN) {
		up_light.enable();
		down_light.enable();
	}
	else on_light.enable();
	drawMode();
	drawBowl();
	drawBoard(curr_board);
	ofSetColor(45, 0, 255);
	ofSetLineWidth(path_thickness);
	ofSetColor(255, 10, 10);
	success_path.draw();//test
	ofSetColor(255);
	if (curr_mode != BEGIN && curr_mode != FREE)drawHand();

	if (curr_mode == BEGIN) {
		down_light.disable();
		up_light.disable();
	}
	else on_light.disable();
	mCam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'c': {
		mCam.reset();
		curr_mode = STEP;
		mCam.disableMouseInput();
		break;
	}
	case 's': {
		mCam.reset();
		curr_mode = MOVE;
		mCam.disableMouseInput();
		break;
	}
	case 'f': {
		curr_mode = FREE;
		mCam.enableMouseInput();
		break;
	}
	case 'h': {
		guiTabBar->toggleVisible();
		break;
	}
	case 'j': {
		success_path.clear();
		if (chess_man.judge_network(0)) {  
			while (!(chess_man.m_success.empty())) {
				success_path.addVertex(chess[teni2eighti(chess_man.m_success.top())].getPosition());
				chess_man.m_success.pop();
			}
		}
		else if (chess_man.judge_network(1)) {  //white?
			while (!(chess_man.m_success.empty())) {
				success_path.addVertex(chess[teni2eighti(chess_man.m_success.top())].getPosition());
				chess_man.m_success.pop();
			}
		}
		break;
	}
	case 'b': {
		if (chess_man.m_step>1) {
			int disappear_chess(-1), show_chess(-1);
			bool color;
			if (!chess_man.back(disappear_chess, show_chess, color)) {	//??????


				if (disappear_chess != -1 && show_chess != -1) {
					curr_board[teni2eighti(disappear_chess)] = 0;
					pre_chess = teni2eighti(show_chess);
				}
			}
			else {	//??????
				curr_board[teni2eighti(disappear_chess)] = 0;


				curr_board[teni2eighti(show_chess)] = !color + 1;
				pre_chess = teni2eighti(show_chess);



				//if ((chess_man.m_step+1) % 2) {  //white?
				//	curr_board[teni2eighti(show_chess)] = 1;
				//	pre_chess = teni2eighti(show_chess);
				//}
				//else {  //black?
				//	curr_board[teni2eighti(show_chess)] = 2;
				//	pre_chess = teni2eighti(show_chess);
				//}
			}
		}


		else {
			int disappear_chess(-1), show_chess(-1);
			bool color;
			chess_man.back(disappear_chess, show_chess, color);
			curr_board[teni2eighti(disappear_chess)] = 0;
			pre_chess = -1;
		}
		break;
	}
	case OF_KEY_HOME: {
		curr_mode = FREE;
		is_selected = false;
		mCam.enableMouseInput();
		for (auto & i : curr_board) {
			i = 0;
		}
		chess_man.m_step = 0;
		moving_chess = -1;
		select_chess = -1;
		pre_chess = -1;
		head_1 = -1;
		head_2 = -1;
		head_3 = -1;
		head_4 = -1;
		tail_1 = -1;
		tail_2 = -1;
		tail_3 = -1;
		tail_4 = -1;
		success_path.clear();

		chess_man.restart();

		//	chess_man.restart_v2(); test by dyy


		//	chess_man.init();
		//ofClear(ofColor(255, 0, 0));
		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	if (curr_mode == MOVE && is_selected) {
		int temp = xytoi(x, y);
		if (temp >= 0 && temp < 64)moving_chess = temp;
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (button == OF_MOUSE_BUTTON_LEFT) {
		switch (curr_mode) {
		case FREE: {
			is_selected = false;
			break;
		}
		case STEP: {
			step(x, y);
			break;
		}
		case MOVE: {
			if (!is_selected) {
				select_chess = select(x, y);
				break;
			}
			else {
				int after_chess = xytoi(x, y);
				int real_x(x), real_y(y);
				screen2board(real_x, real_y);
				if (chess_man.move(select_chess % 8 + 1, select_chess / 8 + 1, after_chess % 8 + 1, after_chess / 8 + 1)) {
					if (chess_man.m_step % 2) {
						++chess_man.m_step;//test
						curr_board[after_chess] = 1;
						curr_board[select_chess] = 0;
						moving_chess = -1;
						select_chess = -1;
						ofClear(ofColor(255, 0, 0));
						is_selected = false;

						head_1 = -1;
						head_2 = -1;
						head_3 = -1;
						head_4 = -1;
						tail_1 = -1;
						tail_2 = -1;
						tail_3 = -1;
						tail_4 = -1;

						if (chess_man.m_doubly_link[real_x][real_y][0].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][0].m_pioneer->m_pos != 0)head_1 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][0].m_pioneer->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][1].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][1].m_pioneer->m_pos != 0)head_2 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][1].m_pioneer->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][2].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][2].m_pioneer->m_pos != 0)head_3 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][2].m_pioneer->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][3].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][3].m_pioneer->m_pos != 0)head_4 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][3].m_pioneer->m_pos);

						if (chess_man.m_doubly_link[real_x][real_y][0].m_next != NULL)tail_1 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][0].m_next->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][1].m_next != NULL)tail_2 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][1].m_next->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][2].m_next != NULL)tail_3 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][2].m_next->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][3].m_next != NULL)tail_4 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][3].m_next->m_pos);
					}
					else {
						++chess_man.m_step;//test
						curr_board[after_chess] = 2;
						curr_board[select_chess] = 0;
						moving_chess = -1;
						select_chess = -1;
						ofClear(ofColor(255, 0, 0));
						is_selected = false;

						head_1 = -1;
						head_2 = -1;
						head_3 = -1;
						head_4 = -1;
						tail_1 = -1;
						tail_2 = -1;
						tail_3 = -1;
						tail_4 = -1;

						if (chess_man.m_doubly_link[real_x][real_y][0].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][0].m_pioneer->m_pos != 0)head_1 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][0].m_pioneer->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][1].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][1].m_pioneer->m_pos != 0)head_2 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][1].m_pioneer->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][2].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][2].m_pioneer->m_pos != 0)head_3 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][2].m_pioneer->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][3].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][3].m_pioneer->m_pos != 0)head_4 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][3].m_pioneer->m_pos);

						if (chess_man.m_doubly_link[real_x][real_y][0].m_next != NULL)tail_1 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][0].m_next->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][1].m_next != NULL)tail_2 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][1].m_next->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][2].m_next != NULL)tail_3 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][2].m_next->m_pos);
						if (chess_man.m_doubly_link[real_x][real_y][3].m_next != NULL)tail_4 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][3].m_next->m_pos);
					}
				}
				break;
			}
		}
		}
	}
	else if (button == OF_MOUSE_BUTTON_RIGHT) {
		is_selected = false;
		moving_chess = -1;
		select_chess = -1;
		ofClear(ofColor(255, 0, 0));
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::drawBoard(vector<int>& now) {
	wood.bind();
	ofDrawBox(0, 0, -15, board.getWidth(), board.getHeight(), -20);
	wood.unbind();
	board.draw(-board.getWidth() / 2, -board.getHeight() / 2);
	if (pre_chess >= 0 && pre_chess < 64) {
		ofSetColor(0, 255, 45);
		ofPoint pos(chess[pre_chess].getPosition());
		pos.z += 5;
		ofDrawSphere(pos, 10);
		ofSetColor(255, 0, 15);
		if (head_1 >= 0 && head_1 < 64) {
			pos = (chess[head_1].getPosition());
			pos.z += 5;
			ofDrawSphere(pos, 15);
		}
		if (head_2 >= 0 && head_2 < 64) {
			pos = (chess[head_2].getPosition());
			pos.z += 5;
			ofDrawSphere(pos, 15);
		}
		if (head_3 >= 0 && head_3 < 64) {
			pos = (chess[head_3].getPosition());
			pos.z += 5;
			ofDrawSphere(pos, 15);
		}
		if (head_4 >= 0 && head_4 < 64) {
			pos = (chess[head_4].getPosition());
			pos.z += 5;
			ofDrawSphere(pos, 15);
		}
		ofSetColor(15, 0, 255);
		if (tail_1 >= 0 && tail_1 < 64) {
			pos = (chess[tail_1].getPosition());
			pos.z += 5;
			ofDrawSphere(pos, 15);
		}
		if (tail_2 >= 0 && tail_2 < 64) {
			pos = (chess[tail_2].getPosition());
			pos.z += 5;
			ofDrawSphere(pos, 15);
		}
		if (tail_3 >= 0 && tail_3 < 64) {
			pos = (chess[tail_3].getPosition());
			pos.z += 5;
			ofDrawSphere(pos, 15);
		}
		if (tail_4 >= 0 && tail_4 < 64) {
			pos = (chess[tail_4].getPosition());
			pos.z += 5;
			ofDrawSphere(pos, 15);
		}
		ofSetColor(255);
	}
	for (int x(0); x < 8; ++x) {
		for (int y(0); y < 8; ++y) {
			switch (now[x + y * 8]) {
			case 0: break;//empty
			case 1: {    //white
				white_chess.bind();
				chess[x + y * 8].drawFaces();
				white_chess.unbind();
				break;
			}
			case 2: {    //black
				black_chess.bind();
				chess[x + y * 8].drawFaces();
				black_chess.unbind();
				break;
			}
			default:break;
			}
		}
	}
	if (curr_mode == MOVE) {
		ofSetColor(255, 0, 0);
		if (select_chess >= 0 && select_chess<64)ofDrawCircle(chess[select_chess].getPosition(), 40);
		ofSetColor(255);
		if (moving_chess >= 0 && moving_chess < 64) {
			if (white_moving && curr_board[moving_chess] == 0) {
				white_chess.bind();
				chess[moving_chess].drawFaces();
				white_chess.unbind();
			}
			else if (!white_moving && curr_board[moving_chess] == 0) {
				black_chess.bind();
				chess[moving_chess].drawFaces();
				black_chess.unbind();
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawBowl() {
	other_light.enable();
	wood_bowl.getTexture().bind();
	chess_bowl1.drawFaces();
	chess_bowl2.drawFaces();
	wood_bowl.getTexture().unbind();
	if (our_first) {
		white_chess.getTexture().bind();
		chess_stack2.drawFaces();
		white_chess.getTexture().unbind();
		black_chess.getTexture().bind();
		chess_stack1.drawFaces();
		black_chess.getTexture().unbind();
	}
	else {
		white_chess.getTexture().bind();
		chess_stack1.drawFaces();
		white_chess.getTexture().unbind();
		black_chess.getTexture().bind();
		chess_stack2.drawFaces();
		black_chess.getTexture().unbind();
	}
	other_light.disable();
}

void ofApp::drawMode() {
	ofVec3f pos(-board.getWidth() / 2 + board.getWidth() / 16 + 7.5 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 4.5* board.getHeight() / 8, 0);
	if (curr_mode == STEP) {
		step_png.draw(pos);
	}
	if (curr_mode == MOVE) {
		move_png.draw(pos);
	}
}

//--------------------------------------------------------------
int ofApp::xytoi(int x, int y) {
	ofVec3f zero(mCam.worldToScreen(ofVec3f(-board.getWidth() / 2, board.getHeight() / 2))),
		max(mCam.worldToScreen(ofVec3f(board.getWidth() / 2, -board.getHeight() / 2)));
	double x_ruler((max.x - zero.x) / 8), y_ruler((max.y - zero.y) / 8);
	return (floor((x - zero.x) / x_ruler) + 8 * floor((y - zero.y) / y_ruler));
}

//--------------------------------------------------------------
int ofApp::teni2eighti(int i) {
	i -= 11;
	return (i / 10) * 8 + i % 10;
}

//--------------------------------------------------------------
void ofApp::screen2board(int & x, int & y) {
	ofVec3f zero(mCam.worldToScreen(ofVec3f(-board.getWidth() / 2, board.getHeight() / 2))),
		max(mCam.worldToScreen(ofVec3f(board.getWidth() / 2, -board.getHeight() / 2)));
	double x_ruler((max.x - zero.x) / 8), y_ruler((max.y - zero.y) / 8);
	x = floor((x - zero.x) / x_ruler) + 1;
	y = floor((y - zero.y) / y_ruler) + 1;
}

//--------------------------------------------------------------
void ofApp::step(int x, int y) {
	unsigned i(xytoi(x, y));
	if (i < 0 || i >= 64)return;
	//success_path.addVertex(chess[i].getPosition());//test
	int real_x(x), real_y(y);
	screen2board(real_x, real_y);
	if ((chess_man.m_step % 2) == 0) {
		//++chess_man.m_step;//test
		if (chess_man.add(real_x, real_y)) {
			head_1 = -1;
			head_2 = -1;
			head_3 = -1;
			head_4 = -1;
			tail_1 = -1;
			tail_2 = -1;
			tail_3 = -1;
			tail_4 = -1;

			if (chess_man.m_doubly_link[real_x][real_y][0].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][0].m_pioneer->m_pos != 0)head_1 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][0].m_pioneer->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][1].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][1].m_pioneer->m_pos != 0)head_2 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][1].m_pioneer->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][2].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][2].m_pioneer->m_pos != 0)head_3 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][2].m_pioneer->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][3].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][3].m_pioneer->m_pos != 0)head_4 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][3].m_pioneer->m_pos);

			if (chess_man.m_doubly_link[real_x][real_y][0].m_next != NULL)tail_1 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][0].m_next->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][1].m_next != NULL)tail_2 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][1].m_next->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][2].m_next != NULL)tail_3 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][2].m_next->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][3].m_next != NULL)tail_4 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][3].m_next->m_pos);
			curr_board[i] = 1;
			pre_chess = i;
		}
		//if (chess_man.feasible(0, real_x, real_y)) { //?§Ø?????????????
		//	++chess_man.m_step;//test
		//	curr_board[i] = 1;
		//	chess_man.add(real_x, real_y);
		//	pre_chess = i;
		//}

	}
	else {
		//++chess_man.m_step;//test
		if (chess_man.add(real_x, real_y)) {
			head_1 = -1;
			head_2 = -1;
			head_3 = -1;
			head_4 = -1;
			tail_1 = -1;
			tail_2 = -1;
			tail_3 = -1;
			tail_4 = -1;

			if (chess_man.m_doubly_link[real_x][real_y][0].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][0].m_pioneer->m_pos != 0)head_1 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][0].m_pioneer->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][1].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][1].m_pioneer->m_pos != 0)head_2 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][1].m_pioneer->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][2].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][2].m_pioneer->m_pos != 0)head_3 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][2].m_pioneer->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][3].m_pioneer != NULL && chess_man.m_doubly_link[real_x][real_y][3].m_pioneer->m_pos != 0)head_4 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][3].m_pioneer->m_pos);

			if (chess_man.m_doubly_link[real_x][real_y][0].m_next != NULL)tail_1 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][0].m_next->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][1].m_next != NULL)tail_2 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][1].m_next->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][2].m_next != NULL)tail_3 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][2].m_next->m_pos);
			if (chess_man.m_doubly_link[real_x][real_y][3].m_next != NULL)tail_4 = teni2eighti(chess_man.m_doubly_link[real_x][real_y][3].m_next->m_pos);
			curr_board[i] = 2;
			pre_chess = i;
		}
		//if (chess_man.feasible(1, real_x, real_y)) {  //?§Ø?????????????
		//	++chess_man.m_step;//test
		//	curr_board[i] = 2;
		//	chess_man.add(real_x, real_y);
		//	pre_chess = i;
		//}
	}
}

//--------------------------------------------------------------
void ofApp::move_step(int x, int y) {
	unsigned i(xytoi(x, y));
	if (i < 0 || i >= 64)return;
	if ((chess_man.m_step % 2) == 0) {
		if (curr_board[i] == 0) { //?§Ø?????????????
			++chess_man.m_step;//test
			curr_board[i] = 2;
			curr_board[select_chess] = 0;
			moving_chess = -1;
			select_chess = -1;
			ofClear(ofColor(255, 0, 0));
			is_selected = false;
		}

	}
	else {
		if (curr_board[i] == 0) {  //?§Ø?????????????
								   //++chess_man.m_step;//test
			curr_board[i] = 1;
			curr_board[select_chess] = 0;
			moving_chess = -1;
			select_chess = -1;
			ofClear(ofColor(255, 0, 0));
			is_selected = false;
		}
	}
}

//--------------------------------------------------------------
int ofApp::select(int x, int y) {
	unsigned i(xytoi(x, y));
	if (i < 0 || i >= 64)return -1;
	else {
		if (((chess_man.m_step % 2) == 0 && curr_board[i] == 2) || ((chess_man.m_step % 2) == 1 && curr_board[i] == 1) || curr_board[i] == 0)return -1;

		if (curr_board[i] == 1)white_moving = true;
		else if (curr_board[i] == 2)white_moving = false;
		is_selected = true;
		return i;
	}
}

//--------------------------------------------------------------
void ofApp::drawHand() {
	unsigned i(xytoi(ofGetMouseX(), ofGetMouseY()));
	if (i != pre_chess) {
		switch (duel_mode) {
		case MvsC: {//or CvsM
			if (i < 64 && i >= 0) {
				if (our_first) {
					if ((chess_man.m_step % 2) == 0) {
						skin.bind();
						human_hand.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						human_hand.drawFaces();
						skin.unbind();
						robot_hand.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 2 * board.getHeight() / 8, 52);
						robot_hand.drawFaces();
					}
					else {
						robot_hand.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						robot_hand.drawFaces();
						skin.bind();
						human_hand.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 5.5 * board.getHeight() / 8, 12);
						human_hand.drawFaces();
						skin.unbind();
					}
				}
				else {
					if ((chess_man.m_step % 2) == 0) {
						robot_hand.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						robot_hand.drawFaces();
						skin.bind();
						human_hand.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 5.5 * board.getHeight() / 8, 12);
						human_hand.drawFaces();
						skin.unbind();
					}
					else {
						skin.bind();
						human_hand.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						human_hand.drawFaces();
						skin.unbind();
						robot_hand.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 2 * board.getHeight() / 8, 52);
						robot_hand.drawFaces();
					}
				}
				break;
			}
			else break;
		}
		case MvsM: {
			if (i < 64 && i >= 0) {
				if (our_first) {
					if ((chess_man.m_step % 2) == 0) {
						skin.bind();
						human_hand.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						human_hand.drawFaces();

						human_hand_sub.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 1.5 * board.getHeight() / 8, 12);
						human_hand_sub.drawFaces();
						skin.unbind();
					}
					else {
						skin.bind();
						human_hand_sub.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						human_hand_sub.drawFaces();

						human_hand.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 5.5 * board.getHeight() / 8, 12);
						human_hand.drawFaces();
						skin.unbind();
					}
				}
				else {
					if ((chess_man.m_step % 2) == 0) {
						skin.bind();
						human_hand_sub.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						human_hand_sub.drawFaces();

						human_hand.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 5.5 * board.getHeight() / 8, 12);
						human_hand.drawFaces();
						skin.unbind();
					}
					else {
						skin.bind();
						human_hand.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						human_hand.drawFaces();

						human_hand_sub.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 1.5 * board.getHeight() / 8, 12);
						human_hand_sub.drawFaces();
						skin.unbind();
					}
				}
				break;
			}
			else break;
		}
		case CvsC: {
			if (i < 64 && i >= 0) {
				if (our_first) {
					if ((chess_man.m_step % 2) == 0) {
						robot_hand_sub.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						robot_hand_sub.drawFaces();
						robot_hand.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 2 * board.getHeight() / 8, 52);
						robot_hand.drawFaces();
					}
					else {
						robot_hand.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						robot_hand.drawFaces();
						robot_hand_sub.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 5 * board.getHeight() / 8, 52);
						robot_hand_sub.drawFaces();
					}
				}
				else {
					if ((chess_man.m_step % 2) == 0) {
						robot_hand.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						robot_hand.drawFaces();
						robot_hand_sub.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 5 * board.getHeight() / 8, 52);
						robot_hand_sub.drawFaces();
					}
					else {
						robot_hand_sub.setPosition(chess[i].getPosition().x, chess[i].getPosition().y, chess[i].getPosition().z);
						robot_hand_sub.drawFaces();
						robot_hand.setPosition(-board.getWidth() / 2 + board.getWidth() / 16 + 8 * board.getWidth() / 8, board.getHeight() / 2 - board.getHeight() / 16 - 2 * board.getHeight() / 8, 52);
						robot_hand.drawFaces();
					}
				}
				break;
			}
			else break;
		}
		}
	}
}

//--------------------------------------------------------------
void ofApp::setguitab() {
	startmenu = new ofxUICanvas();
	startmenu->setDrawBack(false);
	startmenu->setColorFill(0);
	startmenu->setColorFillHighlight(ofColor(255, 0, 0));
	startmenu->setName("menu");
	startmenu->setFontSize(OFX_UI_FONT_SMALL, 60);
	startmenu->setFontSize(OFX_UI_FONT_LARGE, 30);
	startmenu->addLabel("Network Game ", OFX_UI_FONT_SMALL);
	startmenu->autoSizeToFitWidgets();
	ofxUILabelToggle* s = new ofxUILabelToggle("Start", false, 0, 0, 0, 0, OFX_UI_FONT_LARGE);
	s->setDrawBack(false);
	ofxUILabel* e = new ofxUILabel("ver 1.0", OFX_UI_FONT_MEDIUM);
	e->setDrawBack(false);
	startmenu->addLabel("????????????????????", OFX_UI_FONT_SMALL);
	startmenu->addWidgetDown(s, OFX_UI_ALIGN_CENTER);
	startmenu->addWidgetDown(e, OFX_UI_ALIGN_BOTTOM);
	startmenu->setPosition(100, 200);
	startmenu->autoSizeToFitWidgets();
	ofAddListener(startmenu->newGUIEvent, this, &ofApp::guiEvent);

	guiTabBar = new ofxUITabBar();
	guiTabBar->addLabel("Network Chess", OFX_UI_FONT_LARGE);
	guiTabBar->addFPS(OFX_UI_FONT_SMALL);
	guiTabBar->addLabel("Press 'h'-> visiable", OFX_UI_FONT_SMALL);
	guiTabBar->addSpacer();
	guiTabBar->setColorBack(10);

	ofxUICanvas* gui1 = new ofxUICanvas();
	gui1->setColorBack(10);
	gui1->setName("PLAYER");
	gui1->addLabel("PLAYER", OFX_UI_FONT_MEDIUM);
	gui1->addSpacer();
	vector<string> names = { "player vs player","player vs computer","computer vs computer" };
	gui1->addRadio("player_type", names);
	gui1->addLabelToggle("RESET", false);
	gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent, this, &ofApp::guiEvent);
	guiTabBar->addCanvas(gui1);

	ofxUICanvas* gui2 = new ofxUICanvas();
	gui2->setColorBack(10);
	gui2->setName("LEVEL");
	gui2->addLabel("LEVEL", OFX_UI_FONT_MEDIUM);
	gui2->addSpacer();
	vector<string> level = { "easy","normal","hard" };
	gui2->addRadio("computer_level", level);
	gui2->autoSizeToFitWidgets();
	ofAddListener(gui2->newGUIEvent, this, &ofApp::guiEvent);
	guiTabBar->addCanvas(gui2);

	ofxUICanvas* gui3 = new ofxUICanvas();
	gui3->setColorBack(10);
	gui3->setName("OPTION");
	gui3->addLabel("OPTION", OFX_UI_FONT_MEDIUM);
	gui3->addSpacer();
	gui3->addLabelToggle("BACK", false);
	gui3->addLabelToggle("HINT", false);
	gui3->addLabelToggle("SURRENDER", false);
	gui3->addLabelToggle("RESTART", false);
	gui3->autoSizeToFitWidgets();
	ofAddListener(gui3->newGUIEvent, this, &ofApp::guiEvent);
	guiTabBar->addCanvas(gui3);
	guiTabBar->toggleVisible();
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs & e) {
	mCam.disableMouseInput();
	string name(e.getName());
	if (name == "Start") {
		startmenu->toggleVisible();
		guiTabBar->toggleVisible();
		mCam.reset();
		curr_mode = FREE;
		mCam.enableMouseInput();
		return;
	}
	else if (name == "player vs player") {
		duel_mode = MvsM;
		robot_hand.setRotation(0, 0, 0, 0, 1);
		human_hand.setRotation(0, 0, 0, 0, 1);
		keyPressed(OF_KEY_HOME);
		return;
	}
	else if (name == "player vs computer") {
		duel_mode = MvsC;
		robot_hand.setRotation(0, 0, 0, 0, 1);
		human_hand.setRotation(0, 0, 0, 0, 1);
		keyPressed(OF_KEY_HOME);
		return;
	}
	else if (name == "computer vs computer") {
		duel_mode = CvsC;
		robot_hand.setRotation(0, 0, 0, 0, 1);
		human_hand.setRotation(0, 0, 0, 0, 1);
		keyPressed(OF_KEY_HOME);
		return;
	}
}
