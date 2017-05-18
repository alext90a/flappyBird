#pragma once
class Bird;

class BirdState
{
public:
	virtual void update();
	void init(Bird* owner);
	virtual float getSpeed()const;
protected:
	Bird* mBird;
};

class FlyState : public BirdState
{
public:
	void update() override;
	float getSpeed()const override;
};

class FallDawnState : public BirdState
{
public:
	void update() override;
};

class Bird : public Component
{
public:
	Bird();
	~Bird();

	void start();
	void update()override;
	void addUpImpulse();
	void setOnPlayerCrashed(std::function<void()> func);
	void setFallDawnState();
	void setCrashedState();
	float getCurUpForce()const;
	void setCurUpForce(float force);
	void addScore(int score);
	void setScore(int score);
	int getScore()const;

	float getSpeed()const;

protected:
	D3DXVECTOR3 mFlyDirection = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 mUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	float mTimeSinceForceAdded = 0.0f;
	float mCurUpForce = 0.0f;
	std::function<void()> mOnPlayerFallDawn = nullptr;
	std::function<void()> mOnPlayerCrashed = nullptr;
	
	std::shared_ptr<BirdState> mCrashedState = std::make_shared<BirdState>();
	std::shared_ptr<FallDawnState> mFallDawnState = std::make_shared<FallDawnState>();
	std::shared_ptr<FlyState> mFlyState = std::make_shared<FlyState>();
	std::shared_ptr<BirdState> mCurState = nullptr;

	int mScore = 0;
};

