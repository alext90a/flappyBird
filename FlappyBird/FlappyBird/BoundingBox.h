#pragma once
class BoundingBox : public Component
{
public:
	BoundingBox();
	~BoundingBox();

	void init(D3DXVECTOR3 topLeft, D3DXVECTOR3 bottomRight, std::vector<const BoundingBox*>& collisionLayer);
	/*
	void setTopLeft(D3DXVECTOR3 topLeft);
	void setBottomRight(D3DXVECTOR3 bottomRight);
	*/

	const D3DXVECTOR3* const getTopLeft()const;
	const D3DXVECTOR3* const getBottomRight()const;

	bool isPointInside(const D3DXVECTOR3* point)const;
	bool isIntersect(const BoundingBox* const)const;

	void onMatrixChanged(const D3DXMATRIXA16* const matrix)override;
protected:
	D3DXVECTOR3 mTopLeft = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	D3DXVECTOR3 mBottomRight = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	D3DXVECTOR3 mLocalTopLeft = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	D3DXVECTOR3 mLocalBottomRight = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
};

