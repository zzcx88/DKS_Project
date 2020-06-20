#include "stdafx.h"
#include "SceneManager.h"
#include "CTestScene.h"
#include "CMenuScene.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	Release();
}

bool SceneManager::ChangeSceneState(SCENESTATE SceneState, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_Scene) m_Scene->ReleaseUploadBuffers();
	if (m_Scene) m_Scene->ReleaseObjects();

	if (nullptr != m_Scene)
	{
		Release();
	}

	if (SceneState == SCENE_MENU)
		GET_MANAGER<UIManager>()->ReleaseUI();
	switch (SceneState)
	{
	case SCENE_TEST:
		m_Scene = new CTestScene;
		break;
	case SCENE_MENU:
		m_Scene = new CMenuScene;
		break;
	}

	if (nullptr == m_Scene)
		return false;
	SetSceneObjManager();

	m_Scene->BuildObjects(pd3dDevice, pd3dCommandList);
	if (SceneState == SCENE_TEST)
		GET_MANAGER<UIManager>()->BuildNumberUI();

	m_CurrentScene = SceneState;

	return true;
}
bool SceneManager::SwapSceneState(SCENESTATE SceneState)
{
	switch (SceneState)
	{
	case SCENE_TEST:
		m_Scene = tScene;
		break;
	case SCENE_MENU:
		m_Scene =  mScene;
		break;
	}

	if (nullptr == m_Scene)
		return false;

	SetSceneObjManager();

	m_CurrentScene = SceneState;

	return true;
}
void SceneManager::SetSceneObjManager()
{
	m_Scene->m_ObjManager = GET_MANAGER<ObjectManager>();
}

void SceneManager::SetObjManagerInPlayer()
{
	m_Scene->m_pPlayer->m_ObjManager = GET_MANAGER<ObjectManager>();
	GET_MANAGER<ObjectManager>()->AddObject(L"player", m_Scene->m_pPlayer, OBJ_PLAYER);
}

int SceneManager::Update(const float& TimeDelta)
{
	m_Scene->AnimateObjects(TimeDelta);
	return 0;
}

void SceneManager::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	m_Scene->Render(pd3dCommandList, pCamera);
}

void SceneManager::ReleaseUploadBuffers()
{
	m_Scene->ReleaseUploadBuffers();
}

void SceneManager::Release()
{
	if (m_Scene)
	{
		delete m_Scene;
		m_Scene = nullptr;
	}
}
void SceneManager::SetStoped(bool b)
{
	m_Scene->SetStoped(b);
}
bool SceneManager::GetStoped()
{
	return m_Scene->GetStoped();
}
void SceneManager::SceneStoped()
{

	KeyManager* keyManager = GET_MANAGER<KeyManager>();
	if (true == keyManager->GetKeyState(STATE_DOWN, VK_G))
	{
		if (m_Scene->GetStoped() == false)
		{
			m_Scene->SetStoped(true);
			for (auto p = m_Scene->m_ObjManager->GetObjFromType(OBJ_UI).begin(); p != m_Scene->m_ObjManager->GetObjFromType(OBJ_UI).end(); ++p)
			{
				(*p).second->SetIsRender(false);
			}

		}
		else
		{
			m_Scene->SetStoped(false);
			for (auto p = m_Scene->m_ObjManager->GetObjFromType(OBJ_UI).begin(); p != m_Scene->m_ObjManager->GetObjFromType(OBJ_UI).end(); ++p)
			{
				(*p).second->SetIsRender(true);
			}

		}
	}
}
