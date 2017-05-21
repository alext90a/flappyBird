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

	mBackgroundObject->setLocalPosX(mPlayer->getGameObject()->getLocalPosX() + Game::kPlayerXOffset);

}

void BackgroundController::setPlayer(std::shared_ptr<PlayerBird> player)
{
	mPlayer = player;
}

