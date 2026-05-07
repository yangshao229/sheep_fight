#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "scene.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* cloths_scene;

class SceneManager
{
public:
    enum class SceneType
    {
        Menu,
        Game,
        Cloths
    };

public:
    SceneManager() = default;
    ~SceneManager() = default;

    void set_current_scene(Scene* scene)
    {
        current_scene = scene;

        if (current_scene != nullptr)
            current_scene->on_enter();
    }

    void switch_to(SceneType type)
    {
        if (current_scene != nullptr)
            current_scene->on_exit();

        switch (type)
        {
        case SceneType::Menu:
            current_scene = menu_scene;
            break;

        case SceneType::Game:
            current_scene = game_scene;
            break;

        case SceneType::Cloths:
            current_scene = cloths_scene;
            break;

        default:
            break;
        }

        if (current_scene != nullptr)
            current_scene->on_enter();
    }

    void on_update(int delta)
    {
        if (current_scene != nullptr)
            current_scene->on_update(delta);
    }

    void on_draw(const Camera& camera)
    {
        if (current_scene != nullptr)
            current_scene->on_draw(camera);
    }

    void on_input(const ExMessage& msg)
    {
        if (current_scene != nullptr)
            current_scene->on_input(msg);
    }

private:
    Scene* current_scene = nullptr;
};

#endif
