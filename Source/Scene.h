#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Map.h"
#include "Gameobject.h"
#include "Point.h"

#include <vector>

enum SceneType : int
{
	TEST,
	INTRO,
	MENU,
	MAIN,
	MAIN_FROM_SAFE,
	END,
	CREDITS
};

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();
	~Scene();

	bool PreUpdate() override;
	bool Update() override;
	bool PostUpdate() override;
	bool CleanUp() override;

	void RecieveEvent(const Event& e) override;

	bool LoadTestScene();
	bool LoadIntroScene();
	bool LoadMenuScene();
	bool LoadMainScene();
	bool ChangeToScene(SceneType scene);

	Gameobject* GetRoot();
	const Gameobject* GetRoot() const;
	Gameobject* AddGameobject(const char* name, Gameobject* parent = nullptr);
	Gameobject* MouseClickSelect(int mouse_x, int mouse_y);

private:

	void GodMode();

public:

	int id_mouse_tex;

	// Selection
	iPoint groupStart,mouseExtend;
	bool groupSelect;
	std::vector<Gameobject*> group;
	
	// HUD
	Gameobject* hud_canvas_go;
	int building_bars_created = 0;
	int unit_bars_created = 0;

private:

	Map map;
	Gameobject root;
	bool god_mode = true;

	// Scene Transitions
	enum Fade : int
	{
		NO_FADE = 0,
		FADE_OUT,
		FADE_IN
	} fading = NO_FADE;
	float fade_timer;
	float fade_duration;
	SceneType current_scene;
	SceneType next_scene;
	/*bool shoot = false;//temp
	std::pair<int, int> pos;
	std::pair<int, int> atkPos;
	float rayCastTimer = 0;*/
	// Player stats
	int resources = 100;
};

#endif // __SCENE_H__