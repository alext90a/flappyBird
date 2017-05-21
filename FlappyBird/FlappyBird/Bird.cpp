#include "stdafx.h"
#include "Bird.h"

void BirdState::init(Bird* bird)
{
	mBird = bird;
}

void BirdState::update()
{

}

float BirdState::getSpeed()const
{
	return 0.0f;
}

void FlyState::update()
{
	mBird->getGameObject()->addLocalPosY(kGravity * GameEngine::mDeltaTime);
	float curUpForce = mBird->getCurUpForce();
	mBird->getGameObject()->addLocalPosY(curUpForce * GameEngine::mDeltaTime);
	if (curUpForce > 0.0f)
	{
		curUpForce -= kImpulseDecreaseSpeed * GameEngine::mDeltaTime;
		mBird->setCurUpForce(curUpForce);
	}
	if (mBird->getGameObject()->getLocalPosY() < -9.0f)
	{
		mBird->getGameObject()->setLocalPosY(-9.0f);
		mBird->setCrashedState();
		
	}
	else
	{
		if (mBird->getGameObject()->getLocalPosY() > 9.0f)
		{
			mBird->getGameObject()->setLocalPosY(9.0f);
		}
		mBird->getGameObject()->addLocalPosX(kPlayerSpeed * GameEngine::mDeltaTime);
	}
}

float FlyState::getSpeed()const
{
	return kPlayerSpeed;
}

void FallDawnState::update()
{
	mBird->getGameObject()->addLocalPosY(kGravity * GameEngine::mDeltaTime);
	if (mBird->getGameObject()->getLocalPosY() < -9.0f)
	{
		mBird->getGameObject()->setLocalPosY(-9.0f);
		mBird->setCrashedState();
	}
}


Bird::Bird()
{
	mCurState = mCrashedState;
	mFlyState->init(this);
	mCrashedState->init(this);
	mFallDawnState->init(this);
}


Bird::~Bird()
{
}

void Bird::update()
{
	mCurState->update();
	
	
}

void Bird::addUpImpulse()
{
	mCurUpForce = kAddedImpulse;
	mTimeSinceForceAdded = 0.0f;
}

float Bird::getCurUpForce()const
{
	return mCurUpForce;
}

void Bird::setCurUpForce(float force)
{
	mCurUpForce = force;
}

void Bird::setOnPlayerCrashed(std::function<void()> func)
{
	mOnPlayerCrashed = func;
}

void Bird::start()
{
	mGameObject->setLocalPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	mCurState = mFlyState;
	mScore = 0;
}

void Bird::setFallDawnState()
{
	mCurState = mFallDawnState;
	if (mOnPlayerFallDawn != nullptr)
	{
		mOnPlayerFallDawn();
	}
}

void Bird::setCrashedState()
{
	mCurState = mCrashedState;
	if (mOnPlayerCrashed != nullptr)
	{
		mOnPlayerCrashed();
	}
}

void Bird::addScore(int score)
{
	mScore += score;
}

void Bird::setScore(int score)
{
	mScore = score;
}

int Bird::getScore()const
{
	return mScore;
}

float Bird::getSpeed()const
{
	return mCurState->getSpeed();
}
