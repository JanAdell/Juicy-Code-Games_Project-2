#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "Canvas.h"
#include "SDL/include/SDL_rect.h"
#include <map>

class Transform;
class Sprite;

class Minimap : public UI_Component
{
public:

	Minimap(Gameobject* go);
	~Minimap();

	void PostUpdate() override;

	static bool AddUnit(double id, int type, Transform* unit);
	static void RemoveUnit(double id);

private:

	static Minimap* minimap;

	Sprite* back;
	int minimap_texture;
	int hud_texture;
	float map_scale;
	bool mouse_moving;
	std::pair<int, int> map_size;
	std::pair<int, int> tile_size;
	std::pair<int, int> total_size;

	enum MinimapTexture : int
	{
		MINIMAP,
		BACKGROUND,
		ICON_ALLIED_UNIT,
		ICON_ENEMY_UNIT,
		ICON_BASE_CENTER,
		ICON_TOWER,
		ICON_BARRACKS,
		ICON_EDGE,
		ICON_SPAWNER,
		MAX_MINIMAP_TEXTURES
	};
	SDL_Rect sections[MAX_MINIMAP_TEXTURES];

	std::map<double, std::pair<MinimapTexture,Transform*>> units;
};

#endif // !__MINIMAP_H__