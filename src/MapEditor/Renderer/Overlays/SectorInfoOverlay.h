#pragma once

#include "OpenGL/Drawing.h"

class MapSector;

class SectorInfoOverlay
{
public:
	SectorInfoOverlay();
	~SectorInfoOverlay() = default;

	void update(MapSector* sector);
	void draw(int bottom, int right, float alpha = 1.0f);
	void drawTexture(float alpha, int x, int y, string_view texture, string_view pos = "Upper") const;

private:
	TextBox text_box_;
	string  ftex_;
	string  ctex_;
	int     last_size_ = 100;
};
