#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "animation.h"
#include "vector2.h"
#include "util.h"
#include "character_select.h"
#include "combat.h"
#include "enemy.h"
#include<list>
#include <graphics.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "audio_manager.h"
extern SceneManager scene_manager;
extern Camera main_camera;
extern AudioManager audio_manager;
extern IMAGE img_xinhua;
extern IMAGE img_xinhua_1;
extern IMAGE img_xinhua_2;
extern IMAGE img_xinhua_3;
extern IMAGE img_xinhua_5;

extern IMAGE img_car_background_1;
extern IMAGE img_car_background_2;
extern IMAGE img_game_background_1;
extern IMAGE img_game_background_2;
extern IMAGE img_basketball_background;
extern IMAGE img_special_background;
extern IMAGE img_special_background_2;

extern CharacterSelect selected_character;

extern Atlas atlas_yang_walk_left;
extern Atlas atlas_yang_walk_right;
extern Atlas atlas_yang_jump_left;
extern Atlas atlas_yang_jump_right;
extern Atlas atlas_attack1_left;
extern Atlas atlas_attack1_right;
extern Atlas atlas_attack2_left;
extern Atlas atlas_attack2_right;

extern Atlas atlas_mlbear_walk_left;
extern Atlas atlas_mlbear_walk_right;
extern Atlas atlas_mlbear_jump_right;
extern Atlas atlas_mlbear_jump_left;
extern Atlas atlas_mlbear_attack1_left;
extern Atlas atlas_mlbear_attack1_right;
extern Atlas atlas_mlbear_attack2_left;
extern Atlas atlas_mlbear_attack2_right;


extern Atlas atlas_enemy_small1_walk_left;
extern Atlas atlas_enemy_small1_walk_right;
extern Atlas atlas_enemy_small1_attack_left;
extern Atlas atlas_enemy_small1_attack_right;
extern Atlas atlas_enemy_small1_hurt_left;
extern Atlas atlas_enemy_small1_hurt_right;


extern Atlas atlas_enemy_small2_walk_left;
extern Atlas atlas_enemy_small2_walk_right;
extern Atlas atlas_enemy_small2_attack_left;
extern Atlas atlas_enemy_small2_attack_right;
extern Atlas atlas_enemy_small2_hurt_left;
extern Atlas atlas_enemy_small2_hurt_right;


extern Atlas atlas_enemy_boss1_idle_left;
extern Atlas atlas_enemy_boss1_idle_right;

extern Atlas atlas_enemy_boss1_walk_left;
extern Atlas atlas_enemy_boss1_walk_right;

extern Atlas atlas_enemy_boss1_attack_left;
extern Atlas atlas_enemy_boss1_attack_right;

extern Atlas atlas_enemy_boss1_hurt_left;
extern Atlas atlas_enemy_boss1_hurt_right;

extern Atlas atlas_enemy_boss1_dead_left;
extern Atlas atlas_enemy_boss1_dead_right;



extern Atlas atlas_enemy_boss2_idle_left;
extern Atlas atlas_enemy_boss2_idle_right;

extern Atlas atlas_enemy_boss2_walk_left;
extern Atlas atlas_enemy_boss2_walk_right;

extern Atlas atlas_enemy_boss2_attack1_left;
extern Atlas atlas_enemy_boss2_attack1_right;

extern Atlas atlas_enemy_boss2_attack2_left;
extern Atlas atlas_enemy_boss2_attack2_right;

extern Atlas atlas_enemy_boss2_attack3_left;
extern Atlas atlas_enemy_boss2_attack3_right;

extern Atlas atlas_enemy_boss2_attack4_left;
extern Atlas atlas_enemy_boss2_attack4_right;

extern Atlas atlas_enemy_boss2_hurt_left;
extern Atlas atlas_enemy_boss2_hurt_right;

extern Atlas atlas_enemy_boss2_dead_left;
extern Atlas atlas_enemy_boss2_dead_right;
class GameScene : public Scene
{
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter()
    {
        std::cout << " GameScene" << std::endl;

        window_width = 1920;
        window_height = 1080;

        if (selected_character == CharacterSelect::yang)
        {
            atlas_current_left = &atlas_yang_walk_left;
            atlas_current_right = &atlas_yang_walk_right;
            atlas_jump_left = &atlas_yang_jump_left;
            atlas_jump_right = &atlas_yang_jump_right;

            character_draw_offset_x = 0;
            character_draw_offset_y = 50;
            character_attack_offset_y = 50;
            character_width = 198;

            combat.init(
                &atlas_attack1_left,
                &atlas_attack1_right,
                &atlas_attack2_left,
                &atlas_attack2_right,
                3,
                2
            );
        }
        else
        {
            atlas_current_left = &atlas_mlbear_walk_left;
            atlas_current_right = &atlas_mlbear_walk_right;
            atlas_jump_left = &atlas_mlbear_jump_left;
            atlas_jump_right = &atlas_mlbear_jump_right;

            character_draw_offset_x = -30;
            character_draw_offset_y = 20;
            character_attack_offset_y = 15;
            character_width = 120;

            combat.init(
                &atlas_mlbear_attack1_left,
                &atlas_mlbear_attack1_right,
                &atlas_mlbear_attack2_left,
                &atlas_mlbear_attack2_right,
                15,
                18
            );
        }

        setup_character_collision();

        map_car_1.background = &img_car_background_1;
        map_car_1.width = img_car_background_1.getwidth();
        map_car_1.height = img_car_background_1.getheight();
        map_car_1.spawn = Vector2(120, 570 + character_draw_offset_y);

        map_car_2.background = &img_car_background_2;
        map_car_2.width = img_car_background_2.getwidth();
        map_car_2.height = img_car_background_2.getheight();
        map_car_2.spawn = Vector2((float)(map_car_2.width / 2), 720.0f + character_draw_offset_y);

        map_game_left.background = &img_game_background_1;
        map_game_left.width = img_game_background_1.getwidth();
        map_game_left.height = img_game_background_1.getheight();
        map_game_left.spawn = Vector2((float)(map_game_left.width - 260), 770.0f + character_draw_offset_y);

        map_game_right.background = &img_game_background_2;
        map_game_right.width = img_game_background_2.getwidth();
        map_game_right.height = img_game_background_2.getheight();
        map_game_right.spawn = Vector2(150.0f, 570.0f + character_draw_offset_y);

        map_basketball.background = &img_basketball_background;
        map_basketball.width = img_basketball_background.getwidth();
        map_basketball.height = img_basketball_background.getheight();
        map_basketball.spawn = Vector2(160.0f, 580.0f + character_draw_offset_y);

        map_special.background = &img_special_background;
        map_special.width = img_special_background.getwidth();
        map_special.height = img_special_background.getheight();
        map_special.spawn = Vector2(150.0f, 660.0f + character_draw_offset_y);

        map_special_2.background = &img_special_background_2;
        map_special_2.width = img_special_background_2.getwidth();
        map_special_2.height = img_special_background_2.getheight();
        map_special_2.spawn = Vector2(150.0f, 760.0f + character_draw_offset_y);

        if (atlas_current_right == nullptr || atlas_current_right->get_size() == 0)
        {
            MessageBox(NULL, _T("当前角色 walk 图集为空！"), _T("错误"), 0);
            exit(-1);
        }

        animation_current.set_atlas(atlas_current_right);
        animation_current.set_interval(100);
        animation_current.set_loop(true);

        animation_jump.set_atlas(atlas_jump_right);
        animation_jump.set_interval(100);
        animation_jump.set_loop(true);

        combat.set_attack_frame_interval(selected_character == CharacterSelect::MALAIbear ? 55 : 85);
        combat.set_combo_input_limit(220);
        combat.set_combo_open_frame_attack1(1);
        setup_combat_audio();
        is_facing_left = false;
        is_moving = false;
        can_switch_map = false;
        switch_target = SwitchTarget::None;

        velocity_y = 0.0f;
        is_jumping = false;
        is_on_ground = true;
        jump_count = 0;
        space_key_down = false;
        jump_timer = 0.0f;
        jump_start_y = 0.0f;

        is_player_hurt = false;
        player_hurt_timer = 0;
        reset_player_attack_damage_state();
        hit_stop_timer = 0;

        boss_warning_timer = 0;
        fade_timer = 0;
        hurt_flash_timer = 0;
        ending_phase = EndingPhase::None;
        ending_timer = 0;
        ending_card_timer = 0;
        ending_card_index = 0;
        ending_finished = false;
        ending_end1_started = false;
        switch_map(&map_car_1);
    }

    void on_update(int delta)
    {
        update_hit_impacts(delta);
        update_scene_feedback(delta);

        if (ending_phase == EndingPhase::End2)
        {
            update_ending(delta);
            update_camera();
            main_camera.on_update(delta);
            return;
        }

        if (ending_phase == EndingPhase::End1)
        {
            update_ending(delta);
        }

        if (hit_stop_timer > 0)
        {
            hit_stop_timer -= delta;

            if (hit_stop_timer < 0)
                hit_stop_timer = 0;

            main_camera.on_update(delta);
            return;
        }

        combat.update(delta);

        if (is_player_hurt)
        {
            update_player_hurt(delta);
            update_enemies(delta);
            remove_dead_enemies();
            update_map_clear_state();

            update_camera();
            update_map_switch_state();
            main_camera.on_update(delta);
            return;
        }

        const float speed = 450.0f;
        float move_step = speed * delta / 1000.0f;

        bool moving_left = false;
        bool moving_right = false;

        is_moving = false;

        float old_player_x = player_pos.x;

        if (!combat.is_in_attack())
        {
            if (GetAsyncKeyState('A') & 0x8000)
            {
                player_pos.x -= move_step;
                moving_left = true;
                is_moving = true;
            }

            if (GetAsyncKeyState('D') & 0x8000)
            {
                player_pos.x += move_step;
                moving_right = true;
                is_moving = true;
            }
        }

        if (moving_left && !is_facing_left)
        {
            is_facing_left = true;
            animation_current.set_atlas(atlas_current_left);
            animation_jump.set_atlas(atlas_jump_left);
        }
        else if (moving_right && is_facing_left)
        {
            is_facing_left = false;
            animation_current.set_atlas(atlas_current_right);
            animation_jump.set_atlas(atlas_jump_right);
        }

        limit_player_in_map();
        resolve_player_enemy_collision(old_player_x);

        update_jump(delta);

        update_enemies(delta);
        update_player_attack_enemy();
        remove_dead_enemies();
        update_map_clear_state();
        update_map_reached_end_state();

        if (is_jumping || !is_on_ground)
        {
            animation_jump.on_update(delta);
        }
        else if (is_moving)
        {
            animation_current.on_update(delta);
        }

        update_camera();
        update_map_switch_state();
        main_camera.on_update(delta);
    }

    void on_draw(const Camera& camera)
    {
        if (current_map == nullptr || current_map->background == nullptr)
            return;

        draw_current_map(camera);
        draw_basketball_extra(camera);

        int map_offset_x = get_current_map_offset_x();
        int map_offset_y = get_current_map_offset_y();

        if (ending_phase == EndingPhase::None)
            draw_enemies(camera, map_offset_x, map_offset_y);

        bool draw_player = true;

        if (ending_phase == EndingPhase::End2)
            draw_player = false;

        if (is_player_hurt && (player_hurt_timer / 80) % 2 == 0)
            draw_player = false;

        if (draw_player)
        {
            if (combat.is_in_attack())
            {
                combat.draw(
                    camera,
                    (int)player_pos.x + character_draw_offset_x + map_offset_x,
                    (int)player_pos.y - character_attack_offset_y + map_offset_y,
                    is_facing_left
                );
            }
            else if (is_jumping || !is_on_ground)
            {
                animation_jump.on_draw(
                    camera,
                    (int)player_pos.x + character_draw_offset_x + map_offset_x,
                    (int)player_pos.y - character_draw_offset_y + map_offset_y
                );
            }
            else
            {
                animation_current.on_draw(
                    camera,
                    (int)player_pos.x + character_draw_offset_x + map_offset_x,
                    (int)player_pos.y - character_draw_offset_y + map_offset_y
                );
            }
        }

        draw_hit_impacts(camera, map_offset_x, map_offset_y);
        if (ending_phase != EndingPhase::None)
        {
            draw_ending();
            return;
        }
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        settextstyle(36, 0, _T("IPix"));
        outtextxy(30, 30, _T("AD移动，空格跳跃，J攻击，E切换场景，K返回菜单"));

        if (can_switch_map)
        {
            settextcolor(RGB(255, 220, 120));

            if (switch_target == SwitchTarget::GameRightToSpecial)
                outtextxy(700, 980, _T("已清场并走完整图，按 E 进入隐藏场景"));
            else
                outtextxy(760, 980, _T("按 E 切换场景"));
        }

        draw_debug_battle_info();

        draw_boss_hp_bar();
        draw_player_hurt_flash();
        draw_fade_layer();
        draw_boss_warning();
    }

    void on_input(const ExMessage& msg)
    {
        if (msg.message == WM_KEYUP)
        {
            if (msg.vkcode == VK_SPACE)
                space_key_down = false;

            return;
        }

        if (msg.message != WM_KEYDOWN)
            return;

        if (ending_phase == EndingPhase::End2)
        {
            if (ending_finished && msg.vkcode == 'J')
            {
                audio_manager.stop_bgm();

                ending_phase = EndingPhase::None;
                ending_timer = 0;
                ending_card_timer = 0;
                ending_card_index = 0;
                ending_finished = false;
                ending_end1_started = false;

                scene_manager.switch_to(SceneManager::SceneType::Menu);
            }

            return;
        }

        if (ending_phase == EndingPhase::End1)
        {

            if (msg.vkcode == VK_SPACE)
            {
                if (space_key_down)
                    return;

                space_key_down = true;
                try_start_jump();
            }
            else if (msg.vkcode == 'E')
            {
                if (can_switch_map)
                {
                    switch_by_target();
                    update_camera();
                    update_map_switch_state();
                }
            }

            return;
        }

        if (is_player_hurt)
            return;

        if (msg.vkcode == 'J')
        {
            combat.on_attack_input();
        }
        else if (msg.vkcode == VK_SPACE)
        {
            if (space_key_down)
                return;

            space_key_down = true;
            try_start_jump();
        }
        else if (msg.vkcode == 'E')
        {
            if (can_switch_map)
            {
                switch_by_target();
                update_camera();
                update_map_switch_state();
            }
        }
        else if (msg.vkcode == 'K')
        {
            scene_manager.switch_to(SceneManager::SceneType::Menu);
        }
    }

    void on_exit()
    {
        std::cout << "退出 GameScene" << std::endl;
    }

private:
    struct MapData
    {
        IMAGE* background = nullptr;
        int width = 0;
        int height = 0;
        Vector2 spawn;
    };

    struct SpriteBounds
    {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
        bool valid = false;
    };

    struct CollisionBox
    {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };
    struct HitImpact
    {
        Vector2 pos;
        int timer = 0;
        int duration = 160;
        bool heavy = false;
        int dir = 1;
    };

    enum class SwitchTarget
    {
        None,
        Car1ToCar2,
        Car2ToGameLeft,
        Car2ToGameRight,
        GameLeftToCar2,
        GameRightToCar2,
        GameRightToBasketball,
        BasketballToGameRight,
        GameRightToSpecial,
        SpecialToGameRight,
        SpecialToSpecial2,
        Special2ToSpecial
    };
    enum class EndingPhase
    {
        None,
        End1,
        End2
    };
private:
    void update_scene_feedback(int delta)
    {
        if (boss_warning_timer > 0)
        {
            boss_warning_timer -= delta;

            if (boss_warning_timer < 0)
                boss_warning_timer = 0;
        }

        if (fade_timer > 0)
        {
            fade_timer -= delta;

            if (fade_timer < 0)
                fade_timer = 0;
        }

        if (hurt_flash_timer > 0)
        {
            hurt_flash_timer -= delta;

            if (hurt_flash_timer < 0)
                hurt_flash_timer = 0;
        }
    }

    void start_fade_in()
    {
        fade_timer = fade_duration;
    }

    void start_boss_warning_if_needed()
    {
        if (current_map == &map_game_left || current_map == &map_basketball)
        {
            boss_warning_timer = boss_warning_duration;
            main_camera.shake(4.0f, 220);
        }
    }

    const TCHAR* get_current_boss_name() const
    {
        if (current_map == &map_game_left)
            return _T("BOSS 1");

        if (current_map == &map_basketball)
            return _T("BOSS 2");

        return _T("BOSS");
    }

    void draw_alpha_screen(COLORREF color, BYTE alpha)
    {
        if (alpha <= 0)
            return;

        IMAGE overlay;
        Resize(&overlay, window_width, window_height);

        SetWorkingImage(&overlay);
        setbkcolor(color);
        cleardevice();
        SetWorkingImage(NULL);

        BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha, 0 };

        AlphaBlend(
            GetImageHDC(GetWorkingImage()),
            0,
            0,
            window_width,
            window_height,
            GetImageHDC(&overlay),
            0,
            0,
            window_width,
            window_height,
            bf
        );
    }

    void draw_fade_layer()
    {
        if (fade_timer <= 0)
            return;

        float rate = (float)fade_timer / (float)fade_duration;

        if (rate < 0.0f) rate = 0.0f;
        if (rate > 1.0f) rate = 1.0f;

        BYTE alpha = (BYTE)(rate * 220);

        draw_alpha_screen(BLACK, alpha);
    }

    void draw_player_hurt_flash()
    {
        if (hurt_flash_timer <= 0)
            return;

        float rate = (float)hurt_flash_timer / 180.0f;

        if (rate < 0.0f) rate = 0.0f;
        if (rate > 1.0f) rate = 1.0f;

        BYTE alpha = (BYTE)(rate * 90);

        draw_alpha_screen(RGB(180, 20, 20), alpha);
    }

    void draw_boss_warning()
    {
        if (boss_warning_timer <= 0)
            return;

        float rate = (float)boss_warning_timer / (float)boss_warning_duration;

        if (rate < 0.0f) rate = 0.0f;
        if (rate > 1.0f) rate = 1.0f;

        int shake_x = rand() % 9 - 4;

        setbkmode(TRANSPARENT);

        settextstyle(72, 0, _T("IPix"));
        settextcolor(RGB(0, 0, 0));
        outtextxy(608 + shake_x, 236, _T("WARNING"));

        settextcolor(RGB(255, 60, 40));
        outtextxy(600 + shake_x, 228, _T("WARNING"));

        settextstyle(46, 0, _T("IPix"));
        settextcolor(RGB(255, 230, 120));
        outtextxy(720 + shake_x, 315, get_current_boss_name());

        settextstyle(30, 0, _T("IPix"));
        settextcolor(RGB(255, 255, 255));
        outtextxy(665 + shake_x, 380, _T("强敌出现，准备战斗"));
    }

    void draw_boss_hp_bar()
    {
        Enemy* boss = nullptr;

        for (auto& enemy : enemy_list)
        {
            if (enemy.is_boss() && enemy.is_alive())
            {
                boss = &enemy;
                break;
            }
        }

        if (boss == nullptr)
            return;

        int max_hp = boss->get_max_hp();
        int hp = boss->get_hp();

        if (max_hp <= 0)
            return;

        float rate = boss->get_hp_rate();

        int bar_x = 360;
        int bar_y = 70;
        int bar_w = 1200;
        int bar_h = 28;

        setfillcolor(RGB(20, 20, 20));
        solidrectangle(bar_x - 6, bar_y - 6, bar_x + bar_w + 6, bar_y + bar_h + 6);

        setfillcolor(RGB(70, 20, 20));
        solidrectangle(bar_x, bar_y, bar_x + bar_w, bar_y + bar_h);

        setfillcolor(RGB(210, 40, 40));
        solidrectangle(bar_x, bar_y, bar_x + (int)(bar_w * rate), bar_y + bar_h);

        setlinecolor(RGB(255, 220, 120));
        rectangle(bar_x - 6, bar_y - 6, bar_x + bar_w + 6, bar_y + bar_h + 6);

        setbkmode(TRANSPARENT);
        settextstyle(32, 0, _T("IPix"));
        settextcolor(RGB(255, 235, 160));

        TCHAR text[128];
        _stprintf_s(text, _T("%s    HP %d / %d"), get_current_boss_name(), hp, max_hp);
        outtextxy(bar_x, bar_y - 42, text);
    }
    void update_map_bgm()
    {
        if (current_map == nullptr)
            return;

        if (ending_phase != EndingPhase::None)
            return;

        if (current_map == &map_game_left)
        {
            audio_manager.play_bgm("boss1", false);
        }
        else if (current_map == &map_basketball)
        {
            audio_manager.play_bgm("boss2", false);
        }
        else if (current_map == &map_special || current_map == &map_special_2)
        {
            audio_manager.stop_bgm();
        }
        else
        {
            audio_manager.play_bgm("xiaobing", false);
        }
    }
    void setup_combat_audio()
    {
        if (selected_character == CharacterSelect::yang)
        {
            combat.set_attack1_callback([]()
                {
                    audio_manager.play_sfx("light_punch");
                });

            combat.set_attack2_callback([]()
                {
                    audio_manager.play_sfx("heavy_punch");
                });
        }
        else
        {
            combat.set_attack1_callback([]()
                {
                    audio_manager.play_sfx("chutui");
                    audio_manager.play_sfx("kick1");
                });

            combat.set_attack2_callback([]()
                {
                    audio_manager.play_sfx("kick2");
                });
        }
    }
    void switch_by_target()
    {
        switch (switch_target)
        {
        case SwitchTarget::Car1ToCar2:
            switch_map(&map_car_2);
            player_pos = map_car_2.spawn;
            break;

        case SwitchTarget::Car2ToGameLeft:
            switch_map(&map_game_left);
            player_pos = map_game_left.spawn;
            break;

        case SwitchTarget::Car2ToGameRight:
            switch_map(&map_game_right);
            player_pos = map_game_right.spawn;
            break;

        case SwitchTarget::GameLeftToCar2:
            switch_map(&map_car_2);
            player_pos = Vector2(280.0f, map_car_2.spawn.y);
            break;

        case SwitchTarget::GameRightToCar2:
            switch_map(&map_car_2);
            player_pos = Vector2((float)(map_car_2.width - 280), map_car_2.spawn.y);
            break;

        case SwitchTarget::GameRightToBasketball:
            switch_map(&map_basketball);
            player_pos = map_basketball.spawn;
            break;

        case SwitchTarget::BasketballToGameRight:
            switch_map(&map_game_right);
            player_pos = Vector2((float)(map_game_right.width - 280), map_game_right.spawn.y);
            break;

        case SwitchTarget::GameRightToSpecial:
            switch_map(&map_special);
            player_pos = map_special.spawn;
            break;

        case SwitchTarget::SpecialToGameRight:
            switch_map(&map_game_right);
            player_pos = Vector2((float)(map_game_right.width / 2), map_game_right.spawn.y);
            break;

        case SwitchTarget::SpecialToSpecial2:
            switch_map(&map_special_2);
            player_pos = map_special_2.spawn;
            break;

        case SwitchTarget::Special2ToSpecial:
            switch_map(&map_special);
            player_pos = Vector2((float)(map_special.width - 280), map_special.spawn.y);
            break;

        default:
            break;
        }

        velocity_y = 0.0f;
        is_jumping = false;
        is_on_ground = true;
        jump_count = 0;
        jump_timer = 0.0f;
    }

    void switch_map(MapData* new_map)
    {
        if (new_map == nullptr)
            return;

        basketball_extra_active = false;

        current_map = new_map;
        player_pos = current_map->spawn;

        enemy_list.clear();

        if (current_map == &map_special || current_map == &map_special_2)
        {

            if (ending_phase == EndingPhase::None)
                start_ending_end1();
        }
        else
        {
            ending_phase = EndingPhase::None;
            ending_timer = 0;
            ending_card_timer = 0;
            ending_card_index = 0;
            ending_finished = false;
            ending_end1_started = false;

            update_map_bgm();
            spawn_enemies_for_current_map();
        }

        velocity_y = 0.0f;
        is_jumping = false;
        is_on_ground = true;
        jump_count = 0;
        jump_timer = 0.0f;

        is_player_hurt = false;
        player_hurt_timer = 0;
        hurt_flash_timer = 0;
        reset_player_attack_damage_state();
        hit_stop_timer = 0;
        hit_impacts.clear();

        main_camera.set_position(0.0f, 0.0f);
        update_camera();
    }
    void start_ending_end1()
    {
        ending_phase = EndingPhase::End1;
        ending_timer = 0;
        ending_card_timer = 0;
        ending_card_index = 0;
        ending_finished = false;
        ending_end1_started = true;

        enemy_list.clear();
        audio_manager.play_bgm_once("end1", true);
    }

    void start_ending_end2()
    {
        ending_phase = EndingPhase::End2;
        ending_timer = 0;
        ending_card_timer = 0;
        ending_card_index = 0;
        ending_finished = false;

        enemy_list.clear();
        audio_manager.play_bgm_once("end2", true);
    }

    void force_start_ending_cards()
    {

        current_map = &map_special_2;
        player_pos = map_special_2.spawn;

        velocity_y = 0.0f;
        is_jumping = false;
        is_on_ground = true;
        jump_count = 0;
        jump_timer = 0.0f;
        hit_stop_timer = 0;
        hit_impacts.clear();
        hurt_flash_timer = 0;

        main_camera.set_position(0.0f, 0.0f);
        update_camera();

        start_ending_end2();
    }

    void update_ending(int delta)
    {
        ending_timer += delta;

        if (ending_phase == EndingPhase::End1)
        {

            if (ending_timer >= ending_end1_duration)
                force_start_ending_cards();

            return;
        }

        if (ending_phase == EndingPhase::End2)
        {
            if (ending_timer < ending_text_start_time)
                return;

            if (ending_finished)
                return;

            ending_card_timer += delta;

            if (ending_card_timer >= ending_card_duration)
            {
                ending_card_timer = 0;
                ending_card_index++;

                if (ending_card_index >= ending_card_count)
                {
                    ending_card_index = ending_card_count - 1;
                    ending_finished = true;
                }
            }
        }
    }
    SpriteBounds get_visible_bounds(Atlas* atlas)
    {
        SpriteBounds bounds;

        if (atlas == nullptr || atlas->get_size() <= 0)
            return bounds;

        IMAGE* image = atlas->get_image(0);

        if (image == nullptr)
            return bounds;

        int image_width = image->getwidth();
        int image_height = image->getheight();

        if (image_width <= 0 || image_height <= 0)
            return bounds;

        DWORD* buffer = GetImageBuffer(image);

        int min_x = image_width;
        int min_y = image_height;
        int max_x = -1;
        int max_y = -1;

        for (int y = 0; y < image_height; y++)
        {
            for (int x = 0; x < image_width; x++)
            {
                DWORD alpha = (buffer[y * image_width + x] & 0xFF000000) >> 24;

                if (alpha <= 8)
                    continue;

                if (x < min_x) min_x = x;
                if (y < min_y) min_y = y;
                if (x > max_x) max_x = x;
                if (y > max_y) max_y = y;
            }
        }

        if (max_x < min_x || max_y < min_y)
        {
            bounds.x = 0;
            bounds.y = 0;
            bounds.width = image_width;
            bounds.height = image_height;
            bounds.valid = true;
            return bounds;
        }

        bounds.x = min_x;
        bounds.y = min_y;
        bounds.width = max_x - min_x + 1;
        bounds.height = max_y - min_y + 1;
        bounds.valid = true;

        return bounds;
    }
    void draw_ending()
    {
        if (ending_phase == EndingPhase::End1)
        {
            draw_ending_end1_text();
            return;
        }

        if (ending_phase == EndingPhase::End2)
        {
            draw_ending_end2_cards();
            return;
        }
    }

    void draw_ending_end1_text()
    {
        setbkmode(TRANSPARENT);
        settextstyle(38, 0, _T("IPix"));
        settextcolor(RGB(235, 235, 235));

        int y = 790;

        draw_center_text(_T("他终于走到了梦的尽头。"), y);
        draw_center_text(_T("可梦的尽头，仍然是自己。"), y + 58);
    }

    void draw_ending_end2_cards()
    {

        if (ending_timer < ending_text_start_time)
            return;

        int card_x = 360;
        int card_y = 250;
        int card_w = 1200;
        int card_h = 520;


        IMAGE card;
        Resize(&card, card_w, card_h);

        SetWorkingImage(&card);
        setbkcolor(BLACK);
        cleardevice();
        SetWorkingImage(NULL);

        BLENDFUNCTION bf = { AC_SRC_OVER, 0, 150, 0 };

        AlphaBlend(
            GetImageHDC(GetWorkingImage()),
            card_x,
            card_y,
            card_w,
            card_h,
            GetImageHDC(&card),
            0,
            0,
            card_w,
            card_h,
            bf
        );

        setlinecolor(RGB(255, 220, 140));
        setlinestyle(PS_SOLID, 2);
        rectangle(card_x, card_y, card_x + card_w, card_y + card_h);
        setlinestyle(PS_SOLID, 1);

        setbkmode(TRANSPARENT);
        settextstyle(42, 0, _T("IPix"));
        settextcolor(RGB(245, 238, 218));

        draw_current_ending_card(card_x, card_y);

        settextstyle(26, 0, _T("IPix"));
        settextcolor(RGB(180, 180, 180));

        TCHAR page_text[64];
        _stprintf_s(page_text, _T("%d / %d"), ending_card_index + 1, ending_card_count);
        outtextxy(card_x + card_w - 120, card_y + card_h - 48, page_text);

        if (ending_finished)
        {
            settextstyle(34, 0, _T("IPix"));
            settextcolor(RGB(255, 220, 120));
            draw_center_text(_T("按 J，醒来。"), 850);
        }
    }

    void draw_current_ending_card(int card_x, int card_y)
    {
        int start_y = card_y + 105;
        int line_gap = 62;

        switch (ending_card_index)
        {
        case 0:
            draw_center_text(_T("我一直以为，"), start_y);
            draw_center_text(_T("答案藏在过去。"), start_y + line_gap);
            break;

        case 1:
            draw_center_text(_T("所以我总会回头。"), start_y);
            draw_center_text(_T("总会惦记那些"), start_y + line_gap * 2);
            draw_center_text(_T("已经失去的机会。"), start_y + line_gap * 3);
            break;

        case 2:
            draw_center_text(_T("每当走进新的阶段，"), start_y);
            draw_center_text(_T("我又开始怀念"), start_y + line_gap * 2);
            draw_center_text(_T("上一个阶段的自己。"), start_y + line_gap * 3);
            break;

        case 3:
            draw_center_text(_T("yang 也是这样。"), start_y);
            draw_center_text(_T("他在梦里，"), start_y + line_gap * 2);
            draw_center_text(_T("打倒了所有敌人。"), start_y + line_gap * 3);
            break;

        case 4:
            draw_center_text(_T("可他始终无法离开。"), start_y);
            draw_center_text(_T("因为困住他的，"), start_y + line_gap * 2);
            draw_center_text(_T("从来不是梦。"), start_y + line_gap * 3);
            break;

        case 5:
            draw_center_text(_T("而是那个"), start_y);
            draw_center_text(_T("一直沉在过去的自己。"), start_y + line_gap);
            break;

        case 6:
            draw_center_text(_T("直到最后，"), start_y);
            draw_center_text(_T("我才明白。"), start_y + line_gap);
            draw_center_text(_T("正解，"), start_y + line_gap * 2);
            draw_center_text(_T("不是回到过去。"), start_y + line_gap * 3);
            break;

        case 7:
            draw_center_text(_T("正解，"), start_y - 20);
            draw_center_text(_T("是与自己和解。"), start_y + line_gap - 20);
            draw_center_text(_T("不溺亡过去，"), start_y + line_gap * 2);
            draw_center_text(_T("不幻想未来。"), start_y + line_gap * 3);
            draw_center_text(_T("只把现在，"), start_y + line_gap * 4);
            draw_center_text(_T("走成自己的答案。"), start_y + line_gap * 5);
            break;

        default:
            break;
        }
    }

    void draw_center_text(LPCTSTR text, int y)
    {
        int x = (window_width - textwidth(text)) / 2;
        outtextxy(x, y, text);
    }
    SpriteBounds shrink_bounds_keep_bottom(SpriteBounds bounds, float width_scale, float height_scale)
    {
        if (!bounds.valid)
            return bounds;

        int original_center_x = bounds.x + bounds.width / 2;
        int original_bottom = bounds.y + bounds.height;

        int new_width = (int)(bounds.width * width_scale);
        int new_height = (int)(bounds.height * height_scale);

        if (new_width < 32)
            new_width = 32;

        if (new_height < 48)
            new_height = 48;

        if (new_width > bounds.width)
            new_width = bounds.width;

        if (new_height > bounds.height)
            new_height = bounds.height;

        bounds.x = original_center_x - new_width / 2;
        bounds.y = original_bottom - new_height;
        bounds.width = new_width;
        bounds.height = new_height;

        return bounds;
    }

    void setup_character_collision()
    {
        SpriteBounds left_bounds = get_visible_bounds(atlas_current_left);
        SpriteBounds right_bounds = get_visible_bounds(atlas_current_right);

        if (!left_bounds.valid && right_bounds.valid)
            left_bounds = right_bounds;

        if (!right_bounds.valid && left_bounds.valid)
            right_bounds = left_bounds;

        if (!left_bounds.valid || !right_bounds.valid)
        {
            left_bounds.x = 0;
            left_bounds.y = 0;
            left_bounds.width = character_width;
            left_bounds.height = 180;
            left_bounds.valid = true;
            right_bounds = left_bounds;
        }

        float width_scale = selected_character == CharacterSelect::MALAIbear ? 0.64f : 0.68f;
        float height_scale = selected_character == CharacterSelect::MALAIbear ? 0.82f : 0.84f;

        left_bounds = shrink_bounds_keep_bottom(left_bounds, width_scale, height_scale);
        right_bounds = shrink_bounds_keep_bottom(right_bounds, width_scale, height_scale);

        character_collision_offset_x_left = character_draw_offset_x + left_bounds.x;
        character_collision_offset_x_right = character_draw_offset_x + right_bounds.x;

        character_collision_width_left = left_bounds.width;
        character_collision_width_right = right_bounds.width;

        character_collision_offset_y = -character_draw_offset_y + right_bounds.y;
        character_collision_height = right_bounds.height;
        character_feet_offset_y = character_collision_offset_y + character_collision_height;

        character_width = character_collision_width_right > character_collision_width_left
            ? character_collision_width_right
            : character_collision_width_left;
    }

    void setup_enemy_collision_from_atlas(EnemyConfig& cfg, Atlas* atlas)
    {
        SpriteBounds bounds = get_visible_bounds(atlas);

        if (!bounds.valid)
        {
            cfg.collision_offset_x = 0;
            cfg.collision_offset_y = 0;
            cfg.collision_width = cfg.width;
            cfg.collision_height = cfg.height;
            cfg.feet_offset_y = cfg.height;
            return;
        }

        float width_scale = cfg.is_boss ? 0.62f : 0.58f;
        float height_scale = cfg.is_boss ? 0.82f : 0.78f;

        bounds = shrink_bounds_keep_bottom(bounds, width_scale, height_scale);

        cfg.collision_offset_x = bounds.x;
        cfg.collision_offset_y = bounds.y;
        cfg.collision_width = bounds.width;
        cfg.collision_height = bounds.height;
        cfg.feet_offset_y = bounds.y + bounds.height;

        cfg.width = bounds.width;
        cfg.height = bounds.height;
    }

    CollisionBox get_player_collision_box() const
    {
        CollisionBox box;

        int offset_x = is_facing_left ? character_collision_offset_x_left : character_collision_offset_x_right;
        int width = is_facing_left ? character_collision_width_left : character_collision_width_right;

        box.left = player_pos.x + offset_x;
        box.top = player_pos.y + character_collision_offset_y;
        box.right = box.left + width;
        box.bottom = box.top + character_collision_height;

        return box;
    }

    float get_visual_ground_y()
    {
        return get_current_ground_y() + character_feet_offset_y;
    }

    Vector2 make_enemy_spawn(float x, const EnemyConfig& cfg)
    {
        return Vector2(x, get_visual_ground_y() - cfg.feet_offset_y);
    }

    void try_start_jump()
    {
        if (combat.is_in_attack())
            return;

        if (is_on_ground)
            jump_count = 0;

        if (jump_count >= max_jump_count)
            return;

        is_jumping = true;
        is_on_ground = false;
        jump_count++;

        jump_timer = 0.0f;
        jump_start_y = player_pos.y;

        if (atlas_jump_right != nullptr && atlas_jump_right->get_size() > 0)
            jump_duration = (float)(atlas_jump_right->get_size() * 100);
        else
            jump_duration = 600.0f;
    }

    void land_player_at(float y)
    {
        player_pos.y = y;
        is_jumping = false;
        is_on_ground = true;
        velocity_y = 0.0f;
        jump_count = 0;
    }

    void update_camera()
    {
        if (current_map == nullptr)
            return;

        float cam_x = player_pos.x - window_width / 2.0f;
        float cam_y = player_pos.y - window_height / 2.0f;

        if (cam_x < 0) cam_x = 0;
        if (cam_y < 0) cam_y = 0;

        if (cam_x > current_map->width - window_width)
            cam_x = (float)(current_map->width - window_width);

        if (cam_y > current_map->height - window_height)
            cam_y = (float)(current_map->height - window_height);

        if (current_map->width <= window_width)
            cam_x = 0;

        if (current_map->height <= window_height)
            cam_y = 0;

        main_camera.set_position(cam_x, cam_y);
    }

    bool are_all_required_maps_cleared() const
    {
        return
            map_car_1_cleared &&
            map_car_2_cleared &&
            map_game_left_cleared &&
            map_game_right_cleared &&
            map_basketball_cleared;
    }

    void update_map_switch_state()
    {
        can_switch_map = false;
        switch_target = SwitchTarget::None;

        if (current_map == &map_car_1)
        {
            if (player_pos.x >= map_car_1.width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::Car1ToCar2;
            }
        }
        else if (current_map == &map_car_2)
        {
            if (player_pos.x <= 120)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::Car2ToGameLeft;
            }
            else if (player_pos.x >= map_car_2.width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::Car2ToGameRight;
            }
        }
        else if (current_map == &map_game_left)
        {
            if (player_pos.x >= map_game_left.width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::GameLeftToCar2;
            }
        }
        else if (current_map == &map_game_right)
        {
            if (player_pos.x <= 120)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::GameRightToCar2;
            }
            else if (player_pos.x >= map_game_right.width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::GameRightToBasketball;
            }
            else
            {
                float hidden_left = map_game_right.width * 0.45f;
                float hidden_right = map_game_right.width * 0.58f;

                if (player_pos.x >= hidden_left && player_pos.x <= hidden_right)
                {
                    if (are_all_required_maps_cleared() && map_game_right_reached_end)
                    {
                        can_switch_map = true;
                        switch_target = SwitchTarget::GameRightToSpecial;
                    }
                }
            }
        }
        else if (current_map == &map_basketball)
        {
            if (player_pos.x <= 120)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::BasketballToGameRight;
            }
        }
        else if (current_map == &map_special)
        {
            if (player_pos.x <= 120)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::SpecialToGameRight;
            }
            else if (player_pos.x >= map_special.width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::SpecialToSpecial2;
            }
        }
        else if (current_map == &map_special_2)
        {
            if (player_pos.x <= 120)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::Special2ToSpecial;
            }
        }
    }

    float get_current_ground_y()
    {
        if (current_map == &map_basketball)
            return 580.0f + character_draw_offset_y;

        if (current_map == &map_special)
            return 660.0f + character_draw_offset_y;

        if (current_map == &map_special_2)
            return 760.0f + character_draw_offset_y;

        if (current_map == &map_game_right)
            return 570.0f + character_draw_offset_y;

        if (current_map == &map_game_left)
            return 770.0f + character_draw_offset_y;

        if (current_map == &map_car_2)
            return 720.0f + character_draw_offset_y;

        if (current_map == &map_car_1)
            return 570.0f + character_draw_offset_y;

        return player_pos.y;
    }

    int get_current_map_offset_x()
    {
        if (current_map == nullptr)
            return 0;

        if (current_map->width < window_width)
            return (window_width - current_map->width) / 2;

        return 0;
    }

    int get_current_map_offset_y()
    {
        if (current_map == nullptr)
            return 0;

        if (current_map->height < window_height)
            return (window_height - current_map->height) / 2;

        return 0;
    }

    void update_jump(int delta)
    {
        if (current_map == nullptr)
            return;

        float ground_y = get_current_ground_y();

        if (current_map == &map_basketball && is_on_ground && !is_jumping)
        {
            float platform_left = 720.0f;
            float platform_right = 1500.0f;
            float platform_y = 500.0f + character_draw_offset_y;

            bool on_platform_height =
                player_pos.y > platform_y - 2.0f &&
                player_pos.y < platform_y + 2.0f;

            CollisionBox player_box = get_player_collision_box();
            bool in_platform_x =
                player_box.right > platform_left &&
                player_box.left < platform_right;

            if (on_platform_height && !in_platform_x)
            {
                is_on_ground = false;
                jump_count = max_jump_count - 1;
                velocity_y = 0.0f;
                basketball_extra_active = false;
            }
        }

        if (is_jumping)
        {
            float previous_y = player_pos.y;

            jump_timer += (float)delta;

            float t = jump_timer / jump_duration;

            if (t > 1.0f)
                t = 1.0f;

            float jump_offset = -4.0f * jump_height * t * (1.0f - t);
            float next_y = jump_start_y + jump_offset;

            if (current_map == &map_basketball && next_y >= previous_y)
            {
                float platform_left = 720.0f;
                float platform_right = 1500.0f;
                float platform_y = 500.0f + character_draw_offset_y;

                CollisionBox player_box = get_player_collision_box();
                bool in_platform_x =
                    player_box.right > platform_left &&
                    player_box.left < platform_right;

                bool cross_platform =
                    previous_y <= platform_y &&
                    next_y >= platform_y;

                if (in_platform_x && cross_platform)
                {
                    land_player_at(platform_y);
                    basketball_extra_active = true;
                    return;
                }
            }

            if (next_y >= ground_y)
            {
                land_player_at(ground_y);
                return;
            }

            player_pos.y = next_y;

            if (jump_timer >= jump_duration)
            {
                float exit_fall_speed = next_y - previous_y;

                if (exit_fall_speed < gravity * 4.0f)
                    exit_fall_speed = gravity * 4.0f;

                is_jumping = false;
                is_on_ground = false;
                velocity_y = exit_fall_speed;
            }

            return;
        }

        if (!is_on_ground)
        {
            float previous_y = player_pos.y;
            float next_y = player_pos.y + velocity_y;
            velocity_y += gravity;

            if (current_map == &map_basketball)
            {
                float platform_left = 720.0f;
                float platform_right = 1500.0f;
                float platform_y = 500.0f + character_draw_offset_y;

                CollisionBox player_box = get_player_collision_box();
                bool in_platform_x =
                    player_box.right > platform_left &&
                    player_box.left < platform_right;

                bool falling = velocity_y >= 0;

                if (falling && in_platform_x)
                {
                    bool cross_platform =
                        previous_y <= platform_y &&
                        next_y >= platform_y;

                    if (cross_platform || (next_y >= platform_y && next_y <= platform_y + 35))
                    {
                        land_player_at(platform_y);
                        basketball_extra_active = true;
                        return;
                    }
                }
            }

            if (next_y >= ground_y)
            {
                land_player_at(ground_y);
                return;
            }

            player_pos.y = next_y;
        }
    }

    void limit_player_in_map()
    {
        if (current_map == nullptr)
            return;

        float min_left = 0.0f;
        float max_right = (float)current_map->width;

        CollisionBox player_box = get_player_collision_box();

        if (player_box.left < min_left)
            player_pos.x += min_left - player_box.left;

        player_box = get_player_collision_box();

        if (player_box.right > max_right)
            player_pos.x -= player_box.right - max_right;
    }

    void resolve_player_enemy_collision(float old_player_x)
    {
        for (const auto& enemy : enemy_list)
        {
            if (!enemy.is_solid())
                continue;

            CollisionBox player_box = get_player_collision_box();
            float player_left = player_box.left;
            float player_right = player_box.right;
            float player_top = player_box.top;
            float player_bottom = player_box.bottom;

            bool overlap =
                player_right >= enemy.get_left() &&
                player_left <= enemy.get_right() &&
                player_bottom >= enemy.get_top() &&
                player_top <= enemy.get_bottom();

            if (overlap)
            {
                player_pos.x = old_player_x;
                return;
            }
        }
    }

    void draw_current_map(const Camera& camera)
    {
        if (current_map == nullptr || current_map->background == nullptr)
            return;

        int map_w = current_map->width;
        int map_h = current_map->height;

        int src_x = (int)camera.get_position().x;
        if (map_w < window_width)
            src_x = 0;

        int src_y = (int)camera.get_position().y;
        if (map_h < window_height)
            src_y = 0;

        if (src_x < 0) src_x = 0;
        if (src_y < 0) src_y = 0;

        int draw_w = (map_w < window_width) ? map_w : window_width;
        int draw_h = (map_h < window_height) ? map_h : window_height;

        int dst_x = get_current_map_offset_x();
        int dst_y = get_current_map_offset_y();

        putimage_alpha(
            dst_x,
            dst_y,
            draw_w,
            draw_h,
            current_map->background,
            src_x,
            src_y
        );
    }

    void draw_basketball_extra(const Camera& camera)
    {
        if (current_map != &map_basketball)
            return;

        if (!basketball_extra_active)
            return;

        int cam_x = (int)camera.get_position().x;
        int cam_y = (int)camera.get_position().y;

        putimage_alpha(100 - cam_x, 430 - cam_y, &img_xinhua);
        putimage_alpha(1100 - cam_x, 610 - cam_y, &img_xinhua_1);
        putimage_alpha(800 - cam_x, 540 - cam_y, &img_xinhua_2);
        putimage_alpha(1800 - cam_x, 640 - cam_y, &img_xinhua_3);
        putimage_alpha(1400 - cam_x, 490 - cam_y, &img_xinhua_5);
    }

private:
    EnemyConfig make_small1_config()
    {
        EnemyConfig cfg;

        cfg.kind = EnemyKind::Small1;

        cfg.hp = 10;
        cfg.damage = 1;

        cfg.width = 100;
        cfg.height = 150;

        cfg.speed = 90.0f;
        cfg.attack_range = 130.0f;

        cfg.attack_frame_count = 7;
        cfg.attack_frame_interval = 100;
        cfg.hit_frame = 3;
        cfg.attack_cooldown = 1200;
        cfg.active_start_frame = 3;
        cfg.active_end_frame = 4;
        cfg.attack_hitbox_reach = 95;
        cfg.attack_hitbox_back = 35;
        cfg.attack_hitbox_offset_y = 25;
        cfg.attack_hitbox_height = 115;

        cfg.hurt_stun_time = 420;
        cfg.knockback_speed = 540.0f;
        cfg.knockback_time = 160;

        cfg.dead_keep_time = 500;
        cfg.is_boss = false;

        cfg.walk_left = &atlas_enemy_small1_walk_left;
        cfg.walk_right = &atlas_enemy_small1_walk_right;

        setup_enemy_collision_from_atlas(cfg, cfg.walk_right);

        cfg.attack_left = &atlas_enemy_small1_attack_left;
        cfg.attack_right = &atlas_enemy_small1_attack_right;

        cfg.hurt_left = &atlas_enemy_small1_hurt_left;
        cfg.hurt_right = &atlas_enemy_small1_hurt_right;

        return cfg;
    }

    EnemyConfig make_small2_config()
    {
        EnemyConfig cfg;

        cfg.kind = EnemyKind::Small2;

        cfg.hp = 10;
        cfg.damage = 1;

        cfg.width = 110;
        cfg.height = 155;

        cfg.speed = 105.0f;
        cfg.attack_range = 135.0f;

        cfg.attack_frame_count = 7;
        cfg.attack_frame_interval = 100;
        cfg.hit_frame = 3;
        cfg.attack_cooldown = 1050;
        cfg.active_start_frame = 3;
        cfg.active_end_frame = 4;
        cfg.attack_hitbox_reach = 110;
        cfg.attack_hitbox_back = 40;
        cfg.attack_hitbox_offset_y = 20;
        cfg.attack_hitbox_height = 125;

        cfg.hurt_stun_time = 420;
        cfg.knockback_speed = 520.0f;
        cfg.knockback_time = 150;

        cfg.dead_keep_time = 500;
        cfg.is_boss = false;

        cfg.walk_left = &atlas_enemy_small2_walk_left;
        cfg.walk_right = &atlas_enemy_small2_walk_right;

        setup_enemy_collision_from_atlas(cfg, cfg.walk_right);

        cfg.attack_left = &atlas_enemy_small2_attack_left;
        cfg.attack_right = &atlas_enemy_small2_attack_right;

        cfg.hurt_left = &atlas_enemy_small2_hurt_left;
        cfg.hurt_right = &atlas_enemy_small2_hurt_right;

        return cfg;
    }

    EnemyConfig make_boss1_config()
    {
        EnemyConfig cfg;

        cfg.kind = EnemyKind::Boss1;

        cfg.hp = 120;
        cfg.damage = 3;

        cfg.width = 150;
        cfg.height = 210;

        cfg.speed = 60.0f;
        cfg.attack_range = 260.0f;

        cfg.attack_frame_count = 8;
        cfg.attack_frame_interval = 125;

        cfg.hit_frame = 4;
        cfg.active_start_frame = 4;
        cfg.active_end_frame = 5;
        cfg.attack_hitbox_reach = 250;
        cfg.attack_hitbox_back = 95;
        cfg.attack_hitbox_offset_y = 20;
        cfg.attack_hitbox_height = 175;

        cfg.attack_cooldown = 1450;

        cfg.hurt_stun_time = 260;
        cfg.knockback_speed = 240.0f;
        cfg.knockback_time = 120;

        cfg.dead_keep_time = 900;
        cfg.is_boss = true;
        cfg.invincible_time_after_hit = 650;
        cfg.hits_before_invincible = 4;

        cfg.idle_left = &atlas_enemy_boss1_idle_left;
        cfg.idle_right = &atlas_enemy_boss1_idle_right;

        cfg.walk_left = &atlas_enemy_boss1_walk_left;
        cfg.walk_right = &atlas_enemy_boss1_walk_right;

        setup_enemy_collision_from_atlas(cfg, cfg.walk_right);

        cfg.attack_left = &atlas_enemy_boss1_attack_left;
        cfg.attack_right = &atlas_enemy_boss1_attack_right;

        cfg.hurt_left = &atlas_enemy_boss1_hurt_left;
        cfg.hurt_right = &atlas_enemy_boss1_hurt_right;

        cfg.dead_left = &atlas_enemy_boss1_dead_left;
        cfg.dead_right = &atlas_enemy_boss1_dead_right;

        return cfg;
    }

    EnemyConfig make_boss2_config()
    {
        EnemyConfig cfg;

        cfg.kind = EnemyKind::Boss2;

        cfg.hp = 160;
        cfg.damage = 3;

        cfg.width = 160;
        cfg.height = 220;

        cfg.speed = 112.0f;
        cfg.attack_range = 285.0f;

        cfg.attack_frame_count = 5;
        cfg.attack_frame_interval = 145;

        cfg.hit_frame = 2;
        cfg.active_start_frame = 2;
        cfg.active_end_frame = 3;
        cfg.attack_hitbox_reach = 230;
        cfg.attack_hitbox_back = 80;
        cfg.attack_hitbox_offset_y = 20;
        cfg.attack_hitbox_height = 185;

        cfg.attack_cooldown = 1050;

        cfg.hurt_stun_time = 220;
        cfg.knockback_speed = 210.0f;
        cfg.knockback_time = 110;

        cfg.dead_keep_time = 700;
        cfg.is_boss = true;
        cfg.invincible_time_after_hit = 850;
        cfg.hits_before_invincible = 4;

        cfg.idle_left = &atlas_enemy_boss2_idle_left;
        cfg.idle_right = &atlas_enemy_boss2_idle_right;

        cfg.walk_left = &atlas_enemy_boss2_walk_left;
        cfg.walk_right = &atlas_enemy_boss2_walk_right;

        setup_enemy_collision_from_atlas(cfg, cfg.walk_right);

        cfg.attack_left = &atlas_enemy_boss2_attack1_left;
        cfg.attack_right = &atlas_enemy_boss2_attack1_right;

        cfg.attack1_left = &atlas_enemy_boss2_attack1_left;
        cfg.attack1_right = &atlas_enemy_boss2_attack1_right;

        cfg.attack2_left = &atlas_enemy_boss2_attack2_left;
        cfg.attack2_right = &atlas_enemy_boss2_attack2_right;

        cfg.attack3_left = &atlas_enemy_boss2_attack3_left;
        cfg.attack3_right = &atlas_enemy_boss2_attack3_right;

        cfg.attack4_left = &atlas_enemy_boss2_attack4_left;
        cfg.attack4_right = &atlas_enemy_boss2_attack4_right;

        cfg.hurt_left = &atlas_enemy_boss2_hurt_left;
        cfg.hurt_right = &atlas_enemy_boss2_hurt_right;

        cfg.dead_left = &atlas_enemy_boss2_dead_left;
        cfg.dead_right = &atlas_enemy_boss2_dead_right;

        return cfg;
    }

    int get_small_enemy_count_for_current_map(int min_count) const
    {
        if (current_map == nullptr)
            return min_count;

        int count = current_map->width / 360;

        if (count < min_count)
            count = min_count;

        if (count > 8)
            count = 8;

        return count;
    }

    float get_scaled_enemy_spawn_x(int index, int count) const
    {
        if (current_map == nullptr)
            return 0.0f;

        float left = 420.0f;
        float right = (float)current_map->width - 420.0f;

        if (right <= left)
        {
            left = (float)current_map->width * 0.25f;
            right = (float)current_map->width * 0.75f;
        }

        return left + (right - left) * (float)(index + 1) / (float)(count + 1);
    }

    void spawn_small_enemies_by_map_width(const EnemyConfig& primary, const EnemyConfig* secondary, int min_count)
    {
        int count = get_small_enemy_count_for_current_map(min_count);

        for (int i = 0; i < count; i++)
        {
            const EnemyConfig& cfg = (secondary != nullptr && i % 2 == 1) ? *secondary : primary;
            enemy_list.emplace_back(cfg, make_enemy_spawn(get_scaled_enemy_spawn_x(i, count), cfg));
        }
    }

    void spawn_enemies_for_current_map()
    {
        enemy_list.clear();

        if (current_map == nullptr)
            return;

        if (current_map == &map_car_1 && map_car_1_cleared)
            return;

        if (current_map == &map_car_2 && map_car_2_cleared)
            return;

        if (current_map == &map_game_left && map_game_left_cleared)
            return;

        if (current_map == &map_game_right && map_game_right_cleared)
            return;

        if (current_map == &map_basketball && map_basketball_cleared)
            return;

        if (current_map == &map_special && map_special_cleared)
            return;

        if (current_map == &map_special_2 && map_special_2_cleared)
            return;

        if (current_map == &map_car_1)
        {
            EnemyConfig small1 = make_small1_config();

            spawn_small_enemies_by_map_width(small1, nullptr, 4);
        }
        else if (current_map == &map_car_2)
        {
            EnemyConfig small2 = make_small2_config();

            spawn_small_enemies_by_map_width(small2, nullptr, 5);
        }
        else if (current_map == &map_game_left)
        {
            EnemyConfig boss1 = make_boss1_config();

            enemy_list.emplace_back(boss1, make_enemy_spawn(260.0f, boss1));
        }
        else if (current_map == &map_game_right)
        {
            EnemyConfig small1 = make_small1_config();
            EnemyConfig small2 = make_small2_config();

            spawn_small_enemies_by_map_width(small1, &small2, 5);
        }
        else if (current_map == &map_basketball)
        {
            EnemyConfig boss2 = make_boss2_config();

            enemy_list.emplace_back(boss2, make_enemy_spawn(1200.0f, boss2));
        }
        else if (current_map == &map_special)
        {
        }
        else if (current_map == &map_special_2)
        {
        }
    }

    void update_enemies(int delta)
    {
        for (auto& enemy : enemy_list)
        {
            if (enemy.should_remove())
                continue;

            CollisionBox player_box = get_player_collision_box();
            Vector2 player_center((player_box.left + player_box.right) / 2.0f, player_pos.y);

            enemy.update(delta, player_center);

            if (enemy.check_hit_player_this_frame(player_box.left, player_box.top, player_box.right, player_box.bottom))
            {
                hurt_player(enemy.get_damage(), enemy.get_position(), enemy.is_boss());
            }
        }
    }

    void hurt_player(int damage, const Vector2& enemy_pos, bool heavy_hit)
    {
        if (is_player_hurt)
            return;

        is_player_hurt = true;
        player_hurt_timer = heavy_hit ? player_hurt_time + 120 : player_hurt_time;
        hurt_flash_timer = heavy_hit ? 180 : 100;
        int knockback_dir = enemy_pos.x < player_pos.x ? 1 : -1;
        float knockback_distance = heavy_hit ? 135.0f : 90.0f;
        player_pos.x += knockback_distance * knockback_dir;

        limit_player_in_map();

        CollisionBox player_box = get_player_collision_box();
        Vector2 impact_pos(
            (player_box.left + player_box.right) / 2.0f,
            player_box.top + (player_box.bottom - player_box.top) * 0.45f
        );

        spawn_hit_impact(impact_pos, heavy_hit, -knockback_dir);

        hit_stop_timer = heavy_hit ? 105 : 70;
        main_camera.shake(heavy_hit ? 9.5f : 4.0f, heavy_hit ? 180 : 120);

        std::cout << "player hurt, enemy damage: " << damage << std::endl;
    }

    void update_player_hurt(int delta)
    {
        if (!is_player_hurt)
            return;

        player_hurt_timer -= delta;

        if (player_hurt_timer <= 0)
        {
            player_hurt_timer = 0;
            is_player_hurt = false;
            hurt_flash_timer = 0;
        }
    }

    void reset_player_attack_damage_state()
    {
        attack_damage_mask = 0;
        attack_last_stage = ComboStage::None;
    }

    bool get_player_attack_profile(
        ComboStage stage,
        int frame,
        int& hit_slot,
        int& damage,
        float& attack_reach,
        float& attack_top_extra,
        float& attack_bottom_extra,
        int& hit_stop,
        float& shake_strength,
        int& shake_duration,
        bool& heavy_hit,
        float& knockback_scale
    )
    {
        hit_slot = -1;
        damage = 0;
        attack_reach = 230.0f;
        attack_top_extra = 35.0f;
        attack_bottom_extra = 45.0f;
        hit_stop = 85;
        shake_strength = 6.0f;
        shake_duration = 130;
        heavy_hit = false;
        knockback_scale = 1.0f;

        bool is_mlbear = selected_character == CharacterSelect::MALAIbear;

        if (stage == ComboStage::Attack1)
        {
            if (is_mlbear)
            {
                if (frame < 4 || frame > 8)
                    return false;

                hit_slot = 0;
                damage = 2;
                attack_reach = 220.0f;
                hit_stop = 70;
                shake_strength = 5.4f;
                shake_duration = 110;
                knockback_scale = 0.7f;
                return true;
            }

            if (frame < 1)
                return false;

            hit_slot = 0;
            damage = 2;
            attack_reach = 230.0f;
            hit_stop = 95;
            shake_strength = 6.5f;
            shake_duration = 150;
            knockback_scale = 0.95f;
            return true;
        }

        if (stage != ComboStage::Attack2)
            return false;

        if (is_mlbear)
        {
            if (frame >= 2 && frame <= 3)
                hit_slot = 0;
            else if (frame >= 5 && frame <= 6)
                hit_slot = 1;
            else if (frame >= 8 && frame <= 9)
                hit_slot = 2;
            else if (frame >= 11 && frame <= 12)
                hit_slot = 3;
            else if (frame >= 14 && frame <= 16)
                hit_slot = 4;
            else
                return false;

            bool final_hit = hit_slot == 4;
            damage = final_hit ? 2 : 1;
            attack_reach = 285.0f;
            attack_top_extra = 70.0f;
            attack_bottom_extra = 35.0f;
            hit_stop = final_hit ? 85 : 45;
            shake_strength = final_hit ? 8.5f : 5.2f;
            shake_duration = final_hit ? 155 : 85;
            heavy_hit = final_hit;
            knockback_scale = final_hit ? 0.85f : 0.18f;
            return true;
        }

        if (frame < 1)
            return false;

        hit_slot = 0;
        damage = 4;
        attack_reach = 270.0f;
        attack_top_extra = 40.0f;
        attack_bottom_extra = 50.0f;
        hit_stop = 120;
        shake_strength = 8.0f;
        shake_duration = 160;
        heavy_hit = true;
        knockback_scale = 1.1f;
        return true;
    }

    void spawn_hit_impact(const Vector2& pos, bool heavy_hit, int direction)
    {
        HitImpact impact;
        impact.pos = pos;
        impact.timer = 0;
        impact.duration = heavy_hit ? 220 : 150;
        impact.heavy = heavy_hit;
        impact.dir = direction == 0 ? 1 : direction;
        hit_impacts.push_back(impact);
    }

    void update_hit_impacts(int delta)
    {
        for (auto& impact : hit_impacts)
            impact.timer += delta;

        hit_impacts.erase(
            std::remove_if(
                hit_impacts.begin(),
                hit_impacts.end(),
                [](const HitImpact& impact)
                {
                    return impact.timer >= impact.duration;
                }
            ),
            hit_impacts.end()
        );
    }

    void draw_hit_impacts(const Camera& camera, int map_offset_x, int map_offset_y)
    {
        for (const auto& impact : hit_impacts)
        {
            float t = impact.duration > 0 ? (float)impact.timer / impact.duration : 1.0f;
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            int sx = (int)(impact.pos.x + map_offset_x - camera.get_position().x);
            int sy = (int)(impact.pos.y + map_offset_y - camera.get_position().y);
            int radius = impact.heavy ? 30 + (int)(48.0f * t) : 20 + (int)(30.0f * t);
            int slash = impact.heavy ? 72 : 48;
            int line_width = impact.heavy ? 5 : 3;

            setlinestyle(PS_SOLID, line_width);
            setlinecolor(impact.heavy ? RGB(255, 230, 70) : RGB(255, 255, 210));
            circle(sx, sy, radius / 2);

            setlinecolor(impact.heavy ? RGB(255, 80, 60) : RGB(255, 190, 80));
            line(sx - impact.dir * slash / 2, sy - slash / 2, sx + impact.dir * slash, sy + slash / 3);
            line(sx - impact.dir * slash / 3, sy + slash / 2, sx + impact.dir * slash / 2, sy - slash / 3);
            line(sx - slash / 2, sy, sx + slash / 2, sy);
            line(sx, sy - slash / 2, sx, sy + slash / 2);
        }

        setlinestyle(PS_SOLID, 1);
    }

    void update_player_attack_enemy()
    {
        if (!combat.is_in_attack())
        {
            reset_player_attack_damage_state();
            return;
        }

        ComboStage stage = combat.get_stage();
        int current_frame = combat.get_frame();

        if (stage != attack_last_stage)
        {
            attack_last_stage = stage;
            attack_damage_mask = 0;
        }

        int hit_slot = -1;
        int damage = 0;
        float attack_reach = 0.0f;
        float attack_top_extra = 0.0f;
        float attack_bottom_extra = 0.0f;
        int hit_stop = 0;
        float shake_strength = 0.0f;
        int shake_duration = 0;
        bool heavy_hit = false;
        float knockback_scale = 1.0f;

        if (!get_player_attack_profile(
            stage,
            current_frame,
            hit_slot,
            damage,
            attack_reach,
            attack_top_extra,
            attack_bottom_extra,
            hit_stop,
            shake_strength,
            shake_duration,
            heavy_hit,
            knockback_scale
        ))
        {
            return;
        }

        int hit_bit = 1 << hit_slot;
        if ((attack_damage_mask & hit_bit) != 0)
            return;

        CollisionBox player_box = get_player_collision_box();

        float attack_left = 0.0f;
        float attack_right = 0.0f;

        if (is_facing_left)
        {
            attack_right = player_box.left + 45.0f;
            attack_left = attack_right - attack_reach;
        }
        else
        {
            attack_left = player_box.right - 45.0f;
            attack_right = attack_left + attack_reach;
        }

        float attack_top = player_box.top - attack_top_extra;
        float attack_bottom = player_box.bottom + attack_bottom_extra;

        for (auto& enemy : enemy_list)
        {
            if (!enemy.is_alive())
                continue;

            bool overlap =
                attack_right >= enemy.get_left() &&
                attack_left <= enemy.get_right() &&
                attack_bottom >= enemy.get_top() &&
                attack_top <= enemy.get_bottom();

            if (overlap)
            {
                int knockback_dir = is_facing_left ? -1 : 1;

                bool damage_applied = enemy.hurt(damage, knockback_dir, knockback_scale);

                if (!damage_applied)
                    continue;

                attack_damage_mask |= hit_bit;

                hit_stop_timer = hit_stop;
                main_camera.shake(shake_strength, shake_duration);
                spawn_hit_impact(enemy.get_hit_effect_position(), heavy_hit, knockback_dir);

                std::cout << "player hit enemy, damage: " << damage << std::endl;

                break;
            }
        }
    }

    void remove_dead_enemies()
    {
        enemy_list.remove_if(
            [](const Enemy& enemy)
            {
                return enemy.should_remove();
            }
        );
    }

    void update_map_clear_state()
    {
        if (!enemy_list.empty())
            return;

        if (current_map == &map_car_1)
            map_car_1_cleared = true;
        else if (current_map == &map_car_2)
            map_car_2_cleared = true;
        else if (current_map == &map_game_left)
            map_game_left_cleared = true;
        else if (current_map == &map_game_right)
            map_game_right_cleared = true;
        else if (current_map == &map_basketball)
            map_basketball_cleared = true;
        else if (current_map == &map_special)
            map_special_cleared = true;
        else if (current_map == &map_special_2)
            map_special_2_cleared = true;
    }

    void update_map_reached_end_state()
    {
        if (current_map == &map_game_right)
        {
            if (player_pos.x >= map_game_right.width - 300)
                map_game_right_reached_end = true;
        }
    }

    void draw_enemies(const Camera& camera, int map_offset_x, int map_offset_y)
    {
        for (auto& enemy : enemy_list)
        {
            enemy.draw(camera, map_offset_x, map_offset_y);
        }
    }

    void draw_debug_battle_info()
    {
        setbkmode(TRANSPARENT);
        settextcolor(RGB(255, 220, 120));
        settextstyle(24, 0, _T("IPix"));

        TCHAR text[128];
        _stprintf_s(text, _T("敌人数量: %d"), (int)enemy_list.size());
        outtextxy(30, 80, text);

        if (current_map == &map_game_right)
        {
            if (map_game_right_reached_end)
                outtextxy(30, 110, _T("已走到右侧地图尽头"));
            else
                outtextxy(30, 110, _T("还没有走完整个右侧地图"));

            if (map_game_right_cleared)
                outtextxy(30, 140, _T("右侧地图已清场"));
            else
                outtextxy(30, 140, _T("右侧地图还有敌人"));
        }
    }

private:
    int window_width = 1920;
    int window_height = 1080;

    MapData map_car_1;
    MapData map_car_2;
    MapData map_game_left;
    MapData map_game_right;
    MapData map_basketball;
    MapData map_special;
    MapData map_special_2;
    MapData* current_map = nullptr;

    Vector2 player_pos = Vector2(300, 700);

    bool is_facing_left = false;
    bool is_moving = false;
    bool can_switch_map = false;

    SwitchTarget switch_target = SwitchTarget::None;

    Atlas* atlas_current_left = nullptr;
    Atlas* atlas_current_right = nullptr;
    Atlas* atlas_jump_left = nullptr;
    Atlas* atlas_jump_right = nullptr;

    Animation animation_current;
    Animation animation_jump;
    Combat combat;

    std::list<Enemy> enemy_list;

    int character_draw_offset_x = 0;
    int character_draw_offset_y = 50;
    int character_attack_offset_y = 50;
    int character_width = 198;

    int character_collision_offset_x_left = 0;
    int character_collision_offset_x_right = 0;
    int character_collision_offset_y = -170;
    int character_collision_width_left = 198;
    int character_collision_width_right = 198;
    int character_collision_height = 190;
    int character_feet_offset_y = 20;

    bool is_jumping = false;
    bool is_on_ground = true;
    bool space_key_down = false;

    int jump_count = 0;
    int max_jump_count = 2;

    float velocity_y = 0.0f;
    float gravity = 1.2f;
    float jump_timer = 0.0f;
    float jump_duration = 600.0f;
    float jump_start_y = 0.0f;
    float jump_height = 210.0f;

    bool basketball_extra_active = false;

    bool is_player_hurt = false;
    int player_hurt_timer = 0;
    int player_hurt_time = 400;

    ComboStage attack_last_stage = ComboStage::None;
    int attack_damage_mask = 0;
    std::vector<HitImpact> hit_impacts;

    int hit_stop_timer = 0;
    int boss_warning_timer = 0;
    int boss_warning_duration = 1200;

    int fade_timer = 0;
    int fade_duration = 350;

    int hurt_flash_timer = 0;
    bool map_car_1_cleared = false;
    bool map_car_2_cleared = false;
    bool map_game_left_cleared = false;
    bool map_game_right_cleared = false;
    bool map_basketball_cleared = false;
    bool map_special_cleared = false;
    bool map_special_2_cleared = false;

    bool map_game_right_reached_end = false;
    EndingPhase ending_phase = EndingPhase::None;

    int ending_timer = 0;
    int ending_card_timer = 0;
    int ending_card_index = 0;

    bool ending_finished = false;
    bool ending_end1_started = false;

    const int ending_text_start_time = 2500;
    const int ending_card_duration = 8400;
    const int ending_card_count = 8;
    const int ending_end1_duration = 31000;
};

#endif 