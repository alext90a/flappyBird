#pragma once
#include "stdafx.h"
class Texture
{
public:
	Texture();
	~Texture();

	HRESULT create(LPDIRECT3DDEVICE9 device, std::string filePath);
	void draw();
	void clean();

protected:
	LPDIRECT3DDEVICE9       g_pd3dDevice;
	LPDIRECT3DTEXTURE9      g_pTexture = NULL; // Our texture
};

