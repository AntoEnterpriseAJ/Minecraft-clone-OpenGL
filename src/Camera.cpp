#include "include/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float yaw, float pitch)
	: m_position{pos}, m_front{front}, m_up{up}, m_worldUp{up}, m_FOV{CameraDefaults::FOV}, m_yaw{yaw}, m_pitch{pitch},
      m_mode{mode::creative}, m_speed{CameraDefaults::speed}, m_mouseSensitivity{CameraDefaults::mouseSensitivity},
	  m_zoomSensitivity{CameraDefaults::zoomSensitivity}, m_isSprinting{false}
{
	updateVectors();
}

glm::mat4 Camera::getViewMatrix() const{
	// view = translation * rotation
	glm::mat4 viewMatrix{
		glm::vec4(m_right.x, m_up.x, -m_front.x, 0.0f),
		glm::vec4(m_right.y, m_up.y, -m_front.y, 0.0f),
		glm::vec4(m_right.z, m_up.z, -m_front.z, 0.0f),
		glm::vec4(-glm::dot(m_right, m_position), -glm::dot(m_up, m_position), glm::dot(m_front, m_position), 1.0f)
	};
	
	return viewMatrix;
}

bool Camera::validPosition(const glm::vec3& newPosition, const World& world) const
{
    constexpr float playerHeight = CameraDefaults::playerHeight;
    constexpr float playerWidth = CameraDefaults::playerWidth;

    glm::vec3 offsets[] = {
        glm::vec3(playerWidth / 2, 0, playerWidth / 2),
        glm::vec3(playerWidth / 2, 0, -playerWidth / 2),
        glm::vec3(-playerWidth / 2, 0, playerWidth / 2),
        glm::vec3(-playerWidth / 2, 0, -playerWidth / 2),
        glm::vec3(playerWidth / 2, playerHeight, playerWidth / 2),
        glm::vec3(playerWidth / 2, playerHeight, -playerWidth / 2),
        glm::vec3(-playerWidth / 2, playerHeight, playerWidth / 2),
        glm::vec3(-playerWidth / 2, playerHeight, -playerWidth / 2)
    };

    for (const auto& offset : offsets)
    {
        glm::vec3 point = newPosition + offset;
        int x = static_cast<int>(glm::round(point.x));
        int y = static_cast<int>(point.y);
        int z = static_cast<int>(glm::round(point.z));

        if (world.getBlockAt(x, z, y - playerHeight + 1).getType() != Block::Type::AIR)
        {
            return false;
        }
    }

    return true;
}

void Camera::processMovement(Movement direction, float deltaTime, const World& world)
{
    float velocity = m_speed * deltaTime;

    if (m_isSprinting)
        velocity *= CameraDefaults::sprintMultiplier;

    if (m_mode == mode::creative)
    {
        glm::vec3 adjustedFront = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
        glm::vec3 adjustedRight = glm::normalize(glm::vec3(m_right.x, 0.0f, m_right.z));
		glm::vec3 newPosition = m_position;

        switch (direction)
        {
            case FORWARD:
                newPosition += adjustedFront * velocity;
                break;
            case LEFT:
                newPosition -= adjustedRight * velocity;
                break;
            case BACKWARD:
                newPosition -= adjustedFront * velocity;
                break;
            case RIGHT:
                newPosition += adjustedRight * velocity;
                break;
            case UP:
                newPosition += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
                break;
            case DOWN:
                newPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
                break;
        }

		if (validPosition(newPosition, world))
		{
			m_position = newPosition;
		} 
    }
}


void Camera::processMouseScroll(float yOffset)
{
	m_FOV -= yOffset * m_zoomSensitivity;

	if (m_FOV > CameraDefaults::FOVUpperBound)
		m_FOV = CameraDefaults::FOVUpperBound;
	if (m_FOV < CameraDefaults::FOVLowerBound)
		m_FOV = CameraDefaults::FOVLowerBound;
}

void Camera::processMouseCursor(double yawOffset, double pitchOffset)
{
	m_yaw += yawOffset * m_mouseSensitivity;
	m_pitch += pitchOffset * m_mouseSensitivity;

	if (m_pitch >= CameraDefaults::pitchUpperBound)
		m_pitch = CameraDefaults::pitchUpperBound;
	if (m_pitch <= -CameraDefaults::pitchUpperBound)
		m_pitch = -CameraDefaults::pitchUpperBound;

	updateVectors();
}

float Camera::getFOV() const
{
	return m_FOV;
}

glm::vec3 Camera::getPosition() const
{
	return m_position;
}

glm::vec3 Camera::getFront() const
{
	return m_front;
}

glm::vec3 Camera::getUp() const
{
	return m_up;
}

glm::vec3 Camera::getRight() const
{
	return m_right;
}

glm::vec3& Camera::getPositionRef()
{
	return m_position;
}

void Camera::startSprinting()
{
	m_isSprinting = true;
}

void Camera::stopSprinting()
{
	m_isSprinting = false;
}

void Camera::updateVectors()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
