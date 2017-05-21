#include "stdafx.h"
#include "GroundObjectController.h"


GroundObjectController::GroundObjectController()
{
}


GroundObjectController::~GroundObjectController()
{
}

void GroundObjectController::setPlayer(std::shared_ptr<PlayerBird> player)
{
	mPlayer = player;
}

void GroundObjectController::update()
{
	for (unsigned int i = 0; i < mGroundObjects.size(); ++i)
	{
		GameObject* curObj = mGroundObjects[i];
		if (mPlayer->getGameObject()->getLocalPosX() - curObj->getLocalPosX() > Game::kObjLiveMaxDistance)
		{
			int nextInd = (i + mGroundObjects.size() - 1) % mGroundObjects.size();
			curObj->setLocalPosX(mGroundObjects[nextInd]->getLocalPosX() + Game::kGroundWidth);
		}
	}
}

void GroundObjectController::setStartPos()
{
	for (unsigned int i = 0; i < mGroundObjects.size(); ++i)
	{
		mGroundObjects[i]->setLocalPosX(i*Game::kGroundWidth);
	}
}

void GroundObjectController::addGroundObject(GameObject* obj)
{
	mGroundObjects.push_back(obj);
}
