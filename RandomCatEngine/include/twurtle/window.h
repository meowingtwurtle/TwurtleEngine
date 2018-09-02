#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include <twurtle/init.h>
#include <twurtle/log.h>
#include <twurtle/window.h>

namespace randomcat::graphics {
    class window {
    public:
        explicit window(std::string _title = "Twurtle Engine", int _width = 600, int _height = 600);

        ~window();

        glm::ivec2 getSize() const { return m_size; }

        void setSize(glm::ivec2 _size) {
            m_size = std::move(_size);
            SDL_SetWindowSize(m_window, m_size.x, m_size.y);
        }

        std::string getTitle() const { return m_title; }

        void setTitle(std::string _title) {
            m_title = std::move(_title);
            SDL_SetWindowTitle(m_window, m_title.c_str());
        }

        void swapBuffers() { SDL_GL_SwapWindow(m_window); }

    private:
        SDL_Window* m_window;
        std::string m_title;
        glm::ivec2 m_size;

        friend void randomcat::graphics::setContext(window const&);
    };
}    // namespace randomcat::graphics
