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

void GameObject::addComponent(std::shared_ptr<Component> component)
{
	component->setGameObject(this);
	mComponents.push_back(component);
}

void GameObject::draw()
{
	if (!mIsEnabled)
	{
		return;
	}
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
	if (!mIsEnabled)
	{
		return;
	}
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
	else
	{
		D3DXMatrixMultiply(&mWorldPosMatrix, &mLocalPosMatrix, mParent->getWorldPosMatrix());
		D3DXMatrixMultiply(&mWorldScaleMatrix, &mLocalScaleMatrix, mParent->getWorldScaleMatrix());
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

void GameObject::parentMatrixChanged(const D3DXMATRIXA16 * mParentPos, const D3DXMATRIXA16* mParentScale)
{
	D3DXMatrixMultiply(&mWorldScaleMatrix, &mLocalScaleMatrix, mParentScale);
	D3DXMatrixMultiply(&mWorldPosMatrix, &mLocalPosMatrix, mParentPos);

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

void GameObject::setWorldScale(float x, float y, float z)
{
	mLocalScaleMatrix._11 = x;
	mLocalScaleMatrix._22 = y;
	mLocalScaleMatrix._33 = z;
	if (mParent != nullptr)
	{
		D3DXMATRIXA16 parentScaleInv;
		float det = D3DXMatrixDeterminant(mParent->getWorldScaleMatrix());
		D3DXMatrixInverse(&parentScaleInv, &det, mParent->getWorldScaleMatrix());
		D3DXMatrixMultiply(&mLocalScaleMatrix, &mLocalScaleMatrix, &parentScaleInv);
	}
	else
	{
		mWorldScaleMatrix._11;
		mWorldScaleMatrix._22;
		mWorldScaleMatrix._33;
	}
	onMatrixChanged();
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
	auto worldMatrix = child->getWorldScaleMatrix();
	child->setWorldScale(worldMatrix->_11, worldMatrix->_22, worldMatrix->_23);
	mChilds.push_back(child);
}

void GameObject::setParent(GameObject* parent)
{
	mParent = parent;
}

const D3DXMATRIXA16* const GameObject::getWorldPosMatrix()const
{
	return &mWorldPosMatrix;
}

const D3DXMATRIXA16* const GameObject::getWorldScaleMatrix()const
{
	return &mWorldScaleMatrix;
}

bool GameObject::isEnabled()const
{
	return mIsEnabled;
}

void GameObject::setEnabled(bool enabled)
{
	mIsEnabled = enabled;
	for (auto curChild : mChilds)
	{
		curChild->setEnabled(enabled);
	}
}


