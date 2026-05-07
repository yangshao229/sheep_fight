#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "atlas.h"
#include "camera.h"
#include "util.h"

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

    void on_attack_input()
    {
        if (!is_attacking)
        {
            start_attack1();
            return;
        }

        if (combo_stage == ComboStage::Attack1)
        {
            if (attack_frame >= combo_open_frame_attack1)
                attack_buffered = true;
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

    void set_combo_open_frame_attack1(int frame_idx)
    {
        combo_open_frame_attack1 = frame_idx;
    }

private:
    void start_attack1()
    {
        is_attacking = true;
        attack_buffered = false;
        combo_stage = ComboStage::Attack1;
        attack_frame = 0;
        attack_timer = 0;
    }

    void start_attack2()
    {
        is_attacking = true;
        attack_buffered = false;
        combo_stage = ComboStage::Attack2;
        attack_frame = 0;
        attack_timer = 0;
    }

    void finish()
    {
        is_attacking = false;
        attack_buffered = false;
        combo_stage = ComboStage::None;
        attack_frame = 0;
        attack_timer = 0;
    }

    void reset_all()
    {
        finish();
    }

    IMAGE* get_current_frame(bool facing_left)
    {
        Atlas* atlas = nullptr;
        int frame_count = 0;

        if (combo_stage == ComboStage::Attack1)
        {
            atlas = facing_left ? atlas_attack1_left : atlas_attack1_right;
            frame_count = attack1_frame_count;
        }
        else if (combo_stage == ComboStage::Attack2)
        {
            atlas = facing_left ? atlas_attack2_left : atlas_attack2_right;
            frame_count = attack2_frame_count;
        }

        if (atlas == nullptr || atlas->get_size() <= 0 || frame_count <= 0)
            return nullptr;

        int idx = attack_frame;

        if (idx < 0)
            idx = 0;

        if (idx >= atlas->get_size())
            idx = atlas->get_size() - 1;

        return atlas->get_image(idx);
    }

private:
    Atlas* atlas_attack1_left = nullptr;
    Atlas* atlas_attack1_right = nullptr;
    Atlas* atlas_attack2_left = nullptr;
    Atlas* atlas_attack2_right = nullptr;

    int attack1_frame_count = 0;
    int attack2_frame_count = 0;

    bool is_attacking = false;
    bool attack_buffered = false;

    ComboStage combo_stage = ComboStage::None;

    int attack_frame = 0;
    int attack_timer = 0;
    int attack_frame_interval = 85;
    int combo_open_frame_attack1 = 1;
};

#endif
