#include "stdafx.h"
#include <SDL2/SDL.h>

#include "icon.h"

wi::Application application;

int sdl_loop()
{
    SDL_Event event;

    bool quit = false;
    while (!quit)
    {
        SDL_PumpEvents();
        application.Run();

        static bool is_startup = false;
        if (!is_startup && wi::initializer::IsInitializeFinished())
        {
            wi::lua::RunFile(wi::helper::GetCurrentPath() + "/first_person.lua");
            is_startup = true;
        }

        while( SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case SDL_QUIT:      
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) 
                    {
                    case SDL_WINDOWEVENT_CLOSE:
                        quit = true;
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        application.SetWindow(application.window);
                        break;
                    default:
                        break;
                    }
                default:
                    break;
            }
        }
    }

    return 0;

}

void set_window_icon(SDL_Window* window) {
    // these masks are needed to tell SDL_CreateRGBSurface(From)
    // to assume the data it gets is byte-wise RGB(A) data
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = (gimp_image.bytes_per_pixel == 3) ? 8 : 0;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = (gimp_image.bytes_per_pixel == 3) ? 0 : 0xff000000;
#endif
    SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*)gimp_image.pixel_data, gimp_image.width,
        gimp_image.height, gimp_image.bytes_per_pixel * 8, gimp_image.bytes_per_pixel * gimp_image.width,
        rmask, gmask, bmask, amask);

    SDL_SetWindowIcon(window, icon);

    SDL_FreeSurface(icon);
}

int main(int argc, char *argv[])
{
    sdl2::sdlsystem_ptr_t system = sdl2::make_sdlsystem(SDL_INIT_EVERYTHING | SDL_INIT_EVENTS);
    sdl2::window_ptr_t window = sdl2::make_window(
            "Wicked Shooter",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            1920, 1080,
            SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    set_window_icon(window.get());

    SDL_Event event;

    application.SetWindow(window.get());

    wi::backlog::setFontColor(wi::Color(130, 250, 180, 255));

    int ret = sdl_loop();

    SDL_Quit();

    return ret;
}
