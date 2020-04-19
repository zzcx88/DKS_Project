#include "stdafx.h"
#include "MinimapManager.h"
#include "CGameObject.h"
#include "CUI.h"
#include "CPlaneMesh.h"

MinimapManager::MinimapManager() 
{
}

MinimapManager::~MinimapManager()
{

}

void MinimapManager::MoveMinimapPoint(ObjectManager::MAPOBJ* PlyList, ObjectManager::MAPOBJ* PPntList, ObjectManager::MAPOBJ* EneList, ObjectManager::MAPOBJ* EPntList)
{
	//////////////////////////�̰��� �ּ��� �κ��� ���� �ø� ������Ʈ �Ŵ����� �÷��̾� ���� �۾��� ��ģ Ŀ���� Ǯ ���� �� ����Ѵ� //////////////////////////
	if (PlyList->begin()->second->m_pUI == NULL)
	{
		//�÷��̾ �̴ϸ� ����Ʈ�� ���ٸ� ����
		CUI* pUI;
		pUI = new CUI();
		pUI->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_pUIPlaneMesh);
		//�ݺ��� ���� �ʿ� ����
		pUI->m_ppUITexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_ppUITexture[7];
		pUI->m_pUIMaterial = new CMaterial(1);
		pUI->m_pUIMaterial->SetTexture(pUI->m_ppUITexture[7]);
		pUI->m_pUIMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_pUIShader);
		pUI->SetMaterial(0, pUI->m_pUIMaterial);
		GET_MANAGER<ObjectManager>()->AddObject(L"MinimapInstance", pUI, OBJ_MINIMAP_PLAYER);

		//������Ʈ ���� m_pUI�� ������ pUI�� �ִ´�. CGameObject::CollisionActive�Լ����� ������ �����ϱ� ����
		PlyList->begin()->second->m_pUI = pUI;
	}
	else
	{
		//�÷��̾ �̴ϸ� ����Ʈ�� �ִٸ� ����
		PlyList->begin()->second->m_pUI->MoveMinimapPoint(PlyList->begin()->second->GetPosition(), PlyList->begin()->second->m_pUI);
	}
	/////////////////////////�̰��� �ּ��� �κ��� ���� �ø� ������Ʈ �Ŵ����� �÷��̾� ���� �۾��� ��ģ Ŀ���� Ǯ ���� �� ����Ѵ�///////////////////////////


	//���� ������Ʈ ���� ��� �ѹ���
	for (auto& Dst : *EneList)
	{
		//�� ������Ʈ�� m_pUI�� ������� ��쿣 ���� pUI�� �����Ҵ��Ѵ�.
		if (Dst.second->m_pUI == NULL)
		{
			CUI* pUI;
			pUI = new CUI();
			pUI->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_pUIPlaneMesh);
			//�ݺ��� ���� �ʿ� ����
			pUI->m_ppUITexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_ppUITexture[8];
			pUI->m_pUIMaterial = new CMaterial(1);
			pUI->m_pUIMaterial->SetTexture(pUI->m_ppUITexture[8]);
			pUI->m_pUIMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_pUIShader);
			pUI->SetMaterial(0, pUI->m_pUIMaterial);
			GET_MANAGER<ObjectManager>()->AddObject(L"MinimapInstance", pUI, OBJ_MINIMAP_ENEMY);

			//������Ʈ ���� m_pUI�� ������ pUI�� �ִ´�. CGameObject::CollisionActive�Լ����� ������ �����ϱ� ����
			Dst.second->m_pUI = pUI;
		}
		//������Ʈ ���� m_pUI�� �Լ��� ���� ��ǥ������ ȣ���Ѵ�.
		Dst.second->m_pUI->MoveMinimapPoint(Dst.second->GetPosition(),Dst.second->m_pUI);
	}
	//���� ������Ʈ ���� ��� �ѹ���



	//���� ������Ʈ �Ʊ��� ��� �ѹ���

	

	//���� ������Ʈ �Ʊ��� ��� �ѹ���
}