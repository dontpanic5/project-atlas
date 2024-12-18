#include "GameControls.h"
#include "ControllerMgr.h"

ButtonBinding	START	= ButtonBinding("START",	&ControllerMgr::A,					&ControllerMgr::SPACEBAR,	&ControllerMgr::LMB);
AxisBinding		MOVE_X	= AxisBinding("MOVE_X",		&ControllerMgr::LEFT_THUMBSTICK_X,	&ControllerMgr::LEFT_RIGHT,	nullptr);
AxisBinding		MOVE_Y	= AxisBinding("MOVE_Y",		&ControllerMgr::LEFT_THUMBSTICK_Y,	&ControllerMgr::UP_DOWN, nullptr);