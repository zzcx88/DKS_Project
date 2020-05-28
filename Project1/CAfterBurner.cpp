#include "stdafx.h"
#include "CAfterBurner.h"
#include "CPlaneMesh.h"
#include "CTestScene.h"
#include "CShaderManager.h"

#define TEXTURES 10

CAfterBurner::CAfterBurner()
{
}

CAfterBurner::CAfterBurner(int nIndex, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, float fWidth, float fHeight, float fDepth)
{
	m_pPlaneMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, fWidth, fHeight, fDepth, XMFLOAT2(0, 0), XMFLOAT2(0, 0), XMFLOAT2(0, 0), XMFLOAT2(0, 0));

	SetMesh(m_pPlaneMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pEffectTexture[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[0]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle.dds", 0);
	m_pEffectTexture[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[1]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle1.dds", 0);
	m_pEffectTexture[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[2]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle2.dds", 0);
	m_pEffectTexture[3] = new CTexture(1, RESOURCE_TEXTURE2D, 0);
	m_pEffectTexture[3]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle3.dds", 0);
	m_pEffectTexture[4] = new CTexture(1, RESOURCE_TEXTURE2D, 0);				
	m_pEffectTexture[4]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle4.dds", 0);
	m_pEffectTexture[5] = new CTexture(1, RESOURCE_TEXTURE2D, 0);			
	m_pEffectTexture[5]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle5.dds", 0);
	m_pEffectTexture[6] = new CTexture(1, RESOURCE_TEXTURE2D, 0);				
	m_pEffectTexture[6]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle6.dds", 0);
	m_pEffectTexture[7] = new CTexture(1, RESOURCE_TEXTURE2D, 0);	
	m_pEffectTexture[7]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle7.dds", 0);
	m_pEffectTexture[8] = new CTexture(1, RESOURCE_TEXTURE2D, 0);	
	m_pEffectTexture[8]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle8.dds", 0);
	m_pEffectTexture[9] = new CTexture(1, RESOURCE_TEXTURE2D, 0);	
	m_pEffectTexture[9]->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Effect/AfterBurner/AfterBurn_Circle9.dds", 0);

	UINT ncbElementBytes = ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255);
	m_PlaneShader = new CPlaneShader();

	m_PlaneShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	m_PlaneShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	for (int i = 0; i < TEXTURES; i++) CTestScene::CreateShaderResourceViews(pd3dDevice, m_pEffectTexture[i], 15, false);
	CTestScene::CreateShaderResourceViews(pd3dDevice, m_pEffectTexture[0], 15, false);
	m_pEffectMaterial = new CMaterial(1);
	m_pEffectMaterial->SetTexture(m_pEffectTexture[0]);
	m_pEffectMaterial->SetShader(m_PlaneShader);
	SetMaterial(0, m_pEffectMaterial);
}

CAfterBurner::~CAfterBurner()
{
}

void CAfterBurner::Animate(float fTimeElapsed)
{
	if (!m_bRefference)
	{
		//m_fBurnerBlendAmount = 0.3f;
		/*m_xmf3Position.x = m_xmf4x4ToParent._41;
		m_xmf3Position.y = m_xmf4x4ToParent._42;
		m_xmf3Position.z = m_xmf4x4ToParent._43;*/
		//SetLookAt(m_pCamera->GetPosition());
	}
}

void CAfterBurner::TextureAnimate()
{
	//�ش� ������Ʈ�� ���۷��� ������Ʈ�� �ƴҰ�쿡�� �ִϸ���Ʈ�� ����
	if (!m_bRefference)
	{
		//m_fFadeFrequence : �ؽ��� ��ü�ֱ�, �󸶸�ŭ�� �ֱ⿡ ���� ��ų�� ���ϴ� ���� �����ϸ� ��
		if (m_fFadeTimeElapsed > m_fFadeFrequence)
		{
			//�ؽ��� ��ü�� �Ͼ��
			m_pEffectMaterial->m_ppTextures[0] = m_pEffectTexture[m_nTextureIndex];
			//�ؽ��� ��ü �ֱ⸦ üũ�ϱ� ���� ���� �ð��� �ٽ� 0���� �ʱ�ȭ �Ѵ�
			m_fFadeTimeElapsed = 0.f;
			//�ؽ��� ��ü�� �غ�� �ؽ��� ���� ��ŭ�� ��ü �ǵ��� �Ѵ�.
			if (m_nTextureIndex < TEXTURES)
			{
				m_nTextureIndex++;
			}
			//else �κ��� ����ϰ� ������ ����ϸ� ��, �Ʒ� �ڵ�� ��� �ؽ��� �ִϸ��̼��� ������ ������Ʈ�� delete�ϴ� �ڵ���
			else
			{
				m_isDead = true;
			}
		}
	}
}

void CAfterBurner::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	if(!m_RenderOff)
		CGameObject::Render(pd3dCommandList, pCamera);
}
