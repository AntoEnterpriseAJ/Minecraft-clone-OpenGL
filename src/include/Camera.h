#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DEFAULT
{
	constexpr float yaw = -90.0f;
	constexpr float pitch = 0.0f;
	constexpr float pitchUpperBound = 89.9f;
	constexpr float speed = 2.0f;
	constexpr float mouseSensitivity = 0.1f;
	constexpr float zoomSensitivity = 2.0f;
	constexpr float FOV = 45.0f;
	constexpr float FOVUpperBound = 120.0f;
	constexpr float FOVLowerBound = 5.0f;
}

class Camera
{
public:
	enum CameraMovement
	{
		FORWARD,
		LEFT,
		BACKWARD,
		RIGHT,
	};

	glm::mat4 getViewMatrix();
	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseScroll(float yOffset);
	void processMouseCursor(double yawOffset, double pitchOffset);
	float getFOV();

	Camera(glm::vec3 pos, glm::vec3 front = { 0.0f, 0.0f, -1.0f }, glm::vec3 up = { 0.0f, 1.0f, 0.0f }, float yaw = DEFAULT::yaw, float pitch = DEFAULT::pitch);

private:
	void updateVectors();
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_worldUp;

	float m_FOV;

	float m_yaw;
	float m_pitch;

	float m_speed;
	float m_mouseSensitivity;
	float m_zoomSensitivity;
};