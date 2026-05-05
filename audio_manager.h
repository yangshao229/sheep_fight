#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <unordered_map>
#include <iostream>

#pragma comment(lib, "winmm.lib")

class AudioManager
{
public:
    AudioManager() = default;

    ~AudioManager()
    {
        close_all();
    }

    void load_bgm(const std::string& alias, const std::string& path)
    {
        close_alias(alias);

        std::string cmd = "open \"" + path + "\" type mpegvideo alias " + alias;
        check_mci(cmd, "加载BGM失败：" + alias + " -> " + path);

        bgm_aliases[alias] = path;
    }

    void load_sfx(const std::string& alias, const std::string& path)
    {
        close_alias(alias);

        std::string cmd = "open \"" + path + "\" type mpegvideo alias " + alias;
        check_mci(cmd, "加载音效失败：" + alias + " -> " + path);

        sfx_aliases[alias] = path;
    }

    // 循环播放 BGM：普通地图音乐用这个
    void play_bgm(const std::string& alias, bool restart = false)
    {
        if (alias.empty())
            return;

        if (current_bgm == alias && !restart)
            return;

        stop_current_bgm_only();

        current_bgm = alias;

        std::string seek_cmd = "seek " + alias + " to start";
        check_mci(seek_cmd, "BGM回到开头失败：" + alias);

        std::string play_cmd = "play " + alias + " repeat";
        check_mci(play_cmd, "播放BGM失败：" + alias);
    }

    // 只播放一次：end1 / end2 用这个
    void play_bgm_once(const std::string& alias, bool restart = true)
    {
        if (alias.empty())
            return;

        if (current_bgm == alias && !restart)
            return;

        stop_current_bgm_only();

        current_bgm = alias;

        std::string seek_cmd = "seek " + alias + " to start";
        check_mci(seek_cmd, "BGM回到开头失败：" + alias);

        std::string play_cmd = "play " + alias;
        check_mci(play_cmd, "播放一次BGM失败：" + alias);
    }

    void stop_bgm()
    {
        stop_current_bgm_only();
        current_bgm.clear();
    }

    void play_sfx(const std::string& alias)
    {
        if (alias.empty())
            return;

        std::string stop_cmd = "stop " + alias;
        mciSendStringA(stop_cmd.c_str(), nullptr, 0, nullptr);

        std::string seek_cmd = "seek " + alias + " to start";
        check_mci(seek_cmd, "音效回到开头失败：" + alias);

        std::string play_cmd = "play " + alias;
        check_mci(play_cmd, "播放音效失败：" + alias);
    }

    bool is_stopped(const std::string& alias) const
    {
        if (alias.empty())
            return true;

        char mode[128] = { 0 };

        std::string cmd = "status " + alias + " mode";
        MCIERROR err = mciSendStringA(cmd.c_str(), mode, 128, nullptr);

        if (err != 0)
            return false;

        std::string result = mode;

        return result == "stopped";
    }

    const std::string& get_current_bgm() const
    {
        return current_bgm;
    }

    void close_all()
    {
        stop_bgm();

        for (auto& pair : bgm_aliases)
            close_alias(pair.first);

        for (auto& pair : sfx_aliases)
            close_alias(pair.first);

        bgm_aliases.clear();
        sfx_aliases.clear();
        current_bgm.clear();
    }

private:
    void stop_current_bgm_only()
    {
        if (current_bgm.empty())
            return;

        std::string stop_cmd = "stop " + current_bgm;
        mciSendStringA(stop_cmd.c_str(), nullptr, 0, nullptr);
    }

    void close_alias(const std::string& alias)
    {
        std::string cmd = "close " + alias;
        mciSendStringA(cmd.c_str(), nullptr, 0, nullptr);
    }

    void check_mci(const std::string& cmd, const std::string& message) const
    {
        MCIERROR err = mciSendStringA(cmd.c_str(), nullptr, 0, nullptr);

        if (err != 0)
        {
            char error_text[256] = { 0 };
            mciGetErrorStringA(err, error_text, 256);

            std::cout << message << std::endl;
            std::cout << "MCI命令：" << cmd << std::endl;
            std::cout << "错误信息：" << error_text << std::endl;
        }
    }

private:
    std::string current_bgm;
    std::unordered_map<std::string, std::string> bgm_aliases;
    std::unordered_map<std::string, std::string> sfx_aliases;
};

#endif