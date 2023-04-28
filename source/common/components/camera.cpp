#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix just to make everything relative to the camera
    // As the view matrix only does translation and rotation ( there is no scaling as the camera doesn't need to scale something is has the same dimensions of the original object in the world space)
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        //Getting model matrix
        auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to the world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt
        glm::vec3 eye = M * glm::vec4(0, 0, 0, 1);
        glm::vec3 center = M * glm::vec4(0, 0, -1, 1);
        glm::vec3 up = M * glm::vec4(0, 1, 0, 1);
        // Creating the view matrix with the pre computed vectors
        return glm::lookAt(eye, center, up);
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    // As aspect ratio defines the difference between width and height as a ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: (Req 8) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
        // It takes fovY, aspect ratio, near and far
        // fov -> vertical field of view angle, something like zooming in and zooming out
        // as the camera doesn't change its position but the angle of view is increased or decreased like the difference between different creatures in seeing objects
        // Calculating aspect ratio as it's required for ortho and perspective projection types which is width/height
        // camera can't see anything nearer than (near) and farther than (far)
        float aspectRatio = float(viewportSize.x) / viewportSize.y;

        if( cameraType == CameraType::ORTHOGRAPHIC )
            return glm::ortho(-orthoHeight*aspectRatio/2 , orthoHeight*aspectRatio/2, -orthoHeight/2, orthoHeight/2);
        else
            return glm::perspective(fovY, aspectRatio, near, far);
    }

}