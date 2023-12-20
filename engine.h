#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include "window.h"
#include "graphics.h"

static void cursorPositionCallBack(GLFWwindow*, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

class Engine
{
  public:
    Engine(const char*  name, int width, int height);
    
    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    
  
  private:
    // Window related variables
    Window *m_window;    
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    bool observation_mode = 0;
    bool key_hold = 0;


    Graphics* m_graphics;
    Camera* m_planetObs;

    //glm::vec3 resetCoords = glm::vec3(0.0, 1.0, 1.0);


    bool m_running;
};

#endif // ENGINE_H
