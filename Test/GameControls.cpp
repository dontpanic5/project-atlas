#include "GameControls.h"
#include "ControllerMgr.h"

ButtonBinding	START	= ButtonBinding("START",	&ControllerMgr::A,					&ControllerMgr::SPACEBAR,	nullptr);
AxisBinding		MOVE_X	= AxisBinding("MOVE LEFT/RIGHT",		&ControllerMgr::LEFT_THUMBSTICK_X,	&ControllerMgr::LEFT_RIGHT,	nullptr);
AxisBinding		MOVE_Y	= AxisBinding("MOVE FORWARDS/BACKWARDS",		&ControllerMgr::LEFT_THUMBSTICK_Y,	&ControllerMgr::UP_DOWN, nullptr);
ButtonBinding	THROW	= ButtonBinding("THROW WORLD",	&ControllerMgr::A,					&ControllerMgr::SPACEBAR, nullptr);
ButtonBinding	RECALL = ButtonBinding("RECALL WORLD", &ControllerMgr::A, &ControllerMgr::SPACEBAR, nullptr);
ButtonBinding	RELOAD_LEVEL=ButtonBinding("RELOAD LEVEL",&ControllerMgr::B,						&ControllerMgr::ENTER,		nullptr);
ButtonBinding	TOGGLE_CAM=ButtonBinding("TOGGLE CAMERA",&ControllerMgr::X,				&ControllerMgr::F,			nullptr);