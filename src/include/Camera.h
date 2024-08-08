#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "World.h"

namespace CameraDefaults
{
	constexpr float yaw	             = -90.0f;
	constexpr float pitch            =  0.0f;
	constexpr float pitchUpperBound  =  89.9f;
	constexpr float speed            =  20.0f;
	constexpr float mouseSensitivity =  0.1f;
	constexpr float zoomSensitivity  =  2.0f;
	constexpr float FOV              =  45.0f;
	constexpr float FOVUpperBound    =  120.0f;
	constexpr float FOVLowerBound    =  5.0f;
	constexpr float sprintMultiplier =  1.5f;
	constexpr float playerHeight = 1.8f;
    constexpr float playerWidth = 0.5f;
}

class Camera
{
public:
	enum Movement
	{
		FORWARD,
		LEFT,
		BACKWARD,
		RIGHT,
		DOWN,
		UP,
		COUNT,
	};

	enum mode
	{
		survival = 0,
		creative = 1,
		spectator = 2,
	};

	explicit Camera(glm::vec3 pos, glm::vec3 front = { 0.0f, 0.0f, -1.0f }, glm::vec3 up = { 0.0f, 1.0f, 0.0f }, float yaw = CameraDefaults::yaw, float pitch = CameraDefaults::pitch);

	glm::mat4 getViewMatrix() const;
	void processMovement(Movement direction, float deltaTime, const World& world);
	void processMouseScroll(float yOffset);
	void processMouseCursor(double yawOffset, double pitchOffset);

	float getFOV() const;
	float getPositionX() const;
	float getPositionY() const;
	float getPositionZ() const;
	glm::vec3 getPosition() const;
	glm::vec3 getFront() const;
	glm::vec3 getUp() const;
	glm::vec3 getRight() const;
	glm::vec3& getPositionRef();

	void startSprinting();
	void stopSprinting();

private:
	bool validPosition(const glm::vec3& newPosition, const World& world) const;
	void updateVectors();
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_worldUp;

	mode m_mode;
	bool m_isSprinting;

	float m_FOV;
	float m_yaw;
	float m_pitch;
	float m_speed;
	float m_mouseSensitivity;
	float m_zoomSensitivity;
};