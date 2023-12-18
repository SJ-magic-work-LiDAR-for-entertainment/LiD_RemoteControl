/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <stdio.h>

#include <ofMain.h>
#include <ofxGui.h>

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	enum{
		kNumInput_ = 2,
	};
	
	ofxGuiGroup Group_Camera_;
		ofxFloatSlider cam_wheel_speed_;
		ofxFloatSlider cam_move_speed_;
		ofxFloatSlider cam_rotate_speed_;
		
	ofxGuiGroup Group_Array_[kNumInput_];
		ofxToggle b_draw_point_[kNumInput_];
		ofxVec3Slider point_ofs_[kNumInput_];
		ofxFloatSlider rot_deg_[kNumInput_];
		ofxFloatSlider scale_lidar_[kNumInput_];
		
		ofxToggle b_apply_sound_sync_[kNumInput_];
		ofxFloatSlider sound_sync_coord_[kNumInput_];
		ofxFloatSlider sound_sync_anim_type_[kNumInput_];
		ofxFloatSlider sound_sync_hue_h_[kNumInput_];
		ofxFloatSlider sound_sync_hue_l_[kNumInput_];
		
	ofxGuiGroup Group_Misc_;
		ofxFloatSlider saturation_;
		ofxFloatSlider brightness_;
		ofxFloatSlider bloom_intensity_;
		ofxToggle b_disp_msg_packet_not_in_order_;
		
	ofxGuiGroup Group_ColorFade_;
		ofxToggle b_col_fade_use_sqr_for_distance_;
		ofxVec3Slider col_fade_org_point_;
		ofxFloatSlider hue_near_;
		ofxFloatSlider hue_far_;
		ofxFloatSlider near_;
		ofxFloatSlider far_;
		
	ofxGuiGroup Group_SoundSync_;
		// ofxFloatSlider sound_sync_size_h_;
		// ofxFloatSlider sound_sync_size_l_;
		ofxToggle b_disp_sound_center_;
		ofxFloatSlider scale_sound_center_;
		
	ofxGuiGroup Group_AreaFilter_;
		ofxToggle b_invert_x_[kNumInput_];
		
		ofxToggle b_limit_x_[kNumInput_];
		ofxFloatSlider min_x_[kNumInput_];
		ofxFloatSlider max_x_[kNumInput_];
		
		ofxToggle b_limit_z_[kNumInput_];
		ofxFloatSlider min_z_[kNumInput_];
		ofxFloatSlider max_z_[kNumInput_];
		
		ofxVec3Slider p_far_away;
		
	ofxGuiGroup Group_Control_;
		ofxToggle b_send_udp_;
		ofxToggle b_reload_gui_setting;
		
	ofxPanel gui;
	
	bool b_Disp = false;
};

/************************************************************
************************************************************/
template <typename T>
T sj_max(T a, T b)
{
	if(a < b)	return b;
	else		return a;
}

/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;
extern FILE* fp_Log;

/************************************************************
************************************************************/

