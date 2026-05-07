#include "scene.h"
#include "scene_manager.h"
#include "menu_scene.h"
#include "cloths_scene.h"
#include "game_scene.h"
#include "camera.h"

#include <graphics.h>
#include <windows.h>

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* cloths_scene = nullptr;

SceneManager scene_manager;
Camera main_camera;

IMAGE img_menu_background;
IMAGE img_cloths_background;
IMAGE img_game_background_1;

void load_game_resources()
{
    loadimage(&img_menu_background, _T("resource/menu_background.PNG"));
    loadimage(&img_cloths_background, _T("resource/cloths_background.PNG"));
    loadimage(&img_game_background_1, _T("resource/game_background_2.png"));
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

        scene_manager.on_update(delta_tick);

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
