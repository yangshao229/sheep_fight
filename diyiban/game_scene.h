#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "scene_manager.h"

#include <graphics.h>

extern SceneManager scene_manager;
extern IMAGE img_game_background_1;

class GameScene : public Scene
{
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter()
    {
    }

    void on_update(int delta)
    {
    }

    void on_draw(const Camera& camera)
    {
        if (img_game_background_1.getwidth() > 0)
        {
            putimage(0, 0, &img_game_background_1);
        }
        else
        {
            setbkcolor(RGB(20, 40, 30));
            cleardevice();
        }

        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        settextstyle(60, 0, _T("Consolas"));
        outtextxy(650, 420, _T("GAME SCENE"));

        settextstyle(36, 0, _T("Consolas"));
        outtextxy(650, 540, _T("Press K to Menu"));
    }

    void on_input(const ExMessage& msg)
    {
        if (msg.message != WM_KEYDOWN)
            return;

        if (msg.vkcode == 'K')
        {
            scene_manager.switch_to(SceneManager::SceneType::Menu);
        }
    }

    void on_exit()
    {
    }
};

#endif
