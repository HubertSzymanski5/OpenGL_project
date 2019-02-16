#include <Camera.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

// Standardowe wartosci kamery
const float DEF_FOV = 45.0f;

//--------------------------------------------------------------------
// KAMERA
//--------------------------------------------------------------------

// konstruktor
Camera::Camera() 
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	 mTargetPos(glm::vec3(0.0f, 0.0f, -1.0f)),
	 mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	 mRight(0.0f, 0.0f, 0.0f),
	 WORLD_UP(0.0f, 1.0f, 0.0f),
	 mYaw(glm::pi<float>()),
	 mPitch(0.0f),
	 mFOV(DEF_FOV) {
}

// pobranie macierzy VIEW
const glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

// pobieranie wektorow Look-przod, Right i Up
const glm::vec3 &Camera::getLook() const {
	return mLook;
}

const glm::vec3 &Camera::getRight() const {
	return mRight;
}

const glm::vec3 &Camera::getUp() const {
	return mUp;
}

const glm::vec3 &Camera::getPosition() const {
	return mPosition;
}

//--------------------------------------------------------------------
// KAMERA FPS
//--------------------------------------------------------------------

// konstruktor
FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch) {
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

// ustaw polozenie kamery
void FPSCamera::setPosition(const glm::vec3 &position) {
	mPosition = position;
}

// poruszanie kamery
void FPSCamera::move(const glm::vec3 &offsetPos) {
	mPosition += offsetPos;
	updateCameraVectors();
}

// obracaj kamera
void FPSCamera::rotate(float yaw, float pitch) {
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	// ograniczamy ruch gora dol (mPitch)
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	updateCameraVectors();
}

// aktualizuj wektory kamery
void FPSCamera::updateCameraVectors() {
	
	// obliczanie wektora kierunku patrzenia
	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	// rekalkulacja wektorow gora i prawo
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;
}


//--------------------------------------------------------------------
// KAMERA ORBITALNA
//--------------------------------------------------------------------

// konstruktor
OrbitCamera::OrbitCamera()
	:mRadius(0.0f) {

}

// ustaw orientacje (srodek orbity)
void OrbitCamera::setLookAt(const glm::vec3 &target) {
	mTargetPos = target;
}

// ustaw promien / odleglosc od celu
void OrbitCamera::setRadius(float radius) {
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

// obracanie wokol
void OrbitCamera::rotate(float yaw, float pitch) {
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	// ograniczamy ruch kamery gora dol (pitch)
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}

// aktualizuj wektory kamery
void OrbitCamera::updateCameraVectors() {
	mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
	mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}