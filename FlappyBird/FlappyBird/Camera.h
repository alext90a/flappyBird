#pragma once
class Camera
{
public:
	Camera();
	~Camera();

	const D3DXVECTOR3* getCameraEyePos();
	const D3DXVECTOR3* getCameraLookAt();
	const D3DXVECTOR3* getCamerUpVec();
	void setCameraEyePos(const D3DXVECTOR3& eyePos);
	void setCameraLookPos(const D3DXVECTOR3& lookPos);

protected:
	D3DXVECTOR3 mEyePt = D3DXVECTOR3(0.0f, 0.0f, -25.0f);
	D3DXVECTOR3 mLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 mUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
};

