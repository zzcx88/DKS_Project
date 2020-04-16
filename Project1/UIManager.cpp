#include "stdafx.h"
#include "UIManager.h"
#include "CGameObject.h"
#include "CUI.h"
#include "CLockOnUI.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::MoveMinimapPoint(ObjectManager::MAPOBJ* PlyList, ObjectManager::MAPOBJ* EneList)
{
	////////////////////////�̰��� �ּ��� �κ��� ���� �ø� ������Ʈ �Ŵ����� �÷��̾� ���� �۾��� ��ģ Ŀ���� Ǯ ���� �� ����Ѵ� //////////////////////////
	if (PlyList->begin()->second->m_pUI == NULL)
	{
		//�÷��̾ �̴ϸ� ����Ʈ�� ���ٸ� ����
		CUI* pUI;
		pUI = new CUI();
		pUI->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_pUIPlaneMesh);
		//pUI->m_ppUITexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_ppUITexture[6];
		pUI->m_pUIMaterial = new CMaterial(1);
		pUI->m_pUIMaterial->SetTexture(pUI->m_ppUITexture[6]);
		pUI->m_pUIMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_pUIShader);
		pUI->SetMaterial(0, pUI->m_pUIMaterial);
		GET_MANAGER<ObjectManager>()->AddObject(L"MinimapInstance", pUI, OBJ_MINIMAP_PLAYER);
		PlyList->begin()->second->m_pUI = pUI;
	}
	else
	{
		//�÷��̾ �̴ϸ� ����Ʈ�� �ִٸ� ����
		PlyList->begin()->second->m_pUI->MoveMinimapPoint(PlyList->begin()->second->GetPosition(), PlyList->begin()->second->m_pUI);
	}
	///////////////////////�̰��� �ּ��� �κ��� ���� �ø� ������Ʈ �Ŵ����� �÷��̾� ���� �۾��� ��ģ Ŀ���� Ǯ ���� �� ����Ѵ�///////////////////////////


	//���� ������Ʈ ���� ��� �ѹ���
	for (auto& Ene : *EneList)
	{
		//�� ������Ʈ�� m_pUI�� ������� ��쿣 ���� pUI�� �����Ҵ��Ѵ�.
		if (Ene.second->m_pUI == NULL)
		{
			CUI* pUI;
			pUI = new CUI();
			pUI->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_pUIPlaneMesh);
			pUI->m_ppUITexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_ppUITexture[8];
			pUI->m_pUIMaterial = new CMaterial(1);
			pUI->m_pUIMaterial->SetTexture(pUI->m_ppUITexture[8]);
			pUI->m_pUIMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_pUIShader);
			pUI->SetMaterial(0, pUI->m_pUIMaterial);
			GET_MANAGER<ObjectManager>()->AddObject(L"MinimapInstance", pUI, OBJ_MINIMAP_ENEMY);

			//������Ʈ ���� m_pUI�� ������ pUI�� �ִ´�. CGameObject::CollisionActive�Լ����� ������ �����ϱ� ����
			Ene.second->m_pUI = pUI;
		}
		//������Ʈ ���� m_pUI�� �Լ��� ���� ��ǥ������ ȣ���Ѵ�.
		Ene.second->m_pUI->MoveMinimapPoint(Ene.second->GetPosition(), Ene.second->m_pUI);
	}
	//���� ������Ʈ ���� ��� �ѹ���



	//���� ������Ʈ �Ʊ��� ��� �ѹ���

	

	//���� ������Ʈ �Ʊ��� ��� �ѹ���
}

void UIManager::MoveLockOnUI(ObjectManager::MAPOBJ* PlyList, ObjectManager::MAPOBJ* EneList)
{
	for (auto& Ene : *EneList)
	{
		if (Ene.second->m_pLockOnUI == NULL)
		{
			CLockOnUI* pLockOnUI;
			pLockOnUI = new CLockOnUI();
			pLockOnUI->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_UI)->m_pLockOnUIPlaneMesh);
			pLockOnUI->m_ppLockOnUITexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_UI)->m_ppLockOnUITexture[0];
			pLockOnUI->m_pLockOnUIMaterial = new CMaterial(1);
			pLockOnUI->m_pLockOnUIMaterial->SetTexture(pLockOnUI->m_ppLockOnUITexture[0]);
			pLockOnUI->m_pLockOnUIMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_UI)->m_pLockOnUIShader);
			pLockOnUI->SetMaterial(0, pLockOnUI->m_pLockOnUIMaterial);
			GET_MANAGER<ObjectManager>()->AddObject(L"LockOnInstance", pLockOnUI, OBJ_UI);
			
			Ene.second->m_pLockOnUI = pLockOnUI;
		}
	
		Ene.second->m_pLockOnUI->MoveLockOnUI(Ene.second->GetScreenPosition(), Ene.second->GetPosition(),
			PlyList->begin()->second->GetPosition(), PlyList->begin()->second->GetLook(), Ene.second->m_pLockOnUI);

		if (Ene.second->m_pLockOnUI->LockOn == true)
		{
			cout << "���� ����" << endl;
			if (Count == 0)
			{
				m_fMinLenth = Ene.second->m_pLockOnUI->m_fLenth;
				Ene.second->m_pLockOnUI->m_pLockOnUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_UI)->m_ppLockOnUITexture[1];
				++Count;
			}
			else
			{
				m_fMinLenth = min(m_fMinLenth, Ene.second->m_pLockOnUI->m_fLenth);

				if(m_fMinLenth == Ene.second->m_pLockOnUI->m_fLenth)
								Ene.second->m_pLockOnUI->m_pLockOnUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_UI)->m_ppLockOnUITexture[1];
				else 
					Ene.second->m_pLockOnUI->m_pLockOnUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_UI)->m_ppLockOnUITexture[0];
			}

		}
		else
		{
			cout << "���� �Ұ���" << endl;
			Ene.second->m_pLockOnUI->m_pLockOnUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_UI)->m_ppLockOnUITexture[0];
		}

	}

}