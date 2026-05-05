#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "atlas.h"
#include "camera.h"
#include "util.h"

#include <functional>

enum class ComboStage
{
    None,
    Attack1,
    Attack2
};

class Combat
{
public:
    Combat() = default;

    void init(
        Atlas* atk1_left,
        Atlas* atk1_right,
        Atlas* atk2_left,
        Atlas* atk2_right,
        int atk1_frames,
        int atk2_frames
    )
    {
        atlas_attack1_left = atk1_left;
        atlas_attack1_right = atk1_right;
        atlas_attack2_left = atk2_left;
        atlas_attack2_right = atk2_right;

        attack1_frame_count = atk1_frames;
        attack2_frame_count = atk2_frames;

        reset_all();
    }

    void set_attack1_callback(std::function<void()> callback)
    {
        callback_attack1 = callback;
    }

    void set_attack2_callback(std::function<void()> callback)
    {
        callback_attack2 = callback;
    }

    void on_attack_input()
    {
        if (!is_attacking)
        {
            start_attack1();
            return;
        }

        // 只有一段攻击期间，第二下 J 才会缓存成二段攻击
        if (combo_stage == ComboStage::Attack1)
        {
            // 如果你想限制必须到某一帧后才能连招，就用这一句
            if (attack_frame >= combo_open_frame_attack1)
                attack_buffered = true;

            // 如果你想任何时候按第二下 J 都能接二段，把上面 if 删掉，改成：
            // attack_buffered = true;
        }
    }

    void update(int delta)
    {
        if (!is_attacking)
            return;

        attack_timer += delta;

        if (attack_timer < attack_frame_interval)
            return;

        attack_timer = 0;
        attack_frame++;

        if (combo_stage == ComboStage::Attack1)
        {
            if (attack_frame >= attack1_frame_count)
            {
                if (attack_buffered)
                    start_attack2();
                else
                    finish();
            }
        }
        else if (combo_stage == ComboStage::Attack2)
        {
            if (attack_frame >= attack2_frame_count)
                finish();
        }
    }

    void draw(const Camera& camera, int x, int y, bool facing_left)
    {
        if (!is_attacking)
            return;

        IMAGE* img = get_current_frame(facing_left);

        if (img != nullptr)
            putimage_alpha(camera, x, y, img);
    }

    bool is_in_attack() const
    {
        return is_attacking;
    }

    ComboStage get_stage() const
    {
        return combo_stage;
    }

    int get_frame() const
    {
        return attack_frame;
    }

    void set_attack_frame_interval(int ms)
    {
        attack_frame_interval = ms;
    }

    void set_combo_input_limit(int ms)
    {
        // 这个函数保留着，防止 game_scene.h 里调用它时报错。
        // 现在二段攻击不再依赖 combo_input_limit。
        combo_input_limit = ms;
    }

    void set_combo_open_frame_attack1(int frame_idx)
    {
        combo_open_frame_attack1 = frame_idx;
    }

private:
    Atlas* atlas_attack1_left = nullptr;
    Atlas* atlas_attack1_right = nullptr;
    Atlas* atlas_attack2_left = nullptr;
    Atlas* atlas_attack2_right = nullptr;

    bool is_attacking = false;
    ComboStage combo_stage = ComboStage::None;

    int attack_frame = 0;
    int attack_timer = 0;

    bool attack_buffered = false;

    int attack_frame_interval = 60;
    int combo_input_limit = 220;

    int attack1_frame_count = 0;
    int attack2_frame_count = 0;

    int combo_open_frame_attack1 = 1;

    std::function<void()> callback_attack1;
    std::function<void()> callback_attack2;

private:
    void start_attack1()
    {
        is_attacking = true;
        combo_stage = ComboStage::Attack1;

        attack_frame = 0;
        attack_timer = 0;
        attack_buffered = false;

        if (callback_attack1)
            callback_attack1();
    }

    void start_attack2()
    {
        is_attacking = true;
        combo_stage = ComboStage::Attack2;

        attack_frame = 0;
        attack_timer = 0;
        attack_buffered = false;

        if (callback_attack2)
            callback_attack2();
    }

    void finish()
    {
        reset_all();
    }

    void reset_all()
    {
        is_attacking = false;
        combo_stage = ComboStage::None;

        attack_frame = 0;
        attack_timer = 0;
        attack_buffered = false;
    }

    IMAGE* get_current_frame(bool facing_left)
    {
        if (combo_stage == ComboStage::Attack1)
        {
            if (facing_left)
                return atlas_attack1_left ? atlas_attack1_left->get_image(attack_frame) : nullptr;
            else
                return atlas_attack1_right ? atlas_attack1_right->get_image(attack_frame) : nullptr;
        }
        else if (combo_stage == ComboStage::Attack2)
        {
            if (facing_left)
                return atlas_attack2_left ? atlas_attack2_left->get_image(attack_frame) : nullptr;
            else
                return atlas_attack2_right ? atlas_attack2_right->get_image(attack_frame) : nullptr;
        }

        return nullptr;
    }
};

#endif // !_COMBAT_H_