#pragma once

enum KEYSTATE
{
	STATE_DOWN,
	STATE_PUSH,
	STATE_UP,
	STATE_END
};

enum SCENESTATE
{
	SCENE_TEST,
	SCENE_END
};

enum MODELTYPE
{
	MODEL_STD,
	MODEL_ANI,
	MODEL_ACE,
	MODEL_COL,
	MODEL_END
};

enum OBJTYPE
{
	OBJ_BACK,
	OBJ_MAP,
	OBJ_PLAYER,
	OBJ_ENEMY,
	OBJ_EFFECT,
	OBJ_MISSLE,
	OBJ_MINIMAP_PLAYER,
	OBJ_MINIMAP_ENEMY,
	OBJ_UI,
	OBJ_TEST,
	OBJ_END
};

enum RENDERTYPE
{
	RENDER_BACKGROUND,
	RENDER_MAP,
	RENDER_OBJ,
	RENDER_EFFECT,
	RENDER_UI,
	RENDER_UI_1,
	RENDER_UI_2,
	RENDER_UI_3,
	RENDER_END
};