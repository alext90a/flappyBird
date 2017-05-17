#pragma once
class GameObject
{
public:
	GameObject();
	~GameObject();
	bool init(LPDIRECT3DDEVICE9 device);
	void addComponent(std::shared_ptr<IComponent> component);
	void update();
	void draw();
	void clean();

	void setPos(float x, float y, float z);
	void setPos(D3DXVECTOR3 pos);
	void setPosX(float x);
	void setPosY(float y);
	void addPos(float x, float y, float z);
	void addPosX(float x);
	void addPosY(float y);
	void setScale(float x, float y, float z);
	D3DXVECTOR3 getPos()const;
	float getPosX()const;
	float getPosY()const;

protected:
	void onMatrixChanged();

protected:
	D3DXMATRIXA16 mPosMatrix;
	D3DXMATRIXA16 mScaleMatrix;
	D3DXMATRIXA16 mResMatrix;
	std::list<std::shared_ptr<IComponent>> mComponents;
	std::list<std::shared_ptr<GameObject>> mChilds;
	LPDIRECT3DDEVICE9       g_pd3dDevice = nullptr;
};

