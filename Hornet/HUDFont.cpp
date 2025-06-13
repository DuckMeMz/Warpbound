#include "HUDFont.h"

FontIndex HUDFont::Font = FontIndex(-1);
FontIndex HUDFont::MonoFont = FontIndex(-1);


void HUDFont::Initialise()
{
    Font = HtGraphics::instance.LoadFont("assets/fonts/HUDFont.ttf", 48);
    MonoFont = HtGraphics::instance.LoadFont("assets/fonts/MonoHUDFont.ttf", 48);
}