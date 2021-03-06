#pragma once
#include "CGameObject.h"
#include "CMissle.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CSphereCollider.h"

class CPlayer : public CGameObject
{
protected:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3					m_xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	XMFLOAT3					m_xmf3FixTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	float Roll_WingsRotateDegree = 0.0f;
	float Pitch_WingsRotateDegree = 0.0f;
	float Yaw_WingsRotateDegree = 0.0f;

	float m_fAircraftSpeed = 200;
	float m_fRollPerformance = 0.0f;
	float m_fPitchPerformance = 0.0f;
	float m_fYawPerformance = 0.0f;

	
	float m_fGunFOV = 60;
	float m_fBurnerElapsed;
	float m_fFarPlaneDistance = 100000.0f;

	float m_fGunFireElapsed = 0.0f;
	float m_fGunFireFrequency = 0.07f;

	float m_fTimeLegElapsed = 0.0f;

	float m_fPushSpaceElapsed = 0.0f;

	float m_fCoolTime_MSL_1 = 1.0f;
	float m_fCoolTime_MSL_2 = 1.0f;

	bool m_bGunFire = false;
	bool m_bMissleCross = false;

	bool m_bGunSoundPlayed = false;
	bool m_bAssertSoundPlayed = false;
	bool m_bStall = false;
	bool m_bDamaged = false;

	XMFLOAT3 xmf3StallRecoverPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3     				m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMFLOAT3					m_xmf3GunCameraPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float           			m_fMaxVelocityXZ = 0.0f;
	float           			m_fMaxVelocityY = 0.0f;
	float           			m_fFriction = 0.0f;

	float						m_fRestartFrequency = 5.f;
	float						m_fRestartElapsed = 0.0f;

	LPVOID						m_pPlayerUpdatedContext = NULL;
	LPVOID						m_pCameraUpdatedContext = NULL;

	// Weapon Count
	int m_nMSL_Count = 100;
	int m_nFLR_Count = 50;
	int m_nScore = 0;

	//Hp Count
	int m_nHp = 5;
public:
	CPlayer();
	virtual ~CPlayer();

	XMFLOAT3 GetPosition() { return(m_xmf3Position); }
	XMFLOAT3 GetLookVector() { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() { return(m_xmf3Right); }

	virtual float GetFov() { return m_fFOV; }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(const XMFLOAT3& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const XMFLOAT3& xmf3Velocity) { m_xmf3Velocity = xmf3Velocity; }
	void SetPosition(const XMFLOAT3& xmf3Position) { Move(XMFLOAT3(xmf3Position.x - m_xmf3Position.x, xmf3Position.y - m_xmf3Position.y, xmf3Position.z - m_xmf3Position.z), false); }
	void SetTargetDir(const XMFLOAT3& xmf3TargetDir) { m_xmf3TargetDirection = xmf3TargetDir; }
	void ReturnEyeFix();

	void SetScale(XMFLOAT3& xmf3Scale) { m_xmf3Scale = xmf3Scale; }

	void SetMSLCount(int nCount) { m_nMSL_Count = nCount; }
	void SetGameOver(bool bGameOver) { m_bGameOver = bGameOver; }
	virtual void SetEngineRefractionPos() {}

	const XMFLOAT3& GetVelocity() const { return(m_xmf3Velocity); }
	float GetAircraftSpeed() const { return(m_fAircraftSpeed); }
	float GetPitchWingsRotateDegree() const { return Pitch_WingsRotateDegree; }
	XMFLOAT3 GetFixTarget() { return XMFLOAT3(m_xmf3FixTarget.x , m_xmf3FixTarget.y, m_xmf3FixTarget.z); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }
	float GetPushSpaceElapsed() { return m_fPushSpaceElapsed; }

	float GetCooltimeMSL1() { return m_fCoolTime_MSL_1; }
	float GetCooltimeMSL2() { return m_fCoolTime_MSL_2; }

	int GetMSLCount() const { return(m_nMSL_Count); }
	bool GetGameOver() const { return(m_bGameOver); }
	int GetHp() const { return(m_nHp); }
	int GetFLRCount() { return m_nFLR_Count; }

	const XMFLOAT3& GetTargetDir() const { return (m_xmf3TargetDirection); }

	ObjectManager* m_ObjManager;
	XMFLOAT3					m_xmf3TargetDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);

	CCamera* GetCamera() { return(m_pCamera); }
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	void SetDamaged() { m_bDamaged = false; }

	void Move(DWORD nDirection, float fDistance, bool bVelocity = false);
	void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	void Rotate(float x, float y, float z);

	virtual void LeftRollAnimation(float fTimeElapsed) {}
	virtual void RightRollAnimation(float fTimeElapsed) {}
	virtual void UpPitchAnimation(float fTimeElapsed) {}
	virtual void DownPitchAnimation(float fTimeElapsed) {}
	virtual void LeftYawAnimation(float fTimeElapsed) {}
	virtual void RightYawAnimation(float fTimeElapsed) {}
	virtual void RollWingReturn(float fTimeElapsed) {}
	virtual void PitchWingReturn(float fTimeElapsed) {}
	virtual void YawWingReturn(float fTimeElapsed) {}
	virtual void MissleLaunch() {}
	virtual void GunFire(float fTimeElapsed) {}
	virtual void GunCameraMove(float fTimeElapsed) {}

	void Update_Input(const float& TimeDelta);
	void Update_PadInput(const float& TimeDelta);
	virtual void WingAnimate(float fTimeElapsed, DWORD Direction);
	virtual void Animate(float fTimeElapsed);

	void CollisionActivate(CGameObject* collideTarget);

	virtual void SetAfterBurnerPosition(float fTimeElapsed) {}

	virtual void OnPlayerUpdateCallback(float fTimeElapsed) { }

	virtual void SetNaviPosition() {}

	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	virtual void OnCameraUpdateCallback(float fTimeElapsed) { }
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	CCamera* OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed) { return(NULL); }
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
};

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext = NULL);
	virtual ~CAirplanePlayer();

	//Wings
	CGameObject* m_pLeft_Deact_Wing = NULL;
	CGameObject* m_pRight_Deact_Wing = NULL;
	CGameObject* m_pLeft_Pitch_Wing = NULL;
	CGameObject* m_pRight_Pitch_Wing = NULL;
	CGameObject* m_pLeft_Roll_Wing = NULL;
	CGameObject* m_pRight_Roll_Wing = NULL;
	CGameObject* m_pYaw_Wing = NULL;

	//Weapons
	CGameObject* m_pMSL_1 = NULL;
	CGameObject* m_pMSL_2 = NULL;
	CGameObject* m_pMSL_3 = NULL;
	CGameObject* m_pMSL_4 = NULL;
	CGameObject* m_pSP_1 = NULL;
	CGameObject* m_pSP_2 = NULL;
	CGameObject* m_pGunMuzzle = NULL;

	// Weapon Count
	int m_nMSL_Count = 0;

	// Score
	int m_nScore = 0;

	bool m_bBoostSoundOn = false;

	CGameObject* m_pRight_AfterBurnerEX;
	CGameObject* m_pRight_AfterBurnerIN;
	CGameObject* m_pLeft_AfterBurnerEX;
	CGameObject* m_pLeft_AfterBurnerIN;

	CLoadedModelInfo* m_pAfterBurnerEXModel;
	CLoadedModelInfo* m_pAfterBurnerINModel;

	CGameObject* m_pLeft_AfterBurner;
	CGameObject* m_pRight_AfterBurner;

	CGameObject* m_pGunCamera = NULL;
	CGameObject* m_pCameraPos = NULL;

	CGameObject* m_pEngineRefraction = NULL;

	CGameObject* m_pNaviPos = NULL;

	//WeaponsXMF
	XMFLOAT4X4 m_xmMSL_1;

	ID3D12Device*										m_pd3dDevice = NULL;
	ID3D12GraphicsCommandList*				m_pd3dCommandList = NULL;
	ID3D12RootSignature*							m_pd3dGraphicsRootSignature = NULL;

private:
	virtual void OnPrepareAnimate();
	
	virtual void LeftRollAnimation(float fTimeElapsed);
	virtual void RightRollAnimation(float fTimeElapsed);
	virtual void UpPitchAnimation(float fTimeElapsed);
	virtual void DownPitchAnimation(float fTimeElapsed);
	virtual void LeftYawAnimation(float fTimeElapsed);
	virtual void RightYawAnimation(float fTimeElapsed);
	virtual void RollWingReturn(float fTimeElapsed);
	virtual void PitchWingReturn(float fTimeElapsed);
	virtual void YawWingReturn(float fTimeElapsed);
	virtual void MissleLaunch();
	virtual void GunFire(float fTimeElapsed);
	virtual void GunCameraMove(float fTimeElapsed);

	virtual void SetAfterBurnerPosition(float fTimeElapsed);

	virtual void SetEngineRefractionPos();

	virtual void SetNaviPosition();
public:
	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender();
};

class CTerrainPlayer : public CPlayer
{
public:
	CTerrainPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext = NULL);
	virtual ~CTerrainPlayer();

public:
	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);

	virtual void OnPlayerUpdateCallback(float fTimeElapsed);
	virtual void OnCameraUpdateCallback(float fTimeElapsed);

	virtual int Update(float fTimeElapsed);
};
