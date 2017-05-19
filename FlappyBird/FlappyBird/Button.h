#pragma once
class Button : public Component
{
public:
	Button();
	~Button();
	void init(BoundingBox* bounBox, std::function<void()> func);
	bool checkClick(const D3DXVECTOR3* vecOrig, const D3DXVECTOR3* vecDir);

protected:
	BoundingBox* mBoundBox;
	std::function<void()> mOnClickFunc;
};

