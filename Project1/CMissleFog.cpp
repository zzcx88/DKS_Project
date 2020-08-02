#include "stdafx.h"
#include "CMissleFog.h"
#include "CPlaneMesh.h"
#include "CTestScene.h"
#include "CShaderManager.h"

CMissleFog::CMissleFog()
{
}

CMissleFog::CMissleFog(int nIndex, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, float fWidth, float fHeight, float fDepth) : CPlane()
{
	m_bReffernce = true;
	m_fBurnerBlendAmount = 1;
	m_pPlaneMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, fWidth, fHeight, fDepth, XMFLOAT2(0, 0), XMFLOAT2(0, 0), XMFLOAT2(0, 0), XMFLOAT2(0, 0));

	SetMesh(m_pPlaneMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	pWaterTexture = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	pWaterTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/water_base.dds", 0);

	//m_pEffectTexture[TEXTURES];
	m_pEffectTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleSmoke.dds", 0);
	m_pEffectTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/CrushSmoke.dds", 0);
	/*m_ppLockOnUITexture[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleFog2.dds", 0);
	m_pEffectTexture[3] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[3]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleSmoke3.dds", 0);
	m_pEffectTexture[4] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[4]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleSmoke4.dds", 0);*/
	/*m_pEffectTexture[5] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[5]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleFog5.dds", 0);
	m_pEffectTexture[6] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[6]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleFog6.dds", 0);
	m_pEffectTexture[7] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[7]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleFog7.dds", 0);
	m_pEffectTexture[8] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[8]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleFog8.dds", 0);
	m_pEffectTexture[9] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[9]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/MissleFog9.dds", 0);*/

 	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	m_EffectShader = new CMissleFogShader();

	m_EffectShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_EffectShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_EffectShader->CreateConstantBufferViews(pd3dDevice, pd3dCommandList, m_nObjects, m_EffectShader->m_pd3dcbBlendAmount, ncbElementBytes);

	for (int i = 0; i < 2; i++) CTestScene::CreateShaderResourceViews(pd3dDevice, m_pEffectTexture[i], 15, false);

	m_pEffectMaterial = new CMaterial(1);
	m_pEffectMaterial->SetTexture(m_pEffectTexture[0]);
	m_pEffectMaterial->SetShader(m_EffectShader);

	SetMaterial(0, m_pEffectMaterial);
}

CMissleFog::~CMissleFog()
{
}

void CMissleFog::Animate(float fTimeElapsed)
{
	m_xmf3Position.x = m_xmf4x4ToParent._41;
	m_xmf3Position.y = m_xmf4x4ToParent._42;
	m_xmf3Position.z = m_xmf4x4ToParent._43;

	//�ð� �̺�Ʈ�� ���� ���� �ð�
	m_fTimeElapsed += fTimeElapsed;
	m_fFadeTimeElapsed += fTimeElapsed;


	if (!m_bReffernce)
	{
		//if (m_fScaleX < 30)
		//{
		//	SetScale(m_fScaleX += m_fTimeElapsed / 2, m_fScaleY += m_fTimeElapsed / 2, 1);
		//}
		//else
		//{
		//	//�������� 7�� ���� �� �� �ؽ��� �ִϸ��̼��� �����Ѵ�.
		//	SetScale(30, 30, 0);
		//	/*if (m_fTimeElapsed > m_fDeleteFogFrequence)
		//	TextureAnimate();*/
		//}
		if (m_bWingFog)
		{
			if (m_fScaleX > 0.1)
				SetScale(m_fScaleX -= m_fTimeElapsed / 20, m_fScaleY -= m_fTimeElapsed / 20, 1);
			else
				SetScale(0.1,0.1,0);
		}
		else if (m_bFlareFog)
		{
			if (m_fScaleX < 25)
				SetScale(m_fScaleX += 1.1 * m_fTimeElapsed, m_fScaleY += 1.1 * m_fTimeElapsed, 1);
			else
				SetScale(25, 25, 0);
		}
		else
		{
			if (m_fScaleX < 50)
				SetScale(m_fScaleX += 1.1 * m_fTimeElapsed , m_fScaleY += 1.1 * m_fTimeElapsed, 1);
			else
				SetScale(50, 50, 0);
		}
		TextureAnimate();
		SetLookAt(m_pCamera->GetPosition());
	}
}

void CMissleFog::TextureAnimate()
{
	if (m_bWingFog)
	{
		if (m_fBurnerBlendAmount >= 0)
			m_fBurnerBlendAmount -= 0.001 * m_fTimeElapsed;
		else
			m_isDead = true;
	}
	else
	{
		if (m_fBurnerBlendAmount >= 0)
			m_fBurnerBlendAmount -= 0.001 * m_fTimeElapsed;
		else
			m_isDead = true;
	}
	////�ش� ������Ʈ�� ���۷��� ������Ʈ�� �ƴҰ�쿡�� �ִϸ���Ʈ�� ����
	//if (!m_bRefference)
	//{
	//	//m_fFadeFrequence : �ؽ��� ��ü�ֱ�, �󸶸�ŭ�� �ֱ⿡ ���� ��ų�� ���ϴ� ���� �����ϸ� ��
	//	if (m_fFadeTimeElapsed > m_fFadeFrequence)
	//	{
	//		//�ؽ��� ��ü�� �Ͼ��
	//		m_pEffectMaterial->m_ppTextures[0] = m_pEffectTexture[m_nTextureIndex];
	//		//�ؽ��� ��ü �ֱ⸦ üũ�ϱ� ���� ���� �ð��� �ٽ� 0���� �ʱ�ȭ �Ѵ�
	//		m_fFadeTimeElapsed = 0.f;
	//		//�ؽ��� ��ü�� �غ�� �ؽ��� ���� ��ŭ�� ��ü �ǵ��� �Ѵ�.
	//		if (m_nTextureIndex < TEXTURES)
	//		{
	//			m_nTextureIndex++;
	//		}
	//		//else �κ��� ����ϰ� ������ ����ϸ� ��, �Ʒ� �ڵ�� ��� �ؽ��� �ִϸ��̼��� ������ ������Ʈ�� delete�ϴ� �ڵ���
	//		else
	//		{
	//			m_isDead = true;
	//		}
	//	}
	//}
}

void CMissleFog::SetLookAt(XMFLOAT3& xmfTarget)
{
	XMFLOAT3 xmfUp(0.0f, 1.0f, 0.0f);
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(xmfTarget, m_xmf3Position, xmfUp);
	m_xmf3Right = XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_xmf3Up = XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_xmf3Look = XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
	m_xmf4x4ToParent._11 = m_xmf3Right.x;			m_xmf4x4ToParent._12 = m_xmf3Right.y;		m_xmf4x4ToParent._13 = m_xmf3Right.z;
	m_xmf4x4ToParent._21 = m_xmf3Up.x;			m_xmf4x4ToParent._22 = m_xmf3Up.y;		m_xmf4x4ToParent._23 = m_xmf3Up.z;
	m_xmf4x4ToParent._31 = m_xmf3Look.x;		m_xmf4x4ToParent._32 = m_xmf3Look.y;	m_xmf4x4ToParent._33 = m_xmf3Look.z;
}

void CMissleFog::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	CGameObject::Render(pd3dCommandList, pCamera);
}
