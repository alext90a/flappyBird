#include "stdafx.h"
#include "BackgroundController.h"


BackgroundController::BackgroundController()
{
}


BackgroundController::~BackgroundController()
{
}

void BackgroundController::insertBackgroundObject(std::shared_ptr<GameObject> backgroundObj)
{
	mBackgroundObject = backgroundObj;
}

void BackgroundController::update()
{

	mBackgroundObject->setLocalPosX(mPlayer->getGameObject()->getLocalPosX() + kPlayerXOffset);

}

void BackgroundController::setPlayer(std::shared_ptr<Bird> player)
{
	mPlayer = player;
}

