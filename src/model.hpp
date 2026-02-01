#include <tiny_obj_loader.h>

const std::string MODEL_PATH = "models/viking_room.obj";
const std::string TEXTURE_PATH = "textures/viking_room.png";

struct model {
    void loadModel();
};

extern model Model;