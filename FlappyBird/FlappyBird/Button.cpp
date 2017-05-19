#include "stdafx.h"
#include "Button.h"


Button::Button()
{
}


Button::~Button()
{
}

void Button::init(BoundingBox* boundBox, std::function<void()> func)
{
	mBoundBox = boundBox;
	mOnClickFunc = func;
}

bool Button::checkClick(const D3DXVECTOR3* vecOrig, const D3DXVECTOR3* vecDir)
{
	if (!mGameObject->isEnabled())
	{
		return false;
	}


	D3DXPLANE buttonPlane;
	D3DXPlaneFromPointNormal(&buttonPlane, mBoundBox->getTopLeft(), &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	D3DXVECTOR3 vecEnd = *vecOrig + *vecDir;
	D3DXVECTOR3 intersection;
	D3DXPlaneIntersectLine(&intersection, &buttonPlane, vecOrig, &vecEnd);

	bool res = mBoundBox->isPointInside(&intersection);
	if (res && mOnClickFunc != nullptr)
	{
		mOnClickFunc();
	}
	return res;
}
