#pragma once
#include "CGameObject.h"

class CTU160 : public CGameObject
{
public:
	CTU160();
	CTU160(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CTU160();

	XMFLOAT3 m_xmf3FallingPoint = XMFLOAT3(0, 0, 0);

	float m_fMoveFowardElapsed = 0.f;
	float m_fElapsedFrequency = 3.f;

	float m_fMissleFireFrequence = 10.0f;
	float m_fAfterFireFrequence = 4.f;
	float m_fMissleFireElapsed = 0.f;

	ObjectManager* m_ObjManager;

private:
	CGameObject* m_pLeftEngine1 = NULL;
	CGameObject* m_pLeftEngine2 = NULL;
	CGameObject* m_pRightEngine1 = NULL;
	CGameObject* m_pRightEngine2 = NULL;

	float m_fAddFogFrequence = 0.001f;
	float m_fAddFogTimeElapsed = 0.f;

	float m_fAddCrushFogFrequence = 0.05f;
	float m_fAddCrushFogTimeElapsed = 0.f;

public:
	virtual void OnPrepareAnimate();
	virtual void Animate(float fTimeElapsed);
	void CollisionActivate(CGameObject* collideTarget);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
};