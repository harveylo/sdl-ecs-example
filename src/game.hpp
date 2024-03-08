#pragma once

#include <any>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <entt/entt.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


namespace cwt {

// ! three components
// * sprite_component for texture and image
struct sprite_component{
    // SDL_Rect holds four elemets, x, y, w, h
    // for the source rectangle, it describes which part of the texture image will be used to
    // render the sprite
    // in other words, the coordinates and size of the texture image to be used
    SDL_Rect src;
    // for the destination rectangle, it describes where the sprite will be rendered on the screen
    // in other words, the screen coordinates and the size of the sprite to be put
    SDL_Rect dst;
    SDL_Texture* texture;
};

// * transform_component for position and velocity
struct transform_component{
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
};

// * 
struct keyinputs_component{ };




struct sprite_system 
{
    // tick every 
    void update(entt::registry& reg)
    {
        auto view = reg.view<sprite_component, transform_component>();
        view.each([](auto &s, auto &t){
                s.dst.x = t.pos_x;
                s.dst.y = t.pos_y;
        });
    }
    void render(entt::registry& reg, SDL_Renderer* renderer)
    {
        auto view = reg.view<sprite_component>();
        view.each([renderer](auto &s){
             SDL_RenderCopy(
                    renderer, 
                    s.texture, 
                    &s.src, 
                    &s.dst
                );
        });
    }
};

struct transform_system 
{
    float dt = 0.1f;
    
    void update(entt::registry& reg)
    {
        auto view = reg.view<transform_component>();
        view.each([](auto &t){
            t.pos_x += t.vel_x*0.1f;
            t.pos_y += t.vel_y*0.1f;            
        });
    }
};

struct movement_system 
{  
    void update(entt::registry& reg)
    {
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        auto view = reg.view<transform_component, keyinputs_component>();
        view.each([&keys](auto &t){
            if (keys[SDL_SCANCODE_A]) { t.vel_x = -1.0f; } 
            if (keys[SDL_SCANCODE_S]) { t.vel_y = 1.0f; }
            if (keys[SDL_SCANCODE_W]) { t.vel_y = -1.0f; }
            if (keys[SDL_SCANCODE_D]) { t.vel_x = 1.0f; }
            if (!keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) { t.vel_x = 0.0f; }
            if (!keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_W]) { t.vel_y = 0.0f; }       
        });
    }
};


class game
{
    public: 
        game(std::size_t width, std::size_t height) : m_width(width), m_height(height)
        {
            m_window = SDL_CreateWindow(
                "sdl window",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                m_width,
                m_height,
                NULL
            );

            if (m_window == NULL) {
                std::cout << "Could not create window: " << SDL_GetError() << '\n';
                m_is_running = false;
            }
            m_renderer = SDL_CreateRenderer(m_window, -1, 0);
            if (!m_renderer) {
                std::cout << "Error creating SDL renderer.\n";
                m_is_running = false;
            }

            m_is_running = true;
        }
        ~game()
        {       
            SDL_DestroyWindow(m_window);
            SDL_Quit();
        }

        entt::registry& get_registry() { return m_registry; }
        SDL_Renderer* get_renderer() { return m_renderer; }

        bool is_running()
        {
            return m_is_running;
        }
        void read_input()
        {
            SDL_Event sdl_event;
            SDL_PollEvent(&sdl_event);            
            const Uint8* keystates = SDL_GetKeyboardState(NULL);

            if (keystates[SDL_SCANCODE_ESCAPE] || sdl_event.type == SDL_QUIT) {
                m_is_running = false;
            }
        }
        void update()
        {
            m_transform_system.update(m_registry);
            m_movement_system.update(m_registry);
            m_sprite_system.update(m_registry);
        }
        void render()
        {
            SDL_RenderClear(m_renderer);

            m_sprite_system.render(m_registry, m_renderer);

            SDL_RenderPresent(m_renderer);
        }

    private:
        std::size_t m_width;
        std::size_t m_height;
        SDL_Window* m_window; 
        SDL_Renderer* m_renderer;
        bool m_is_running;

        entt::registry m_registry;

        sprite_system m_sprite_system;
        transform_system m_transform_system;
        movement_system m_movement_system;
};


} // namespace cwt

