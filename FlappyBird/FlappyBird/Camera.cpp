#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

const D3DXVECTOR3* Camera::getCameraEyePos()
{
	return &mEyePt;
}

const D3DXVECTOR3* Camera::getCameraLookAt()
{
	return &mLookatPt;
}

const D3DXVECTOR3* Camera::getCamerUpVec()
{
	return &mUpVec;
}

void Camera::setCameraEyePos(const D3DXVECTOR3& eyePos)
{
	mEyePt = eyePos;
}

void Camera::setCameraLookPos(const D3DXVECTOR3& lookPos)
{
	mLookatPt = lookPos;
}

