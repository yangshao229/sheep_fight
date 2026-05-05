#ifndef _CLOTHS_SCENE_H_
#define _CLOTHS_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "character_select.h"
#include "animation.h"
#include "atlas.h"
#include "audio_manager.h"

#include <graphics.h>
#include <iostream>

extern SceneManager scene_manager;
extern AudioManager audio_manager;

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
        audio_manager.play_bgm("xuanren", false);

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
        else if (current_index == 1)
        {
            animation_malai_preview.on_update(delta);
        }
    }

    void on_draw(const Camera& camera)
    {
        putimage(90, 0, &img_cloths_background);

        draw_character_info();
        draw_big_portrait();
        draw_slots(camera);
        draw_bottom_tips();
    }

    void on_input(const ExMessage& msg)
    {
        if (msg.message != WM_KEYDOWN)
            return;

        if (msg.vkcode == VK_LEFT)
        {
            audio_manager.play_sfx("queren");

            current_index--;

            if (current_index < 0)
                current_index = 1;
        }
        else if (msg.vkcode == VK_RIGHT)
        {
            audio_manager.play_sfx("queren");

            current_index++;

            if (current_index > 1)
                current_index = 0;
        }
        else if (msg.vkcode == 'J')
        {
            audio_manager.play_sfx("queren");

            if (current_index == 0)
                selected_character = CharacterSelect::yang;
            else
                selected_character = CharacterSelect::MALAIbear;

            scene_manager.switch_to(SceneManager::SceneType::Game);
        }
        else if (msg.vkcode == 'K')
        {
            audio_manager.play_sfx("queren");
            scene_manager.switch_to(SceneManager::SceneType::Menu);
        }
    }

    void on_exit()
    {
        std::cout << "退出选人场景" << std::endl;
    }

private:
    void draw_character_info()
    {
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);

        if (current_index == 0)
        {
            settextstyle(52, 0, _T("IPix"));
            outtextxy(900, 300, _T("yang"));

            settextstyle(42, 0, _T("IPix"));
            outtextxy(900, 380, _T("风格：嘎嘎大拳击"));
            outtextxy(900, 460, _T("特点：左拳高伤害，右拳伤害高"));
        }
        else
        {
            settextstyle(52, 0, _T("IPix"));
            outtextxy(900, 300, _T("MALAIbear"));

            settextstyle(42, 0, _T("IPix"));
            outtextxy(900, 380, _T("风格：陀螺小腿踢"));
            outtextxy(900, 460, _T("特点：见下图，像陀螺一样踢"));
        }
    }

    void draw_big_portrait()
    {
        if (current_index == 0)
        {
            putimage_alpha(190, 100, &img_yang_lihui);
        }
        else
        {
            putimage_alpha(370, 100, &img_mlbear_lihui);
        }
    }

    void draw_slots(const Camera& camera)
    {
        draw_slot(400, 760, current_index == 0);
        draw_slot(640, 760, current_index == 1);

        if (current_index == 0)
        {
            animation_yang_preview.on_draw(camera, 440, 770);

            IMAGE* malai_idle = atlas_mlbear_box.get_image(0);
            if (malai_idle != nullptr)
                putimage_alpha(655, 740, malai_idle);
        }
        else
        {
            IMAGE* yang_idle = atlas_yang_attack_left.get_image(0);
            if (yang_idle != nullptr)
                putimage_alpha(440, 770, yang_idle);

            animation_malai_preview.on_draw(camera, 650, 740);
        }
    }

    void draw_bottom_tips()
    {
        setbkmode(TRANSPARENT);
        settextcolor(RGB(255, 220, 120));
        settextstyle(42, 0, _T("IPix"));

        outtextxy(640, 940, _T("← → 切换人物   J 确认   K 返回"));
    }

    void draw_slot(int x, int y, bool selected)
    {
        if (selected)
            setlinecolor(RGB(255, 200, 0));
        else
            setlinecolor(RGB(120, 120, 120));

        setlinestyle(PS_SOLID, selected ? 4 : 2);
        rectangle(x, y, x + 210, y + 150);

        setlinestyle(PS_SOLID, 1);
    }

private:
    int current_index = 0;

    Animation animation_yang_preview;
    Animation animation_malai_preview;
};

#endif // !_CLOTHS_SCENE_H_