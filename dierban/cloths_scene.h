#ifndef _CLOTHS_SCENE_H_
#define _CLOTHS_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "character_select.h"
#include "animation.h"
#include "atlas.h"

#include <graphics.h>

extern SceneManager scene_manager;

extern IMAGE img_cloths_background;
extern IMAGE img_yang_lihui;
extern IMAGE img_mlbear_lihui;

extern Atlas atlas_yang_attack_left;
extern Atlas atlas_mlbear_box;

extern CharacterSelect selected_character;

class ClothsScene : public Scene
{
public:
    ClothsScene() = default;
    ~ClothsScene() = default;

    void on_enter()
    {
        current_index = 0;

        animation_yang_preview.set_atlas(&atlas_yang_attack_left);
        animation_yang_preview.set_interval(100);
        animation_yang_preview.set_loop(true);

        animation_malai_preview.set_atlas(&atlas_mlbear_box);
        animation_malai_preview.set_interval(120);
        animation_malai_preview.set_loop(true);
    }

    void on_update(int delta)
    {
        if (current_index == 0)
        {
            animation_yang_preview.on_update(delta);
        }
        else
        {
            animation_malai_preview.on_update(delta);
        }
    }

    void on_draw(const Camera& camera)
    {
        if (img_cloths_background.getwidth() > 0)
        {
            putimage(90, 0, &img_cloths_background);
        }
        else
        {
            setbkcolor(RGB(25, 25, 35));
            cleardevice();
        }

        draw_big_portrait();
        draw_slots(camera);
        draw_text();
    }

    void on_input(const ExMessage& msg)
    {
        if (msg.message != WM_KEYDOWN)
            return;

        if (msg.vkcode == VK_LEFT)
        {
            current_index--;

            if (current_index < 0)
                current_index = 1;
        }
        else if (msg.vkcode == VK_RIGHT)
        {
            current_index++;

            if (current_index > 1)
                current_index = 0;
        }
        else if (msg.vkcode == 'J')
        {
            if (current_index == 0)
                selected_character = CharacterSelect::yang;
            else
                selected_character = CharacterSelect::MALAIbear;

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

private:
    void draw_big_portrait()
    {
        IMAGE* img = nullptr;

        if (current_index == 0)
            img = &img_yang_lihui;
        else
            img = &img_mlbear_lihui;

        if (img != nullptr && img->getwidth() > 0)
        {
            putimage_alpha(190, 180, img);
        }
    }

    void draw_slots(const Camera& camera)
    {
        draw_slot(560, 760, current_index == 0, _T("yang"));
        draw_slot(820, 760, current_index == 1, _T("MALAIbear"));

        if (current_index == 0)
            animation_yang_preview.on_draw(camera, 600, 660);
        else
            animation_malai_preview.on_draw(camera, 855, 650);
    }

    void draw_slot(int x, int y, bool selected, LPCTSTR name)
    {
        setlinecolor(selected ? RGB(255, 200, 0) : RGB(120, 120, 120));
        setfillcolor(RGB(30, 30, 30));
        fillrectangle(x, y, x + 220, y + 140);

        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        settextstyle(28, 0, _T("Consolas"));
        outtextxy(x + 35, y + 95, name);
    }

    void draw_text()
    {
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);

        settextstyle(48, 0, _T("Consolas"));

        if (current_index == 0)
            outtextxy(930, 300, _T("yang"));
        else
            outtextxy(930, 300, _T("MALAIbear"));

        settextstyle(32, 0, _T("Consolas"));
        outtextxy(930, 390, _T("LEFT / RIGHT : select"));
        outtextxy(930, 450, _T("J : enter game"));
        outtextxy(930, 510, _T("K : back menu"));
    }

private:
    int current_index = 0;
    Animation animation_yang_preview;
    Animation animation_malai_preview;
};

#endif
