#include "CameraController.hpp"

#define HALF_PIf 3.14159265358979f * 0.5f

bool CameraController::on_update(const glm::vec2& mouse_delta, const Wolf::Time& dt)
{

    bool moved = false;
    _yaw += _sensibility.x * mouse_delta.x;
    _pitch += _sensibility.y * mouse_delta.y;
    // Camera rotation
    if (mouse_delta.x != 0.0f || mouse_delta.y != 0.0f) {
        moved = true;

        constexpr float eps = 0.001f;
        if (_pitch > HALF_PIf - eps)
            _pitch = HALF_PIf - eps;
        if (_pitch < -(HALF_PIf - eps))
            _pitch = -(HALF_PIf - eps);

        Vec3f forward_dir;
        forward_dir.x = cos(_yaw) * cos(_pitch);
        forward_dir.y = sin(_pitch);
        forward_dir.z = sin(_yaw) * cos(_pitch);
        _look_at = _eye + forward_dir;
    }

    // Camera translation
    Vec3f direction = glm::normalize(_look_at - _eye);
    float frame_speed = dt.seconds() * _speed;

    Vec3f left = normalize(cross(_up, direction));

    Vec3f offset(0.0f);
    if (Wolf::Input::is_key_down(KeyCode::KEY_W)) {
        offset += direction * frame_speed;
        moved = true;
    }

    if (Wolf::Input::is_key_down(KeyCode::KEY_S)) {
        offset -= direction * frame_speed;
        moved = true;
    }

    if (Wolf::Input::is_key_down(KeyCode::KEY_D)) {
        offset -= left * frame_speed;
        moved = true;
    }

    if (Wolf::Input::is_key_down(KeyCode::KEY_A)) {
        offset += left * frame_speed;
        moved = true;
    }

    if (Wolf::Input::is_key_down(KeyCode::KEY_Q)) {
        offset += _up * frame_speed;
        moved = true;
    }

    if (Wolf::Input::is_key_down(KeyCode::KEY_E)) {
        offset -= _up * frame_speed;
        moved = true;
    }

    _eye += offset;
    _look_at += offset;
    return moved;
}

void CameraController::_recalculate_angles()
{
    glm::vec3 direction = glm::normalize(_look_at - _eye);
    _yaw = atan2f(direction.z, direction.x);
    _pitch = asinf(direction.y);
}