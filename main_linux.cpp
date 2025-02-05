#include "WickedEngine.h"
#include "config.h"

#include <SDL2/SDL.h>

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
            wi::lua::RunFile(wi::helper::GetCurrentPath() + "/" + std::string(config::main_script_file));
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

int main(int argc, char *argv[])
{
    sdl2::sdlsystem_ptr_t system = sdl2::make_sdlsystem(SDL_INIT_EVERYTHING | SDL_INIT_EVENTS);
    sdl2::window_ptr_t window = sdl2::make_window(
            "Wicked Shooter",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            1920, 1080,
            SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Event event;

    application.SetWindow(window.get());

    wi::backlog::setFontColor(config::backlog_color);

    int ret = sdl_loop();

    SDL_Quit();

    return ret;
}
