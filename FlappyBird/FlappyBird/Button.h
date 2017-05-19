#pragma once
class Button : public Component
{
public:
	Button();
	~Button();
	void init(BoundingBox* bounBox);
	bool checkClick(const D3DXVECTOR3* vecOrig, const D3DXVECTOR3* vecDir);
	void setFunc(std::function<void()> func);
protected:
	BoundingBox* mBoundBox;
	std::function<void()> mOnClickFunc;
};

