#pragma once

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
class Geometry
{
	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 position; // The position
		D3DCOLOR color;    // The color
#ifndef SHOW_HOW_TO_USE_TCI
		FLOAT tu, tv;   // The texture coordinates
#endif
	};
public:
	Geometry();
	~Geometry();

	HRESULT init(LPDIRECT3DDEVICE9 device);
	void draw();
	void clean();




protected:
	LPDIRECT3DDEVICE9       g_pd3dDevice = nullptr;
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices
	LPDIRECT3DINDEXBUFFER9 g_pIB = NULL;
};

