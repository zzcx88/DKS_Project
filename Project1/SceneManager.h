#pragma once
#include "SingletonBase.h"
#include "CScene.h"
#include "CCamera.h"
#include "CPlayer.h"

class SceneManager : public SingletonBase<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

public:
	SCENESTATE GetCurrentSceneState() { return m_CurrentScene; }

	bool GetSceneStoped() { return m_Scene->GetStoped(); }
	ID3D12RootSignature* GetGraphicsRootSignature() { return m_Scene->GetGraphicsRootSignature(); }
	ID3D12RootSignature* GetComputeRootSignature() { return m_Scene->GetComputeRootSignature(); }
	ID3D12DescriptorHeap* GetCbvSrvDescriptorHeap() { return m_Scene->GetCbvSrvDescriptorHeap(); }

public:
	bool SwapSceneState(SCENESTATE SceneState);
	bool ChangeSceneState(SCENESTATE SceneState, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void SetSceneObjManager();
	void SetPlayer(CPlayer* pPlayer) { m_Scene->m_pPlayer = pPlayer; }
	void SetObjManagerInPlayer();
	void SceneStoped();
	void SetStoped(bool);
	bool GetStoped();

	int m_nWave = 0;
	int m_nWaveCnt = 0;
	int m_nTgtObject = 0;

	bool m_bStageClear = false;
	bool m_bStageFail = false;
	bool m_bCreateShip = false;

public:
	int Update(const float& TimeDelta);

	void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void OnPreRender(ID3D12Device* pd3dDevice, ID3D12CommandQueue* pd3dCommandQueue, ID3D12Fence* pd3dFence, HANDLE hFenceEvent);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, bool bPreRender = false);
	void ReleaseUploadBuffers();

	void Release();

private:
	CScene* m_Scene = nullptr;
	CScene* tScene = nullptr;
	CScene* mScene = nullptr;

	SCENESTATE m_CurrentScene = SCENE_END;
};

