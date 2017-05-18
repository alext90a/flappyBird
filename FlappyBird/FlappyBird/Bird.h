#pragma once
class Bird : public Component
{
public:
	Bird();
	~Bird();

	void update()override;
	void addUpImpulse();
	void setOnPlayerCrashed(std::function<void()> func);

protected:
	D3DXVECTOR3 mFlyDirection = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 mUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	float mTimeSinceForceAdded = 0.0f;
	float mCurUpForce = 0.0f;
	std::function<void()> mOnPlayerCrashed = nullptr;
	
};

