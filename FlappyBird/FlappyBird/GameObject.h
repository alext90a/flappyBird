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
	void addPos(float x, float y, float z);
	void setScale(float x, float y, float z);
	D3DXVECTOR3 getPos()const;

protected:
	void onMatrixChanged();

protected:
	D3DXMATRIXA16 mPosMatrix;
	D3DXMATRIXA16 mScaleMatrix;
	D3DXMATRIXA16 mResMatrix;
	std::vector<std::shared_ptr<IComponent>> mComponents;
	LPDIRECT3DDEVICE9       g_pd3dDevice = nullptr;
};

