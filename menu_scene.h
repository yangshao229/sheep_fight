#ifndef _MENU_SCENE_H
#define _MENU_SCENE_H

#include "scene_manager.h"
#include "scene.h"
#include "animation.h"
#include "audio_manager.h"

#include <graphics.h>
#include <iostream>

extern SceneManager scene_manager;
extern IMAGE img_menu_background;
extern AudioManager audio_manager;

class MenuScene : public Scene
{
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter()
    {
        audio_manager.play_bgm("xuanren", false);
    }

    void on_update(int delta)
    {
    }

    void on_draw(const Camera& camera)
    {
        putimage(90, 0, &img_menu_background);
    }

    void on_input(const ExMessage& msg)
    {
        if (msg.message == WM_KEYUP)
        {
            audio_manager.play_sfx("queren");
            scene_manager.switch_to(SceneManager::SceneType::Cloths);
        }
    }

    void on_exit()
    {
    }
};

#endif