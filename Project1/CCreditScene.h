#pragma once
#include "CScene.h"
#include "CPlayer.h"
#include "CPlane.h"
#include "CUI.h"
#include "CLockOnUI.h"

class CCreditScene : public CScene
{
public:
	CCreditScene();
	~CCreditScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	void BuildDefaultLightsAndMaterials() {};
	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void CreateStageObject() {}
	void ReleaseObjects();

	bool ProcessInput(UCHAR* pKeysBuffer);
	void AnimateObjects(float fTimeElapsed);
	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) {}
	virtual void OnPreRender(ID3D12Device* pd3dDevice, ID3D12CommandQueue* pd3dCommandQueue, ID3D12Fence* pd3dFence, HANDLE hFenceEvent) {}
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL, bool bPreRender = false, ID3D12Resource* pCurrentBackBuffer = NULL);

	void ReleaseUploadBuffers();

public:
	float fx, fy = 0.f;

	float								m_fElapsedTime = 0.0f;
	int m_nMinute = 0;
	int m_nHour = 0;
	bool m_bSceneStart = true;

	int									m_nGameObjects = 0;
	CGameObject** m_ppGameObjects = NULL;

	int									m_nShaders = 0;
	CShader** m_ppShaders = NULL;


	CGameTimer					m_GameTimer;

};

