#pragma once
#include "stdafx.h"
class Texture
{
	friend class TextureManager;
public:
	
	~Texture();
	HRESULT create(LPDIRECT3DDEVICE9 device, const std::string& filePath);
	void draw();

	

protected:
	Texture();
	void clean();

protected:
	LPDIRECT3DDEVICE9       g_pd3dDevice;
	IDirect3DTexture9*      g_pTexture = NULL; // Our texture
};

