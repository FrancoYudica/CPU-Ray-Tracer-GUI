#ifndef __CAMERA_CONTROLLER__
#define __CAMERA_CONTROLLER__
#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include "WEngine.h"

using namespace RT;

class CameraController {
public:
    CameraController()
        : _eye(Vec3f(0.0))
        , _look_at(Vec3f(0.0, 0.0, -1.0))
        , _speed(50.0f)
        , _up(Vec3f(0, 1, 0))
        , _sensibility(Vec2f(1.0f))
    {
        _recalculate_angles();
    }

    bool on_update(const glm::vec2& mouse_delta, const Wolf::Time& dt);

    inline Vec3f get_eye() const { return _eye; }
    inline Vec3f get_look_at() const { return _look_at; }
    inline Vec3f get_up() const { return _up; }
    inline float get_speed() const { return _speed; }
    inline Vec2f get_sensibility() const { return _sensibility; }

    void set_eye(const Vec3f& eye)
    {
        _eye = eye;
        _recalculate_angles();
    }
    void set_look_at(const Vec3f& look_at)
    {
        _look_at = look_at;
        _recalculate_angles();
    }
    void set_up(const Vec3f& up) { _up = up; }
    void set_speed(const float& speed) { _speed = speed; }
    void set_sensibility(const Vec2f& s) { _sensibility = s; }

private:
    /// @brief Recalculates yaw and pitch based on eye and look_at
    void _recalculate_angles();

private:
    Vec3f _eye;
    Vec3f _look_at;
    Vec3f _up;
    float _speed;
    Vec2f _sensibility;
    Vec2f _previous_mouse_pos;
    float _yaw;
    float _pitch;
};

#endif