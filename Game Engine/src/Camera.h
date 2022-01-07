#pragma once
#include "CrossPlatform.h"
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
		);

		//	Position
		float GetPositionX() { return m_Position.x; }
		float GetPositionY() { return m_Position.y; }
		float GetPositionZ() { return m_Position.z; }
		glm::vec3 GetPosition() { return m_Position; }

		void SetPositionX(float x);
		void SetPositionY(float y);
		void SetPositionZ(float z);
		void SetPosition(glm::vec3 position);

		//	Target
		float GetTargetX() { return m_Target.x; }
		float GetTargetY() { return m_Target.y; }
		float GetTargetZ() { return m_Target.z; }
		glm::vec3 GetTarget() { return m_Target; }

		void SetTargetX(float x);
		void SetTargetY(float y);
		void SetTargetZ(float z);
		void SetTarget(glm::vec3 target);

		//	Camera Attrs
		glm::vec3 GetUpDirection() { return m_UpDirection; }
		void SetUpDirection(glm::vec3 upDirection);

		float GetFieldOfView() { return m_FieldOfView; }
		void SetFieldOfView(float fieldOfView);

		float GetAspectRatio() { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio);

		float GetNearClippingPlane() { return m_NearClippingPlane; }
		void SetNearClippingPlane(float nearClippingPlane);

		float GetFarClippingPlane() { return m_FarClippingPlane; }
		void SetFarClippingPlane(float farClippingPlane);

		//	Matrices
		glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
		glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }

		//	Axis
		float GetPitch() { return m_Pitch; }
		float GetYaw() { return m_Yaw; }

		void SetPitch(float pitch);
		void SetYaw(float yaw);

		//	Set old mouse vars
		int GetOldMouseX() { return m_OldMouseX; }
		int GetOldMouseY() { return m_OldMouseY; }

		void SetOldMouseX(int oldMouseX) { m_OldMouseX = oldMouseX; }
		void SetOldMouseY(int oldMouseY) { m_OldMouseY = oldMouseY;	}

		void UpdateCameraMatrices();

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
