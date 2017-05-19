#include "stdafx.h"

int Component::mAlived(0);

std::list<Component*> Component::mComs = std::list<Component*>();
Component::Component()
{
	mComs.push_back(this);
	++mAlived;
}

Component::~Component()
{
	mComs.remove(this);
	--mAlived;
}

void Component::setGameObject(GameObject* gameObj)
{
	mGameObject = gameObj;
}

GameObject* Component::getGameObject()const
{
	return mGameObject;
}