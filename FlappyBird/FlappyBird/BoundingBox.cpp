#include "stdafx.h"
#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}

void BoundingBox::setTopLeft(D3DXVECTOR3 topLeft)
{
	mTopLeft = topLeft;
}

void BoundingBox::setBottomRight(D3DXVECTOR3 bottomRight)
{
	mBottomRight = bottomRight;
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

	D3DXVec3Transform(&vector, &mTopLeft, matrix);
	mTopLeft.x = vector.x;
	mTopLeft.y = vector.y;
	mTopLeft.z = vector.z;

	D3DXVec3Transform(&vector, &mBottomRight, matrix);
	mBottomRight.x = vector.x;
	mBottomRight.y = vector.y;
	mBottomRight.z = vector.z;
}

bool BoundingBox::isPointInside(const D3DXVECTOR3* point)const
{
	if ((mTopLeft.x <= point->x && point->x <= mBottomRight.x) &&
		(mTopLeft.y >= point->y && point->y <= mBottomRight.y))
	{
		return true;
	}
	return false;
}
