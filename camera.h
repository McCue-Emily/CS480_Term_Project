#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void updateView(glm::vec3 cameraFront);
    void cameraPosDepth(float speed);
    void cameraPosHorz(float speed);
    void cameraPosVert(float speed);
    void zoom(float fov);

    void PlanetaryObsMode(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
  
  private:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
    glm::vec3 cameraFront = glm::vec3(0.0, 1.0, 1.0);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;

    //glm::vec3 resetCoords = glm::vec3(0.0, 1.0, 1.0);

};

#endif /* CAMERA_H */
