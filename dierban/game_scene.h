#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "animation.h"
#include "vector2.h"
#include "util.h"
#include "character_select.h"
#include "combat.h"

#include <graphics.h>
#include <windows.h>

extern SceneManager scene_manager;
extern Camera main_camera;

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
extern Atlas atlas_mlbear_jump_left;
extern Atlas atlas_mlbear_jump_right;
extern Atlas atlas_mlbear_attack1_left;
extern Atlas atlas_mlbear_attack1_right;
extern Atlas atlas_mlbear_attack2_left;
extern Atlas atlas_mlbear_attack2_right;

class GameScene : public Scene
{
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter()
    {
        window_width = 1920;
        window_height = 1080;

        setup_character_by_selected();

        setup_maps();

        animation_current.set_interval(100);
        animation_current.set_loop(true);

        animation_jump.set_interval(100);
        animation_jump.set_loop(true);

        combat.set_attack_frame_interval(selected_character == CharacterSelect::MALAIbear ? 55 : 85);
        combat.set_combo_open_frame_attack1(1);

        is_facing_left = false;
        is_moving = false;
        can_switch_map = false;
        switch_target = SwitchTarget::None;

        velocity_y = 0.0f;
        is_jumping = false;
        is_on_ground = true;
        jump_count = 0;
        space_key_down = false;

        switch_map(&map_car_1);
    }

    void on_update(int delta)
    {
        combat.update(delta);

        const float speed = 450.0f;
        float move_step = speed * delta / 1000.0f;

        bool moving_left = false;
        bool moving_right = false;

        is_moving = false;

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
        update_jump(delta);

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

        int map_offset_x = get_current_map_offset_x();
        int map_offset_y = get_current_map_offset_y();

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
            if (animation_jump.get_frame() != nullptr)
            {
                animation_jump.on_draw(
                    camera,
                    (int)player_pos.x + character_draw_offset_x + map_offset_x,
                    (int)player_pos.y - character_draw_offset_y + map_offset_y
                );
            }
            else
            {
                draw_player_placeholder(camera, map_offset_x, map_offset_y);
            }
        }
        else
        {
            if (animation_current.get_frame() != nullptr)
            {
                animation_current.on_draw(
                    camera,
                    (int)player_pos.x + character_draw_offset_x + map_offset_x,
                    (int)player_pos.y - character_draw_offset_y + map_offset_y
                );
            }
            else
            {
                draw_player_placeholder(camera, map_offset_x, map_offset_y);
            }
        }

        draw_tips();
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
    }

private:
    struct MapData
    {
        IMAGE* background = nullptr;
        int width = 0;
        int height = 0;
        Vector2 spawn;
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

private:
    void setup_character_by_selected()
    {
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

        animation_current.set_atlas(atlas_current_right);
        animation_jump.set_atlas(atlas_jump_right);
    }

    void setup_maps()
    {
        map_car_1.background = &img_car_background_1;
        map_car_1.width = img_car_background_1.getwidth();
        map_car_1.height = img_car_background_1.getheight();
        map_car_1.spawn = Vector2(120, 620);

        map_car_2.background = &img_car_background_2;
        map_car_2.width = img_car_background_2.getwidth();
        map_car_2.height = img_car_background_2.getheight();
        map_car_2.spawn = Vector2((float)(map_car_2.width / 2), 760);

        map_game_left.background = &img_game_background_1;
        map_game_left.width = img_game_background_1.getwidth();
        map_game_left.height = img_game_background_1.getheight();
        map_game_left.spawn = Vector2((float)(map_game_left.width - 260), 820);

        map_game_right.background = &img_game_background_2;
        map_game_right.width = img_game_background_2.getwidth();
        map_game_right.height = img_game_background_2.getheight();
        map_game_right.spawn = Vector2(150, 620);

        map_basketball.background = &img_basketball_background;
        map_basketball.width = img_basketball_background.getwidth();
        map_basketball.height = img_basketball_background.getheight();
        map_basketball.spawn = Vector2(160, 630);

        map_special.background = &img_special_background;
        map_special.width = img_special_background.getwidth();
        map_special.height = img_special_background.getheight();
        map_special.spawn = Vector2(150, 710);

        map_special_2.background = &img_special_background_2;
        map_special_2.width = img_special_background_2.getwidth();
        map_special_2.height = img_special_background_2.getheight();
        map_special_2.spawn = Vector2(150, 810);
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
    }

    void switch_map(MapData* new_map)
    {
        if (new_map == nullptr)
            return;

        current_map = new_map;
        player_pos = current_map->spawn;

        velocity_y = 0.0f;
        is_jumping = false;
        is_on_ground = true;
        jump_count = 0;

        main_camera.set_position(0.0f, 0.0f);
        update_camera();
        update_map_switch_state();
    }

    void try_start_jump()
    {
        if (jump_count >= 1)
            return;

        velocity_y = -920.0f;
        is_jumping = true;
        is_on_ground = false;
        jump_count++;

        animation_jump.reset();
    }

    void update_jump(int delta)
    {
        if (is_on_ground && !is_jumping)
            return;

        const float gravity = 2400.0f;

        velocity_y += gravity * delta / 1000.0f;
        player_pos.y += velocity_y * delta / 1000.0f;

        float ground_y = get_ground_y();

        if (player_pos.y >= ground_y)
        {
            player_pos.y = ground_y;
            velocity_y = 0.0f;
            is_jumping = false;
            is_on_ground = true;
            jump_count = 0;
        }
    }

    float get_ground_y() const
    {
        if (current_map == &map_car_1)
            return 620.0f;

        if (current_map == &map_car_2)
            return 760.0f;

        if (current_map == &map_game_left)
            return 820.0f;

        if (current_map == &map_game_right)
            return 620.0f;

        if (current_map == &map_basketball)
            return 630.0f;

        if (current_map == &map_special)
            return 710.0f;

        if (current_map == &map_special_2)
            return 810.0f;

        return 720.0f;
    }

    void limit_player_in_map()
    {
        if (current_map == nullptr)
            return;

        float left_limit = 0.0f;
        float right_limit = (float)current_map->width - character_width;

        if (right_limit < left_limit)
            right_limit = left_limit;

        if (player_pos.x < left_limit)
            player_pos.x = left_limit;

        if (player_pos.x > right_limit)
            player_pos.x = right_limit;
    }

    void update_camera()
    {
        if (current_map == nullptr)
            return;

        float camera_x = player_pos.x + character_width / 2.0f - window_width / 2.0f;
        float camera_y = 0.0f;

        float max_camera_x = (float)current_map->width - window_width;

        if (max_camera_x < 0)
            max_camera_x = 0;

        if (camera_x < 0)
            camera_x = 0;

        if (camera_x > max_camera_x)
            camera_x = max_camera_x;

        main_camera.set_position(camera_x, camera_y);
    }

    void update_map_switch_state()
    {
        can_switch_map = false;
        switch_target = SwitchTarget::None;

        if (current_map == nullptr)
            return;

        float x = player_pos.x;

        if (current_map == &map_car_1)
        {
            if (x >= current_map->width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::Car1ToCar2;
            }
        }
        else if (current_map == &map_car_2)
        {
            if (x <= 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::Car2ToGameLeft;
            }
            else if (x >= current_map->width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::Car2ToGameRight;
            }
        }
        else if (current_map == &map_game_left)
        {
            if (x >= current_map->width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::GameLeftToCar2;
            }
        }
        else if (current_map == &map_game_right)
        {
            if (x <= 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::GameRightToCar2;
            }
            else if (x >= current_map->width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::GameRightToBasketball;
            }
            else if (x > current_map->width / 2 - 120 && x < current_map->width / 2 + 120)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::GameRightToSpecial;
            }
        }
        else if (current_map == &map_basketball)
        {
            if (x <= 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::BasketballToGameRight;
            }
        }
        else if (current_map == &map_special)
        {
            if (x <= 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::SpecialToGameRight;
            }
            else if (x >= current_map->width - 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::SpecialToSpecial2;
            }
        }
        else if (current_map == &map_special_2)
        {
            if (x <= 260)
            {
                can_switch_map = true;
                switch_target = SwitchTarget::Special2ToSpecial;
            }
        }
    }

    int get_current_map_offset_x() const
    {
        if (current_map == nullptr)
            return 0;

        if (current_map->width >= window_width)
            return 0;

        return (window_width - current_map->width) / 2;
    }

    int get_current_map_offset_y() const
    {
        if (current_map == nullptr)
            return 0;

        if (current_map->height >= window_height)
            return 0;

        return (window_height - current_map->height) / 2;
    }

    void draw_current_map(const Camera& camera)
    {
        if (current_map == nullptr || current_map->background == nullptr)
            return;

        if (current_map->background->getwidth() <= 0 || current_map->background->getheight() <= 0)
        {
            setbkcolor(RGB(30, 30, 30));
            cleardevice();
            return;
        }

        int map_offset_x = get_current_map_offset_x();
        int map_offset_y = get_current_map_offset_y();

        if (current_map->width <= window_width && current_map->height <= window_height)
        {
            putimage(map_offset_x, map_offset_y, current_map->background);
        }
        else
        {
            const Vector2& camera_pos = camera.get_position();

            int src_x = (int)camera_pos.x;
            int src_y = (int)camera_pos.y;

            if (src_x < 0)
                src_x = 0;

            if (src_y < 0)
                src_y = 0;

            int draw_w = window_width;
            int draw_h = window_height;

            if (src_x + draw_w > current_map->width)
                draw_w = current_map->width - src_x;

            if (src_y + draw_h > current_map->height)
                draw_h = current_map->height - src_y;

            putimage(0, 0, draw_w, draw_h, current_map->background, src_x, src_y);
        }
    }

    void draw_player_placeholder(const Camera& camera, int map_offset_x, int map_offset_y)
    {
        const Vector2& camera_pos = camera.get_position();

        int x = (int)(player_pos.x - camera_pos.x) + map_offset_x;
        int y = (int)(player_pos.y - camera_pos.y) + map_offset_y;

        setfillcolor(RGB(255, 220, 120));
        solidrectangle(x, y - 160, x + 100, y);
    }

    void draw_tips()
    {
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        settextstyle(32, 0, _T("Consolas"));
        outtextxy(30, 30, _T("A/D move   SPACE jump   J attack animation   E switch map   K menu"));

        if (can_switch_map)
        {
            settextcolor(RGB(255, 220, 120));
            settextstyle(36, 0, _T("Consolas"));
            outtextxy(760, 980, _T("Press E to switch map"));
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

    Vector2 player_pos;

    Atlas* atlas_current_left = nullptr;
    Atlas* atlas_current_right = nullptr;
    Atlas* atlas_jump_left = nullptr;
    Atlas* atlas_jump_right = nullptr;

    Animation animation_current;
    Animation animation_jump;
    Combat combat;

    bool is_facing_left = false;
    bool is_moving = false;

    int character_draw_offset_x = 0;
    int character_draw_offset_y = 0;
    int character_attack_offset_y = 0;
    int character_width = 120;

    float velocity_y = 0.0f;
    bool is_jumping = false;
    bool is_on_ground = true;
    int jump_count = 0;
    bool space_key_down = false;

    bool can_switch_map = false;
    SwitchTarget switch_target = SwitchTarget::None;
};

#endif
