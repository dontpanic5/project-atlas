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
/* no mouse movement for this game
#ifndef PLATFORM_WEB
    else if (controllerPrompt && ControllerMgr::Instance().RMB.GetPressed())
    {
        ControllerMgr::Instance().SetCurControlDevice(CONTROL_DEVICE::MOUSE);
        controllerPrompt = false;
    }
#endif
*/
    else if (START.GetPressed())
    {
        HideCursor();
        ControllerMgr::Instance().TrapCursor(true);
        finishScreen = 2;
    }

    // TODO
    /*if (TOGGLE_MUSIC.GetPressed())
    {
        music
    }*/
}

void DrawTitleScreen()
{
    if (controllerPrompt)
    {
        char msg[512];
        sprintf(
            msg,
            "PRESS %s TO PLAY ON CONTROLLER\n"
/* no mouse movement for this game
#ifndef PLATFORM_WEB
            "OR PRESS %s TO PLAY ON MOUSE\n"
#endif // !PLATFORM_WEB
*/
            "OR PRESS %s TO PLAY ON KEYBOARD\n",
            ControllerMgr::Instance().A.GetName(),
/* no mouse movement for this game
#ifndef PLATFORM_WEB
            ControllerMgr::Instance().RMB.GetName(),
#endif // !PLATFORM_WEB*/
            ControllerMgr::Instance().SPACEBAR.GetName()
        );
        DrawUiText(msg, 0.2f, 0.7f, FontSize::s);

        DrawUiText("ATLAS HUGGED", 0.1f, 0.1f, FontSize::l);
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

        const char controlFormat[] = "%s WITH %s\n";
        char format[512];
        memset(format, 0, sizeof(format));
        strncat(format, controlFormat, _batty_countof(format) - strlen(format) - 1);
        strncat(format, controlFormat, _batty_countof(format) - strlen(format) - 1);
        strncat(format, controlFormat, _batty_countof(format) - strlen(format) - 1);
        strncat(format, controlFormat, _batty_countof(format) - strlen(format) - 1);
        strncat(format, controlFormat, _batty_countof(format) - strlen(format) - 1);
        strncat(format, controlFormat, _batty_countof(format) - strlen(format) - 1);
        //strncat(format, controlFormat, _batty_countof(format) - strlen(format) - 1);
        sprintf(
            msg,
            format,
            MOVE_X.GetName(), MOVE_X.GetControlName(),
            MOVE_Y.GetName(), MOVE_Y.GetControlName(),
            THROW.GetName(), THROW.GetControlName(),
            RECALL.GetName(), RECALL.GetControlName(),
            TOGGLE_CAM.GetName(), TOGGLE_CAM.GetControlName(),
            RELOAD_LEVEL.GetName(), RELOAD_LEVEL.GetControlName()/*,
            TOGGLE_MUSIC.GetName(), TOGGLE_MUSIC.GetControlName()*/
        );
        DrawUiText(msg, 0.2f, 0.2f, FontSize::s);
    }
}

void UnloadTitleScreen()
{

}

int FinishTitleScreen()
{
    return finishScreen;
}