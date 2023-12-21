

#include "engine.h"
#include "glm/ext.hpp"

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

Engine::Engine(const char* name, int width, int height)
{
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
    delete m_window;
    delete m_graphics;
    m_window = NULL;
    m_graphics = NULL;
}

bool Engine::Initialize()
{

    // Start a window
    m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
  if(!m_window->Initialize())
    {
        printf("The window failed to initialize.\n");
        return false;
    }

    // Start the graphics
    m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
    {
        printf("The graphics failed to initialize.\n");
        return false;
    }

    glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);
    glfwSetScrollCallback(m_window->getWindow(), scroll_callback);

  // Check that raw mouse motion works and enable it
  if (glfwRawMouseMotionSupported()) {
      glfwSetInputMode(m_window->getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }
  else {
      printf("This system does not support raw mouse motion.\n");
  }

  // make the cursor invisible
  glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); // cursor must be disabled for raw mouse motion

    // No errors
    return true;
}

void Engine::Run()
{
    m_running = true;

    while (!glfwWindowShouldClose(m_window->getWindow()))
    {
        ProcessInput();
        Display(m_window->getWindow(), glfwGetTime());
        glfwPollEvents();

    }

    m_running = false;

}

void Engine::ProcessInput()
{
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);

    double xpos;
    double ypos;

    // Else continue
    glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(m_window->getWindow(), &xpos, &ypos);
 
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;
   
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    glm::vec3 cameraFront = glm::normalize(front);

    m_graphics->getCamera()->updateView(cameraFront);
    m_graphics->getCamera()->zoom(fov);

    if (!observation_mode) {
        // if in exploration mode

        //  If 'p' is selected, enter planetary observation mode on the next loop in Run()
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_PRESS) {
            if (!key_hold) {
                observation_mode = !observation_mode;
            }
            key_hold = 1;
        }
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_RELEASE) {
            key_hold = 0;
        }


        //if (glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_PRESS)
        //    observation_mode = !observation_mode;

        // Move right
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            m_graphics->getCamera()->cameraPosHorz(0.5f);
        // Move left
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            m_graphics->getCamera()->cameraPosHorz(-0.5f);
        // Zoom in
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            m_graphics->getCamera()->cameraPosVert(0.5f);
        // Zoom out
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            m_graphics->getCamera()->cameraPosVert(-0.5f);
    }
    else {
        // if in observation mode

        //  If 'p' is selected, enter planetary observation mode on the next loop in Run()
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_PRESS) {
            if (!key_hold) {
                observation_mode = !observation_mode;
            }
            key_hold = 1;
        }
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_RELEASE) {
            key_hold = 0;
        }

        // Move right
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            m_graphics->getCamera()->Rotate(1.0f, glm::vec3(0, 1, 0));  // the vec specifies that we will rotate in the y axis
        // Move left
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            m_graphics->getCamera()->Rotate(-1.0f / 180.0f, glm::vec3(0, 1, 0));
        // Zoom in
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            m_graphics->getCamera()->cameraPosVert(0.5f);
        // Zoom out
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            m_graphics->getCamera()->cameraPosVert(-0.5f);
    }
        

    m_graphics->set_mode(observation_mode);
}

void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
 
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Position: (" << xpos << ":" << ypos << ")";
}

unsigned int Engine::getDT()
{
  //long long TimeNowMillis = GetCurrentTimeMillis();
  //assert(TimeNowMillis >= m_currentTimeMillis);
  //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  //m_currentTimeMillis = TimeNowMillis;
  //return DeltaTimeMillis;
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
  //timeval t;
  //gettimeofday(&t, NULL);
  //long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  //return ret;
    return (long long) glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();
    m_graphics->HierarchicalUpdate2(time);
}

static void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) {
    //cout << xpos << " " << ypos << endl;
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset * 2.0;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}