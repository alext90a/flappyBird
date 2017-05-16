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
	D3DXMatrixIdentity(&mMatrix);
	return true;
}

void GameObject::addComponent(std::shared_ptr<IComponent> component)
{
	mComponents.push_back(component);
}

void GameObject::draw()
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mMatrix);
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
	for (auto curComponent : mComponents)
	{
		curComponent->onMatrixChanged(&mMatrix);
	}
}

void GameObject::setPos(float x, float y, float z)
{
	mMatrix._41 = x;
	mMatrix._42 = y;
	mMatrix._43 = z;

	onMatrixChanged();
}

void GameObject::setPos(D3DXVECTOR3 pos)
{
	mMatrix._41 = pos.x;
	mMatrix._42 = pos.y;
	mMatrix._43 = pos.z;

	onMatrixChanged();
}

void GameObject::addPos(float x, float y, float z)
{
	mMatrix._41 += x;
	mMatrix._42 += y;
	mMatrix._43 += z;

	onMatrixChanged();
}

D3DXVECTOR3 GameObject::getPos()const
{
	D3DXVECTOR3 pos;
	pos.x = mMatrix._41;
	pos.y = mMatrix._42;
	pos.z = mMatrix._43;
	return pos;
}

void GameObject::clean()
{
	for (auto curComponent : mComponents)
	{
		curComponent->clean();
	}
}
