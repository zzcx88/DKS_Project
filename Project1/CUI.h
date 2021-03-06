#pragma once
#include "CPlane.h"


class CUI : public CPlane
{

public:
	CUI();
	CUI(int nIndex, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, float fWidth, float fHeight, float fDepth, XMFLOAT2 xmf2LeftTop, XMFLOAT2 xmf2LeftBot, XMFLOAT2 xmf2RightBot, XMFLOAT2 xmf2RightTop);
	virtual ~CUI();

	void MoveMinimapPoint(XMFLOAT3& xmfPlayer, CGameObject* pGameOBJ);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
};

