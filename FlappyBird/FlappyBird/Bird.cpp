#include "stdafx.h"
#include "Bird.h"


Bird::Bird()
{
}


Bird::~Bird()
{
}

void Bird::update()
{
	
	
	mGameObject->addLocalPosY(kGravity * Game::mDeltaTime);
	mGameObject->addLocalPosY(mCurUpForce * Game::mDeltaTime);
	if (mCurUpForce > 0.0f)
	{
		mCurUpForce -= kImpulseDecreaseSpeed * Game::mDeltaTime;
	}
	if (mGameObject->getLocalPosY() < -9.0f)
	{
		mGameObject->setLocalPosY(-9.0f);
		if (mOnPlayerCrashed != nullptr)
		{
			mOnPlayerCrashed();
		}
	}
	else
	{
		if (mGameObject->getLocalPosY() > 9.0f)
		{
			mGameObject->setLocalPosY(9.0f);
		}
		mGameObject->addLocalPosX(kPlayerSpeed * Game::mDeltaTime);
	}
}

void Bird::addUpImpulse()
{
	mCurUpForce = kAddedImpulse;
	mTimeSinceForceAdded = 0.0f;
}

void Bird::setOnPlayerCrashed(std::function<void()> func)
{
	mOnPlayerCrashed = func;
}
