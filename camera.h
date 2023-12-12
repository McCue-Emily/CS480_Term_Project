#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    void Update(int dx, int dy, int dz, double drx, double dry, double drz);
    void Update(glm::mat4 dview);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

  
  private:

    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    double rx = 0.0;
    double ry = 0.0;
    double rz = 0.0;

    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
