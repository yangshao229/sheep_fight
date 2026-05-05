#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#include"atlas.h"
#include"util.h"
#include"camera.h"
#include<functional>
#include <vector>
#include <graphics.h>
class Animation
{
public:

	Animation() = default;
	~Animation() = default;
	void reset()
	{
		timer = 0;
		idx_frame = 0;
	}
	void set_atlas(Atlas* new_atlas)
	{
		reset();
		atlas = new_atlas;
        frame_intervals.clear();
	}
	void set_loop(bool flag)
	{
		is_loop = flag;

	}
	void set_interval(int ms)
	{
		interval = ms;
	}
    void set_frame_intervals(const int* intervals, int count)
    {
        frame_intervals.clear();

        if (intervals == nullptr || count <= 0)
            return;

        for (int i = 0; i < count; i++)
            frame_intervals.push_back(intervals[i]);
    }
	int get_idx_frame()
	{
		return idx_frame;
	}
	IMAGE* get_frame()
	{
		return atlas->get_image(idx_frame);
	}
	bool check_finished()
	{
		if (is_loop) return false;
		return (idx_frame == atlas->get_size() - 1);
	}
	void on_update(int delta)
	{
        if (atlas == nullptr || atlas->get_size() <= 0)
            return;

		timer += delta;

        int current_interval = get_current_interval();

		while (timer >= current_interval)
		{
            timer -= current_interval;
			idx_frame++;
			if (idx_frame >= atlas->get_size())
			{
				idx_frame = is_loop ? 0 : atlas->get_size() - 1;
				if (!is_loop && callback)
					callback();

                if (!is_loop)
                {
                    timer = 0;
                    break;
                }
			}

            current_interval = get_current_interval();
		}
	}
	void on_draw(const Camera& camera,int x, int y) const
	{
		if (atlas == nullptr)
		{
			std::cout << "atlasÎª¿Õ£¡" << std::endl;
			return;
		}
		putimage_alpha(camera,x, y, atlas->get_image(idx_frame));
	}
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
private:
    int get_current_interval() const
    {
        if (idx_frame >= 0 &&
            idx_frame < (int)frame_intervals.size() &&
            frame_intervals[idx_frame] > 0)
        {
            return frame_intervals[idx_frame];
        }

        if (interval > 0)
            return interval;

        return 1;
    }

private:
	int timer = 0;
	int interval = 0;
	int idx_frame = 0;
	bool is_loop = true;
    std::vector<int> frame_intervals;
	Atlas* atlas = nullptr;
	std::function<void()> callback;
};
#endif // !_ANIMATION_H_

