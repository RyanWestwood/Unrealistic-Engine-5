#include "./Camera.h"

namespace UE {

	Camera::Camera(
		glm::vec3 position,
		glm::vec3 target,
		glm::vec3 upDirection,
		float fieldOfView, float aspectRatio, float nearClippingPlane, float farClippingPane
	)
	{
		m_Position = position;
		m_Target = target;
		m_UpDirection = upDirection;

		m_FieldOfView = fieldOfView;
		m_AspectRatio = aspectRatio;
		m_NearClippingPlane = nearClippingPlane;
		m_FarClippingPlane = farClippingPane;

		UpdateCameraMatrices();

		SDL_GetMouseState(&m_OldMouseX, &m_OldMouseY);
	};

	// Positions
	void Camera::SetPositionX(float x) 
	{
		m_Position.x = x;
		UpdateCameraMatrices();
	}

	void Camera::SetPositionY(float y) 
	{
		m_Position.y = y;
		UpdateCameraMatrices();
	}

	void Camera::SetPositionZ(float z) 
	{
		m_Position.z = z;
		UpdateCameraMatrices();
	}

	void Camera::SetPosition(glm::vec3 position) 
	{
		m_Position = position;
		UpdateCameraMatrices();
	}

	//Targets
	void Camera::SetTargetX(float x) 
	{
		m_Target.x = x;
		UpdateCameraMatrices();
	}

	void Camera::SetTargetY(float y) 
	{
		m_Target.y = y;
		UpdateCameraMatrices();
	}

	void Camera::SetTargetZ(float z) 
	{
		m_Target.z = z;
		UpdateCameraMatrices();
	}

	void Camera::SetTarget(glm::vec3 target)
	{
		m_Target = target;
		UpdateCameraMatrices();
	}


	// Camera Attributes
	void Camera::SetUpDirection(glm::vec3 upDirection) 
	{
		m_UpDirection = upDirection;
		UpdateCameraMatrices();
	}

	void Camera::SetFieldOfView(float fieldOfView) 
	{
		m_FieldOfView = fieldOfView;
		UpdateCameraMatrices();
	}

	void Camera::SetAspectRatio(float aspectRatio) 
	{
		m_AspectRatio = aspectRatio;
		UpdateCameraMatrices();
	}

	void Camera::SetNearClippingPlane(float nearClippingPlane) 
	{
		m_NearClippingPlane = nearClippingPlane;
		UpdateCameraMatrices();
	}

	void Camera::SetFarClippingPlane(float farClippingPlane) 
	{
		m_FarClippingPlane = farClippingPlane;
		UpdateCameraMatrices();
	}

	// Axis
	void Camera::SetPitch(float pitch) 
	{
		m_Pitch = pitch;
		if (pitch > 70.0f) m_Pitch = 70.0f;
		if (pitch < -70.0f) m_Pitch = -70.0f;
	}

	void Camera::SetYaw(float yaw) 
	{
		m_Yaw = yaw;
	}

	// Update
	void Camera::UpdateCameraMatrices() {
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Target, m_UpDirection);
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), m_AspectRatio, m_NearClippingPlane, m_FarClippingPlane);
	}

}