#include "camera.h"

Camera::Camera()
{

}

Camera::Camera(Camera& sample) {
    cameraPos = sample.cameraPos;
    cameraFront = sample.cameraFront;
    cameraUp = sample.cameraUp;
    x = sample.x;
    y = sample.y;
    z = sample.z;
    projection = sample.projection;
    view = sample.view;
}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  //view = glm::lookAt( glm::vec3(x, y, z), //Eye Position
  //                    glm::vec3(0.0, 0.0, 0.0), //Focus point
  //                    glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
  
  view = glm::lookAt(cameraPos, cameraFront + cameraPos , cameraUp);

  projection = glm::perspective( glm::radians(40.f), //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::SetProjection(glm::mat4 p)
{
    projection = p;
}

void Camera::SetView(glm::mat4 v)
{
    view = v;
}

glm::vec3 Camera::GetPos() {
    return cameraPos;
}

glm::vec3 Camera::GetFront() {
    return cameraFront;
}

glm::vec3 Camera::GetUp() {
    return cameraUp;
}
void Camera::SetPos(glm::vec3 p) {
    cameraPos = p;
}
void Camera::SetFront(glm::vec3 f) {
    cameraPos = f;
}

void Camera::SetUp(glm::vec3 u) {
    cameraPos = u;
}

double Camera::Getx() {
    return x;
}

double Camera::Gety() {
    return y;
}

double Camera::Getz() {
    return z;
}

void Camera::Setx(double n) {
    x = n;
}
void Camera::Sety(double n) {
    y = n;
}
void Camera::Setz(double n) {
    z = n;
}

void Camera::updateView(glm::vec3 cameraUpdate) {
    cameraFront = cameraUpdate;
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::cameraPosVert(float speed) {
    cameraPos += speed * cameraFront;
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::cameraPosHorz(float speed) {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}


void Camera::zoom(float fov) {
    projection = glm::perspective(glm::radians(fov), //the FoV typically 90 degrees is good which is what this is set to
        float(800) / float(600), //Aspect Ratio, so Circles stay Circular
        0.01f, //Distance to the near plane, normally a small value like this
        100.0f); //Distance to the far plane, 

}

void Camera::PlanetaryObsMode(glm::vec3 pos, glm::vec3 front, glm::vec3 up) {
    view = glm::lookAt(pos, front, up);
}

void Camera::PlanetaryObsMode(glm::vec3 pos) {  // todo make camera look at planet
    cameraPos = pos;
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::Rotate(float angle, glm::vec3 axes) {
    cameraFront = glm::rotate(cameraFront, angle, axes);
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}
