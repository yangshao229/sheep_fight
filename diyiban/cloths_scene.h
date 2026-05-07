#ifndef _CLOTHS_SCENE_H_
#define _CLOTHS_SCENE_H_

#include "scene.h"
#include "scene_manager.h"

#include <graphics.h>

extern SceneManager scene_manager;
extern IMAGE img_cloths_background;

class ClothsScene : public Scene
{
public:
    ClothsScene() = default;
    ~ClothsScene() = default;

    void on_enter()
    {
    }

    void on_update(int delta)
    {
    }

    void on_draw(const Camera& camera)
    {
        if (img_cloths_background.getwidth() > 0)
        {
            putimage(0, 0, &img_cloths_background);
        }
        else
        {
            setbkcolor(RGB(30, 30, 40));
            cleardevice();
        }

        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        settextstyle(60, 0, _T("Consolas"));
        outtextxy(610, 380, _T("CLOTHS SCENE"));

        settextstyle(36, 0, _T("Consolas"));
        outtextxy(610, 500, _T("Press J to Game"));
        outtextxy(610, 560, _T("Press K to Menu"));
    }

    void on_input(const ExMessage& msg)
    {
        if (msg.message != WM_KEYDOWN)
            return;

        if (msg.vkcode == 'J')
        {
            scene_manager.switch_to(SceneManager::SceneType::Game);
        }
        else if (msg.vkcode == 'K')
        {
            scene_manager.switch_to(SceneManager::SceneType::Menu);
        }
    }

    void on_exit()
    {
    }
};

#endif
