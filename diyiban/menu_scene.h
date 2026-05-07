#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene.h"
#include "scene_manager.h"

#include <graphics.h>

extern SceneManager scene_manager;
extern IMAGE img_menu_background;

class MenuScene : public Scene
{
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter()
    {
    }

    void on_update(int delta)
    {
    }

    void on_draw(const Camera& camera)
    {
        if (img_menu_background.getwidth() > 0)
        {
            putimage(0, 0, &img_menu_background);
        }
        else
        {
            setbkcolor(BLACK);
            cleardevice();

            setbkmode(TRANSPARENT);
            settextcolor(WHITE);
            settextstyle(60, 0, _T("Consolas"));
            outtextxy(650, 420, _T("MENU SCENE"));

            settextstyle(36, 0, _T("Consolas"));
            outtextxy(620, 520, _T("Press any key to start"));
        }
    }

    void on_input(const ExMessage& msg)
    {
        if (msg.message == WM_KEYDOWN)
        {
            scene_manager.switch_to(SceneManager::SceneType::Cloths);
        }
    }

    void on_exit()
    {
    }
};

#endif
