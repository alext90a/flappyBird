#include "stdafx.h"
#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}

void BoundingBox::init(D3DXVECTOR3 topLeft, D3DXVECTOR3 bottomRight, std::vector<const BoundingBox*>* collisionLayer)
{
	mLocalTopLeft = topLeft;
	mLocalBottomRight = bottomRight;
	if (collisionLayer != nullptr)
	{
		collisionLayer->push_back(this);
	}
	
}

const D3DXVECTOR3* const BoundingBox::getTopLeft()const
{
	return &mTopLeft;
}

const D3DXVECTOR3* const BoundingBox::getBottomRight()const
{
	return &mBottomRight;
}



void BoundingBox::onMatrixChanged(const D3DXMATRIXA16* const matrix)
{

	D3DXVECTOR4 vector;
	

	D3DXVec3Transform(&vector, &mLocalTopLeft, matrix);
	mTopLeft.x = vector.x;
	mTopLeft.y = vector.y;
	mTopLeft.z = vector.z;

	D3DXVec3Transform(&vector, &mLocalBottomRight, matrix);
	mBottomRight.x = vector.x;
	mBottomRight.y = vector.y;
	mBottomRight.z = vector.z;

	mWorldPos.x = matrix->_41;
	mWorldPos.y = matrix->_42;
	mWorldPos.z = matrix->_43;
}

bool BoundingBox::isPointInside(const D3DXVECTOR3* point)const
{
	if ((mTopLeft.x <= point->x && point->x <= mBottomRight.x) &&
		(mTopLeft.y >= point->y && point->y >= mBottomRight.y))
	{
		return true;
	}
	return false;
}

const D3DXVECTOR3* const BoundingBox::getWorldPos()const
{
	return &mWorldPos;
}

bool BoundingBox::isIntersect(const BoundingBox* otherBox)const
{
	return (abs(mWorldPos.x - otherBox->getWorldPos()->x) * 2 < (mBottomRight.x- mTopLeft.x + otherBox->getBottomRight()->x - otherBox->getTopLeft()->x)) &&
		(abs(mWorldPos.y - otherBox->getWorldPos()->y) * 2 < (mTopLeft.y - mBottomRight.y + otherBox->getTopLeft()->y - otherBox->getBottomRight()->y));

}
