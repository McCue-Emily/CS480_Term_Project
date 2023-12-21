#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    Camera(Camera& sample);
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    glm::vec3 GetPos();
    glm::vec3 GetFront();
    glm::vec3 GetUp();
    void SetProjection(glm::mat4 p);
    void SetView(glm::mat4 p);
    void SetPos(glm::vec3 p);
    void SetFront(glm::vec3 f);
    void SetUp(glm::vec3 u);

    void updateView(glm::vec3 cameraFront);
    void cameraPosDepth(float speed);
    void cameraPosHorz(float speed);
    void cameraPosVert(float speed);
    void zoom(float fov);

    void PlanetaryObsMode(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
    void PlanetaryObsMode(glm::vec3 pos);
    void Rotate(float angle, glm::vec3 axes);
  
  private:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
    glm::vec3 cameraFront = glm::vec3(0.0, 1.0, 1.0);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
