#include "stdafx.h"


Component::Component()
{
}

Component::~Component()
{
}

void Component::setGameObject(GameObject* gameObj)
{
	mGameObject = gameObj;
}

GameObject* Component::getGameObject()const
{
	return mGameObject;
}