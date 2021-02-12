#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace UE {

	class Camera {
	public:
		Camera(
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

		~Camera(){}

		//	Position
		float GetPositionX() { return m_Position.x; }
		float GetPositionY() { return m_Position.y; }
		float GetPositionZ() { return m_Position.z; }
		glm::vec3 GetPosition() { return m_Position; }

		void SetPositionX(float x) {
			m_Position.x = x;
			UpdateCameraMatrices();
		}
		void SetPositionY(float y) {
			m_Position.y = y;
			UpdateCameraMatrices();
		}
		void SetPositionZ(float z) {
			m_Position.z = z;
			UpdateCameraMatrices();
		}
		void SetPosition(glm::vec3 position) {
			m_Position = position;
			UpdateCameraMatrices();
		}

		//	Target
		float GetTargetX() { return m_Target.x; }
		float GetTargetY() { return m_Target.y; }
		float GetTargetZ() { return m_Target.z; }
		glm::vec3 GetTarget() { return m_Target; }

		void SetTargetX(float x) {
			m_Target.x = x;
			UpdateCameraMatrices();
		}
		void SetTargetY(float y) {
			m_Target.y = y;
			UpdateCameraMatrices();
		}
		void SetTargetZ(float z) {
			m_Target.z = z;
			UpdateCameraMatrices();
		}
		void SetTarget(glm::vec3 target) {
			m_Target = target;
			UpdateCameraMatrices();
		}

		//	Up Direction
		glm::vec3 GetUpDirection() { return m_UpDirection; }
		void SetUpDirection(glm::vec3 upDirection) {
			m_UpDirection = upDirection;
			UpdateCameraMatrices();
		}

		//	Field Of View
		float GetFieldOfView() { return m_FieldOfView; }
		void SetFieldOfView(float fieldOfView) {
			m_FieldOfView = fieldOfView;
			UpdateCameraMatrices();
		}

		//	Aspect Ratio
		float GetAspectRatio() { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) {
			m_AspectRatio = aspectRatio;
			UpdateCameraMatrices();
		}

		// Near Clipping Plane
		float GetNearClippingPlane() { return m_NearClippingPlane; }
		void SetNearClippingPlane(float nearClippingPlane) {
			m_NearClippingPlane = nearClippingPlane;
			UpdateCameraMatrices();
		}

		// Far Clipping Plane
		float GetFarClippingPlane() { return m_FarClippingPlane; }
		void SetFarClippingPlane(float farClippingPlane) {
			m_FarClippingPlane = farClippingPlane;
			UpdateCameraMatrices();
		}

		//	Matrices
		glm::mat4 GetViewMatrix() {
			return m_ViewMatrix;
		}
		glm::mat4 GetProjectionMatrix() {
			return m_ProjectionMatrix;
		}

		//	Set Pitch and Yaw
		float GetPitch() { return m_Pitch; }
		float GetYaw() { return m_Yaw; }

		void SetPitch(float pitch) {
			m_Pitch = pitch;
			if (pitch > 70.0f) m_Pitch = 70.0f;
			if (pitch < -70.0f) m_Pitch = -70.0f;
		}
		void SetYaw(float yaw) {
			m_Yaw = yaw;
		}

		//	Set old mouse vars
		float GetOldMouseX() { return m_OldMouseX; }
		float GetOldMouseY() { return m_OldMouseY; }

		void SetOldMouseX(float oldMouseX) {
			m_OldMouseX = oldMouseX;
		}
		void SetOldMouseY(float oldMouseY) {
			m_OldMouseY = oldMouseY;
		}

		void UpdateCameraMatrices() {
			m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Target, m_UpDirection);
			m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), m_AspectRatio, m_NearClippingPlane, m_FarClippingPlane);
		}

	private:

		glm::vec3 m_Position;
		glm::vec3 m_Target;
		glm::vec3 m_UpDirection;

		float m_FieldOfView;
		float m_AspectRatio;
		float m_NearClippingPlane;
		float m_FarClippingPlane;

		float m_Pitch = 0.0f;
		float m_Yaw = -90.0f;

		int m_OldMouseX, m_OldMouseY;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
	};
}
