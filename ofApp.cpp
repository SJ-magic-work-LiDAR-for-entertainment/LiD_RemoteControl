/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(){
}

/******************************
******************************/
ofApp::~ofApp(){
	if(Gui_Global) delete Gui_Global;
	if(fp_Log)		fclose(fp_Log);
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("LiD:RemoteControl");
	
	ofSetWindowShape( static_cast<int>(WindowSize::kWidth_), static_cast<int>(WindowSize::kHeight_) );
	ofSetVerticalSync(false);	// trueとどっちがいいんだろう？
	ofSetFrameRate(30);
	
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	ofEnableAntiAliasing();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
	
	/********************
	********************/
	setup_Gui();
	LoadGuiSettingFile("gui.xml");
	
	/********************
	********************/
	SetupUdp();
}

/******************************
******************************/
void ofApp::SetupUdp(){
	/********************
	********************/
	{
		ofxUDPSettings settings;
		settings.sendTo("127.0.0.1", 12346);
		// settings.sendTo("10.0.0.10", 12345);
		settings.blocking = false;
		
		udp_Send_.Setup(settings);
	}
}
/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("param", "gui.xml", 10, 10);
}

/******************************
******************************/
void ofApp::LoadGuiSettingFile(string file_name){
	char buf[100];
	snprintf(buf, std::size(buf), "../../../data/%s", file_name.c_str());
	
	if(is_FileExist(buf)){
		Gui_Global->gui.loadFromFile(file_name);
		printf("> load gui setting\n\n");
		fflush(stdout);
	}else{
		printf("> GUI setting file not exists\n\n");
		fflush(stdout);
	}
}

/******************************
******************************/
bool ofApp::is_FileExist(string str_FileName){
	std::ifstream in{str_FileName, std::ios::in};
	if(in.is_open()){
		in.close();
		return true;
	}else{
		return false;
	}
	
	/*
	FILE* fp = fopen(str_FileName.c_str(), "r");
	if(fp){
		fclose(fp);
		return true;
	}else{
		return false;
	}
	*/
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	if(Gui_Global->b_reload_gui_setting)	{ Gui_Global->b_reload_gui_setting = false; LoadGuiSettingFile("gui.xml"); }
	
	/********************
	********************/
	PrepAndSendUdp();
}

/******************************
******************************/
void ofApp::PrepAndSendUdp(){
	/********************
	********************/
	if(!Gui_Global->b_send_udp_){
		if(b_log_send_message){
			printf("You should start sending udp to log.\n");
			fflush(stdout);
			b_log_send_message = false;
		}
		return;
	}
	
	/********************
	********************/
	string str_message = "/RemoteControl,";
	
	char buf[100];
	// Group_Camera_
	float cam_wheel_speed	= Gui_Global->cam_wheel_speed_;
	float cam_move_speed	= Gui_Global->cam_move_speed_;
	float cam_rotate_speed	= Gui_Global->cam_rotate_speed_;
	snprintf(buf, std::size(buf), "%.3f,%.3f,%.3f,", cam_wheel_speed, cam_move_speed, cam_rotate_speed);
	str_message += buf;
	
	// Group_Misc_
	float saturation					= Gui_Global->saturation_;
	float brightness					= Gui_Global->brightness_;
	float bloom_intensity				= Gui_Global->bloom_intensity_;
	bool b_disp_msg_packet_not_in_order	= Gui_Global->b_disp_msg_packet_not_in_order_;
	snprintf(buf, std::size(buf), "%.3f,%.3f,%.3f,%d,", saturation, brightness, bloom_intensity, b_disp_msg_packet_not_in_order);
	str_message += buf;
	
	// Group_ColorFade_
	bool b_col_fade_use_sqr_for_distance	= Gui_Global->b_col_fade_use_sqr_for_distance_;
	glm::vec3 col_fade_org_point			= Gui_Global->col_fade_org_point_;
	float hue_near							= Gui_Global->hue_near_;
	float hue_far							= Gui_Global->hue_far_;
	float near								= Gui_Global->near_;
	float far								= Gui_Global->far_;
	snprintf(buf, std::size(buf), "%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,", b_col_fade_use_sqr_for_distance, col_fade_org_point.x, col_fade_org_point.y, col_fade_org_point.z, hue_near, hue_far, near, far);
	str_message += buf;
	
	// Group_SoundSync
	// float sound_sync_size_h		= Gui_Global->sound_sync_size_h_;
	// float sound_sync_size_l		= Gui_Global->sound_sync_size_l_;
	bool b_disp_sound_center	= Gui_Global->b_disp_sound_center_;
	float scale_sound_center	= Gui_Global->scale_sound_center_;
	snprintf(buf, std::size(buf), "%d,%f,", b_disp_sound_center, scale_sound_center);
	str_message += buf;
	
	// Group_Array_[0]/ [1]
	for(int i = 0; i < GUI_GLOBAL::kNumInput_; i++){
		bool b_draw_point			= Gui_Global->b_draw_point_[i];
		glm::vec3 point_ofs			= Gui_Global->point_ofs_[i];
		float rot_deg				= Gui_Global->rot_deg_[i];
		float scale_lidar			= Gui_Global->scale_lidar_[i];
		bool b_apply_sound_sync		= Gui_Global->b_apply_sound_sync_[i];
		int sound_sync_coord		= (int)Gui_Global->sound_sync_coord_[i];
		int sound_sync_anim_type	= (int)Gui_Global->sound_sync_anim_type_[i];
		float sound_sync_hue_h		= Gui_Global->sound_sync_hue_h_[i];
		float sound_sync_hue_l		= Gui_Global->sound_sync_hue_l_[i];
		snprintf(buf, std::size(buf), "%d,%.3f,%.3f,%.3f,%.3f,%.3f,%d,%d,%d,%.3f,%.3f,", b_draw_point, point_ofs.x, point_ofs.y, point_ofs.z, rot_deg, scale_lidar, b_apply_sound_sync, sound_sync_coord, sound_sync_anim_type, sound_sync_hue_h, sound_sync_hue_l);
		str_message += buf;
	}
	
	// Group_AreaFilter_
	for(int i = 0; i < GUI_GLOBAL::kNumInput_; i++){
		bool b_invert_x		= Gui_Global->b_invert_x_[i];
		
		bool b_limit_x		= Gui_Global->b_limit_x_[i];
		float min_x			= Gui_Global->min_x_[i];
		float max_x			= Gui_Global->max_x_[i];
		
		bool b_limit_z		= Gui_Global->b_limit_z_[i];
		float min_z			= Gui_Global->min_z_[i];
		float max_z			= Gui_Global->max_z_[i];
		
		snprintf(buf, std::size(buf), "%d,%d,%.3f,%.3f,%d,%.3f,%.3f,", b_invert_x, b_limit_x, min_x, max_x, b_limit_z, min_z, max_z);
		str_message += buf;
	}
	
	glm::vec3 p_far_away	= Gui_Global->p_far_away;
	snprintf(buf, std::size(buf), "%.2f,%.2f,%.2f", p_far_away.x, p_far_away.y, p_far_away.z);
	str_message += buf;
	
	/********************
	********************/
	if(b_log_send_message)	LogUdpMessage(str_message);
	
	/********************
	********************/
	udp_Send_.Send(str_message.c_str(), str_message.length());
}

/******************************
******************************/
void ofApp::LogUdpMessage(string& str_message){
	/********************
	********************/
	fp_Log = fopen("../../../data/Log/Log.csv", "w");
	
	/********************
	********************/
	string str_title = "";
	str_title += getDatetimeStr();
	str_title += "\n";
	
	/********************
	********************/
	str_title += "address,";
	
	str_title += "cam_wheel_speed,move_speed,rotate_speed,";																								// Group_Camera_
	str_title += "saturation,brightness,bloom,b_disp_msg_pct_not_in_order,";																				// Group_Misc_
	str_title += "b_col_fade_use_sqr_for_distance,col_fade_org_point.x,y,z,hue_near,hue_far,near,far,";														// Group_ColorFade_
	str_title += "b_disp_sound_center,scale_sound_center,";																									// Group_SoundSync
	str_title += "[0]b_draw_point,point_ofs.x,y,z,rot_deg,scale_lidar,b_apply_sound_sync,sound_sync_coord,anim_type,sound_sync_hue_h,sound_sync_hue_l,";	// Group_Array_[0]
	str_title += "[1]b_draw_point,point_ofs.x,y,z,rot_deg,scale_lidar,b_apply_sound_sync,sound_sync_coord,anim_type,sound_sync_hue_h,sound_sync_hue_l,";	// Group_Array_[1]
	str_title += "[0]b_invert_x,b_limit_x,min_x,max_x,b_limit_z,min_z,max_z,";																				// Group_AreaFilter_[0]
	str_title += "[1]b_invert_x,b_limit_x,min_x,max_x,b_limit_z,min_z,max_z,";																				// Group_AreaFilter_[0]
	str_title += "p_far_away.x,y,z";																														// Group_AreaFilter_
	
	fprintf(fp_Log, "%s\n", str_title.c_str());
	
	/********************
	********************/
	fprintf(fp_Log, "%s\n", str_message.c_str());
	
	/********************
	********************/
	fclose(fp_Log);
	
	/********************
	********************/
	printf("Logged udp message\n");
	fflush(stdout);
	
	b_log_send_message = false;
}

/******************************
******************************/
string ofApp::getDatetimeStr() {
    time_t t = time(nullptr);
    const tm* localTime = localtime(&t);
	
    std::stringstream s;
    s << localTime->tm_year + 1900 << "/";
	
    // setw(),setfill()で0詰め
    s << setw(2) << setfill('0') << localTime->tm_mon + 1 << "/";
    s << setw(2) << setfill('0') << localTime->tm_mday << "/ ";
    s << setw(2) << setfill('0') << localTime->tm_hour << ":";
    s << setw(2) << setfill('0') << localTime->tm_min << ":";
    s << setw(2) << setfill('0') << localTime->tm_sec;
	
    return s.str(); // std::stringにして値を返す
}

/******************************
******************************/
void ofApp::draw(){
	ofBackground(0);
	
	Gui_Global->gui.draw();
	
	ofSetColor(255);
	string info;
	info += "FPS = " + ofToString(ofGetFrameRate(), 2) + "\n";
	ofDrawBitmapString(info, 30, 30);
}

/******************************
******************************/
void ofApp::exit(){
	printf("> Good-bye\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case 'o':
			Gui_Global->gui.maximizeAll();
			break;
			
		case 'm':
			Gui_Global->gui.minimizeAll();
			break;
			
		case 'l':
			b_log_send_message = true;
			break;
	}
}

/******************************
******************************/
void ofApp::keyReleased(int key){

}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
