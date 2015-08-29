#include "camera.h"
#include "Math.h"
#include "system.h"
#include "Render.h"
#include <cmath>
#include <stdlib.h>

namespace Camera
{	
	Vec3f camPos(0, 0, 10), camTar(0, 0, -1), camRot;
	float lookSensitivity = .003f, movementSpeed = .1f;
}

Vec3f Camera::GetPosition() { return camPos; }
Vec3f Camera::GetDirection() { return camTar; }
Vec3f Camera::GetRotation() { return camRot; }

void Camera::HandleAim()
{
	int mouseX, mouseY;
	System::GetRelativeMousePos(mouseX, mouseY);
	if (mouseX)
	{
		if (mouseX > 0)
			camRot.y -= lookSensitivity * abs(mouseX);
		else if (mouseX < 0)
			camRot.y += lookSensitivity * abs(mouseX);
	}

	if (mouseY)
	{
		if (mouseY > 0)
			camRot.x -= lookSensitivity * abs(mouseY);
		else if (mouseY < 0)
			camRot.x += lookSensitivity * abs(mouseY);
		// Cap vertical looking angle
		if (camRot.x > PI / 4)
			camRot.x = PI / 4;
		else if (camRot.x < -PI / 3)
			camRot.x = -PI / 3;
		
	}

	if (mouseX || mouseY)
	{
		// Rotate on the y-axis
		Matrix rotY = MatrixForYRotation(camRot.y);
		// Rotate on the x-axis
		Matrix rotX = MatrixForXRotation(camRot.x);
		// Combine matrices
		Matrix comboRot = rotY * rotX;
		camTar = comboRot.TransformP(Vec3f(0, 0, -1));
	}
}

void Camera::Move(MOVEMENT_DIRECTION dir)
{
	switch (dir)
	{
		case FORWARD: 
			camPos += camTar * movementSpeed;
			break;

		case BACKWARD:
			camPos -= camTar * movementSpeed;
			break;

		case LEFT:
			camPos += Vec3f(0, 1, 0) * camTar * movementSpeed;
			break;

		case RIGHT:
			camPos -= Vec3f(0, 1, 0) * camTar * movementSpeed;
			break;

		case UP:
			camPos.y += cos(camRot.x) * movementSpeed;
			break;

		case DOWN:
			camPos.y -= cos(-camRot.x) * movementSpeed;
			break;
	}
}

void Camera::SetMovementSpeed(float movementSpeed)
{
	Camera::movementSpeed = movementSpeed;
}

void Camera::SetLookSensitivity(float lookSensitivity)
{
	Camera::lookSensitivity = lookSensitivity;
}

void Camera::LookAt()
{
	Vec3f center(camPos + camTar);
	Render::LookAt(camPos.x, camPos.y, camPos.z, center.x, center.y, center.z);
}
