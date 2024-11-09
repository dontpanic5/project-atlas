#include "ControllerMgr.h"
#include "Ui.h"
#include "GameControls.h"

static int finishScreen = 0;
static bool controllerPrompt = true;

void InitTitleScreen()
{
}

void UpdateTitleScreen()
{
	if (controllerPrompt && ControllerMgr::Instance().A.GetPressed())
	{
		ControllerMgr::Instance().SetCurControlDevice(CONTROL_DEVICE::CONTROLLER);
		controllerPrompt = false;
	}
    else if (controllerPrompt && ControllerMgr::Instance().SPACEBAR.GetPressed())
    {
        ControllerMgr::Instance().SetCurControlDevice(CONTROL_DEVICE::KEYBOARD);
        controllerPrompt = false;
    }
#ifndef PLATFORM_WEB
    else if (controllerPrompt && ControllerMgr::Instance().RMB.GetPressed())
    {
        ControllerMgr::Instance().SetCurControlDevice(CONTROL_DEVICE::MOUSE);
        controllerPrompt = false;
    }
#endif
    else if (START.GetPressed())
    {
        HideCursor();
        ControllerMgr::Instance().TrapCursor(true);
        finishScreen = 2;
    }
}

void DrawTitleScreen()
{
    if (controllerPrompt)
    {
        char msg[512];
        sprintf(
            msg,
            "PRESS %s TO PLAY ON CONTROLLER\n"
#ifndef PLATFORM_WEB
            "OR PRESS %s TO PLAY ON MOUSE\n"
#endif // !PLATFORM_WEB
            "OR PRESS %s TO PLAY ON KEYBOARD\n",
            ControllerMgr::Instance().A.GetName(),
#ifndef PLATFORM_WEB
            ControllerMgr::Instance().RMB.GetName(),
#endif // !PLATFORM_WEB
            ControllerMgr::Instance().SPACEBAR.GetName()
        );
        DrawUiText(msg, 0.2f, 0.7f, FontSize::s);
    }
    else
    {
        char msg[256];
        sprintf(
            msg,
            "PRESS %s TO START",
            START.GetControlName()
        );
        DrawUiText(msg, 0.2f, 0.7f, FontSize::m);
    }
}

void UnloadTitleScreen()
{

}

int FinishTitleScreen()
{
    return finishScreen;
}