#pragma once
#include "stdafx.h"
#include "Texture.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	void init(LPDIRECT3DDEVICE9 device);
	std::shared_ptr<Texture> createTexture(std::string texturePath);
	void clean();
protected:
	LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
	std::list < std::shared_ptr<Texture>> mUsedTexture;
};

