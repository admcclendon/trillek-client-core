#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include "systems/dispatcher.hpp"
#include "systems/transform-system.hpp"
#include "transform.hpp"
#include <memory>
#include "os.hpp"

namespace trillek {
namespace graphics {

class CameraBase :
    public event::Subscriber<KeyboardEvent> {
public:
    /**
    * \brief Activates this camera with the given eneity ID.
    *
    * If you want to activate the camera with the same ID just pass in
    * CameraBase::GetEntityID();.
    * \param const unsigned int entity_id The ID of the entity whose transform we need to use.
    */
    void Activate(const unsigned int entity_id) {
        this->active = true;
        this->entity_id = entity_id;
        event::Dispatcher<KeyboardEvent>::GetInstance()->Subscribe(0, this);
        this->camera_transform = TransformMap::GetTransform(entity_id);
    }

    /**
    * \brief Deactivates this camera.
    *
    */
    void Deactivate() {
        this->active = false;
        event::Dispatcher<KeyboardEvent>::GetInstance()->Unsubscribe(this->entity_id, this);
        this->camera_transform = nullptr;
    }

    /**
    * \brief Returns if this camera is active.
    *
    */
    bool IsActive() {
        return this->active;
    }

    /**
    * \brief Get the current camera's entity ID.
    *
    */
    unsigned int GetEntityID() {
        return this->entity_id;
    }

    /**
    * \brief Compute and return the camera's view matrix.
    *
    * \return glm::mat4 The computed view matrix, or identity matrix if the camera's entity transform is null.
    */
    virtual glm::mat4 GetViewMatrix() = 0;

    /**
    * \brief Handles keyboard events.
    *
    * This is used to updated the camera's entity transform.
    */
    virtual void Notify(const unsigned int entity_id, const KeyboardEvent* key_event) = 0;
protected:
    std::shared_ptr<Transform> camera_transform;
    unsigned int entity_id;
    bool active;
};

} // End of graphics
} // End of trillek
#endif