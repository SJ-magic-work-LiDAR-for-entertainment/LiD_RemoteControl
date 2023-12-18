/************************************************************
************************************************************/
#include <sj_common.h>

/************************************************************
************************************************************/

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;
FILE* fp_Log = nullptr;


/************************************************************
func
************************************************************/

/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	Group_Camera_.setup("> Camera");
		Group_Camera_.add(cam_wheel_speed_.setup("wheel_speed", 10.0, 1.0, 30.0));
		Group_Camera_.add(cam_move_speed_.setup("move_speed", 0.1, 0.01, 0.5));
		Group_Camera_.add(cam_rotate_speed_.setup("rotate_speed", 0.3, 0.1, 1.0));
	gui.add(&Group_Camera_);
	
	Group_Array_[0].setup("> Array_0");
		Group_Array_[0].add(b_draw_point_[0].setup("b_draw_point_0", true));
		{
			float ofs_max = 50.0f;
			ofVec3f initVec3f	= ofVec3f(0, -0.75, 12.5);
			ofVec3f minVec3f	= ofVec3f(-ofs_max, -ofs_max, -ofs_max);
			ofVec3f maxVec3f	= ofVec3f( ofs_max,  ofs_max,  ofs_max);
			
			Group_Array_[0].add(point_ofs_[0].setup("point_ofs_0", initVec3f, minVec3f, maxVec3f));
		}
		Group_Array_[0].add(rot_deg_[0].setup("rot_deg_0", 0.0, 0.0, 180));
		Group_Array_[0].add(scale_lidar_[0].setup("scale_lidar_0", 0.03, 0.01, 0.2));
		
		Group_Array_[0].add(b_apply_sound_sync_[0].setup("b_apply_sound_sync_0", true));
		Group_Array_[0].add(sound_sync_coord_[0].setup("sound_sync_coord_0", 0.0, 0.0, 3.0));
		Group_Array_[0].add(sound_sync_anim_type_[0].setup("sound_sync_anim_type_0", 3.0, 0.0, 3.0));
		
		Group_Array_[0].add(sound_sync_hue_h_[0].setup("sound_sync_hue_h_0", 0.25, 0.0, 1.0));
		Group_Array_[0].add(sound_sync_hue_l_[0].setup("sound_sync_hue_l_0", 0.65, 0.0, 1.0));
	gui.add(&Group_Array_[0]);
	
	Group_Array_[1].setup("> Array_1");
		Group_Array_[1].add(b_draw_point_[1].setup("b_draw_point_1", true));
		{
			float ofs_max = 50.0f;
			ofVec3f initVec3f	= ofVec3f(0, 0, 0);
			ofVec3f minVec3f	= ofVec3f(-ofs_max, -ofs_max, -ofs_max);
			ofVec3f maxVec3f	= ofVec3f( ofs_max,  ofs_max,  ofs_max);
			
			Group_Array_[1].add(point_ofs_[1].setup("point_ofs_1", initVec3f, minVec3f, maxVec3f));
		}
		Group_Array_[1].add(rot_deg_[1].setup("rot_deg_1", 180, 0.0, 180));
		Group_Array_[1].add(scale_lidar_[1].setup("scale_lidar_1", 0.035, 0.01, 0.2));
		
		Group_Array_[1].add(b_apply_sound_sync_[1].setup("b_apply_sound_sync_1", true));
		Group_Array_[1].add(sound_sync_coord_[1].setup("sound_sync_coord_1", 0.0, 0.0, 3.0));
		Group_Array_[1].add(sound_sync_anim_type_[1].setup("sound_sync_anim_type_1", 2.0, 0.0, 3.0));
		
		Group_Array_[1].add(sound_sync_hue_h_[1].setup("sound_sync_hue_h_1", 0.1, 0.0, 1.0));
		Group_Array_[1].add(sound_sync_hue_l_[1].setup("sound_sync_hue_l_1", 0.65, 0.0, 1.0));
	gui.add(&Group_Array_[1]);
	
	Group_Misc_.setup("> Misc");
		Group_Misc_.add(saturation_.setup("saturation_", 0.75, 0.0, 1.0));
		Group_Misc_.add(brightness_.setup("brightness_", 0.8, 0.0, 1.0));
		Group_Misc_.add(bloom_intensity_.setup("bloom", 3.3, 0.0, 20.0));
		Group_Misc_.add(b_disp_msg_packet_not_in_order_.setup("warn_pct_not_in_order_", true));
	gui.add(&Group_Misc_);
	
	Group_ColorFade_.setup("> ColorFade");
		Group_ColorFade_.add(b_col_fade_use_sqr_for_distance_.setup("use_sqr_for_distance", true));
		{
			float ofs_max = 100.0f;
			ofVec3f initVec3f	= ofVec3f(0, 0, 0);
			ofVec3f minVec3f	= ofVec3f(-ofs_max, -ofs_max, -ofs_max);
			ofVec3f maxVec3f	= ofVec3f( ofs_max,  ofs_max,  ofs_max);
			
			Group_ColorFade_.add(col_fade_org_point_.setup("col_fade_org_point", initVec3f, minVec3f, maxVec3f));
		}
		Group_ColorFade_.add(hue_near_.setup("hue_near", 0.0, 0.0, 1.0));
		Group_ColorFade_.add(hue_far_.setup("hue_far", 0.65, 0.0, 1.0));
		Group_ColorFade_.add(near_.setup("near", 1.0, 0.0, 200.0));
		Group_ColorFade_.add(far_.setup("far", 80.0, 0.0, 200.0));
	gui.add(&Group_ColorFade_);
	
	Group_SoundSync_.setup("> SoundSync");
		// Group_SoundSync_.add(sound_sync_size_h_.setup("sound_sync_size_h", 0.07, 0.0, 0.2));
		// Group_SoundSync_.add(sound_sync_size_l_.setup("sound_sync_size_l", 0.03, 0.0, 0.2));
		Group_SoundSync_.add(b_disp_sound_center_.setup("b_disp_sound_center", false));
		Group_SoundSync_.add(scale_sound_center_.setup("scale_sound_center_", 1.0, 0.1, 5.0));
	gui.add(&Group_SoundSync_);
		
	Group_AreaFilter_.setup("> AreaFilter");
		/* */
		Group_AreaFilter_.add(b_invert_x_[0].setup("b_invert_x_0", true));
		
		Group_AreaFilter_.add(b_limit_x_[0].setup("b_limit_x_0", true));
		Group_AreaFilter_.add(min_x_[0].setup("min_x_0", -1.3f, -50.0f, 0.0f));
		Group_AreaFilter_.add(max_x_[0].setup("max_x_0", 0.62f, 0.0, 50.0f));
		
		Group_AreaFilter_.add(b_limit_z_[0].setup("b_limit_z_0", true));
		Group_AreaFilter_.add(min_z_[0].setup("min_z_0", 0.0f, 0.0, 50.0f));
		Group_AreaFilter_.add(max_z_[0].setup("max_z_0", 4.0f, 0.0, 50.0f));
		
		/* */
		Group_AreaFilter_.add(b_invert_x_[1].setup("b_invert_x_1", true));
		
		Group_AreaFilter_.add(b_limit_x_[1].setup("b_limit_x_1", false));
		Group_AreaFilter_.add(min_x_[1].setup("min_x_1", -10.0f, -50.0f, 0.0f));
		Group_AreaFilter_.add(max_x_[1].setup("max_x_1",  10.0f, 0.0, 50.0f));
		
		Group_AreaFilter_.add(b_limit_z_[1].setup("b_limit_z_1", true));
		Group_AreaFilter_.add(min_z_[1].setup("min_z_1", 3.7f, 0.0, 50.0f));
		Group_AreaFilter_.add(max_z_[1].setup("max_z_1", 300.0f, 0.0, 300.0f));
		
		/* */
		{
			float ofs_max = 1000.0f;
			ofVec3f initVec3f	= ofVec3f(0, 0, -100);
			ofVec3f minVec3f	= ofVec3f(-ofs_max, -ofs_max, -ofs_max);
			ofVec3f maxVec3f	= ofVec3f( ofs_max,  ofs_max,  ofs_max);
			
			Group_AreaFilter_.add(p_far_away.setup("p_far_away", initVec3f, minVec3f, maxVec3f));
		}
	gui.add(&Group_AreaFilter_);
	
	Group_Control_.setup("> Control");
		Group_Control_.add(b_send_udp_.setup("b_send_udp_", false));
		Group_Control_.add(b_reload_gui_setting.setup("ReloadSetting", false));
	gui.add(&Group_Control_);
	
	/********************
	********************/
	gui.minimizeAll();
}

