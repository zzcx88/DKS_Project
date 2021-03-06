#include "stdafx.h"
#include "UIManager.h"
#include "CGameObject.h"
#include "CUI.h"
#include "CNumber.h"
#include "CLockOnUI.h"

UIManager::UIManager()
{

	speed.reserve(4);
	alt.reserve(5);
	missile.reserve(3);
	timeMS.reserve(2);
	timeS.reserve(2);
	timeM.reserve(2);
	score.reserve(6);
	distance.reserve(5);
	hp.reserve(3);
	flare.reserve(2);

	FighterOBJs.reserve(50);
	ShipOBJs.reserve(50);

}

UIManager::~UIManager()
{
}

void UIManager::BuildNumberUI() {

	
}
void UIManager::ReleaseUI() {

}
void UIManager::MoveMinimapPoint(ObjectManager::MAPOBJ* PlyList, ObjectManager::MAPOBJ* EneList)
{

	if (PlyList->begin()->second->m_pMUI == NULL)
	{
		CMinimap* pUI;
		pUI = new CMinimap();
		pUI->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_pMinimapPlaneMesh);
		pUI->m_ppMinimapTexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_ppMinimapTexture[3];
		pUI->m_pMinimapMaterial = new CMaterial(1);
		pUI->m_pMinimapMaterial->SetTexture(pUI->m_ppMinimapTexture[0]);
		pUI->m_pMinimapMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui7_minimap_green", OBJ_MINIMAP_PLAYER)->m_pMinimapShader);
		pUI->SetMaterial(0, pUI->m_pMinimapMaterial);
		PlyList->begin()->second->m_pMUI = pUI;
		GET_MANAGER<ObjectManager>()->AddObject(L"MinimapInstance", pUI, OBJ_MINIMAP_PLAYER);
	}
	else
	{
		PlyList->begin()->second->m_pMUI->MoveMinimapPoint(PlyList->begin()->second->GetPosition(), PlyList->begin()->second->m_pMUI);

		if (GET_MANAGER<SceneManager>()->GetSceneStoped() == true)
			PlyList->begin()->second->m_pMUI->SetIsRender(false);
		else
			PlyList->begin()->second->m_pMUI->SetIsRender(true);
	}

	//공중 오브젝트 적은 묶어서 한번에
	for (auto& Ene : *EneList)
	{
		if (Ene.second->m_pMUI == NULL && Ene.second->m_bReffernce == false)
		{
			CMinimap* pUI;
			pUI = new CMinimap();
			pUI->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_pMinimapPlaneMesh);
			pUI->m_ppMinimapTexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_ppMinimapTexture[4];
			pUI->m_pMinimapMaterial = new CMaterial(1);
			pUI->m_pMinimapMaterial->SetTexture(pUI->m_ppMinimapTexture[0]);
			pUI->m_pMinimapMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui10_minimap_red", OBJ_MINIMAP_ENEMY)->m_pMinimapShader);
			pUI->SetMaterial(0, pUI->m_pMinimapMaterial);
			Ene.second->m_pMUI = pUI;

			GET_MANAGER<ObjectManager>()->AddObject(L"MinimapInstance", pUI, OBJ_MINIMAP_ENEMY);

		}

		if (Ene.second->m_bReffernce == false)
		{
			Ene.second->m_pMUI->MoveMinimapPoint(Ene.second->GetPosition(), Ene.second->m_pMUI);

			if (GET_MANAGER<SceneManager>()->GetSceneStoped() == true)
				Ene.second->m_pMUI->SetIsRender(false);
			else
				Ene.second->m_pMUI->SetIsRender(true);
		}

	}

	//지상 오브젝트 적도 묶어서 한번에



	//공중 오브젝트 아군도 묶어서 한번에



	//지상 오브젝트 아군도 묶어서 한번에
}

void UIManager::MoveLockOnUI(ObjectManager::MAPOBJ* PlyList, ObjectManager::MAPOBJ* EneList)
{
	for (int i = 24; i < 29; ++i)
	{
		if (PlyList->begin()->second->ppNumObjects[i] == NULL)
		{
			CNumber* pnum;
			pnum = new CNumber();
			pnum->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_pUIPlaneMesh);
			for (int j = 0; j < GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_nNumTex; ++j)
				pnum->m_ppUITexture[j] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[j];
			pnum->m_pUIMaterial = new CMaterial(1);
			pnum->m_pUIMaterial->SetTexture(pnum->m_ppUITexture[0]);
			pnum->m_pUIMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_pUIShader);
			pnum->SetMaterial(0, pnum->m_pUIMaterial);
			if (24 <= i && i < 29)
			{
				pnum->SetScale(0.6f, 0.6f, 0.f);
			}
			pnum->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[i] = pnum;

			GET_MANAGER<ObjectManager>()->AddObject(L"NumInstance", pnum, OBJ_SPEED_UI);

			
		}
	}

	for (auto& Ene : *EneList)
	{
		if (Ene.second->m_pLockOnUI == NULL && Ene.second->m_bReffernce == false)
		{
			CLockOnUI* pLockOnUI;
			pLockOnUI = new CLockOnUI();
			pLockOnUI->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_LOCKONUI)->m_pLockOnUIPlaneMesh);
			pLockOnUI->m_ppLockOnUITexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_LOCKONUI)->m_ppLockOnUITexture[0];
			pLockOnUI->m_pLockOnUIMaterial = new CMaterial(1);
			pLockOnUI->m_pLockOnUIMaterial->SetTexture(pLockOnUI->m_ppLockOnUITexture[0]);
			pLockOnUI->m_pLockOnUIMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui8_lockon", OBJ_LOCKONUI)->m_pLockOnUIShader);
			pLockOnUI->SetMaterial(0, pLockOnUI->m_pLockOnUIMaterial);
			Ene.second->m_pLockOnUI = pLockOnUI;

			GET_MANAGER<ObjectManager>()->AddObject(L"LockOnInstance", pLockOnUI, OBJ_LOCKONUI);
			if (GET_MANAGER<ObjectManager>()->GetTagFromObj(Ene.second, OBJ_ENEMY) != L"052C")
			{
				FighterOBJs.emplace_back(Ene.second);
			}
			else
			{
				ShipOBJs.emplace_back(Ene.second);
			}
			/*if(GameOBJs.size() > 1)
			sort(begin(GameOBJs), end(GameOBJs), [](const CGameObject* a, const CGameObject* b) {
				return a->m_xmf4x4World._41 < b->m_xmf4x4World._41;
				});*/
		}

		if (Ene.second->m_bReffernce == false)
		{
			//Ene.second->m_pLockOnUI->m_ppMaterials[0]->SetRedShader();

			Ene.second->m_pLockOnUI->MoveLockOnUI(Ene.second->GetScreenPosition(), Ene.second->GetPosition(),
				PlyList->begin()->second->GetPosition(), PlyList->begin()->second->GetLook(), Ene.second->m_pLockOnUI, PlyList->begin()->second->m_pCamera);
			
		
			if (Ene.second->m_pLockOnUI->bDetectable == true)
			{

				if (Ene.second->m_bAiming == true && Ene.second->GetState() != true)
				{
					if (Ene.second->m_pLockOnUI->GetCameraAxis() > 0.f)
					{
						float fx = Ene.second->GetScreenPosition().x - ((float)FRAME_BUFFER_WIDTH / 2.f);
						float fy = (Ene.second->GetScreenPosition().y - ((float)FRAME_BUFFER_HEIGHT / 2.f)) * -1;

						nDistance = Ene.second->LenthToPlayer;
						Ene.second->m_pLockOnUI->m_fLenth = nDistance;
						if (nDistance < 0)
							nDistance = 1;
						//cout << nDistance << " 거리" << endl;
						while (nDistance != 0) {
							distance.emplace_back(nDistance % 10);
							nDistance /= 10;
						}

						//cout << distance.size() << endl;

						if (distance.size() >= 0)
						{
							PlyList->begin()->second->ppNumObjects[24]->SetIsRender(false);
							PlyList->begin()->second->ppNumObjects[25]->SetIsRender(false);
							PlyList->begin()->second->ppNumObjects[26]->SetIsRender(false);
							PlyList->begin()->second->ppNumObjects[27]->SetIsRender(false);
							PlyList->begin()->second->ppNumObjects[28]->SetIsRender(true);
							PlyList->begin()->second->ppNumObjects[28]->SetPosition(fx - 40.f, fy + 20.f, 0.f);

							PlyList->begin()->second->ppNumObjects[28]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[0]];
						}
						if (distance.size() > 1)
						{
							PlyList->begin()->second->ppNumObjects[27]->SetIsRender(true);
							PlyList->begin()->second->ppNumObjects[27]->SetPosition(fx - 32.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[28]->SetPosition(fx - 40.f, fy + 20.f, 0.f);

							PlyList->begin()->second->ppNumObjects[28]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[1]];
							PlyList->begin()->second->ppNumObjects[27]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[0]];

						}
						if (distance.size() > 2)
						{
							PlyList->begin()->second->ppNumObjects[26]->SetIsRender(true);
							PlyList->begin()->second->ppNumObjects[26]->SetPosition(fx - 24.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[27]->SetPosition(fx - 32.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[28]->SetPosition(fx - 40.f, fy + 20.f, 0.f);

							PlyList->begin()->second->ppNumObjects[28]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[2]];
							PlyList->begin()->second->ppNumObjects[27]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[1]];
							PlyList->begin()->second->ppNumObjects[26]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[0]];

						}
						if (distance.size() > 3)
						{
							PlyList->begin()->second->ppNumObjects[25]->SetIsRender(true);
							PlyList->begin()->second->ppNumObjects[25]->SetPosition(fx - 16.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[26]->SetPosition(fx - 24.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[27]->SetPosition(fx - 32.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[28]->SetPosition(fx - 40.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[28]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[3]];
							PlyList->begin()->second->ppNumObjects[27]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[2]];
							PlyList->begin()->second->ppNumObjects[26]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[1]];
							PlyList->begin()->second->ppNumObjects[25]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[0]];

						}
						if (distance.size() > 4)
						{
							PlyList->begin()->second->ppNumObjects[24]->SetIsRender(true);

							PlyList->begin()->second->ppNumObjects[24]->SetPosition(fx - 8.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[25]->SetPosition(fx - 16.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[26]->SetPosition(fx - 24.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[27]->SetPosition(fx - 32.f, fy + 20.f, 0.f);
							PlyList->begin()->second->ppNumObjects[28]->SetPosition(fx - 40.f, fy + 20.f, 0.f);

							PlyList->begin()->second->ppNumObjects[28]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[4]];
							PlyList->begin()->second->ppNumObjects[27]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[3]];
							PlyList->begin()->second->ppNumObjects[26]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[2]];
							PlyList->begin()->second->ppNumObjects[25]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[1]];
							PlyList->begin()->second->ppNumObjects[24]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[distance[0]];


						}

						distance.clear();
					}
					else
					{
						PlyList->begin()->second->ppNumObjects[24]->SetIsRender(false);
						PlyList->begin()->second->ppNumObjects[25]->SetIsRender(false);
						PlyList->begin()->second->ppNumObjects[26]->SetIsRender(false);
						PlyList->begin()->second->ppNumObjects[27]->SetIsRender(false);
						PlyList->begin()->second->ppNumObjects[28]->SetIsRender(false);
					}

					if (Ene.second->m_pLockOnUI->bLockOn == true)
					{

						Ene.second->m_pLockOnUI->m_nTextureRender = 0;
						Ene.second->m_pLockOnUI->m_pLockOnUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()
							->GetObjFromTag(L"player_ui8_lockon", OBJ_LOCKONUI)->m_ppLockOnUITexture[1];

						Ene.second->m_bCanFire = true;
						if (m_bLockOnSoundPlayed == false)
						{
							GET_MANAGER<SoundManager>()->PlaySound(L"LockOn.mp3", CH_LOCKON, true);
							m_bLockOnSoundPlayed = true;
						}
					}
					else
					{
						GET_MANAGER<SoundManager>()->StopSound(CH_LOCKON);
						m_bLockOnSoundPlayed = false;

						Ene.second->m_pLockOnUI->m_pLockOnUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()
							->GetObjFromTag(L"player_ui8_lockon", OBJ_LOCKONUI)->m_ppLockOnUITexture[0];
						Ene.second->m_pLockOnUI->TextureAnimate();

						Ene.second->m_bCanFire = false;
					}

				}
				else
				{
					Ene.second->m_pLockOnUI->m_nTextureRender = 0;
					Ene.second->m_pLockOnUI->m_pLockOnUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()
						->GetObjFromTag(L"player_ui8_lockon", OBJ_LOCKONUI)->m_ppLockOnUITexture[0];
				}

			}
			else
			{
				Ene.second->m_bAiming = false;
				Ene.second->m_pLockOnUI->m_pLockOnUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()
					->GetObjFromTag(L"player_ui8_lockon", OBJ_LOCKONUI)->m_ppLockOnUITexture[0];
			}

			if (GET_MANAGER<SceneManager>()->GetSceneStoped() == true)
				Ene.second->m_pLockOnUI->m_nTextureRender = 1;
			if (GET_MANAGER<SceneManager>()->GetSceneStoped() == true)
			{
				PlyList->begin()->second->ppNumObjects[24]->SetIsRender(false);
				PlyList->begin()->second->ppNumObjects[25]->SetIsRender(false);
				PlyList->begin()->second->ppNumObjects[26]->SetIsRender(false);
				PlyList->begin()->second->ppNumObjects[27]->SetIsRender(false);
				PlyList->begin()->second->ppNumObjects[28]->SetIsRender(false);
			}
		}
	}

	if (m_bFighterType == true)
	{
		for (auto p = ShipOBJs.begin(); p != ShipOBJs.end(); ++p)
		{
			(*p)->m_bAiming = false;
		}
		for (auto p = FighterOBJs.begin(); p != FighterOBJs.end(); ++p)
		{
			if (*p != NULL)
			{
				if ((*p)->GetState() != true)
				{
					if (p == FighterOBJs.begin() + Count)
						(*p)->m_bAiming = true;
					else
					{
						(*p)->m_bAiming = false;
						(*p)->m_bCanFire = false;
					}
				}
				else
				{
					if (FighterOBJs.size() > 1)
					{
						p = FighterOBJs.erase(p);
						Count = 0;
						p = FighterOBJs.begin() + Count;
					}
					else
					{
						FighterOBJs.clear();
						p = FighterOBJs.end();

						break;
					}
			
					sort(begin(FighterOBJs), end(FighterOBJs), [](const CGameObject* a, const CGameObject* b) {
						return a->LenthToPlayer < b->LenthToPlayer;
						});
				}
			}
		}
	}
	else
	{
		for (auto p = FighterOBJs.begin(); p != FighterOBJs.end(); ++p)
		{
			(*p)->m_bAiming = false;
		}
		for (auto p = ShipOBJs.begin(); p != ShipOBJs.end(); ++p)
		{
			if (*p != NULL)
			{
				if ((*p)->GetState() != true)
				{
					if (p == ShipOBJs.begin() + Count)
						(*p)->m_bAiming = true;
					else
					{
						(*p)->m_bAiming = false;
						(*p)->m_bCanFire = false;
					}
				}
				else
				{
					if (ShipOBJs.size() > 1)
					{
						p = ShipOBJs.erase(p);
						Count = 0;
						p = ShipOBJs.begin() + Count;
					}
					else
					{
						ShipOBJs.clear();
						p = ShipOBJs.end();

						break;
					}

					sort(begin(ShipOBJs), end(ShipOBJs), [](const CGameObject* a, const CGameObject* b) {
						return a->LenthToPlayer < b->LenthToPlayer;
						});
				}
			}
		}
	}

	KeyManager* keyManager = GET_MANAGER<KeyManager>();
	DWORD dwDirection = 0;

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_LSHIFT) || true == keyManager->GetPadState(STATE_DOWN, XINPUT_GAMEPAD_BACK))
	{
		dwDirection |= VK_LSHIFT;
		if (m_bFighterType == true)
		{
			m_bFighterType = false;
			Count = 0;
		}
		else
		{
			m_bFighterType = true;
			Count = 0;

		}
	}

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_F) || true == keyManager->GetPadState(STATE_DOWN, XINPUT_GAMEPAD_Y))
	{
		if (m_bFighterType == true)
		{
			dwDirection |= VK_F;
			Count++;
			if (FighterOBJs.size() <= Count)
				Count = 0;
		}
		else
		{
			dwDirection |= VK_F;
			Count++;
			if (ShipOBJs.size() <= Count)
				Count = 0;

		}
		cout << "전투기 사이즈 : " << FighterOBJs.size() << endl;
	}
}


void UIManager::NumberTextureAnimate(ObjectManager::MAPOBJ* PlyList, const float& TimeDelta)
{
	for (int i = 0; i < 34; ++i)
	{
		if (24 <= i && i < 29)
			continue;

		if (PlyList->begin()->second->ppNumObjects[i] == NULL)
		{
			pPlayer = (CPlayer*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER);

			CNumber* pnum;
			pnum = new CNumber();
			pnum->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_pUIPlaneMesh);
			for (int j = 0; j < GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_nNumTex; ++j)
				pnum->m_ppUITexture[j] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[j];
			pnum->m_pUIMaterial = new CMaterial(1);
			pnum->m_pUIMaterial->SetTexture(pnum->m_ppUITexture[0]);
			pnum->m_pUIMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_pUIShader);
			pnum->SetMaterial(0, pnum->m_pUIMaterial);
			
			// Speed font position
			if (i < 4)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui4_speed", OBJ_UI)->GetPosition().x - 18.f + 12.f * i,
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui4_speed", OBJ_UI)->GetPosition().y+ 2.2f, 0.f);
			
			// Alt font position
			if (4 <= i && i < 9)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui5_alt", OBJ_UI)->GetPosition().x - 45.f + 12.f * (i - 3),
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui5_alt", OBJ_UI)->GetPosition().y+ 2.2f, 0.f);
			
			// Missile font position
			if (9 <= i && i < 12)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui2_weapon", OBJ_UI)->GetPosition().x + 10.f + 11.f * (i - 8),
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui2_weapon", OBJ_UI)->GetPosition().y - 4.f, 0.f);
			
			// time M, S, MS font position
			if (12 <= i && i < 14)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui3_time_score", OBJ_UI)->GetPosition().x + 15.f + 14.f * (i - 11),
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui3_time_score", OBJ_UI)->GetPosition().y + 24.f, 0.f);
			if (14 <= i && i < 16)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui3_time_score", OBJ_UI)->GetPosition().x + 48.f + 14.f * (i - 13),
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui3_time_score", OBJ_UI)->GetPosition().y + 24.f, 0.f);
			if (16 <= i && i < 18)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui3_time_score", OBJ_UI)->GetPosition().x + 80.f + 14.f * (i - 15),
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui3_time_score", OBJ_UI)->GetPosition().y + 24.f, 0.f);
			
			// Score font position
			if (18 <= i && i < 24)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui3_time_score", OBJ_UI)->GetPosition().x + 25.f + 14.f * (i - 17),
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui3_time_score", OBJ_UI)->GetPosition().y - 3.f, 0.f);
			
			// Hp font position
			if (29 <= i && i < 32)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui2_weapon", OBJ_UI)->GetPosition().x - 5.f + 11.f * (i - 28),
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui2_weapon", OBJ_UI)->GetPosition().y - 54.f, 0.f);
			
			// flare font position
			if (32 <= i && i < 34)
				pnum->SetPosition(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui2_weapon", OBJ_UI)->GetPosition().x - 12.f + 11.f * (i - 28),
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui2_weapon", OBJ_UI)->GetPosition().y - 29.f, 0.f);

			pnum->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[i] = pnum;

			GET_MANAGER<ObjectManager>()->AddObject(L"NumInstance", pnum, OBJ_SPEED_UI);

		}
	}

	if (GET_MANAGER<SceneManager>()->GetSceneStoped() == true || GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->m_bGameOver)
	{
		for (int i = 0; i < 24; ++i)
			PlyList->begin()->second->ppNumObjects[i]->SetIsRender(false);
		for (int i = 29; i < 34; ++i)
			PlyList->begin()->second->ppNumObjects[i]->SetIsRender(false);
	}
	else
	{
		fElapsedTime -= TimeDelta*100;
		
		if (fElapsedTime <= 0.f)
		{
			fElapsedTime = 99.f;
			n_second -= 1;
			if (n_second <= 0)
			{
				if (n_minute != 0)
					n_minute -= 1;

				n_second = 59;
			}
		}


		flare_number = pPlayer->GetFLRCount();
		hp_number = (int)(fabs(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER)->GetPlayerHp()-5)) * 100/5;
		speed_number = (int)1800.f / 1000.f * PlyList->begin()->second->GetPlayerSpeed();
		alt_number = fabs((int)PlyList->begin()->second->GetPosition().y - 200);
		missile_number = PlyList->begin()->second->GetPlayerMSL();
		score_number = PlyList->begin()->second->GetScore();
		nMsecond = (int)fElapsedTime;
		nSecond = n_second;
		nMin = n_minute;

		while (speed_number != 0) {
			speed.push_back(speed_number % 10);
			speed_number /= 10;
		}
		while (alt_number != 0) {
			alt.push_back(alt_number % 10);
			alt_number /= 10;
		}
		while (missile_number != 0) {
			missile.push_back(missile_number % 10);
			missile_number /= 10;
		}
		while (nMsecond != 0) {
			timeMS.push_back(nMsecond % 10);
			nMsecond /= 10;
		}
		while (nSecond != 0) {
			timeS.push_back(nSecond % 10);
			nSecond /= 10;
		}
		while (nMin != 0) {
			timeM.push_back(nMin % 10);
			nMin /= 10;
		}
		while (score_number != 0) {
			score.push_back(score_number % 10);
			score_number /= 10;
		}
		while (hp_number != 0) {
			hp.push_back(hp_number % 10);
			hp_number /= 10;
		}
		while (flare_number != 0) {
			flare.push_back(flare_number % 10);
			flare_number /= 10;
		}

		// Font animation(flare)
		if (flare.size() == 0)
		{
			PlyList->begin()->second->ppNumObjects[32]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[33]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[33]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
		}
		if (flare.size() > 0)
		{
			PlyList->begin()->second->ppNumObjects[32]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[33]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[33]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[flare[0]];
		}
		if (flare.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[32]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[33]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[32]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[flare[1]];
			PlyList->begin()->second->ppNumObjects[33]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[flare[0]];
		}

		// Font animation(hp)
		if (hp.size() >= 0)
		{
			PlyList->begin()->second->ppNumObjects[29]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[30]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[31]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[31]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
		}
		if (hp.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[29]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[30]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[31]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[30]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[hp[1]];
			PlyList->begin()->second->ppNumObjects[31]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[hp[0]];
		}
		if (hp.size() > 2)
		{
			PlyList->begin()->second->ppNumObjects[29]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[30]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[31]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[29]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[hp[2]];
			PlyList->begin()->second->ppNumObjects[30]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[hp[1]];
			PlyList->begin()->second->ppNumObjects[31]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[hp[0]];
		}
	

		// Font animation(speed)
		if (speed.size() > 0)
		{
			PlyList->begin()->second->ppNumObjects[0]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[1]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[2]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[3]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[0]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[0]];
		}
		if (speed.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[1]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[0]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[1]];
			PlyList->begin()->second->ppNumObjects[1]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[0]];
		}
		if (speed.size() > 2)
		{
			PlyList->begin()->second->ppNumObjects[2]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[0]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[2]];
			PlyList->begin()->second->ppNumObjects[1]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[1]];
			PlyList->begin()->second->ppNumObjects[2]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[0]];
		}
		if (speed.size() > 3)
		{
			PlyList->begin()->second->ppNumObjects[3]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[0]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[3]];
			PlyList->begin()->second->ppNumObjects[1]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[2]];
			PlyList->begin()->second->ppNumObjects[2]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[1]];
			PlyList->begin()->second->ppNumObjects[3]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[speed[0]];
		}

		//// Font animation(alt)
		if (alt.size() > 0)
		{
			PlyList->begin()->second->ppNumObjects[4]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[5]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[6]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[7]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[8]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[8]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[0]];
		}
		if (alt.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[7]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[8]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[0]];
			PlyList->begin()->second->ppNumObjects[7]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[1]];
		}
		if (alt.size() > 2)
		{
			PlyList->begin()->second->ppNumObjects[6]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[8]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[0]];
			PlyList->begin()->second->ppNumObjects[7]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[1]];
			PlyList->begin()->second->ppNumObjects[6]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[2]];
		}
		if (alt.size() > 3)
		{
			PlyList->begin()->second->ppNumObjects[5]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[8]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[0]];
			PlyList->begin()->second->ppNumObjects[7]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[1]];
			PlyList->begin()->second->ppNumObjects[6]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[2]];
			PlyList->begin()->second->ppNumObjects[5]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[3]];
		}
		if (alt.size() > 4)
		{
			PlyList->begin()->second->ppNumObjects[4]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[8]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[0]];
			PlyList->begin()->second->ppNumObjects[7]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[1]];
			PlyList->begin()->second->ppNumObjects[6]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[2]];
			PlyList->begin()->second->ppNumObjects[5]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[3]];
			PlyList->begin()->second->ppNumObjects[4]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[alt[4]];
		}

		//// Font animation(missile)
		if (missile.size() == 0)
		{
			PlyList->begin()->second->ppNumObjects[9]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[10]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[11]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[11]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
		}

		if (missile.size() > 0)
		{
			PlyList->begin()->second->ppNumObjects[9]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[10]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[11]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[11]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[missile[0]];
		}
		if (missile.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[10]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[10]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[missile[1]];
			PlyList->begin()->second->ppNumObjects[11]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[missile[0]];
		}
		if (missile.size() > 2)
		{
			PlyList->begin()->second->ppNumObjects[9]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[9]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[missile[2]];
			PlyList->begin()->second->ppNumObjects[10]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[missile[1]];
			PlyList->begin()->second->ppNumObjects[11]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[missile[0]];
		}

		// Font animation(time hour)
		if (timeM.size() == 0)
		{
			PlyList->begin()->second->ppNumObjects[12]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[13]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[13]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
		}
		if (timeM.size() > 0)
		{
			PlyList->begin()->second->ppNumObjects[12]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[13]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[12]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
			PlyList->begin()->second->ppNumObjects[13]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeM[0]];
		}
		if (timeM.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[12]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeM[1]];
			PlyList->begin()->second->ppNumObjects[13]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeM[0]];
		}

		// Font animation(time min)
		if (timeS.size() == 0)
		{
			PlyList->begin()->second->ppNumObjects[14]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[15]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[14]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
			PlyList->begin()->second->ppNumObjects[15]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
		}
		if (timeS.size() > 0)
		{
			PlyList->begin()->second->ppNumObjects[14]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[15]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[14]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
			PlyList->begin()->second->ppNumObjects[15]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeS[0]];
		}
		if (timeS.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[14]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeS[1]];
			PlyList->begin()->second->ppNumObjects[15]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeS[0]];
		}

		// Font animation(time sec)
		if (timeMS.size() > 0)
		{
			PlyList->begin()->second->ppNumObjects[16]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[17]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[17]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeMS[0]];
		}
		if (timeMS.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[16]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeMS[1]];
			PlyList->begin()->second->ppNumObjects[17]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[timeMS[0]];
		}

		// Font animation(score)
		if (score.size() == 0)
		{
			PlyList->begin()->second->ppNumObjects[18]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[19]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[20]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[21]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[22]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[23]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[23]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[0];
		}
		if (score.size() > 0)
		{
			PlyList->begin()->second->ppNumObjects[18]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[19]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[20]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[21]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[22]->SetIsRender(false);
			PlyList->begin()->second->ppNumObjects[23]->SetIsRender(true);

			PlyList->begin()->second->ppNumObjects[23]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[0]];
		}
		if (score.size() > 1)
		{
			PlyList->begin()->second->ppNumObjects[22]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[22]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[1]];
			PlyList->begin()->second->ppNumObjects[23]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[0]];
		}
		if (score.size() > 2)
		{
			PlyList->begin()->second->ppNumObjects[21]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[21]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[2]];
			PlyList->begin()->second->ppNumObjects[22]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[1]];
			PlyList->begin()->second->ppNumObjects[23]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[0]];
		}
		if (score.size() > 3)
		{
			PlyList->begin()->second->ppNumObjects[20]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[20]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[3]];
			PlyList->begin()->second->ppNumObjects[21]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[2]];
			PlyList->begin()->second->ppNumObjects[22]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[1]];
			PlyList->begin()->second->ppNumObjects[23]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[0]];
		}
		if (score.size() > 4)
		{
			PlyList->begin()->second->ppNumObjects[19]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[19]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[4]];
			PlyList->begin()->second->ppNumObjects[20]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[3]];
			PlyList->begin()->second->ppNumObjects[21]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[2]];
			PlyList->begin()->second->ppNumObjects[22]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[1]];
			PlyList->begin()->second->ppNumObjects[23]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[0]];
		}
		if (score.size() > 5)
		{
			PlyList->begin()->second->ppNumObjects[18]->SetIsRender(true);
			PlyList->begin()->second->ppNumObjects[18]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[5]];
			PlyList->begin()->second->ppNumObjects[19]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[4]];
			PlyList->begin()->second->ppNumObjects[20]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[3]];
			PlyList->begin()->second->ppNumObjects[21]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[2]];
			PlyList->begin()->second->ppNumObjects[22]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[1]];
			PlyList->begin()->second->ppNumObjects[23]->m_pUIMaterial->m_ppTextures[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui11_speed_number_o", OBJ_SPEED_UI)->m_ppUITexture[score[0]];
		}

		speed.clear();
		alt.clear();
		missile.clear();
		timeMS.clear();
		timeS.clear();
		timeM.clear();
		score.clear();
		hp.clear();
		flare.clear();

	}

}