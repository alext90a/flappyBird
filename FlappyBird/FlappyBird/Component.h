#pragma once

class GameObject;
class Component
{
public:
	Component() {}
	~Component() {}
	virtual void draw() {};
	virtual void update() {};
	virtual void clean() {};
	virtual void onMatrixChanged(const D3DXMATRIXA16* const matrix) {};

	GameObject* getGameObject()const;
	void setGameObject(GameObject* gameObj);
protected:
	GameObject* mGameObject;
};