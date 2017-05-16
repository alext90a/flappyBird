#pragma once
class BoundingBox : public IComponent
{
public:
	BoundingBox();
	~BoundingBox();

	void setTopLeft(D3DXVECTOR3 topLeft);
	void setBottomRight(D3DXVECTOR3 bottomRight);

	const D3DXVECTOR3* const getTopLeft()const;
	const D3DXVECTOR3* const getBottomRight()const;

	bool isPointInside(const D3DXVECTOR3* point)const;

	void onMatrixChanged(const D3DXMATRIXA16* const matrix)override;
protected:
	D3DXVECTOR3 mTopLeft = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 mBottomRight = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
};

