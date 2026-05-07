#include "scene.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "scene_manager.h"
#include "character_select.h"
#include "cloths_scene.h"
#include "atlas.h"
#include "util.h"

#include <graphics.h>
#include <windows.h>

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* cloths_scene = nullptr;

Camera main_camera;
SceneManager scene_manager;

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

IMAGE img_yang_lihui;
IMAGE img_mlbear_lihui;

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
    loadimage(&img_car_background_1, _T("resource/car_background_1.png"));
    loadimage(&img_car_background_2, _T("resource/car_background_2.png"));

    loadimage(&img_yang_lihui, _T("resource/yang_lihui.png"));
    loadimage(&img_mlbear_lihui, _T("resource/red_lihui.png"));

    atlas_yang_walk_left.load_from_file(_T("resource/yang_left_walk_%d.PNG"), 7);
    flip_atlas(atlas_yang_walk_left, atlas_yang_walk_right);

    atlas_yang_jump_left.load_from_file(_T("resource/yang_left_jump_%d.png"), 4);
    flip_atlas(atlas_yang_jump_left, atlas_yang_jump_right);

    atlas_attack1_left.load_from_file(_T("resource/yang_left_attack1_%d.png"), 3);
    flip_atlas(atlas_attack1_left, atlas_attack1_right);

    atlas_attack2_left.load_from_file(_T("resource/yang_left_attack2_%d.png"), 2);
    flip_atlas(atlas_attack2_left, atlas_attack2_right);

    atlas_yang_attack_left.load_from_file(_T("resource/yang_left_box_%d.png"), 5);

    atlas_mlbear_walk_left.load_from_file(_T("resource/mlbear_left_walk_%d.png"), 8);
    flip_atlas(atlas_mlbear_walk_left, atlas_mlbear_walk_right);

    atlas_mlbear_jump_left.load_from_file(_T("resource/mlbear_left_jump_%d.png"), 4);
    flip_atlas(atlas_mlbear_jump_left, atlas_mlbear_jump_right);

    atlas_mlbear_attack1_left.load_from_file(_T("resource/mlbear_left_attack1_%d.png"), 15);
    flip_atlas(atlas_mlbear_attack1_left, atlas_mlbear_attack1_right);

    atlas_mlbear_attack2_left.load_from_file(_T("resource/mlbear_left_attack2_%d.png"), 18);
    flip_atlas(atlas_mlbear_attack2_left, atlas_mlbear_attack2_right);

    atlas_mlbear_box.load_from_file(_T("resource/mlbear_box_%d.png"), 5);
}

int main()
{
    ExMessage msg;
    const int FPS = 60;

    initgraph(1920, 1080, EW_SHOWCONSOLE);

    load_game_resources();

    BeginBatchDraw();

    menu_scene = new MenuScene();
    cloths_scene = new ClothsScene();
    game_scene = new GameScene();

    scene_manager.set_current_scene(menu_scene);

    DWORD last_tick_time = GetTickCount();

    while (true)
    {
        DWORD frame_start_time = GetTickCount();

        while (peekmessage(&msg))
        {
            scene_manager.on_input(msg);
        }

        DWORD current_tick_time = GetTickCount();
        DWORD delta_tick = current_tick_time - last_tick_time;
        last_tick_time = current_tick_time;

        scene_manager.on_update((int)delta_tick);

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

    delete menu_scene;
    delete cloths_scene;
    delete game_scene;

    return 0;
}
