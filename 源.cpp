#include"scene.h"
#include"menu_scene.h"
#include"game_scene.h"	
#include"scene_manager.h"
#include<graphics.h>
#include "character_select.h"
#include"cloths_scene.h"
#include"atlas.h"
#include"util.h"
#include<iostream>
#include "audio_manager.h"

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Camera main_camera;
SceneManager scene_manager;
Scene* cloths_scene = nullptr;
AudioManager audio_manager;
CharacterSelect selected_character = CharacterSelect::yang;
IMAGE img_menu_background;
IMAGE img_cloths_background;


IMAGE img_game_background_1;
IMAGE img_game_background_2;
IMAGE img_basketball_background;
IMAGE img_special_background;
IMAGE img_special_background_2;
IMAGE img_car_background_1;
IMAGE img_car_background_2;

IMAGE img_xd;
IMAGE img_picture_1;
IMAGE img_picture_2;
IMAGE img_xinhua;
IMAGE img_xinhua_1;
IMAGE img_xinhua_2;
IMAGE img_xinhua_3;
IMAGE img_xinhua_5;
IMAGE img_yang_lihui;
IMAGE img_mlbear_lihui;

Atlas atlas_yang_idle_left;
Atlas atlas_yang_idle_right;
Atlas atlas_yang_jump_left;
Atlas atlas_yang_jump_right;
Atlas atlas_attack1_left;
Atlas atlas_attack1_right;
Atlas atlas_attack2_left;
Atlas atlas_attack2_right;
Atlas atlas_yang_attack_left;
Atlas atlas_yang_walk_left;
Atlas atlas_yang_walk_right;

Atlas atlas_mlbear_walk_left;
Atlas atlas_mlbear_walk_right;
Atlas atlas_mlbear_jump_right;
Atlas atlas_mlbear_jump_left;
Atlas atlas_mlbear_attack1_left;
Atlas atlas_mlbear_attack1_right;
Atlas atlas_mlbear_attack2_left;
Atlas atlas_mlbear_attack2_right;
Atlas atlas_mlbear_box;

Atlas atlas_enemy_small1_walk_right;
Atlas atlas_enemy_small1_walk_left;
Atlas atlas_enemy_small1_attack_right;
Atlas atlas_enemy_small1_attack_left;
Atlas atlas_enemy_small1_hurt_right;
Atlas atlas_enemy_small1_hurt_left;


Atlas atlas_enemy_small2_walk_right;
Atlas atlas_enemy_small2_walk_left;
Atlas atlas_enemy_small2_attack_right;
Atlas atlas_enemy_small2_attack_left;
Atlas atlas_enemy_small2_hurt_right;
Atlas atlas_enemy_small2_hurt_left;

Atlas atlas_enemy_boss1_idle_right;
Atlas atlas_enemy_boss1_idle_left;
Atlas atlas_enemy_boss1_walk_right;
Atlas atlas_enemy_boss1_walk_left;
Atlas atlas_enemy_boss1_attack_right;
Atlas atlas_enemy_boss1_attack_left;
Atlas atlas_enemy_boss1_hurt_right;
Atlas atlas_enemy_boss1_hurt_left;
Atlas atlas_enemy_boss1_dead_right;
Atlas atlas_enemy_boss1_dead_left;


Atlas atlas_enemy_boss2_idle_right;
Atlas atlas_enemy_boss2_idle_left;

Atlas atlas_enemy_boss2_walk_right;
Atlas atlas_enemy_boss2_walk_left;
Atlas atlas_enemy_boss2_attack1_right;
Atlas atlas_enemy_boss2_attack1_left;

Atlas atlas_enemy_boss2_attack2_right;
Atlas atlas_enemy_boss2_attack2_left;

Atlas atlas_enemy_boss2_attack3_right;
Atlas atlas_enemy_boss2_attack3_left;

Atlas atlas_enemy_boss2_attack4_right;
Atlas atlas_enemy_boss2_attack4_left;

Atlas atlas_enemy_boss2_hurt_right;
Atlas atlas_enemy_boss2_hurt_left;
Atlas atlas_enemy_boss2_dead_right;
Atlas atlas_enemy_boss2_dead_left;
       
void flip_atlas(Atlas& src, Atlas& dst)
{
	dst.clear();
	for (int i = 0; i < src.get_size(); i++)
	{
		IMAGE img_flipped;
		flip_image(src.get_image(i), &img_flipped);
		dst.add_image(img_flipped);
	}
}


		
void load_game_resources()
{
	
	AddFontResourceEx(_T("resource/IPix.ttf"), FR_PRIVATE, NULL);
	loadimage(&img_menu_background, _T("resource/menu_background.PNG"));
	loadimage(&img_cloths_background, _T("resource/cloths_background.PNG"));
	loadimage(&img_game_background_1, _T("resource/game_background_2.png"));
	loadimage(&img_game_background_2, _T("resource/game_background_3.png"));
	loadimage(&img_basketball_background, _T("resource/basketball_background.png"));
	loadimage(&img_special_background, _T("resource/special_background2.png"));
	loadimage(&img_special_background_2, _T("resource/special_background_2.png"));
	loadimage(&img_xd, _T("resource/xd.png"));
	loadimage(&img_picture_1, _T("resource/picture1.png"));
	loadimage(&img_picture_2, _T("resource/picture2.png"));
	loadimage(&img_xinhua, _T("resource/xinhua.png"));
	loadimage(&img_xinhua_1, _T("resource/xinhua_1.png"));
	loadimage(&img_xinhua_2, _T("resource/xinhua_2.png"));
	loadimage(&img_xinhua_3, _T("resource/xinhua_3.png"));
	loadimage(&img_car_background_1, _T("resource/car_background_1.png"));
	loadimage(&img_car_background_2, _T("resource/car_background_2.png"));
	loadimage(&img_xinhua_5, _T("resource/xinhua_5.png"));
	loadimage(&img_yang_lihui, _T("resource/yang_lihui.png"));
	loadimage(&img_mlbear_lihui, _T("resource/red_lihui.png"));
	atlas_yang_walk_left.load_from_file(_T("resource/yang_left_walk_%d.PNG"), 7);
	flip_atlas(atlas_yang_walk_left, atlas_yang_walk_right);

	atlas_attack1_left.load_from_file(_T("resource/yang_left_attack1_%d.png"), 3);
	flip_atlas(atlas_attack1_left, atlas_attack1_right);

	atlas_attack2_left.load_from_file(_T("resource/yang_left_attack2_%d.png"), 2);
	flip_atlas(atlas_attack2_left, atlas_attack2_right);
	 
	atlas_yang_attack_left.load_from_file(_T("resource/yang_left_box_%d.png"), 5);
	atlas_yang_jump_left.load_from_file(_T("resource/yang_left_jump_%d.png"),4);
	flip_atlas(atlas_yang_jump_left, atlas_yang_jump_right);

	atlas_mlbear_walk_right.load_from_file(_T("resource/red_walk_right%d.png"), 8);
	flip_atlas(atlas_mlbear_walk_right, atlas_mlbear_walk_left);

	atlas_mlbear_jump_right.load_from_file(_T("resource/red_jump_right%d.png"), 9);
	flip_atlas(atlas_mlbear_jump_right, atlas_mlbear_jump_left);

	atlas_mlbear_attack1_right.load_from_file(_T("resource/red_right_attack1_%d.png"), 15);
	flip_atlas(atlas_mlbear_attack1_right, atlas_mlbear_attack1_left);

	atlas_mlbear_attack2_right.load_from_file(_T("resource/red_right_attack2_%d.png"), 18);
	flip_atlas(atlas_mlbear_attack2_right, atlas_mlbear_attack2_left);
	atlas_mlbear_box.load_from_file(_T("resource/red_box_%d.png"), 33);
	
	
	atlas_enemy_small1_walk_right.load_from_file(_T("resource/xiaobing1_!%d.png"), 5);
	flip_atlas(atlas_enemy_small1_walk_right, atlas_enemy_small1_walk_left);

	atlas_enemy_small1_attack_right.load_from_file(_T("resource/xiaobing1_attack_%d.png"), 7);
	flip_atlas(atlas_enemy_small1_attack_right, atlas_enemy_small1_attack_left);

	atlas_enemy_small1_hurt_right.load_from_file(_T("resource/xiaobing1_hit_%d.png"), 1);
	flip_atlas(atlas_enemy_small1_hurt_right, atlas_enemy_small1_hurt_left);


	
	atlas_enemy_small2_walk_right.load_from_file(_T("resource/xiaobing2_walk_%d.png"), 5);
	flip_atlas(atlas_enemy_small2_walk_right, atlas_enemy_small2_walk_left);

	atlas_enemy_small2_attack_right.load_from_file(_T("resource/xiaobing2_attack_%d.png"), 7);
	flip_atlas(atlas_enemy_small2_attack_right, atlas_enemy_small2_attack_left);

	atlas_enemy_small2_hurt_right.load_from_file(_T("resource/xiaobing2_hit_%d.png"), 1);
	flip_atlas(atlas_enemy_small2_hurt_right, atlas_enemy_small2_hurt_left);

	atlas_enemy_boss1_idle_right.load_from_file(_T("resource/boss1_ilde_%d.png"), 7);
	flip_atlas(atlas_enemy_boss1_idle_right, atlas_enemy_boss1_idle_left);
	
	atlas_enemy_boss1_walk_right.load_from_file(_T("resource/boss1_fly_%d.png"), 6);
	flip_atlas(atlas_enemy_boss1_walk_right, atlas_enemy_boss1_walk_left);

	atlas_enemy_boss1_attack_right.load_from_file(_T("resource/boss1_attack_%d.png"), 8);
	flip_atlas(atlas_enemy_boss1_attack_right, atlas_enemy_boss1_attack_left);

	atlas_enemy_boss1_hurt_right.load_from_file(_T("resource/boss1_hit_%d.png"), 6);
	flip_atlas(atlas_enemy_boss1_hurt_right, atlas_enemy_boss1_hurt_left);

	atlas_enemy_boss1_dead_right.load_from_file(_T("resource/boss1_death_%d.png"), 8);
	flip_atlas(atlas_enemy_boss1_dead_right, atlas_enemy_boss1_dead_left);

	
	atlas_enemy_boss2_idle_right.load_from_file(_T("resource/boss2_idle_%d.png"), 3);
	flip_atlas(atlas_enemy_boss2_idle_right, atlas_enemy_boss2_idle_left);

	atlas_enemy_boss2_walk_right.load_from_file(_T("resource/01_run_0%d.png"), 6);
	flip_atlas(atlas_enemy_boss2_walk_right, atlas_enemy_boss2_walk_left);

	atlas_enemy_boss2_attack1_right.load_from_file(_T("resource/02_combo_claw_0%d.png"), 5);
	flip_atlas(atlas_enemy_boss2_attack1_right, atlas_enemy_boss2_attack1_left);

	
	atlas_enemy_boss2_attack2_right.load_from_file(_T("resource/03_upper_claw_0%d.png"), 5);
	flip_atlas(atlas_enemy_boss2_attack2_right, atlas_enemy_boss2_attack2_left);

	
	atlas_enemy_boss2_attack3_right.load_from_file(_T("resource/04_down_claw_0%d.png"), 5);
	flip_atlas(atlas_enemy_boss2_attack3_right, atlas_enemy_boss2_attack3_left);

	
	atlas_enemy_boss2_attack4_right.load_from_file(_T("resource/07_energy_claw_0%d.png"), 9);
	flip_atlas(atlas_enemy_boss2_attack4_right, atlas_enemy_boss2_attack4_left);

	
	atlas_enemy_boss2_hurt_right.load_from_file(_T("resource/08_hit_0%d.png"), 1);
	flip_atlas(atlas_enemy_boss2_hurt_right, atlas_enemy_boss2_hurt_left);
	
	atlas_enemy_boss2_dead_right.load_from_file(_T("resource/boss2_death_%d.png"), 4);
	flip_atlas(atlas_enemy_boss2_dead_right, atlas_enemy_boss2_dead_left);
	audio_manager.load_bgm("xuanren", "resource/xuanren.mp3");
	audio_manager.load_bgm("xiaobing", "resource/xiaobing.mp3");
	audio_manager.load_bgm("boss1", "resource/boss1.mp3");
	audio_manager.load_bgm("boss2", "resource/boss2.mp3");

	audio_manager.load_sfx("queren", "resource/queren.mp3");

	audio_manager.load_sfx("light_punch", "resource/light punch.mp3");
	audio_manager.load_sfx("heavy_punch", "resource/heavy punch.mp3");

	audio_manager.load_sfx("chutui", "resource/chutui.mp3");
	audio_manager.load_sfx("kick1", "resource/kick 1.mp3");
	audio_manager.load_sfx("kick2", "resource/kick 2.mp3");
	audio_manager.load_bgm("end1", "resource/end1.mp3");
	audio_manager.load_bgm("end2", "resource/end2.mp3");
}
int main()
{
	ExMessage msg;
	const int FPS = 60;
	
	initgraph(1920, 1080, EW_SHOWCONSOLE);
	load_game_resources();
	BeginBatchDraw();

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	cloths_scene = new ClothsScene();
	scene_manager.set_current_scene(menu_scene);


	while (true)
	{
		DWORD frame_start_time = GetTickCount();
		while (peekmessage(&msg))
		{
			scene_manager.on_input(msg);
		}
		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick = current_tick_time - last_tick_time;

		scene_manager.on_update(delta_tick);
		last_tick_time = current_tick_time;
		cleardevice();
		scene_manager.on_draw(main_camera);
		FlushBatchDraw();
		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS)
		{
			Sleep(1000 / FPS - frame_delta_time);
		}
	}
	EndBatchDraw();

}
