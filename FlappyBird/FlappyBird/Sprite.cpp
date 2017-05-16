#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}

HRESULT Sprite::init(LPDIRECT3DDEVICE9 device)
{
	g_pd3dDevice = device;

	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer. We are setting the tu and tv texture
	// coordinates, which range from 0.0 to 1.0
	CUSTOMVERTEX* pVertices;
	if (FAILED(g_pVB->Lock(0, 4, (void**)&pVertices, 0)))
		return E_FAIL;

	pVertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	pVertices[0].color = 0xffffffff;
	pVertices[0].tu = 0.0f;
	pVertices[0].tv = 1.0f;

	pVertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	pVertices[1].color = 0xffffffff;
	pVertices[1].tu = 0.0f;
	pVertices[1].tv = 0.0f;

	pVertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	pVertices[2].color = 0xffffffff;
	pVertices[2].tu = 1.0f;
	pVertices[2].tv = 1.0f;

	pVertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	pVertices[3].color = 0xffffffff;
	pVertices[3].tu = 1.0f;
	pVertices[3].tv = 0.0f;

	g_pVB->Unlock();
	
	VOID* pVoid; 
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3

	};

	// create an index buffer interface called i_buffer
	if (FAILED(g_pd3dDevice->CreateIndexBuffer(6 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIB,
		NULL)))
	{
		return E_FAIL;
	}

	// lock i_buffer and load the indices into it
	g_pIB->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	g_pIB->Unlock();

	mBoundingBox.setTopLeft(D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
	mBoundingBox.setBottomRight(D3DXVECTOR3(1.0f, -1.0f, 0.0f));
	


	return S_OK;
}

void Sprite::draw()
{

	

	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}


void Sprite::clean()
{
	if (g_pVB != NULL)
		g_pVB->Release();
}


const BoundingBox* const Sprite::getBoundBox()const 
{
	return &mBoundingBox;
}
