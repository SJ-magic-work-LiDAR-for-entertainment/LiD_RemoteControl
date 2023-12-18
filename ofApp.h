/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxNetwork.h"

#include "sj_common.h"

/************************************************************
************************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	/********************
	********************/
	enum class WindowSize{
		kWidth_		= 300,
		kHeight_	= 1450,
	};
	
	/********************
	********************/
	ofxUDPManager udp_Send_;
	
	bool b_log_send_message = false;
	
	/****************************************
	****************************************/
	void SetupUdp();
	void setup_Gui();
	void LoadGuiSettingFile(string file_name);
	bool is_FileExist(string str_FileName);
	void PrepAndSendUdp();
	void LogUdpMessage(string& str_message);
	string getDatetimeStr();
	
public:
	/****************************************
	****************************************/
	ofApp();
	~ofApp();
	
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;
	
	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y ) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	void mouseEntered(int x, int y) override;
	void mouseExited(int x, int y) override;
	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;
};
