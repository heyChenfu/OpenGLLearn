#include "Camera.h"

static Camera* camera = NULL;

Camera* Camera::getInstence()
{
    if (camera == nullptr) {
        camera = new Camera();
    }
    return camera;
}