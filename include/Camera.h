#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// ABSTRACT CAMERA CLASS
class Camera {
public:

	const glm::mat4 getViewMatrix() const;

	virtual void setPosition(const glm::vec3 &position) {}
	virtual void rotate(float yaw, float pitch) {} // w stopniach
	virtual void move(const glm::vec3 &offsetPos) {}

	const glm::vec3 &getLook() const;
	const glm::vec3 &getRight() const;
	const glm::vec3 &getUp() const;
	const glm::vec3 &getPosition() const;

	float getFOV() const { return mFOV; }
	void setFOV(float fov) { mFOV = fov; }	// w stopniach

protected:

	Camera();

	virtual void updateCameraVectors() {}

	// macierze pozycji, celu, gory
	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mUp;
	glm::vec3 mLook;
	glm::vec3 mRight;
	const glm::vec3 WORLD_UP;

	// katy Eulera jako radiany
	float mYaw;
	float mPitch;

	// parametr kamery
	float mFOV; // stopnie
};

// FPS KAMERA
class FPSCamera : public Camera {
public:

	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f); // (yaw) na poczatku kier -Z

	virtual void setPosition(const glm::vec3 &position);
	virtual void rotate(float yaw, float pitch);	// w stopniach
	virtual void move(const glm::vec3 &offsetPos);

private:
	void updateCameraVectors();

};

// ORBITALNA KAMERKA
class OrbitCamera : public Camera {
public:

	OrbitCamera();

	virtual void rotate(float yaw, float pitch); // in degrees

	void setLookAt(const glm::vec3 &target);
	void setRadius(float radius);

private:

	void updateCameraVectors();
	float mRadius;
};

#endif // CAMERA_H