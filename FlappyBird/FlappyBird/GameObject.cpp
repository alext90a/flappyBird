#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

bool GameObject::init(LPDIRECT3DDEVICE9 device)
{
	g_pd3dDevice = device;
	D3DXMatrixIdentity(&mLocalPosMatrix);
	D3DXMatrixIdentity(&mLocalScaleMatrix);
	D3DXMatrixIdentity(&mWorldPosMatrix);
	D3DXMatrixIdentity(&mWorldScaleMatrix);

	
	return true;
}

void GameObject::addComponent(std::shared_ptr<IComponent> component)
{
	mComponents.push_back(component);
}

void GameObject::draw()
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mWorldResMatrix);
	for (auto curComponent : mComponents)
	{
		curComponent->draw();
	}

	for (auto curChild : mChilds)
	{
		curChild->draw();
	}
}

void GameObject::update()
{
	for (auto curComponent : mComponents)
	{
		curComponent->update();
	}

	for (auto curChild : mChilds)
	{
		curChild->update();
	}
}



void GameObject::onMatrixChanged()
{
	if (mParent == nullptr)
	{
		mWorldPosMatrix = mLocalPosMatrix;
		mWorldScaleMatrix = mLocalScaleMatrix;
	}
	D3DXMatrixMultiply(&mWorldResMatrix, &mWorldScaleMatrix, &mWorldPosMatrix);
	for (auto curComponent : mComponents)
	{
		curComponent->onMatrixChanged(&mWorldResMatrix);
	}

	for (auto curChild : mChilds)
	{
		curChild->parentMatrixChanged(&mWorldPosMatrix, &mWorldScaleMatrix);
	}
}

void GameObject::setLocalPos(float x, float y, float z)
{
	mLocalPosMatrix._41 = x;
	mLocalPosMatrix._42 = y;
	mLocalPosMatrix._43 = z;

	onMatrixChanged();
}

void GameObject::setLocalPos(D3DXVECTOR3 pos)
{
	mLocalPosMatrix._41 = pos.x;
	mLocalPosMatrix._42 = pos.y;
	mLocalPosMatrix._43 = pos.z;

	onMatrixChanged();
}

void GameObject::setLocalPosX(float x)
{
	mLocalPosMatrix._41 = x;
	onMatrixChanged();
}

void GameObject::setLocalPosY(float y)
{
	mLocalPosMatrix._42 = y;
	onMatrixChanged();
}

void GameObject::addLocalPos(float x, float y, float z)
{
	mLocalPosMatrix._41 += x;
	mLocalPosMatrix._42 += y;
	mLocalPosMatrix._43 += z;

	onMatrixChanged();
}

void GameObject::addLocalPosX(float x)
{
	mLocalPosMatrix._41 += x;
	onMatrixChanged();
}

void GameObject::addLocalPosY(float y)
{
	mLocalPosMatrix._42 += y;
	onMatrixChanged();
}

void GameObject::setLocalScale(float x, float y, float z)
{
	mLocalScaleMatrix._11 = x;
	mLocalScaleMatrix._22 = y;
	mLocalScaleMatrix._33 = z;
	onMatrixChanged();
}

D3DXVECTOR3 GameObject::getPos()const
{
	D3DXVECTOR3 pos;
	pos.x = mLocalPosMatrix._41;
	pos.y = mLocalPosMatrix._42;
	pos.z = mLocalPosMatrix._43;
	return pos;
}

void GameObject::clean()
{
	for (auto curComponent : mComponents)
	{
		curComponent->clean();
	}
}

float GameObject::getLocalPosX()const
{
	return mLocalPosMatrix._41;
}

float GameObject::getLocalPosY()const
{
	return mLocalPosMatrix._42;
}

void GameObject::addChild(std::shared_ptr<GameObject> child)
{
	child->setParent(this);
	mChilds.push_back(child);
}

void GameObject::setParent(GameObject* parent)
{
	mParent = parent;
}

void GameObject::parentMatrixChanged(const D3DXMATRIXA16 * mParentPos, const D3DXMATRIXA16* mParentScale)
{
	D3DXMatrixMultiply(&mWorldScaleMatrix, &mLocalScaleMatrix, mParentScale);
	D3DXMatrixMultiply(&mWorldPosMatrix, &mLocalPosMatrix, mParentPos);

	onMatrixChanged();
}
