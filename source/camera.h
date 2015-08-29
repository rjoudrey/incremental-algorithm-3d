#ifndef CAM_UTILITY_H
#define CAM_UTILITY_H

class Vec3f;
namespace Camera
{
	extern void HandleAim();
	extern void LookAt();
	extern Vec3f GetPosition();
	extern Vec3f GetDirection();
	extern Vec3f GetRotation();
	extern void SetMovementSpeed(float movementSpeed);
	extern void SetLookSensitivity(float lookSensitivity);
	enum MOVEMENT_DIRECTION { FORWARD, BACKWARD, UP, DOWN, LEFT, RIGHT };
	extern void Move(MOVEMENT_DIRECTION dir);
};

#endif