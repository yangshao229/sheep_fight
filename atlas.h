#ifndef _ATLAS_H_
#define _ATLAS_H_

#include "util.h"
#include<iostream>

#include <vector>
#include <graphics.h>
using namespace std;

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

    void load_from_file(LPCTSTR path_template, int num)
    {
        img_list.clear();
        img_list.resize(num);

        TCHAR path_file[256];

        for (int i = 0; i < num; i++)
        {
            _stprintf_s(path_file, path_template, i + 1);

            loadimage(&img_list[i], path_file);

            if (img_list[i].getwidth() == 0 || img_list[i].getheight() == 0)
            {
                std::wcout << L"Í¼Æ¬¼ÓÔØÊ§°Ü£º"
                    << path_file
                    << L"  width="
                    << img_list[i].getwidth()
                    << L" height="
                    << img_list[i].getheight()
                    << std::endl;

                MessageBox(NULL, path_file, _T("Í¼Æ¬¼ÓÔØÊ§°Ü"), MB_OK);
            }
            else
            {
                std::wcout << L"Í¼Æ¬¼ÓÔØ³É¹¦£º"
                    << path_file
                    << L"  width="
                    << img_list[i].getwidth()
                    << L" height="
                    << img_list[i].getheight()
                    << std::endl;
            }
        }
    }

	void clear()
	{
		img_list.clear();
	}

	int get_size()
	{
		return (int)img_list.size();
	}

	IMAGE* get_image(int idx)
	{
		if (idx < 0 || idx >= img_list.size())
			return nullptr;

		return &img_list[idx];
	}

	void add_image(const IMAGE& img)
	{
		img_list.push_back(img);
	}

private:
	std::vector<IMAGE> img_list;
};

#endif // !_ATLAS_H_


