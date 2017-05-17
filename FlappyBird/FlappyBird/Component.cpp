#include "stdafx.h"

void Component::setGameObject(GameObject* gameObj)
{
	mGameObject = gameObj;
}

GameObject* Component::getGameObject()const
{
	return mGameObject;
}