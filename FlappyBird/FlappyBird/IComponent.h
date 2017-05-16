#pragma once

class IComponent
{
public:
	IComponent() {}
	~IComponent() {}
	virtual void draw() {};
	virtual void update() {};
	virtual void clean() {};
	virtual void onMatrixChanged(const D3DXMATRIXA16* const matrix) {};
};