#include "Minimap.h"
#include "Application.h"
#include "TextureManager.h"
#include "Render.h"
#include "Scene.h"
#include "Window.h"
#include "Editor.h"
#include "Map.h"
#include "Input.h"
#include "Log.h"
#include "JuicyMath.h"
#include "Transform.h"

#include <string>

Minimap::Minimap(Gameobject* go) :
	C_Image(go)
{
	LOG("Starting minimap");

	ally = { 0, 0, 0, 0 };
	enemy = { 0, 0, 0, 0 };
	building = { 0, 0, 0, 0 };

	minimap_camera = { 0, 0, 4, 4 };
	camera_color = { 255, 255, 255, 255 };

	App->win->GetWindowSize(window_width, window_height);
}

Minimap::~Minimap()
{
}

void Minimap::Update()
{
	SDL_Rect camera_getter = App->render->GetCameraRect();

	App->render->DrawQuad(output, { 255, 0, 0, 255 }, false, EDITOR, false);

	//From map to minimap viewport
	float tile_width, tile_height;
	map.GetTileSize_F(tile_width, tile_height);

	float map_width, map_height;
	map.GetMapSize(map_width, map_height);

	float map_pixelwidth = map_width * tile_width;
	float map_pixelheight = map_height * tile_height;

	float scale_x, scale_y;
	scale_x = output.w / map_pixelwidth;
	scale_y = output.h / map_pixelheight;

	minimap_camera.x = camera_getter.x;
	minimap_camera.y = camera_getter.y;
	minimap_camera.w = camera_getter.w * scale_x;
	minimap_camera.h = camera_getter.h * scale_y;
				 
	minimap_camera.x = minimap_camera.x * scale_x;
	minimap_camera.y = minimap_camera.y * scale_y;

	minimap_camera.x += output.x + output.w / 2;
	minimap_camera.y += output.y;

	App->render->DrawQuad(minimap_camera, camera_color, false, EDITOR, false);
//-------------------------------------------------------------------------------------
	
	//From minimap to map drag viewport
	if (App->input->GetMouseButtonDown(0) == KeyState::KEY_REPEAT)
	{
		int x, y;
		App->input->GetMousePosition(x, y);

		if (mouse_inside = JMath::PointInsideRect(x, y, output))
		{
			x -= output.x;
			y -= output.y;

			App->render->cam.x = (x / scale_x) - map_pixelwidth / 2 - App->render->cam.w / 2;
			App->render->cam.y = y / scale_y - App->render->cam.h / 2;
		}
	}
//-------------------------------------------------------------------------------------

	for (std::list<Gameobject*>::iterator it = object_queue.begin(); it != object_queue.end(); it++)
	{
		SDL_Rect representation;
		vec unit_pos = (*it)->GetTransform()->GetGlobalPosition();
		std::pair<float,float> world_pos= map.F_MapToWorld(unit_pos);

		representation.x = scale_x * world_pos.first - 1;
		representation.y = scale_y * world_pos.second - 1;
		representation.w = 2;
		representation.h = 2;

		representation.x += output.x + output.w / 2;
		representation.y += output.y;

		App->render->DrawQuad(representation, { 255, 255, 255, 255 }, true, EDITOR, false);
		//App->render->BlitNorm(0, representation, NULL, EDITOR);
	}
}

void Minimap::AddToMinimap(Gameobject* object)
{
	object_queue.push_back(object);
}

void Minimap::GetMinimap()
{
	ComputeOutputRect(float(section.w), float(section.h));
	tex_id = App->render->GetMinimap(output.w, output.h, 0.05f, { output.w / 2, 0, 12, 12 });
	Event::Push(UPDATE_MINIMAP_TEXTURE, App->render);
}
