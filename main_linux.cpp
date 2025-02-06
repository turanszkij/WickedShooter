#include "WickedEngine.h"
#include "config.h"

#include <SDL2/SDL.h>

wi::Application application;

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
            switch (event.type)
            {
                bool textinput_action_delete = false;

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
                    editor.is_window_active = false;
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    editor.is_window_active = true;
                    break;
                default:
                    break;
                }
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE
                    || event.key.keysym.scancode == SDL_SCANCODE_DELETE
                    || event.key.keysym.scancode == SDL_SCANCODE_KP_BACKSPACE) {
                    wi::gui::TextInputField::DeleteFromInput();
                    textinput_action_delete = true;
                }
                break;
            case SDL_TEXTINPUT:
                if (!textinput_action_delete) {
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
