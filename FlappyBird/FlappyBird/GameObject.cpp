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
	D3DXMatrixIdentity(&mPosMatrix);
	D3DXMatrixIdentity(&mScaleMatrix);
	
	return true;
}

void GameObject::addComponent(std::shared_ptr<IComponent> component)
{
	mComponents.push_back(component);
}

void GameObject::draw()
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mResMatrix);
	for (auto curComponent : mComponents)
	{
		curComponent->draw();
	}
}

void GameObject::update()
{
	for (auto curComponent : mComponents)
	{
		curComponent->update();
	}
}



void GameObject::onMatrixChanged()
{
	D3DXMatrixMultiply(&mResMatrix, &mScaleMatrix, &mPosMatrix);
	for (auto curComponent : mComponents)
	{
		curComponent->onMatrixChanged(&mResMatrix);
	}
}

void GameObject::setPos(float x, float y, float z)
{
	mPosMatrix._41 = x;
	mPosMatrix._42 = y;
	mPosMatrix._43 = z;

	onMatrixChanged();
}

void GameObject::setPos(D3DXVECTOR3 pos)
{
	mPosMatrix._41 = pos.x;
	mPosMatrix._42 = pos.y;
	mPosMatrix._43 = pos.z;

	onMatrixChanged();
}

void GameObject::setPosX(float x)
{
	mPosMatrix._41 = x;
	onMatrixChanged();
}

void GameObject::setPosY(float y)
{
	mPosMatrix._42 = y;
	onMatrixChanged();
}

void GameObject::addPos(float x, float y, float z)
{
	mPosMatrix._41 += x;
	mPosMatrix._42 += y;
	mPosMatrix._43 += z;

	onMatrixChanged();
}

void GameObject::addPosX(float x)
{
	mPosMatrix._41 += x;
	onMatrixChanged();
}

void GameObject::addPosY(float y)
{
	mPosMatrix._42 += y;
	onMatrixChanged();
}

void GameObject::setScale(float x, float y, float z)
{
	mScaleMatrix._11 = x;
	mScaleMatrix._22 = y;
	mScaleMatrix._33 = z;
	onMatrixChanged();
}

D3DXVECTOR3 GameObject::getPos()const
{
	D3DXVECTOR3 pos;
	pos.x = mPosMatrix._41;
	pos.y = mPosMatrix._42;
	pos.z = mPosMatrix._43;
	return pos;
}

void GameObject::clean()
{
	for (auto curComponent : mComponents)
	{
		curComponent->clean();
	}
}

float GameObject::getPosX()const
{
	return mPosMatrix._41;
}

float GameObject::getPosY()const
{
	return mPosMatrix._42;
}
