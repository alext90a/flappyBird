#include "stdafx.h"
#include "BackgroundController.h"


BackgroundController::BackgroundController()
{
}


BackgroundController::~BackgroundController()
{
	mBackgroundObjects.clear();
}

void BackgroundController::insertBackgroundObject(std::shared_ptr<GameObject> backgroundObj)
{
	mBackgroundObjects.push_back(backgroundObj);
}

void BackgroundController::update()
{
	for (unsigned int i = 0; i < mBackgroundObjects.size(); ++i)
	{

		float displacement = mPlayer->getSpeed() * Game::mDeltaTime;
		mBackgroundObjects[i]->addLocalPosX(displacement);


	}
}

void BackgroundController::setPlayer(std::shared_ptr<Bird> player)
{
	mPlayer = player;
}

void BackgroundController::setStartPos()
{
	float startX = -kBackgroundWidth;
	float halfWidth = kBackgroundWidth / 2.0f;
	for (unsigned int i = 0; i < mBackgroundObjects.size(); ++i)
	{
		mBackgroundObjects[i]->setLocalPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		mBackgroundObjects[i]->addLocalPos(startX + 2.0f* halfWidth*(float)i, 0.0f, 1.0f);
	}
}