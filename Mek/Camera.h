#pragma once

#include "lib\glm\glm.hpp"

class Camera {
public:
	const glm::vec3& position() const;
	void setPosition(const glm::vec3& position);
	void offsetPosition(const glm::vec3& offset);

	float fieldOfView() const;
	void setFieldOfView(float fieldOfView);

	float nearPlane() const;

	float farPlane() const;

	void setNearAndFarPlanes(float nearPlane, float farPlane);

	glm::mat4 orientation() const;

	void offsetOrientation(float upAngle, float rightAngle);

	void lookAt(glm::vec3 position);
	void lookAtPoint(glm::vec3 worldpos);

	float viewportAspectRatio() const;
	void setViewportAspectRatio(float viewportAspectRatio);

	glm::vec3 forward() const;

	glm::vec3 right() const;

	glm::vec3 up() const;

	glm::mat4 matrix() const;

	glm::mat4 projection() const;

	glm::mat4 view() const;

	static Camera& getInstance()
	{
		static Camera instance;
		return instance;
	}

	float _horizontalAngle;
	float _verticalAngle;
private:

	Camera();

	glm::vec3 _position;
	float _fieldOfView;
	float _nearPlane;
	float _farPlane;
	float _viewportAspectRatio;

	void normalizeAngles();
};

