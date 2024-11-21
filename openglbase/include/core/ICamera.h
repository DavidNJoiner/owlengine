#ifndef ICAMERA_H
#define ICAMERA_H

class Vec3;
class Matrix;
class Quaternion;

class ICamera {
public:
    virtual ~ICamera() = default;
    virtual void SetPosition(const Vec3& position) = 0;
    virtual void SetRotation(const Quaternion& rotation) = 0;
    virtual Vec3 GetPosition() const = 0;
    virtual Quaternion GetRotation() const = 0;
    virtual Matrix GetViewMatrix() const = 0;
    virtual Matrix GetProjectionMatrix() const = 0;
};

#endif