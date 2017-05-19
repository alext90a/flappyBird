#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	bool init(LPDIRECT3DDEVICE9 device);
	void addComponent(std::shared_ptr<Component> component);
	void update();
	void draw();
	void clean();
	void addChild(std::shared_ptr<GameObject> child);

	void setWorldScale(float x, float y, float z);
	void setLocalPos(float x, float y, float z);
	void setLocalPos(D3DXVECTOR3 pos);
	void setLocalPosX(float x);
	void setLocalPosY(float y);
	void addLocalPos(float x, float y, float z);
	void addLocalPosX(float x);
	void addLocalPosY(float y);
	void setLocalScale(float x, float y, float z);
	D3DXVECTOR3 getPos()const;
	float getLocalPosX()const;
	float getLocalPosY()const;
	void setParent(GameObject* parent);

	bool isEnabled()const;
	void setEnabled(bool enabled);

	static int mObjects;
protected:
	void onMatrixChanged();
	const D3DXMATRIXA16* const getWorldPosMatrix()const;
	const D3DXMATRIXA16* const getWorldScaleMatrix()const;

	void parentMatrixChanged(const D3DXMATRIXA16 * mParentPos, const D3DXMATRIXA16* mParentScale);

protected:
	D3DXMATRIXA16 mWorldPosMatrix;
	D3DXMATRIXA16 mWorldScaleMatrix;
	D3DXMATRIXA16 mWorldResMatrix;

	D3DXMATRIXA16 mLocalPosMatrix;
	D3DXMATRIXA16 mLocalScaleMatrix;
	D3DXMATRIXA16 mLocalResMatrix;
	std::list<std::shared_ptr<Component>> mComponents;
	std::list<std::shared_ptr<GameObject>> mChilds;
	GameObject* mParent = nullptr;
	LPDIRECT3DDEVICE9       g_pd3dDevice = nullptr;

	bool mIsEnabled = true;
	
};

