#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "vector2.h"
#include "camera.h"
#include "atlas.h"
#include "animation.h"
#include "util.h"

#include <graphics.h>
#include <array>
#include <cmath>
#include <iostream>

enum class EnemyKind
{
    Small1,
    Small2,
    Boss1,
    Boss2
};

enum class EnemyState
{
    Idle,
    Move,
    Attack,
    Hurt,
    Dead
};

struct EnemyConfig
{
    EnemyKind kind = EnemyKind::Small1;

    int hp = 5;
    int damage = 1;

    int width = 100;
    int height = 150;

    int collision_offset_x = 0;
    int collision_offset_y = 0;
    int collision_width = 100;
    int collision_height = 150;
    int feet_offset_y = 150;

    float speed = 100.0f;
    float attack_range = 130.0f;

    int attack_frame_count = 7;
    int attack_frame_interval = 100;
    int hit_frame = 3;
    int active_start_frame = -1;
    int active_end_frame = -1;
    int attack_cooldown = 1000;

    int attack_hitbox_reach = 0;
    int attack_hitbox_back = 0;
    int attack_hitbox_offset_y = 0;
    int attack_hitbox_height = 0;

    int hurt_stun_time = 350;

    float knockback_speed = 420.0f;
    int knockback_time = 160;

    int dead_keep_time = 500;

    bool is_boss = false;


    int invincible_time_after_hit = 0;

    int hits_before_invincible = 1;

    Atlas* idle_left = nullptr;
    Atlas* idle_right = nullptr;

    Atlas* walk_left = nullptr;
    Atlas* walk_right = nullptr;

    Atlas* attack_left = nullptr;
    Atlas* attack_right = nullptr;

    Atlas* hurt_left = nullptr;
    Atlas* hurt_right = nullptr;

    Atlas* dead_left = nullptr;
    Atlas* dead_right = nullptr;

    Atlas* attack1_left = nullptr;
    Atlas* attack1_right = nullptr;

    Atlas* attack2_left = nullptr;
    Atlas* attack2_right = nullptr;

    Atlas* attack3_left = nullptr;
    Atlas* attack3_right = nullptr;

    Atlas* attack4_left = nullptr;
    Atlas* attack4_right = nullptr;

};

class Enemy
{
public:
    Enemy() = default;

    Enemy(const EnemyConfig& config, const Vector2& spawn_pos)
    {
        this->config = config;
        this->pos = spawn_pos;

        if (this->config.collision_width <= 0)
            this->config.collision_width = this->config.width;

        if (this->config.collision_height <= 0)
            this->config.collision_height = this->config.height;

        if (this->config.feet_offset_y <= 0)
            this->config.feet_offset_y = this->config.collision_offset_y + this->config.collision_height;

        hp = config.hp;
        state = EnemyState::Idle;

        alive = true;
        removable = false;

        facing_left = true;

        current_attack_left = config.attack_left;
        current_attack_right = config.attack_right;
        current_attack_frame_count = config.attack_frame_count;
        current_attack_hit_frame = config.hit_frame;
        current_attack_interval = config.attack_frame_interval;
        current_attack_active_start_frame = get_active_start_frame();
        current_attack_active_end_frame = get_active_end_frame();
        current_attack_damage = config.damage;
        current_attack_is_laser = false;
        current_attack_use_frame_intervals = false;
        current_attack_frame_intervals = {};
        current_attack_hitbox_reach = get_default_attack_hitbox_reach();
        current_attack_hitbox_back = get_default_attack_hitbox_back();
        current_attack_hitbox_offset_y = get_default_attack_hitbox_offset_y();
        current_attack_hitbox_height = get_default_attack_hitbox_height();

        setup_animation_by_state();
    }

    void update(int delta, const Vector2& player_pos)
    {
        if (removable)
            return;

        if (state == EnemyState::Dead)
        {
            dead_timer -= delta;

            if (dead_timer <= 0)
                removable = true;

            update_animation(delta);
            return;
        }

        if (attack_cd_timer > 0)
            attack_cd_timer -= delta;

        if (invincible_timer > 0)
        {
            invincible_timer -= delta;
            if (invincible_timer < 0)
                invincible_timer = 0;
        }

        update_facing(player_pos);
        update_knockback(delta);

        if (knockback_timer > 0)
        {
            update_animation(delta);
            return;
        }

        if (state == EnemyState::Hurt)
        {
            hurt_timer -= delta;

            if (hurt_timer <= 0)
            {
                hurt_timer = 0;
                state = EnemyState::Idle;
                setup_animation_by_state();
            }

            update_animation(delta);
            return;
        }

        if (state == EnemyState::Attack)
        {
            update_attack(delta);
            update_animation(delta);
            return;
        }

        float enemy_center_x = get_center_x();
        float dx = player_pos.x - enemy_center_x;
        float distance = std::abs(dx);

        if (distance <= config.attack_range)
        {
            try_start_attack();
        }
        else
        {
            move_to_player(delta, player_pos);
        }

        update_animation(delta);
    }

    bool check_hit_player_this_frame(float player_left, float player_top, float player_right, float player_bottom)
    {
        if (state != EnemyState::Attack)
            return false;

        if (has_hit_player)
            return false;

        if (current_attack_frame < current_attack_active_start_frame ||
            current_attack_frame > current_attack_active_end_frame)
            return false;

        float enemy_left = 0.0f;
        float enemy_right = 0.0f;
        float enemy_top = 0.0f;
        float enemy_bottom = 0.0f;

        get_current_attack_box(enemy_left, enemy_top, enemy_right, enemy_bottom);

        bool overlap =
            enemy_right >= player_left &&
            enemy_left <= player_right &&
            enemy_bottom >= player_top &&
            enemy_top <= player_bottom;

        if (overlap)
        {
            has_hit_player = true;
            return true;
        }

        return false;
    }

    void draw(const Camera& camera, int map_offset_x = 0, int map_offset_y = 0)
    {
        if (removable)
            return;

        if (animation_has_atlas)
        {
            animation.on_draw(
                camera,
                (int)pos.x + map_offset_x,
                (int)pos.y + map_offset_y
            );
        }
        else
        {
            draw_placeholder(camera, map_offset_x, map_offset_y);
        }

        draw_boss2_laser(camera, map_offset_x, map_offset_y);
    }

    bool hurt(int damage, int knockback_dir, float knockback_scale = 1.0f)
    {
        if (!alive || state == EnemyState::Dead)
            return false;

        if (config.is_boss && invincible_timer > 0)
            return false;

        hp -= damage;

        if (config.is_boss)
        {
            boss_hit_count_after_invincible++;

            if (config.hits_before_invincible <= 0)
                config.hits_before_invincible = 1;

            if (boss_hit_count_after_invincible >= config.hits_before_invincible)
            {
                boss_hit_count_after_invincible = 0;

                if (config.invincible_time_after_hit > 0)
                    invincible_timer = config.invincible_time_after_hit;
            }
        }

        if (hp <= 0)
        {
            hp = 0;
            alive = false;
            state = EnemyState::Dead;
            dead_timer = config.dead_keep_time;
            invincible_timer = 0;
            boss_hit_count_after_invincible = 0;

            attack_timer = 0;
            current_attack_frame = 0;
            has_hit_player = false;

            setup_animation_by_state();
            return true;
        }

        state = EnemyState::Hurt;
        hurt_timer = config.hurt_stun_time;

        knockback_timer = config.knockback_time;
        knockback_velocity = config.knockback_speed * knockback_scale * knockback_dir;

        attack_timer = 0;
        current_attack_frame = 0;
        has_hit_player = false;

        setup_animation_by_state();
        return true;
    }

    bool is_alive() const
    {
        return alive;
    }

    bool should_remove() const
    {
        return removable;
    }

    bool is_solid() const
    {
        return alive && state != EnemyState::Dead && !removable;
    }

    int get_damage() const
    {
        return current_attack_damage;
    }

    Vector2 get_position() const
    {
        return pos;
    }

    int get_width() const
    {
        return config.collision_width;
    }

    int get_height() const
    {
        return config.collision_height;
    }

    bool is_boss() const
    {
        return config.is_boss;
    }
    int get_hp() const
    {
        return hp;
    }

    int get_max_hp() const
    {
        return config.hp;
    }

    float get_hp_rate() const
    {
        if (config.hp <= 0)
            return 0.0f;

        float rate = (float)hp / (float)config.hp;

        if (rate < 0.0f) rate = 0.0f;
        if (rate > 1.0f) rate = 1.0f;

        return rate;
    }

    bool is_invincible() const
    {
        return invincible_timer > 0;
    }
    Vector2 get_hit_effect_position() const
    {
        return Vector2(get_center_x(), get_top() + config.collision_height * 0.45f);
    }

    EnemyState get_state() const
    {
        return state;
    }

    float get_left() const
    {
        return pos.x + config.collision_offset_x;
    }

    float get_right() const
    {
        return pos.x + config.collision_offset_x + config.collision_width;
    }

    float get_top() const
    {
        return pos.y + config.collision_offset_y;
    }

    float get_bottom() const
    {
        return pos.y + config.collision_offset_y + config.collision_height;
    }

    float get_center_x() const
    {
        return (get_left() + get_right()) / 2.0f;
    }

private:
    void update_facing(const Vector2& player_pos)
    {
        float enemy_center_x = get_center_x();

        if (player_pos.x < enemy_center_x)
            facing_left = true;
        else if (player_pos.x > enemy_center_x)
            facing_left = false;
    }

    void update_knockback(int delta)
    {
        if (knockback_timer <= 0)
            return;

        pos.x += knockback_velocity * delta / 1000.0f;

        knockback_timer -= delta;

        if (knockback_timer <= 0)
        {
            knockback_timer = 0;
            knockback_velocity = 0.0f;
        }
    }

    void move_to_player(int delta, const Vector2& player_pos)
    {
        if (state != EnemyState::Move)
        {
            state = EnemyState::Move;
            setup_animation_by_state();
        }

        float enemy_center_x = get_center_x();

        if (player_pos.x < enemy_center_x)
            pos.x -= config.speed * delta / 1000.0f;
        else if (player_pos.x > enemy_center_x)
            pos.x += config.speed * delta / 1000.0f;
    }

    void try_start_attack()
    {
        if (attack_cd_timer > 0)
        {
            if (state != EnemyState::Idle)
            {
                state = EnemyState::Idle;
                setup_animation_by_state();
            }

            return;
        }

        state = EnemyState::Attack;

        attack_timer = 0;
        current_attack_frame = 0;
        attack_cd_timer = config.attack_cooldown;
        has_hit_player = false;

        prepare_current_attack();
        setup_animation_by_state();
    }

    int get_active_start_frame() const
    {
        if (config.active_start_frame >= 0)
            return config.active_start_frame;

        return config.hit_frame;
    }

    int get_active_end_frame() const
    {
        if (config.active_end_frame >= 0)
            return config.active_end_frame;

        return config.hit_frame;
    }

    int get_default_attack_hitbox_reach() const
    {
        if (config.attack_hitbox_reach > 0)
            return config.attack_hitbox_reach;

        return config.collision_width / 2 + 35;
    }

    int get_default_attack_hitbox_back() const
    {
        if (config.attack_hitbox_back > 0)
            return config.attack_hitbox_back;

        return config.collision_width / 3;
    }

    int get_default_attack_hitbox_offset_y() const
    {
        return config.attack_hitbox_offset_y;
    }

    int get_default_attack_hitbox_height() const
    {
        if (config.attack_hitbox_height > 0)
            return config.attack_hitbox_height;

        return config.collision_height;
    }

    void get_current_attack_box(float& left, float& top, float& right, float& bottom) const
    {
        float front = facing_left ? get_left() : get_right();

        if (facing_left)
        {
            left = front - current_attack_hitbox_reach;
            right = front + current_attack_hitbox_back;
        }
        else
        {
            left = front - current_attack_hitbox_back;
            right = front + current_attack_hitbox_reach;
        }

        top = get_top() + current_attack_hitbox_offset_y;
        bottom = top + current_attack_hitbox_height;
    }
    void prepare_current_attack()
    {
        current_attack_frame_count = config.attack_frame_count;
        current_attack_hit_frame = config.hit_frame;
        current_attack_interval = config.attack_frame_interval;
        current_attack_active_start_frame = get_active_start_frame();
        current_attack_active_end_frame = get_active_end_frame();
        current_attack_damage = config.damage;
        current_attack_is_laser = false;
        current_attack_use_frame_intervals = false;
        current_attack_frame_intervals = {};
        current_attack_hitbox_reach = get_default_attack_hitbox_reach();
        current_attack_hitbox_back = get_default_attack_hitbox_back();
        current_attack_hitbox_offset_y = get_default_attack_hitbox_offset_y();
        current_attack_hitbox_height = get_default_attack_hitbox_height();

        current_attack_left = config.attack_left;
        current_attack_right = config.attack_right;

        if (config.kind == EnemyKind::Boss2)
        {
            int attack_index = boss2_next_attack_index;

            if (attack_index == 0)
            {
                current_attack_left = config.attack1_left;
                current_attack_right = config.attack1_right;
                current_attack_damage = 2;
                current_attack_active_start_frame = 1;
                current_attack_active_end_frame = 2;
                current_attack_hitbox_reach = 210;
                current_attack_hitbox_back = 75;
                current_attack_hitbox_offset_y = 35;
                current_attack_hitbox_height = config.collision_height - 30;
            }
            else if (attack_index == 1)
            {
                current_attack_left = config.attack2_left;
                current_attack_right = config.attack2_right;
                current_attack_damage = 3;
                current_attack_active_start_frame = 2;
                current_attack_active_end_frame = 3;
                current_attack_hitbox_reach = 190;
                current_attack_hitbox_back = 70;
                current_attack_hitbox_offset_y = -20;
                current_attack_hitbox_height = config.collision_height;
            }
            else if (attack_index == 2)
            {
                current_attack_left = config.attack3_left;
                current_attack_right = config.attack3_right;
                current_attack_damage = 3;
                current_attack_active_start_frame = 2;
                current_attack_active_end_frame = 4;
                current_attack_hitbox_reach = 230;
                current_attack_hitbox_back = 80;
                current_attack_hitbox_offset_y = 55;
                current_attack_hitbox_height = config.collision_height - 45;
            }
            else if (attack_index == 3)
            {
                current_attack_left = config.attack4_left;
                current_attack_right = config.attack4_right;
                current_attack_frame_count = 9;
                current_attack_hit_frame = 4;
                current_attack_damage = 4;
                current_attack_active_start_frame = 3;
                current_attack_active_end_frame = 7;
                current_attack_hitbox_reach = 760;
                current_attack_hitbox_back = 95;
                current_attack_hitbox_offset_y = 42;
                current_attack_hitbox_height = 132;
                current_attack_frame_intervals = { 100, 120, 150, 95, 80, 80, 100, 135, 180 };
                current_attack_use_frame_intervals = true;
                current_attack_is_laser = true;
            }

            boss2_next_attack_index = attack_index + 1;

            if (boss2_next_attack_index > 3)
                boss2_next_attack_index = 0;
        }

        if (current_attack_left == nullptr)
            current_attack_left = config.attack_left;

        if (current_attack_right == nullptr)
            current_attack_right = config.attack_right;
    }

    int get_current_frame_interval() const
    {
        if (current_attack_use_frame_intervals &&
            current_attack_frame >= 0 &&
            current_attack_frame < (int)current_attack_frame_intervals.size() &&
            current_attack_frame_intervals[current_attack_frame] > 0)
        {
            return current_attack_frame_intervals[current_attack_frame];
        }

        return current_attack_interval;
    }

    void finish_attack()
    {
        state = EnemyState::Idle;

        attack_timer = 0;
        current_attack_frame = 0;
        has_hit_player = false;
        current_attack_is_laser = false;
        current_attack_use_frame_intervals = false;
        current_attack_frame_intervals = {};

        setup_animation_by_state();
    }

    void update_attack(int delta)
    {
        attack_timer += delta;

        while (attack_timer >= get_current_frame_interval())
        {
            attack_timer -= get_current_frame_interval();
            current_attack_frame++;

            if (current_attack_frame >= current_attack_frame_count)
            {
                finish_attack();
                return;
            }
        }
    }

    void update_animation(int delta)
    {
        if (animation_has_atlas)
            animation.on_update(delta);
    }

    void setup_animation_by_state()
    {
        Atlas* target = nullptr;

        if (state == EnemyState::Idle)
        {
            target = facing_left ? config.idle_left : config.idle_right;

            if (target == nullptr)
                target = facing_left ? config.walk_left : config.walk_right;
        }
        else if (state == EnemyState::Move)
        {
            target = facing_left ? config.walk_left : config.walk_right;
        }
        else if (state == EnemyState::Attack)
        {
            target = facing_left ? current_attack_left : current_attack_right;

            if (target == nullptr)
                target = facing_left ? config.attack_left : config.attack_right;
        }
        else if (state == EnemyState::Hurt)
        {
            target = facing_left ? config.hurt_left : config.hurt_right;
        }
        else if (state == EnemyState::Dead)
        {
            target = facing_left ? config.dead_left : config.dead_right;
        }

        if (target != nullptr && target->get_size() > 0)
        {
            animation.set_atlas(target);
            animation.set_interval(state == EnemyState::Attack ? current_attack_interval : config.attack_frame_interval);

            if (state == EnemyState::Attack && current_attack_use_frame_intervals)
                animation.set_frame_intervals(current_attack_frame_intervals.data(), current_attack_frame_count);

            animation.set_loop(state != EnemyState::Dead);
            animation_has_atlas = true;
        }
        else
        {
            animation_has_atlas = false;
        }
    }

    void draw_boss2_laser(const Camera& camera, int map_offset_x, int map_offset_y) const
    {
        if (!current_attack_is_laser || state != EnemyState::Attack)
            return;

        if (current_attack_frame < 0 || current_attack_frame > 8)
            return;

        int muzzle_x = (int)((facing_left ? get_left() : get_right()) + map_offset_x - camera.get_position().x);
        int muzzle_y = (int)(get_top() + current_attack_hitbox_offset_y + current_attack_hitbox_height * 0.5f + map_offset_y - camera.get_position().y);
        int direction = facing_left ? -1 : 1;
        int frame = current_attack_frame;

        int charge_radius = 18 + frame * 6;
        int core_radius = 6 + frame * 2;

        if (frame >= 4)
        {
            charge_radius = 42 + (frame % 2) * 6;
            core_radius = 16;
        }

        if (frame >= 7)
        {
            charge_radius = 34;
            core_radius = 11;
        }

        setlinecolor(RGB(45, 95, 255));
        setlinestyle(PS_SOLID, 2);
        circle(muzzle_x, muzzle_y, charge_radius + 10);

        setlinecolor(RGB(120, 230, 255));
        setlinestyle(PS_SOLID, 3);
        circle(muzzle_x, muzzle_y, charge_radius);
        circle(muzzle_x, muzzle_y, charge_radius / 2);

        setfillcolor(RGB(245, 255, 255));
        solidcircle(muzzle_x, muzzle_y, core_radius);

        if (frame < 3)
        {
            setlinestyle(PS_SOLID, 1);
            return;
        }

        int length = 620;
        int core_width = 12;
        int glow_width = 32;

        if (frame == 4)
        {
            length = 760;
            core_width = 18;
            glow_width = 50;
        }
        else if (frame == 5 || frame == 6)
        {
            length = 790;
            core_width = 20;
            glow_width = 54;
        }
        else if (frame == 7)
        {
            length = 700;
            core_width = 13;
            glow_width = 36;
        }
        else if (frame == 8)
        {
            length = 560;
            core_width = 8;
            glow_width = 24;
        }

        int start_x = muzzle_x + direction * 10;
        int end_x = muzzle_x + direction * length;

        setlinecolor(RGB(25, 80, 210));
        setlinestyle(PS_SOLID, glow_width);
        line(start_x, muzzle_y, end_x, muzzle_y);

        setlinecolor(RGB(60, 180, 255));
        setlinestyle(PS_SOLID, glow_width / 2);
        line(start_x, muzzle_y, end_x, muzzle_y);

        setlinecolor(RGB(250, 255, 255));
        setlinestyle(PS_SOLID, core_width);
        line(start_x, muzzle_y, end_x, muzzle_y);

        setlinecolor(RGB(190, 245, 255));
        setlinestyle(PS_SOLID, 3);
        line(muzzle_x, muzzle_y - glow_width / 2, end_x - direction * 60, muzzle_y - 10);
        line(muzzle_x, muzzle_y + glow_width / 2, end_x - direction * 100, muzzle_y + 12);

        if (frame >= 4 && frame <= 7)
        {
            setlinecolor(RGB(230, 250, 255));
            setlinestyle(PS_SOLID, 2);

            for (int i = 1; i <= 3; i++)
            {
                int spark_x = muzzle_x + direction * (length * i / 4);
                int spark_y = muzzle_y + (((frame + i) % 2 == 0) ? -1 : 1) * (8 + i * 3);
                circle(spark_x, spark_y, 4 + i);
            }
        }

        setlinestyle(PS_SOLID, 1);
    }

    void draw_placeholder(const Camera& camera, int map_offset_x, int map_offset_y)
    {
        int sx = (int)(get_left() + map_offset_x - camera.get_position().x);
        int sy = (int)(get_top() + map_offset_y - camera.get_position().y);

        if (state == EnemyState::Dead)
            setlinecolor(RGB(120, 120, 120));
        else if (state == EnemyState::Hurt)
            setlinecolor(RGB(255, 80, 80));
        else if (state == EnemyState::Attack)
            setlinecolor(RGB(255, 160, 0));
        else if (config.is_boss)
            setlinecolor(RED);
        else
            setlinecolor(YELLOW);

        setlinestyle(PS_SOLID, config.is_boss ? 4 : 2);
        rectangle(sx, sy, sx + config.collision_width, sy + config.collision_height);
        setlinestyle(PS_SOLID, 1);

        setbkmode(TRANSPARENT);
        settextcolor(WHITE);

        if (config.is_boss)
            outtextxy(sx, sy - 35, _T("BOSS"));
        else
            outtextxy(sx, sy - 35, _T("ENEMY"));

        if (state == EnemyState::Attack)
        {
            TCHAR atk_text[64];
            _stprintf_s(atk_text, _T("ATK:%d"), current_attack_frame);
            outtextxy(sx, sy + config.collision_height + 8, atk_text);
        }
    }

private:
    EnemyConfig config;

    Vector2 pos = Vector2(0, 0);

    int hp = 1;

    EnemyState state = EnemyState::Idle;

    bool facing_left = true;
    bool alive = true;
    bool removable = false;

    int attack_timer = 0;
    int current_attack_frame = 0;
    int attack_cd_timer = 0;
    bool has_hit_player = false;

    int hurt_timer = 0;

    int invincible_timer = 0;
    int boss_hit_count_after_invincible = 0;

    int dead_timer = 0;

    int knockback_timer = 0;
    float knockback_velocity = 0.0f;

    Animation animation;
    bool animation_has_atlas = false;

    Atlas* current_attack_left = nullptr;
    Atlas* current_attack_right = nullptr;

    int current_attack_frame_count = 0;
    int current_attack_hit_frame = 0;
    int current_attack_interval = 100;
    int current_attack_active_start_frame = 0;
    int current_attack_active_end_frame = 0;
    int current_attack_damage = 1;
    bool current_attack_is_laser = false;
    bool current_attack_use_frame_intervals = false;
    std::array<int, 9> current_attack_frame_intervals = {};
    int current_attack_hitbox_reach = 0;
    int current_attack_hitbox_back = 0;
    int current_attack_hitbox_offset_y = 0;
    int current_attack_hitbox_height = 0;

    int boss2_next_attack_index = 0;
};

#endif 