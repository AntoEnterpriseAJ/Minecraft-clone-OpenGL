#include "include/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = m_speed * deltaTime;
	switch (direction)
	{
		case FORWARD:
		{
			m_position += m_front * velocity;
			break;
		}
		case LEFT:
		{
			m_position -= m_right * velocity;
			break;
		}
		case BACKWARD:
		{
			m_position -= m_front * velocity;
			break;
		}
		case RIGHT:
		{
			m_position += m_right * velocity;
			break;
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

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float yaw, float pitch)
	: m_position{pos}, m_front{front}, m_up{up}, m_worldUp{up}, m_FOV{CameraDefaults::FOV}, m_yaw{yaw}, m_pitch{pitch},
	  m_speed{CameraDefaults::speed}, m_mouseSensitivity{CameraDefaults::mouseSensitivity}, m_zoomSensitivity{CameraDefaults::zoomSensitivity}
{
	updateVectors();
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
