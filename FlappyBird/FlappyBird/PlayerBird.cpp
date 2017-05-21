#include "stdafx.h"

void BirdState::init(PlayerBird* bird)
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


PlayerBird::PlayerBird()
{
	mCurState = mCrashedState;
	mFlyState->init(this);
	mCrashedState->init(this);
	mFallDawnState->init(this);
}


PlayerBird::~PlayerBird()
{
}

void PlayerBird::update()
{
	mCurState->update();
	
	
}

void PlayerBird::addUpImpulse()
{
	mCurUpForce = kAddedImpulse;
	mTimeSinceForceAdded = 0.0f;
}

float PlayerBird::getCurUpForce()const
{
	return mCurUpForce;
}

void PlayerBird::setCurUpForce(float force)
{
	mCurUpForce = force;
}

void PlayerBird::setOnPlayerCrashed(std::function<void()> func)
{
	mOnPlayerCrashed = func;
}

void PlayerBird::start()
{
	mGameObject->setLocalPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	mCurState = mFlyState;
	mScore = 0;
}

void PlayerBird::setFallDawnState()
{
	mCurState = mFallDawnState;
	if (mOnPlayerFallDawn != nullptr)
	{
		mOnPlayerFallDawn();
	}
}

void PlayerBird::setCrashedState()
{
	mCurState = mCrashedState;
	if (mOnPlayerCrashed != nullptr)
	{
		mOnPlayerCrashed();
	}
}

void PlayerBird::addScore(int score)
{
	mScore += score;
}

void PlayerBird::setScore(int score)
{
	mScore = score;
}

int PlayerBird::getScore()const
{
	return mScore;
}

float PlayerBird::getSpeed()const
{
	return mCurState->getSpeed();
}
