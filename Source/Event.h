#ifndef __EVENT_H__
#define __EVENT_H__

#include "Cvar.h"
#include <queue>
#include <vector>

class EventListener;

enum EventType : char
{
	// APP
	REQUEST_DEFAULT_CONF,
	REQUEST_LOAD,
	REQUEST_SAVE,
	REQUEST_QUIT,

	// Window
	WINDOW_SHOW,
	WINDOW_HIDDEN,
	WINDOW_EXPOSED,
	WINDOW_MOVED,
	WINDOW_SIZE_CHANGED,
	WINDOW_MINIMIZED,
	WINDOW_MAXIMIZED,
	WINDOW_RESTORED,
	WINDOW_ENTER,
	WINDOW_LEAVE,
	WINDOW_FOCUS_GAINED,
	WINDOW_FOCUS_LEAVE,
	WINDOW_QUIT,

	// Audio
	PLAY_FX,

	// Scene
	SCENE_PLAY,
	SCENE_PAUSE,
	SCENE_TICK,
	SCENE_STOP,
	SCENE_CHANGE,
	PLACE_BUILDING,
	UPDATE_STAT,
	GAMEPLAY,
	SPAWN_UNIT,

	//// Scene states
	//NEXT_STATE,
	
	// Gameobjects
	ON_PLAY,
	ON_PAUSE,
	ON_STOP,
	ON_SELECT,
	ON_UNSELECT,
	ON_DESTROY,
	ON_RIGHT_CLICK,
	TRANSFORM_MODIFIED,
	PARENT_TRANSFORM_MODIFIED,

	// UI
	HOVER_IN,
	HOVER_OUT,
	MOUSE_DOWN,
	MOUSE_REPEAT,
	MOUSE_UP,

	// Renderer
	SET_VSYNC,
	CAMERA_MOVED,
	UPDATE_MINIMAP_TEXTURE,

	//Behaviour
	DAMAGE,
	IMPULSE,
	BUILD_GATHERER,
	BUILD_MELEE,
	BUILD_RANGED,
	BUILD_SUPER,
	BUILD_TOWER,
	BUILD_CENTER,
	BUILD_WALL,
	BUILD_LAB,
	BUILD_BARRACKS,
	DO_UPGRADE,
	UPDATE_PATH,

	MAX_EVENT_TYPES
};

class Event
{
public:

	Event(EventType t, EventListener* lis, Cvar data = Cvar(), Cvar data2 = Cvar());
	Event(const Event& e);
	virtual ~Event();

	static void Push(EventType t, EventListener* lis, Cvar data = Cvar(), Cvar data2 = Cvar());
	static void Push(EventType t, std::vector<EventListener*>& lis, Cvar data = Cvar(), Cvar data2 = Cvar());
	static void Push(const Event e);
	static void PumpAll();
	static void Pump();
	static unsigned int RemainingEvents();

	static void ResumeEvents();
	static void PauseEvents();
	static bool isPaused();

private:

	void CallListener() const;
	bool IsValid() const;
	void Clear();

public:

	EventType type;
	Cvar data1;
	Cvar data2;
	const unsigned int timestamp;
	EventListener* listener = nullptr;

private:

	static bool paused;
	static std::queue<Event> events_queue;
};

#endif // __EVENT_H__
