#include "WickedEngine.h"
#include "config.h"
#include "icon.c"

#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    // Init SDL and create window:
    sdl2::sdlsystem_ptr_t system = sdl2::make_sdlsystem(SDL_INIT_EVERYTHING | SDL_INIT_EVENTS);
    sdl2::window_ptr_t window = sdl2::make_window(
        "Wicked Shooter",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1920, 1080,
        SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI
    );


    // Set window icon:
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
    SDL_Surface* icon = SDL_CreateRGBSurfaceFrom(
        (void*)gimp_image.pixel_data, gimp_image.width,
        gimp_image.height, gimp_image.bytes_per_pixel * 8, gimp_image.bytes_per_pixel * gimp_image.width,
        rmask, gmask, bmask, amask
    );
    SDL_SetWindowIcon(window.get(), icon);
    SDL_FreeSurface(icon);


    // Create and run Wicked Engine application:
    wi::Application application;
    application.SetWindow(window.get());

    wi::backlog::setFontColor(config::backlog_color); // customize backlog color for this app

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

        while (SDL_PollEvent(&event))
        {
            bool textinput_action_delete = false;

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
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    application.is_window_active = false;
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    application.is_window_active = true;
                    break;
                default:
                    break;
                }
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE
                    || event.key.keysym.scancode == SDL_SCANCODE_DELETE
                    || event.key.keysym.scancode == SDL_SCANCODE_KP_BACKSPACE) 
                {
                    wi::gui::TextInputField::DeleteFromInput();
                    textinput_action_delete = true;
                }
                break;
            case SDL_TEXTINPUT:
                if (!textinput_action_delete) 
                {
                    if (event.text.text[0] >= 21) {
                        wi::gui::TextInputField::AddInput(event.text.text[0]);
                    }
                }
                break;
            default:
                break;
            }
            wi::input::sdlinput::ProcessEvent(event);
        }
    }

    wi::jobsystem::ShutDown();

    SDL_Quit();

    return 0;
}
