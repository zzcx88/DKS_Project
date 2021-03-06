#include "stdafx.h"
#include "CHeightMapTerrain.h"
#include "CAfterBurner.h"
#include "CGunshipObject.h"
#include "CLockOnUI.h"
#include "CPlayer.h"
#include "CEngineRafraction.h"
#include "CFlare.h"

CPlayer::CPlayer()
{
	m_pCamera = NULL;

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
}

CPlayer::~CPlayer()
{
	ReleaseShaderVariables();

	if (m_pCamera) delete m_pCamera;
}

void CPlayer::ReturnEyeFix()
{
	m_pCamera->SetLookPlayer(false);
	XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
	xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, 0.6);
	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, xmf3Shift));
	xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -4.1);
	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, xmf3Shift));
}

void CPlayer::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	if (m_pCamera) m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CPlayer::ReleaseShaderVariables()
{
	if (m_pCamera) m_pCamera->ReleaseShaderVariables();
}

void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);

		Move(xmf3Shift, bUpdateVelocity);
	}
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
		m_pCamera->Move(xmf3Shift);
	}
}

void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		m_pCamera->Rotate(x, y, z);
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
		if (z != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
}

void CPlayer::Update_Input(const float& TimeDelta)
{
	KeyManager* keyManager = GET_MANAGER<KeyManager>();
	DWORD dwDirection = 0;

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_TAB))
	{
		//m_bGameOver = true;
		if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE))
		{
			if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE)->m_bMissleLockCamera == false)
			{
				dwDirection |= VK_TAB;
				if (m_bEye_fixation == false)
				{
					m_bEye_fixation = true;
				}
				else
				{
					m_bEye_fixation = false;
					if (m_bGunFire == true)
					{
						m_fGunFOV = 60;
						ReturnEyeFix();
					}
				}
			}
		}
		else
		{
			dwDirection |= VK_TAB;
			if (m_bEye_fixation == false)
			{
				m_bEye_fixation = true;
			}
			else
			{
				m_bEye_fixation = false;
				if (m_bGunFire == true)
				{
					m_fGunFOV = 60;
					ReturnEyeFix();
				}
			}
		}
	}

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_T))
	{
		if (m_nFLR_Count > 0)
		{
			dwDirection |= VK_T;
			CFlare* pFlareRef = (CFlare*)m_ObjManager->GetObjFromTag(L"flareRef", OBJ_EFFECT);
			CFlare* pFlare = new CFlare(GetUp());
			pFlare->m_FromType = OBJ_PLAYER;
			pFlare->SetMesh(pFlareRef->m_pPlaneMesh);
			pFlare->m_pMaterial = new CMaterial(1);
			pFlare->m_pMaterial->SetTexture(pFlareRef->m_pTexture);
			pFlare->m_pMaterial->SetShader(pFlareRef->m_pShader);
			pFlare->SetMaterial(0, pFlare->m_pMaterial);
			pFlare->SetPosition(GetPosition());
			pFlare->m_fFlareSpeed = m_fAircraftSpeed;
			pFlare->m_xmf3Look = GetLook();
			m_ObjManager->AddObject(L"flareInstance", pFlare, OBJ_EFFECT);
			m_nFLR_Count--;
			GET_MANAGER<SoundManager>()->PlaySound(L"FlareCut.wav", CH_ETC);
			GET_MANAGER<SoundManager>()->SetVolume(CH_ETC, 1.f);
		}
	}

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_CAPITAL))
	{
		if (m_pCamera->m_bDefaultCameraMode == false)
			m_pCamera->m_bDefaultCameraMode = true;
		else
			m_pCamera->m_bDefaultCameraMode = false;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_LCONTROL))
	{
		if (m_bGunSoundPlayed == false)
		{
			GET_MANAGER<SoundManager>()->PlaySound(L"GunFire.mp3", CH_AIRGUN);
			GET_MANAGER<SoundManager>()->SetVolume(CH_AIRGUN, 0.3f);
			m_bGunSoundPlayed = true;
		}
		if(!m_bEye_fixation)
			m_fFOV = 60;
		m_bGunFire = true;
		m_fGunFireElapsed += TimeDelta;
		dwDirection |= VK_LCONTROL;
		GunFire(TimeDelta);
	}

	if (true == keyManager->GetKeyState(STATE_UP, VK_LCONTROL))
	{
		GET_MANAGER<SoundManager>()->StopSound(CH_AIRGUN);
		m_bGunSoundPlayed = false;
		if (m_bGunFire != false && m_bEye_fixation == false)
		{
			m_bGunFire = false;
			m_fGunFOV = 60.f;
			m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV);
		}
		dwDirection |= VK_LCONTROL;
	}


	if (true == keyManager->GetKeyState(STATE_PUSH, VK_SPACE))
	{
		dwDirection |= VK_SPACE;
		if (m_fPushSpaceElapsed == 0.0f)
		{
			MissleLaunch();
		}
		m_fPushSpaceElapsed+= 1* TimeDelta;
		if (m_fPushSpaceElapsed > 0.3f)
		{
			if(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE))
				GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE)->m_bMissleLockCamera = true;
			m_bMissleLockCamera = true;
		}
	}

	if (true == keyManager->GetKeyState(STATE_UP, VK_SPACE))
	{
		m_fPushSpaceElapsed = 0.0f;
		if(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE))
			GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE)->m_bMissleLockCamera = false;
		m_bMissleLockCamera = false;
		if (m_bEye_fixation == false && m_bGunFire == false)
		{
			if (m_fFOV < 60)
				m_fFOV = 60;
			m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV);
		}
	}

	if (keyManager->GetKeyState(STATE_PUSH, VK_RIGHT) || keyManager->GetKeyState(STATE_PUSH, VK_LEFT))
	{
		m_fTimeLegElapsed += TimeDelta;
		if (true == keyManager->GetKeyState(STATE_PUSH, VK_RIGHT))
		{
			if (!(true == keyManager->GetKeyState(STATE_PUSH, VK_LEFT)))
			{
				dwDirection |= VK_RIGHT;
				Rotate(0.0f, 0.0f, -Roll_WingsRotateDegree * m_fRollPerformance * TimeDelta);
				RightRollAnimation(TimeDelta);
			}
			else
			{
				if (Roll_WingsRotateDegree != 0)
					Rotate(0.0f, 0.0f, -Roll_WingsRotateDegree * m_fRollPerformance * TimeDelta);
				RollWingReturn(TimeDelta);
			}
		}

		if (true == keyManager->GetKeyState(STATE_PUSH, VK_LEFT))
		{
			if (!(true == keyManager->GetKeyState(STATE_PUSH, VK_RIGHT)))
			{
				dwDirection |= VK_LEFT;
				Rotate(0.0f, 0.0f, -Roll_WingsRotateDegree * m_fRollPerformance * TimeDelta);
				LeftRollAnimation(TimeDelta);
			}
		}
	}
	else
	{
		if (Roll_WingsRotateDegree != 0)
			Rotate(0.0f, 0.0f, -Roll_WingsRotateDegree * m_fRollPerformance * TimeDelta);
		RollWingReturn(TimeDelta);
	}

	if (keyManager->GetKeyState(STATE_PUSH, VK_UP) || keyManager->GetKeyState(STATE_PUSH, VK_DOWN))
	{
		m_fTimeLegElapsed += TimeDelta;
		if (true == keyManager->GetKeyState(STATE_PUSH, VK_UP))
		{
			if (!(true == keyManager->GetKeyState(STATE_PUSH, VK_DOWN)))
			{
				dwDirection |= VK_UP;
				Rotate(Pitch_WingsRotateDegree * m_fPitchPerformance * TimeDelta, 0.0f, 0.0f);
				DownPitchAnimation(TimeDelta);
			}
			else
			{
				if (Pitch_WingsRotateDegree != 0)
					Rotate(Pitch_WingsRotateDegree * m_fPitchPerformance * TimeDelta, 0.0f, 0.0f);
				PitchWingReturn(TimeDelta);
			}
		}

		if (true == keyManager->GetKeyState(STATE_PUSH, VK_DOWN))
		{
			if (!(true == keyManager->GetKeyState(STATE_PUSH, VK_UP)))
			{
				dwDirection |= VK_DOWN;
				Rotate(Pitch_WingsRotateDegree * m_fPitchPerformance * TimeDelta, 0.0f, 0.0f);
				UpPitchAnimation(TimeDelta);
			}
		}
	}
	else
	{
		if (Pitch_WingsRotateDegree != 0)
			Rotate(Pitch_WingsRotateDegree * m_fPitchPerformance * TimeDelta, 0.0f, 0.0f);
		PitchWingReturn(TimeDelta);
	}


	if (keyManager->GetKeyState(STATE_PUSH, VK_Q) || keyManager->GetKeyState(STATE_PUSH, VK_E))
	{
		m_fTimeLegElapsed += TimeDelta;
		if (true == keyManager->GetKeyState(STATE_PUSH, VK_Q))
		{
			if (!(true == keyManager->GetKeyState(STATE_PUSH, VK_E)))
			{
				dwDirection |= VK_Q;
				Rotate(0.0f, Yaw_WingsRotateDegree * m_fYawPerformance * TimeDelta, 0.0f);
				LeftYawAnimation(TimeDelta);
			}
			else
			{
				if (Yaw_WingsRotateDegree != 0)
					Rotate(0.0f, Yaw_WingsRotateDegree * m_fYawPerformance * TimeDelta, 0.0f);
				YawWingReturn(TimeDelta);
			}
		}

		if (true == keyManager->GetKeyState(STATE_PUSH, VK_E))
		{
			if (!(true == keyManager->GetKeyState(STATE_PUSH, VK_Q)))
			{
				dwDirection |= VK_E;
				Rotate(0.0f, Yaw_WingsRotateDegree * m_fYawPerformance * TimeDelta, 0.0f);
				RightYawAnimation(TimeDelta);
			}
		}
	}
	else
	{
		if(Yaw_WingsRotateDegree != 0)
			Rotate(0.0f, Yaw_WingsRotateDegree * m_fYawPerformance * TimeDelta, 0.0f);
		YawWingReturn(TimeDelta);
	}


	if (keyManager->GetKeyState(STATE_PUSH, VK_W) || keyManager->GetKeyState(STATE_PUSH, VK_S))
	{
		if (true == keyManager->GetKeyState(STATE_PUSH, VK_W))
		{
			if (!(true == keyManager->GetKeyState(STATE_PUSH, VK_S)))
			{
				dwDirection |= VK_W;
				if (m_fAircraftSpeed < 1000)
				{
					m_fAircraftSpeed += 100 * TimeDelta;
				}
				if (m_fFOV < 70 && m_bEye_fixation == false)
				{
					if(!m_bGunFire)
						m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV += 10.f * TimeDelta);
				}
				if (m_fBurnerElapsed < 100)
				{
					m_fBurnerElapsed += 100 * TimeDelta;
				}
			}
		}

		if (true == keyManager->GetKeyState(STATE_PUSH, VK_S))
		{
			dwDirection |= VK_S;
			if (m_fAircraftSpeed > 150)
			{
				m_fAircraftSpeed -= 150 * TimeDelta;
			}
			if (m_fFOV > 60)
			{
				if (!m_bGunFire)
					m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV -= 20.f * TimeDelta);
			}
			if (m_fBurnerElapsed > 0)
			{
				m_fBurnerElapsed -= 100 * TimeDelta;
			}
		}
	}
	else
	{
		if (m_fAircraftSpeed > 200)
		{
			m_fAircraftSpeed -= 100 * TimeDelta;
		}
		if (m_fFOV > 60)
		{
			m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV -= 10.f * TimeDelta);
		}
		if (m_fBurnerElapsed > 0)
		{
			m_fBurnerElapsed -= 100 * TimeDelta;
		}

		if (m_fAircraftSpeed < 1000 && m_fAircraftSpeed > 200)
		{
			m_fAircraftSpeed -= 50 * TimeDelta;
		}
	}

	if (m_fAircraftSpeed >= 148 && m_fAircraftSpeed < 200)
	{
		if (m_fAircraftSpeed < 150)
		{
			m_fAircraftSpeed = 150;
		}
		m_fAircraftSpeed += 50 * TimeDelta;
	}

	if (m_fAircraftSpeed >= 148 && m_fAircraftSpeed < 200)
	{
		if (m_fAircraftSpeed < 150)
		{
			m_fAircraftSpeed = 150;
		}
		m_fAircraftSpeed += 50 * TimeDelta;
	}

	if (m_bEye_fixation == true)
	{
		m_pCamera->SetPosition(XMFLOAT3(GetPosition().x - m_pCamera->GetLookVector().x * 7, GetPosition().y + 1.3 - m_pCamera->GetLookVector().y * 7,
			GetPosition().z - m_pCamera->GetLookVector().z * 7));
		for (auto& Ene : m_ObjManager->GetObjFromType(OBJ_ENEMY))
		{
			if (Ene.second->m_bAiming == true && Ene.second->GetState() != true)
			{
				m_xmf3FixTarget = Ene.second->GetPosition();
				m_pCamera->SetLookAt(XMFLOAT3(m_xmf3FixTarget));
			}
		}
			if (m_fFOV > 25)
			{
				m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV);
				m_fFOV -= 80 * TimeDelta;
			}
	}
	if (m_bEye_fixation == false && m_fFOV < 60)
	{
		m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV);
		m_fFOV += 30.f * TimeDelta;
		if(m_fFOV > 60.f)
			m_fFOV = 60.f;
	}

	if (m_bEye_fixation == false && m_bGunFire == false && m_bMissleLockCamera == false)
	{
		m_pCamera->SetLookPlayer();
	}

	if (m_fAircraftSpeed > 700)
	{
		CEngineRafraction* RefractObj = (CEngineRafraction*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"EngineRefractionObj", OBJ_EFFECT);
		if (RefractObj->m_bWaterDrop == false)
			GET_MANAGER<CDeviceManager>()->SetBulrSwitch(true);
		else
			GET_MANAGER<CDeviceManager>()->SetBulrSwitch(false);
	}
	if (m_fAircraftSpeed < 600 && GET_MANAGER<CDeviceManager>()->GetBlurAmount() == 0)
		GET_MANAGER<CDeviceManager>()->SetBulrSwitch(false);

	if (m_bStall == true)
	{
		XMFLOAT3 xmf3NewLook = XMFLOAT3(0, -1, 0);
		XMVECTOR Dest = XMLoadFloat3(&xmf3NewLook);
		XMVECTOR Start = XMLoadFloat3(&GetLookVector());
		XMVECTOR Result = XMVectorLerp(Start, Dest, 5.f * TimeDelta);
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, Result);
		m_xmf3Look = xmf3Result;
		Rotate(0,0,0);

		if (GetPosition().y < 9000)
			m_bStall = false;
	}

	Move(DIR_FORWARD, m_fAircraftSpeed * TimeDelta, true);
	MoveForward(8.0f);
	WingAnimate(TimeDelta, dwDirection);
	GunCameraMove(TimeDelta);
}

void CPlayer::Update_PadInput(const float& TimeDelta)
{
	KeyManager* keyManager = GET_MANAGER<KeyManager>();
	DWORD dwDirection = 0;

	if (true == keyManager->GetPadState(STATE_DOWN, XINPUT_GAMEPAD_X))
	{
		//m_bGameOver = true;
		if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE))
		{
			if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE)->m_bMissleLockCamera == false)
			{
				dwDirection |= XINPUT_GAMEPAD_X;
				if (m_bEye_fixation == false)
				{
					m_bEye_fixation = true;
				}
				else
				{
					m_bEye_fixation = false;
					if (m_bGunFire == true)
					{
						m_fGunFOV = 60;
						ReturnEyeFix();
					}
				}
			}
		}
		else
		{
			dwDirection |= XINPUT_GAMEPAD_Y;
			if (m_bEye_fixation == false)
			{
				m_bEye_fixation = true;
			}
			else
			{
				m_bEye_fixation = false;
				if (m_bGunFire == true)
				{
					m_fGunFOV = 60;
					ReturnEyeFix();
				}
			}
		}
	}

	if (true == keyManager->GetPadState(STATE_DOWN, XINPUT_GAMEPAD_RIGHT_THUMB))
	{
		if (m_nFLR_Count > 0)
		{
			dwDirection |= XINPUT_GAMEPAD_RIGHT_THUMB;
			CFlare* pFlareRef = (CFlare*)m_ObjManager->GetObjFromTag(L"flareRef", OBJ_EFFECT);
			CFlare* pFlare = new CFlare(GetUp());
			pFlare->m_FromType = OBJ_PLAYER;
			pFlare->SetMesh(pFlareRef->m_pPlaneMesh);
			pFlare->m_pMaterial = new CMaterial(1);
			pFlare->m_pMaterial->SetTexture(pFlareRef->m_pTexture);
			pFlare->m_pMaterial->SetShader(pFlareRef->m_pShader);
			pFlare->SetMaterial(0, pFlare->m_pMaterial);
			pFlare->SetPosition(GetPosition());
			pFlare->m_fFlareSpeed = m_fAircraftSpeed;
			pFlare->m_xmf3Look = GetLook();
			m_ObjManager->AddObject(L"flareInstance", pFlare, OBJ_EFFECT);
			m_nFLR_Count--;
			GET_MANAGER<SoundManager>()->PlaySound(L"FlareCut.wav", CH_ETC);
			GET_MANAGER<SoundManager>()->SetVolume(CH_ETC, 1.f);
		}
	}

	if (true == keyManager->GetPadState(STATE_DOWN, XINPUT_GAMEPAD_DPAD_DOWN))
	{
		if (m_pCamera->m_bDefaultCameraMode == false)
			m_pCamera->m_bDefaultCameraMode = true;
		else
			m_pCamera->m_bDefaultCameraMode = false;
	}

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_CAPITAL))
	{
		m_bGameOver = true;
		GET_MANAGER<SceneManager>()->m_bStageClear = true;
	}

	if (true == keyManager->GetKeyState(STATE_DOWN, VK_TAB))
	{
		m_nHp = 99999;
	}

	if (true == keyManager->GetPadState(STATE_PUSH, XINPUT_GAMEPAD_A))
	{
		if (m_bGunSoundPlayed == false)
		{
			GET_MANAGER<SoundManager>()->PlaySound(L"GunFire.mp3", CH_AIRGUN);
			GET_MANAGER<SoundManager>()->SetVolume(CH_AIRGUN, 0.3f);
			m_bGunSoundPlayed = true;
		}
		if (!m_bEye_fixation)
			m_fFOV = 60;
		m_bGunFire = true;
		m_fGunFireElapsed += TimeDelta;
		dwDirection |= XINPUT_GAMEPAD_A;
		GunFire(TimeDelta);
	}

	if (true == keyManager->GetPadState(STATE_UP, XINPUT_GAMEPAD_A))
	{
		GET_MANAGER<SoundManager>()->StopSound(CH_AIRGUN);
		m_bGunSoundPlayed = false;
		if (m_bGunFire != false && m_bEye_fixation == false)
		{
			m_bGunFire = false;
			m_fGunFOV = 60.f;
			m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV);
		}
		dwDirection |= XINPUT_GAMEPAD_A;
	}


	if (true == keyManager->GetPadState(STATE_PUSH, XINPUT_GAMEPAD_B))
	{
		dwDirection |= XINPUT_GAMEPAD_B;
		if (m_fPushSpaceElapsed == 0.0f)
		{
			MissleLaunch();
		}
		m_fPushSpaceElapsed += 1 * TimeDelta;
		if (m_fPushSpaceElapsed > 0.3f)
		{
			if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE))
				GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE)->m_bMissleLockCamera = true;
			m_bMissleLockCamera = true;
		}
	}

	if (true == keyManager->GetPadState(STATE_UP, XINPUT_GAMEPAD_B))
	{
		m_fPushSpaceElapsed = 0.0f;
		if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE))
			GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_missle", OBJ_ALLYMISSLE)->m_bMissleLockCamera = false;
		m_bMissleLockCamera = false;
		if (m_bEye_fixation == false && m_bGunFire == false)
		{
			if (m_fFOV < 60)
				m_fFOV = 60;
			m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV);
		}
	}

	if (true == keyManager->normalizedLX > 0 || true == keyManager->normalizedLX < 0)
	{
		m_fTimeLegElapsed += TimeDelta;
		if (true == keyManager->normalizedLX > 0)
		{
			if (!(true == keyManager->normalizedLX < 0))
			{
				dwDirection |= VK_RIGHT;
				Rotate(0.0f, 0.0f, -Roll_WingsRotateDegree * m_fRollPerformance * TimeDelta);
				RightRollAnimation(TimeDelta);
			}
			else
			{
				if (Roll_WingsRotateDegree != 0)
					Rotate(0.0f, 0.0f, -Roll_WingsRotateDegree * m_fRollPerformance * TimeDelta);
				RollWingReturn(TimeDelta);
			}
		}

		if (true == keyManager->normalizedLX < 0)
		{
			if (!(true == keyManager->normalizedLX > 0))
			{
				dwDirection |= VK_LEFT;
				Rotate(0.0f, 0.0f, -Roll_WingsRotateDegree * m_fRollPerformance * TimeDelta);
				LeftRollAnimation(TimeDelta);
			}
		}
	}
	else
	{
		if (Roll_WingsRotateDegree != 0)
			Rotate(0.0f, 0.0f, -Roll_WingsRotateDegree * m_fRollPerformance * TimeDelta);
		RollWingReturn(TimeDelta);
	}

	if (true == keyManager->normalizedLY > 0 || true == keyManager->normalizedLY < 0)
	{
		m_fTimeLegElapsed += TimeDelta;
		if (true == keyManager->normalizedLY > 0)
		{
			if (!(true == keyManager->normalizedLY < 0))
			{
				dwDirection |= VK_UP;
				Rotate(Pitch_WingsRotateDegree * m_fPitchPerformance * TimeDelta, 0.0f, 0.0f);
				DownPitchAnimation(TimeDelta);
			}
			else
			{
				if (Pitch_WingsRotateDegree != 0)
					Rotate(Pitch_WingsRotateDegree * m_fPitchPerformance * TimeDelta, 0.0f, 0.0f);
				PitchWingReturn(TimeDelta);
			}
		}

		if (true == keyManager->normalizedLY < 0)
		{
			if (!(true == keyManager->normalizedLY > 0))
			{
				dwDirection |= VK_DOWN;
				Rotate(Pitch_WingsRotateDegree * m_fPitchPerformance * TimeDelta, 0.0f, 0.0f);
				UpPitchAnimation(TimeDelta);
			}
		}
	}
	else
	{
		if (Pitch_WingsRotateDegree != 0)
			Rotate(Pitch_WingsRotateDegree * m_fPitchPerformance * TimeDelta, 0.0f, 0.0f);
		PitchWingReturn(TimeDelta);
	}


	if (keyManager->GetPadState(STATE_PUSH, XINPUT_GAMEPAD_LEFT_SHOULDER) || keyManager->GetPadState(STATE_PUSH, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		m_fTimeLegElapsed += TimeDelta;
		if (true == keyManager->GetPadState(STATE_PUSH, XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			if (!(true == keyManager->GetPadState(STATE_PUSH, XINPUT_GAMEPAD_RIGHT_SHOULDER)))
			{
				dwDirection |= XINPUT_GAMEPAD_LEFT_SHOULDER;
				Rotate(0.0f, Yaw_WingsRotateDegree * m_fYawPerformance * TimeDelta, 0.0f);
				LeftYawAnimation(TimeDelta);
			}
			else
			{
				if (Yaw_WingsRotateDegree != 0)
					Rotate(0.0f, Yaw_WingsRotateDegree * m_fYawPerformance * TimeDelta, 0.0f);
				YawWingReturn(TimeDelta);
			}
		}

		if (true == keyManager->GetPadState(STATE_PUSH, XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			if (!(true == keyManager->GetPadState(STATE_PUSH, XINPUT_GAMEPAD_LEFT_SHOULDER)))
			{
				dwDirection |= XINPUT_GAMEPAD_RIGHT_SHOULDER;
				Rotate(0.0f, Yaw_WingsRotateDegree * m_fYawPerformance * TimeDelta, 0.0f);
				RightYawAnimation(TimeDelta);
			}
		}
	}
	else
	{
		if (Yaw_WingsRotateDegree != 0)
			Rotate(0.0f, Yaw_WingsRotateDegree * m_fYawPerformance * TimeDelta, 0.0f);
		YawWingReturn(TimeDelta);
	}


	if (keyManager->ComputePadLeftTrigerState() > 0.5f || keyManager->ComputePadRightTrigerState() > 0.5f)
	{
		if (true == keyManager->ComputePadRightTrigerState() > 0.5f)
		{
			if (!(true == keyManager->ComputePadLeftTrigerState() > 0.5f))
			{
				//dwDirection |= VK_W;
				if (m_fAircraftSpeed < 1000)
				{
					m_fAircraftSpeed += 100 * TimeDelta;
				}
				if (m_fFOV < 70 && m_bEye_fixation == false)
				{
					if (!m_bGunFire)
						m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV += 10.f * TimeDelta);
				}
				if (m_fBurnerElapsed < 100)
				{
					m_fBurnerElapsed += 100 * TimeDelta;
				}
			}
		}

		if (true == keyManager->ComputePadLeftTrigerState() > 0.5f)
		{
			//dwDirection |= VK_S;
			if (m_fAircraftSpeed > 150)
			{
				m_fAircraftSpeed -= 150 * TimeDelta;
			}
			if (m_fFOV > 60)
			{
				if (!m_bGunFire)
					m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV -= 20.f * TimeDelta);
			}
			if (m_fBurnerElapsed > 0)
			{
				m_fBurnerElapsed -= 100 * TimeDelta;
			}
		}
	}
	else
	{
		if (m_fAircraftSpeed > 200)
		{
			m_fAircraftSpeed -= 100 * TimeDelta;
		}
		if (m_fFOV > 60)
		{
			m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV -= 10.f * TimeDelta);
		}
		if (m_fBurnerElapsed > 0)
		{
			m_fBurnerElapsed -= 100 * TimeDelta;
		}

		if (m_fAircraftSpeed < 1000 && m_fAircraftSpeed > 200)
		{
			m_fAircraftSpeed -= 50 * TimeDelta;
		}
	}

	if (m_fAircraftSpeed >= 148 && m_fAircraftSpeed < 200)
	{
		if (m_fAircraftSpeed < 150)
		{
			m_fAircraftSpeed = 150;
		}
		m_fAircraftSpeed += 50 * TimeDelta;
	}

	if (m_fAircraftSpeed >= 148 && m_fAircraftSpeed < 200)
	{
		if (m_fAircraftSpeed < 150)
		{
			m_fAircraftSpeed = 150;
		}
		m_fAircraftSpeed += 50 * TimeDelta;
	}

	if (m_bEye_fixation == true)
	{
		m_pCamera->SetPosition(XMFLOAT3(GetPosition().x - m_pCamera->GetLookVector().x * 7, GetPosition().y + 1.3 - m_pCamera->GetLookVector().y * 7,
			GetPosition().z - m_pCamera->GetLookVector().z * 7));
		for (auto& Ene : m_ObjManager->GetObjFromType(OBJ_ENEMY))
		{
			if (Ene.second->m_bAiming == true && Ene.second->GetState() != true)
			{
				m_xmf3FixTarget = Ene.second->GetPosition();
				m_pCamera->SetLookAt(XMFLOAT3(m_xmf3FixTarget));
			}
		}
		if (m_fFOV > 25)
		{
			m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV);
			m_fFOV -= 80 * TimeDelta;
		}
	}
	if (m_bEye_fixation == false && m_fFOV < 60)
	{
		m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fFOV);
		m_fFOV += 30.f * TimeDelta;
		if (m_fFOV > 60.f)
			m_fFOV = 60.f;
	}

	if (m_bEye_fixation == false && m_bGunFire == false && m_bMissleLockCamera == false)
	{
		m_pCamera->SetLookPlayer();
	}

	if (m_fAircraftSpeed > 700)
	{
		CEngineRafraction* RefractObj = (CEngineRafraction*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"EngineRefractionObj", OBJ_EFFECT);
		if (RefractObj->m_bWaterDrop == false)
			GET_MANAGER<CDeviceManager>()->SetBulrSwitch(true);
		else
			GET_MANAGER<CDeviceManager>()->SetBulrSwitch(false);
	}
	if (m_fAircraftSpeed < 600 && GET_MANAGER<CDeviceManager>()->GetBlurAmount() == 0)
		GET_MANAGER<CDeviceManager>()->SetBulrSwitch(false);

	if (m_bStall == true)
	{
		XMFLOAT3 xmf3NewLook = XMFLOAT3(0, -1, 0);
		XMVECTOR Dest = XMLoadFloat3(&xmf3NewLook);
		XMVECTOR Start = XMLoadFloat3(&GetLookVector());
		XMVECTOR Result = XMVectorLerp(Start, Dest, 5.f * TimeDelta);
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, Result);
		m_xmf3Look = xmf3Result;
		Rotate(0, 0, 0);

		if (GetPosition().y < 9000)
			m_bStall = false;
	}

	Move(DIR_FORWARD, m_fAircraftSpeed * TimeDelta, true);
	MoveForward(8.0f);
	WingAnimate(TimeDelta, dwDirection);
	GunCameraMove(TimeDelta);
}

void CPlayer::Animate(float fTimeElapsed)
{
	if (GET_MANAGER<SceneManager>()->GetCurrentSceneState() == SCENE_TEST)
	{
		m_pCamera->m_fTimeElapsed = fTimeElapsed;
		if (m_fCoolTime_MSL_1 <= 1)
		{
			m_fCoolTime_MSL_1 += 0.5f * fTimeElapsed;
		}
		if (m_fCoolTime_MSL_2 <= 1)
		{
			m_fCoolTime_MSL_2 += 0.5f * fTimeElapsed;
		}
		if (m_bGameOver == false)
		{
			if (GetPosition().y >= 10000)
			{
				m_bStall = true;
				std::default_random_engine dre(time(NULL) * GetPosition().z);
				std::uniform_real_distribution<float>fXPos(-400, 400);
				std::uniform_real_distribution<float>fZPos(200, 400);
				xmf3StallRecoverPosition = XMFLOAT3(GetPosition().x + fXPos(dre), -2000.f, GetPosition().z + fZPos(dre));
			}

			m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Gravity, fTimeElapsed, false));
			float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
			float fMaxVelocityXZ = m_fMaxVelocityXZ * fTimeElapsed;
			if (fLength > m_fMaxVelocityXZ)
			{
				m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
				m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
			}
			float fMaxVelocityY = m_fMaxVelocityY * fTimeElapsed;
			fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
			if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

			Move(m_xmf3Velocity, false);

			//if (m_pPlayerUpdatedContext) OnPlayerUpdateCallback(fTimeElapsed);

			DWORD nCurrentCameraMode = m_pCamera->GetMode();
			if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(m_xmf3Position, fTimeElapsed);
			//if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);
			if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(m_xmf3Position);
			m_pCamera->RegenerateViewMatrix();

			fLength = Vector3::Length(m_xmf3Velocity);
			float fDeceleration = (m_fFriction * fTimeElapsed);
			if (fDeceleration > fLength) fDeceleration = fLength;
			m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));

			if (GET_MANAGER<SceneManager>()->GetCurrentSceneState() != SCENE_MENU && m_ObjManager != NULL)
			{
				if (GET_MANAGER<KeyManager>()->m_pPadConnecter->IsConnected() == false)
					Update_Input(fTimeElapsed);
				else
					Update_PadInput(fTimeElapsed);
			}

			//cout << m_xmf3Look.x << " " << m_xmf3Look.y << " " << m_xmf3Look.z << endl;
			SetEngineRefractionPos();

			SetAfterBurnerPosition(fTimeElapsed);

			SetNaviPosition();

			if (m_AiMissleAssert == true)
			{
				if (m_bAssertSoundPlayed == false)
				{
					GET_MANAGER<SoundManager>()->PlaySound(L"Missle_Alert.mp3", CH_ALERT, true);
					m_bAssertSoundPlayed = true;
				}
			}
			else
			{
				GET_MANAGER<SoundManager>()->StopSound(CH_ALERT);
				m_bAssertSoundPlayed = false;
			}
		}
		else
		{
			m_fRestartElapsed += 1.3f * fTimeElapsed;
			if (m_fRestartElapsed < m_fRestartFrequency)
			{
				m_pCamera->SetPosition(XMFLOAT3(GetPosition().x, m_xmf4x4World._42 + 500, GetPosition().z));
				m_pCamera->SetLookAt(GetPosition());
				m_pCamera->RegenerateViewMatrix();

				if (GET_MANAGER<SceneManager>()->GetCurrentSceneState() == SCENE_TEST && !GET_MANAGER<SceneManager>()->m_bStageClear)
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui17_mission_failed", OBJ_EFFECT)->SetIsRender(true);
				else if (GET_MANAGER<SceneManager>()->GetCurrentSceneState() == SCENE_TEST && GET_MANAGER<SceneManager>()->m_bStageClear)
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui21_mission_accomplished", OBJ_EFFECT)->SetIsRender(true);
			}
			else
			{
				if (GET_MANAGER<SceneManager>()->GetCurrentSceneState() == SCENE_TEST && !GET_MANAGER<SceneManager>()->m_bStageClear)
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui17_mission_failed", OBJ_EFFECT)->SetIsRender(false);
				else if (GET_MANAGER<SceneManager>()->GetCurrentSceneState() == SCENE_TEST && GET_MANAGER<SceneManager>()->m_bStageClear)
					GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui21_mission_accomplished", OBJ_EFFECT)->SetIsRender(false);

				m_fRestartElapsed = 0;
				if (GET_MANAGER<SceneManager>()->m_bStageClear == false)
				{
					m_bGameOver = false;
					GET_MANAGER<SceneManager>()->m_bStageClear = false;
				}

				for (auto i = (int)OBJ_ENEMY; i <= OBJ_UI; ++i)
				{
					if (i == OBJ_ENEMY)
					{
						for (auto p = GET_MANAGER<ObjectManager>()->GetObjFromType((OBJTYPE)i).begin(); p != GET_MANAGER<ObjectManager>()->GetObjFromType((OBJTYPE)i).end(); ++p)
						{
							(*p).second->m_isDead = true;
							(*p).second->m_pMUI->m_isDead = true;
							(*p).second->m_pLockOnUI->m_isDead = true;
						}
					}
				}
				GET_MANAGER<SceneManager>()->m_bCreateShip = false;
				GET_MANAGER<SceneManager>()->m_nWaveCnt = 0;
				GET_MANAGER<SceneManager>()->m_nTgtObject = 0;
				GET_MANAGER<UIManager>()->FighterOBJs.clear();
				GET_MANAGER<UIManager>()->ShipOBJs.clear();
				GET_MANAGER<UIManager>()->SetMin(15);
				GET_MANAGER<UIManager>()->SetSec(30);

				SetPlayerScore(0);

				GET_MANAGER<ObjectManager>()->ReleaseFromType(OBJ_ENEMY);
				m_nHp = 5;
				SetPosition(XMFLOAT3(0, 1000, 0));
				m_xmf3Look = XMFLOAT3(0, 0, 1);
				m_xmf3Up = XMFLOAT3(0, 1, 0);
				m_xmf3Right = XMFLOAT3(1, 0, 0);
				m_bEye_fixation = false;
				m_nFLR_Count = 50;
				m_nMSL_Count = 100;
				m_fAircraftSpeed = 300;
			}
		}

		if (m_pCamera->m_bShakeSwitch == true)
		{
			m_pCamera->ShakingCamera();
		}
		/*if (-1 ==Update_Input(fTimeElapsed))
		{
			return -1;
		}*/
	}
}

void CPlayer::CollisionActivate(CGameObject* collideTarget)
{
	if (collideTarget->m_ObjType == OBJ_ENEBULLET || collideTarget->m_ObjType == OBJ_ENEMISSLE)
	{
		if (m_bGunFire == false)
		{
			/*if ((int)(GetPosition().x + GetPosition().y + GetPosition().z) / 2 == 0)
				m_pCamera->Rotate(20, 0, 0);
			else
				m_pCamera->Rotate(-20, 0, 0);*/
			m_pCamera->m_bShakeSwitch = true;
		}
		cout << "플레이어 충돌!" << endl;

		// 플레이어 체력, 게임 오버 시 작동 되는 코드
		if (m_nHp > 1&&!m_bGameOver)
		{
			m_nHp -= 1;
			cout << " 피가 이만큼 남았어요: " << m_nHp << endl;
		}
		else
		{
			m_bGameOver = true;
		}
	}
}

CCamera* CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	CCamera* pNewCamera = NULL;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = new CFirstPersonCamera(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = new CThirdPersonCamera(m_pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = new CSpaceShipCamera(m_pCamera);
		break;
	}
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));

		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
		if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return(pNewCamera);
}

void CPlayer::OnPrepareRender()
{
	m_xmf4x4ToParent._11 = m_xmf3Right.x; m_xmf4x4ToParent._12 = m_xmf3Right.y; m_xmf4x4ToParent._13 = m_xmf3Right.z;
	m_xmf4x4ToParent._21 = m_xmf3Up.x; m_xmf4x4ToParent._22 = m_xmf3Up.y; m_xmf4x4ToParent._23 = m_xmf3Up.z;
	m_xmf4x4ToParent._31 = m_xmf3Look.x; m_xmf4x4ToParent._32 = m_xmf3Look.y; m_xmf4x4ToParent._33 = m_xmf3Look.z;
	m_xmf4x4ToParent._41 = m_xmf3Position.x; m_xmf4x4ToParent._42 = m_xmf3Position.y; m_xmf4x4ToParent._43 = m_xmf3Position.z;

	m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixScaling(m_xmf3Scale.x, m_xmf3Scale.y, m_xmf3Scale.z), m_xmf4x4ToParent);

	UpdateTransform(NULL);
}

void CPlayer::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	if (m_bGameOver == false)
	{
		DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
		//if (nCameraMode == THIRD_PERSON_CAMERA) CGameObject::Render(pd3dCommandList, pCamera);
		if (GET_MANAGER<SceneManager>()->GetCurrentSceneState() == SCENE_TEST) CGameObject::Render(pd3dCommandList, pCamera);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
CAirplanePlayer::CAirplanePlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{
	GET_MANAGER<SoundManager>()->PlaySound(L"AfterBurner_Base.mp3", CH_BNNRBASE);
	GET_MANAGER<SoundManager>()->PlaySound(L"AfterBurner_Boost.mp3", CH_BNNRBOST);
	GET_MANAGER<SoundManager>()->SetVolume(CH_BNNRBOST, 0.0f);


	m_pCamera = ChangeCamera(SPACESHIP_CAMERA, 0.0f);
	SphereCollider = new CSphereCollider(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);
	SphereCollider->SetScale(100, 100, 100);
	SphereCollider->SetSphereCollider(GetPosition() , 10.0f);

	//m_pMissleModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Missle.bin", NULL, MODEL_ACE);
	//m_pMissleModelCol = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/Sphere.bin", NULL, MODEL_COL);

	m_nMSL_Count = CPlayer::GetMSLCount();

	m_pAfterBurnerEXModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/AfterBurnerEX.bin", NULL, MODEL_EFC);
	m_pAfterBurnerINModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/AfterBurnerIN.bin", NULL, MODEL_EFC);

	CLoadedModelInfo* pModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/F-4E_Phantom_II.bin", NULL, MODEL_ACE);
	SetChild(pModel->m_pModelRootObject);

	m_fRollPerformance = 100.0f;
	m_fPitchPerformance = 70.0f;
	m_fYawPerformance = 20.0f;

	m_pd3dDevice = pd3dDevice;
	m_pd3dCommandList = pd3dCommandList;
	m_pd3dGraphicsRootSignature = pd3dGraphicsRootSignature;

	//MissleLaunch(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature,m_xmf3Position);

	OnPrepareAnimate();

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	if (pModel) delete pModel;
}

CAirplanePlayer::~CAirplanePlayer()
{
}

void CAirplanePlayer::OnPrepareAnimate()
{
	m_pLeft_Deact_Wing = FindFrame("Left_Deact_Wing");
	m_pRight_Deact_Wing = FindFrame("Right_Deact_Wing");
	m_pLeft_Pitch_Wing = FindFrame("Left_Pitch_Wing");
	m_pRight_Pitch_Wing = FindFrame("Right_Pitch_Wing");
	m_pLeft_Roll_Wing = FindFrame("Left_Roll_Wing");
	m_pRight_Roll_Wing = FindFrame("Right_Roll_Wing");
	m_pYaw_Wing = FindFrame("Yaw_Wing");

	m_pMSL_1 = FindFrame("MSL_1");
	m_pMSL_2 = FindFrame("MSL_2");
	m_pMSL_3 = FindFrame("MSL_3");
	m_pMSL_4 = FindFrame("MSL_4");
	m_pSP_1 = FindFrame("SP_1");
	m_pGunMuzzle = FindFrame("GunMuzzle");
	
	m_pGunCamera = FindFrame("GunCamera");
	m_pCameraPos = FindFrame("CameraPos");

	m_pLeft_AfterBurner = FindFrame("Left_AfterBuner");
	m_pRight_AfterBurner = FindFrame("Right_AfterBuner");

	m_pRight_AfterBurnerEX = FindFrame("Right_AfterBurnerEX");
	m_pRight_AfterBurnerIN = FindFrame("Right_AfterBurnerIN");
	m_pLeft_AfterBurnerEX = FindFrame("Left_AfterBurnerEX");
	m_pLeft_AfterBurnerIN = FindFrame("Left_AfterBurnerIN");
	m_pEngineRefraction = FindFrame("EngineRefraction");
	m_pNaviPos = FindFrame("Nevi_Position");

	CAfterBurner* pBurner;
	pBurner = new CAfterBurner();
	pBurner->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"AfterBurner", OBJ_EFFECT)->m_pPlaneMesh);
	pBurner->m_pCamera = m_pCamera;
	pBurner->m_pEffectTexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"AfterBurner", OBJ_EFFECT)->m_pEffectTexture[0];
	pBurner->m_pEffectMaterial = new CMaterial(1);
	pBurner->m_pEffectMaterial->SetTexture(pBurner->m_pEffectTexture[0]);
	pBurner->m_pEffectMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"AfterBurner", OBJ_EFFECT)->m_PlaneShader);
	pBurner->SetMaterial(0, pBurner->m_pEffectMaterial);
	m_pLeft_AfterBurner->m_pAfterBurner = pBurner;
	m_pLeft_AfterBurner->SetScale(0.095f, 0.095f, 1);
	GET_MANAGER<ObjectManager>()->AddObject(L"AfterBurnerInstance", m_pLeft_AfterBurner->m_pAfterBurner, OBJ_BURNER);

	CAfterBurner* pRBurner;
	pRBurner = new CAfterBurner();
	pRBurner->SetMesh((CMesh*)GET_MANAGER<ObjectManager>()->GetObjFromTag(L"AfterBurner", OBJ_EFFECT)->m_pPlaneMesh);
	pRBurner->m_pCamera = m_pCamera;
	pRBurner->m_pEffectTexture[0] = GET_MANAGER<ObjectManager>()->GetObjFromTag(L"AfterBurner", OBJ_EFFECT)->m_pEffectTexture[0];
	pRBurner->m_pEffectMaterial = new CMaterial(1);
	pRBurner->m_pEffectMaterial->SetTexture(pRBurner->m_pEffectTexture[0]);
	pRBurner->m_pEffectMaterial->SetShader(GET_MANAGER<ObjectManager>()->GetObjFromTag(L"AfterBurner", OBJ_EFFECT)->m_PlaneShader);
	pRBurner->SetMaterial(0, pRBurner->m_pEffectMaterial);
	m_pRight_AfterBurner->m_pAfterBurner = pRBurner;
	m_pRight_AfterBurner->SetScale(0.095f, 0.095f, 1);
	GET_MANAGER<ObjectManager>()->AddObject(L"AfterBurnerInstance", m_pRight_AfterBurner->m_pAfterBurner, OBJ_BURNER);
	}
	//m_xmMSL_1 = m_pMSL_1->m_xmf4x4World;

void CPlayer::WingAnimate(float fTimeElapsed, DWORD Direction)
{
	CGameObject::Animate(fTimeElapsed);
	if (SphereCollider)SphereCollider->SetPosition(m_xmf3Position);
	if (SphereCollider)SphereCollider->m_xmf4x4ToParent = Matrix4x4::Multiply(XMMatrixScaling(5, 5, 5), m_xmf4x4ToParent);
	if (SphereCollider)SphereCollider->Animate(fTimeElapsed, GetPosition());
}

void CAirplanePlayer::LeftRollAnimation(float fTimeElapsed)
{
	if (Roll_WingsRotateDegree > 0)
	{
		RollWingReturn(fTimeElapsed);
		CGameObject::Animate(fTimeElapsed);
		return;
	}
	if (Roll_WingsRotateDegree > -1.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(Roll_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pLeft_Roll_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pLeft_Roll_Wing->m_xmf4x4ToParent);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationX(XMConvertToRadians(-Roll_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pRight_Roll_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate1, m_pRight_Roll_Wing->m_xmf4x4ToParent);

		Roll_WingsRotateDegree -= fTimeElapsed;
		CGameObject::Animate(fTimeElapsed);
	}
}

void CAirplanePlayer::RightRollAnimation(float fTimeElapsed)
{
	if (Roll_WingsRotateDegree < 0)
	{
		RollWingReturn(fTimeElapsed);
		CGameObject::Animate(fTimeElapsed);
		return;
	}
	if (Roll_WingsRotateDegree < 1.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(-Roll_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pRight_Roll_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pRight_Roll_Wing->m_xmf4x4ToParent);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationX(XMConvertToRadians(Roll_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pLeft_Roll_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate1, m_pLeft_Roll_Wing->m_xmf4x4ToParent);

		Roll_WingsRotateDegree += fTimeElapsed;
		CGameObject::Animate(fTimeElapsed);
	}
}

void CAirplanePlayer::UpPitchAnimation(float fTimeElapsed)
{
	//MissleLaunch();
	if (Pitch_WingsRotateDegree > 0)
	{
		PitchWingReturn(fTimeElapsed);
		CGameObject::Animate(fTimeElapsed);
		return;
	}
	if (Pitch_WingsRotateDegree > -1.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(Pitch_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pLeft_Pitch_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pLeft_Pitch_Wing->m_xmf4x4ToParent);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationX(XMConvertToRadians(Pitch_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pRight_Pitch_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate1, m_pRight_Pitch_Wing->m_xmf4x4ToParent);

		Pitch_WingsRotateDegree -= fTimeElapsed;
		CGameObject::Animate(fTimeElapsed);
	}
}

void CAirplanePlayer::DownPitchAnimation(float fTimeElapsed)
{
	if (Pitch_WingsRotateDegree < 0)
	{
		PitchWingReturn(fTimeElapsed);
		CGameObject::Animate(fTimeElapsed);
		return;
	}
	if (Pitch_WingsRotateDegree < 1.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(Pitch_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pLeft_Pitch_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pLeft_Pitch_Wing->m_xmf4x4ToParent);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationX(XMConvertToRadians(Pitch_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pRight_Pitch_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate1, m_pRight_Pitch_Wing->m_xmf4x4ToParent);

		Pitch_WingsRotateDegree += fTimeElapsed;
		CGameObject::Animate(fTimeElapsed);

	}
}

void CAirplanePlayer::LeftYawAnimation(float fTimeElapsed)
{
	if (Yaw_WingsRotateDegree > 0)
	{
		YawWingReturn(fTimeElapsed);
		CGameObject::Animate(fTimeElapsed);
		return;
	}
	if (Yaw_WingsRotateDegree > -0.7f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationZ(XMConvertToRadians(-Yaw_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pYaw_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pYaw_Wing->m_xmf4x4ToParent);

		Yaw_WingsRotateDegree -= fTimeElapsed;
		CGameObject::Animate(fTimeElapsed);
	}
}

void CAirplanePlayer::RightYawAnimation(float fTimeElapsed)
{
	if (Yaw_WingsRotateDegree < 0)
	{
		YawWingReturn(fTimeElapsed);
		CGameObject::Animate(fTimeElapsed);
		return;
	}
	if (Yaw_WingsRotateDegree < 0.7f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationZ(XMConvertToRadians(-Yaw_WingsRotateDegree * 50.0f) * fTimeElapsed);
		m_pYaw_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pYaw_Wing->m_xmf4x4ToParent);

		Yaw_WingsRotateDegree += fTimeElapsed;
		CGameObject::Animate(fTimeElapsed);
	}
}

void CAirplanePlayer::RollWingReturn(float fTimeElapsed)
{
	if (Roll_WingsRotateDegree < 0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(-Roll_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pLeft_Roll_Wing)m_pLeft_Roll_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pLeft_Roll_Wing->m_xmf4x4ToParent);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationX(XMConvertToRadians(Roll_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pRight_Roll_Wing)m_pRight_Roll_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate1, m_pRight_Roll_Wing->m_xmf4x4ToParent);

		Roll_WingsRotateDegree += fTimeElapsed;
		if (Roll_WingsRotateDegree >= 0)
		{
			Roll_WingsRotateDegree = 0;
			m_pLeft_Roll_Wing->m_xmf4x4ToParent._22 = 0;
			m_pRight_Roll_Wing->m_xmf4x4ToParent._22 = 0;
		}
	}
	if (Roll_WingsRotateDegree > 0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(-Roll_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pLeft_Roll_Wing)m_pLeft_Roll_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pLeft_Roll_Wing->m_xmf4x4ToParent);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationX(XMConvertToRadians(Roll_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pRight_Roll_Wing)m_pRight_Roll_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate1, m_pRight_Roll_Wing->m_xmf4x4ToParent);

		Roll_WingsRotateDegree -= fTimeElapsed;
		if (Roll_WingsRotateDegree <= 0)
		{
			Roll_WingsRotateDegree = 0;
			m_pLeft_Roll_Wing->m_xmf4x4ToParent._22 = 0;
			m_pRight_Roll_Wing->m_xmf4x4ToParent._22 = 0;
		}
	}
}
void CAirplanePlayer::PitchWingReturn(float fTimeElapsed)
{
	//pitch
	if (Pitch_WingsRotateDegree <= 0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(-Pitch_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pLeft_Pitch_Wing)m_pLeft_Pitch_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pLeft_Pitch_Wing->m_xmf4x4ToParent);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationX(XMConvertToRadians(-Pitch_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pRight_Pitch_Wing)m_pRight_Pitch_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate1, m_pRight_Pitch_Wing->m_xmf4x4ToParent);

		Pitch_WingsRotateDegree += fTimeElapsed;
		if (Pitch_WingsRotateDegree > 0)
		{
			Pitch_WingsRotateDegree = 0;
			m_pRight_Pitch_Wing->m_xmf4x4ToParent._22 = 0;
			m_pLeft_Pitch_Wing->m_xmf4x4ToParent._22 = 0;
		}
	}
	if (Pitch_WingsRotateDegree >= 0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(-Pitch_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pLeft_Pitch_Wing)m_pLeft_Pitch_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pLeft_Pitch_Wing->m_xmf4x4ToParent);

		XMMATRIX xmmtxRotate1 = XMMatrixRotationX(XMConvertToRadians(-Pitch_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pRight_Pitch_Wing)m_pRight_Pitch_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate1, m_pRight_Pitch_Wing->m_xmf4x4ToParent);

		Pitch_WingsRotateDegree -= fTimeElapsed;
		if (Pitch_WingsRotateDegree <= 0)
		{
			Pitch_WingsRotateDegree = 0;
			m_pRight_Pitch_Wing->m_xmf4x4ToParent._22 = 0;
			m_pLeft_Pitch_Wing->m_xmf4x4ToParent._22 = 0;
		}
	}
}

void CAirplanePlayer::YawWingReturn(float fTimeElapsed)
{
	if (Yaw_WingsRotateDegree < 0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationZ(XMConvertToRadians(Yaw_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pYaw_Wing)m_pYaw_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pYaw_Wing->m_xmf4x4ToParent);

		Yaw_WingsRotateDegree += fTimeElapsed;
		if (Yaw_WingsRotateDegree > 0)
		{
			Yaw_WingsRotateDegree = 0;
			m_pYaw_Wing->m_xmf4x4ToParent._21 = 0;
		}
	}
	if (Yaw_WingsRotateDegree > 0)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationZ(XMConvertToRadians(Yaw_WingsRotateDegree * 50.0f) * fTimeElapsed);
		if (m_pYaw_Wing)m_pYaw_Wing->m_xmf4x4ToParent = Matrix4x4::Multiply(xmmtxRotate, m_pYaw_Wing->m_xmf4x4ToParent);

		Yaw_WingsRotateDegree -= fTimeElapsed;
		if (Yaw_WingsRotateDegree < 0)
		{
			Yaw_WingsRotateDegree = 0;
			m_pYaw_Wing->m_xmf4x4ToParent._21 = 0;
		}
	}
}

void CAirplanePlayer::MissleLaunch()
{
	CMissle* pMissle;
	XMFLOAT3* temp = NULL;
	m_nMSL_Count = CPlayer::GetMSLCount();
	for (auto& Ene : m_ObjManager->GetObjFromType(OBJ_ENEMY))
	{
		if (Ene.second->m_bCanFire == true && Ene.second->GetState() != true && m_nMSL_Count != 0 && Ene.second->m_bDestroyed == false)
		{
			temp = Ene.second->GetPositionForMissle();
			Ene.second->m_AiMissleAssert = true;
			pMissle = new CMissle(this);
			pMissle->m_xmfTarget = temp;
			pMissle->m_bLockOn = true;
			if (m_bMissleCross == false && m_fCoolTime_MSL_1 >= 1.f)
			{
				m_bMissleCross = true;
				pMissle->SetPosition(m_pMSL_1->GetPosition().x - GetUp().x * 20, m_pMSL_1->GetPosition().y - GetUp().y * 20, m_pMSL_1->GetPosition().z - GetUp().z * 20);
				pMissle->m_fTheta = 60.f;
				m_ObjManager->AddObject(L"player_missle", pMissle, OBJ_ALLYMISSLE);
				m_fCoolTime_MSL_1 = 0;
				GET_MANAGER<SoundManager>()->PlaySound(L"Missile_launch.mp3", CH_MISSLE);
				GET_MANAGER<SoundManager>()->SetVolume(CH_MISSLE, 0.2f);
				CPlayer::SetMSLCount(--m_nMSL_Count);
			}
			else
			{
				if (m_fCoolTime_MSL_2 >= 1.f)
				{
					m_bMissleCross = false;
					pMissle->SetPosition(m_pMSL_2->GetPosition().x - GetUp().x * 20, m_pMSL_2->GetPosition().y - GetUp().y * 20, m_pMSL_2->GetPosition().z - GetUp().z * 20);
					pMissle->m_fTheta = 60.f;
					m_ObjManager->AddObject(L"player_missle", pMissle, OBJ_ALLYMISSLE);
					m_fCoolTime_MSL_2 = 0;
					GET_MANAGER<SoundManager>()->PlaySound(L"Missile_launch.mp3", CH_MISSLE);
					GET_MANAGER<SoundManager>()->SetVolume(CH_MISSLE, 0.2f);
					CPlayer::SetMSLCount(--m_nMSL_Count);
				}
			}
			return;
		}

	}
	if (m_nMSL_Count > 0)
	{
		pMissle = new CMissle(this);
		pMissle->m_xmfTarget = temp;
		pMissle->m_bLockOn = false;
		if (m_bMissleCross == false && m_fCoolTime_MSL_1 >= 1.f)
		{
			m_bMissleCross = true;
			pMissle->SetPosition(m_pMSL_1->GetPosition().x - GetUp().x * 20, m_pMSL_1->GetPosition().y - GetUp().y * 20, m_pMSL_1->GetPosition().z - GetUp().z * 20);
			pMissle->m_pCamera = m_pCamera;
			m_ObjManager->AddObject(L"player_missle", pMissle, OBJ_ALLYMISSLE);
			m_fCoolTime_MSL_1 = 0;
			GET_MANAGER<SoundManager>()->PlaySound(L"Missile_launch.mp3", CH_MISSLE);
			GET_MANAGER<SoundManager>()->SetVolume(CH_MISSLE, 0.2f);
			CPlayer::SetMSLCount(--m_nMSL_Count);
		}
		else
		{
			if (m_fCoolTime_MSL_2 >= 1.f)
			{
				m_bMissleCross = false;
				pMissle->SetPosition(m_pMSL_2->GetPosition().x - GetUp().x * 20, m_pMSL_2->GetPosition().y - GetUp().y * 20, m_pMSL_2->GetPosition().z - GetUp().z * 20);
				pMissle->m_pCamera = m_pCamera;
				m_ObjManager->AddObject(L"player_missle", pMissle, OBJ_ALLYMISSLE);
				m_fCoolTime_MSL_2 = 0;
				GET_MANAGER<SoundManager>()->PlaySound(L"Missile_launch.mp3", CH_MISSLE);
				GET_MANAGER<SoundManager>()->SetVolume(CH_MISSLE, 0.2f);
				CPlayer::SetMSLCount(--m_nMSL_Count);
			}
		}
	}

}

void CAirplanePlayer::GunFire(float fTimeElapsed)
{
	if (m_fGunFireElapsed > m_fGunFireFrequency)
	{
		CBullet* pBullet;
		pBullet = new CBullet(XMFLOAT3(m_pGunMuzzle->GetPosition().x - GetLookVector().x * 3, m_pGunMuzzle->GetPosition().y - GetLookVector().y * 3, m_pGunMuzzle->GetPosition().z - GetLookVector().z * 3));
		pBullet->m_xmf3Look = m_xmf3Look;
		pBullet->m_xmf4x4ToParent = m_xmf4x4ToParent;
		pBullet->m_fBulletSpeed = m_fAircraftSpeed + 300.f;
		pBullet->m_pEffectMaterial = new CMaterial(1);
		pBullet->m_pEffectMaterial->SetTexture(m_ObjManager->GetObjFromTag(L"bulletRef", OBJ_ALLYBULLET)->m_pBulletTexture);
		pBullet->m_pEffectMaterial->SetShader(m_ObjManager->GetObjFromTag(L"bulletRef", OBJ_ALLYBULLET)->m_pBulletShader);
		pBullet->SetMaterial(0, pBullet->m_pEffectMaterial);
		pBullet->SetPosition(XMFLOAT3(m_pGunMuzzle->GetPosition().x - GetLookVector().x * 3, m_pGunMuzzle->GetPosition().y - GetLookVector().y * 3, m_pGunMuzzle->GetPosition().z - GetLookVector().z * 3));
		m_ObjManager->AddObject(L"player_bullet", pBullet, OBJ_EFFECT);
		m_fGunFireElapsed = 0.0f;	

		////구충돌체 별도 생성중
		CBullet* pColliedBullet;
		pColliedBullet = new CBullet(XMFLOAT3(m_pGunMuzzle->GetPosition().x - GetLookVector().x * 3, m_pGunMuzzle->GetPosition().y - GetLookVector().y * 3, m_pGunMuzzle->GetPosition().z - GetLookVector().z * 3));
		pColliedBullet->m_ColliedObj = true;
		pColliedBullet->m_xmf3Look = m_xmf3Look;
		pColliedBullet->m_xmf4x4ToParent = m_xmf4x4ToParent;
		pColliedBullet->m_fBulletSpeed = m_fAircraftSpeed + 1300.f;
		pColliedBullet->SetPosition(XMFLOAT3(m_pGunMuzzle->GetPosition().x - GetLookVector().x * 3, m_pGunMuzzle->GetPosition().y - GetLookVector().y * 3, m_pGunMuzzle->GetPosition().z - GetLookVector().z * 3));
		m_ObjManager->AddObject(L"player_bullet_collide", pColliedBullet, OBJ_ALLYBULLET);
		m_fGunFireElapsed = 0.0f;
	}
}

void CAirplanePlayer::GunCameraMove(float fTimeElapsed)
{
	if (!m_bEye_fixation)
	{
		if (m_bGunFire)
		{
			if (m_fGunFOV > 50)
			{
				m_fGunFOV -= 40.f * fTimeElapsed;
				m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, m_fGunFOV);
			}
			XMVECTOR Dest = XMLoadFloat3(&m_pGunCamera->GetPosition());
			XMVECTOR Start = XMLoadFloat3(&m_pCamera->GetPosition());
			XMVECTOR Result = XMVectorLerp(Start, Dest, 10.f * fTimeElapsed);
			XMFLOAT3 xmf3Result;
			XMStoreFloat3(&xmf3Result, Result);
			m_pCamera->SetPosition(xmf3Result);
		}
		else
		{
		}
	}
}

void CAirplanePlayer::SetAfterBurnerPosition(float fTimeElapsed)
{
	float volume = m_fBurnerElapsed / 100.f;
	if (volume > 1.f)
		volume = 1.f;
	GET_MANAGER<SoundManager>()->SetVolume(CH_BNNRBOST, volume);

	if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"jetFlameLeft", OBJ_EFFECT2))
	{
		float fPosValue = 0.0f - m_fBurnerElapsed / 100.f;
		if (m_pRight_AfterBurnerEX != NULL)
		{
			GET_MANAGER<ObjectManager>()->GetObjFromTag(L"jetFlameLeft", OBJ_EFFECT2)->SetPosition(m_pRight_AfterBurnerEX->GetPosition().x - m_pRight_AfterBurnerEX->GetLook().x * fPosValue,
				m_pRight_AfterBurnerEX->GetPosition().y - m_pRight_AfterBurnerEX->GetLook().y * fPosValue, m_pRight_AfterBurnerEX->GetPosition().z - m_pRight_AfterBurnerEX->GetLook().z* fPosValue);
			if (m_fBurnerElapsed / 100.f <= 1.0f)
				GET_MANAGER<ObjectManager>()->GetObjFromTag(L"jetFlameLeft", OBJ_EFFECT2)->m_fBurnerBlendAmount = m_fBurnerElapsed / 100.f;
		}

		if (m_pLeft_AfterBurnerEX != NULL)
		{
			GET_MANAGER<ObjectManager>()->GetObjFromTag(L"jetFlameRight", OBJ_EFFECT2)->SetPosition(m_pLeft_AfterBurnerEX->GetPosition().x - m_pLeft_AfterBurnerEX->GetLook().x * fPosValue,
				m_pLeft_AfterBurnerEX->GetPosition().y - m_pLeft_AfterBurnerEX->GetLook().y * fPosValue, m_pLeft_AfterBurnerEX->GetPosition().z - m_pLeft_AfterBurnerEX->GetLook().z * fPosValue);
			if (m_fBurnerElapsed / 100.f <= 1.0f)
				GET_MANAGER<ObjectManager>()->GetObjFromTag(L"jetFlameRight", OBJ_EFFECT2)->m_fBurnerBlendAmount = m_fBurnerElapsed / 100.f;
		}
		/*if (m_fBurnerElapsed / 150 <= 0.4f)
		{
			m_pLeft_AfterBurnerEX->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
			m_pLeft_AfterBurnerIN->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
			m_pRight_AfterBurnerEX->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
			m_pRight_AfterBurnerIN->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
		}*/

		/*m_pLeft_AfterBurner[i]->m_pAfterBurner->UpdateTransform(&m_pLeft_AfterBurner[i]->m_xmf4x4World);
		m_pRight_AfterBurner[i]->m_pAfterBurner->UpdateTransform(&m_pRight_AfterBurner[i]->m_xmf4x4World);
		if (m_fBurnerElapsed / 100 < 0.8)
		{
			m_pLeft_AfterBurner[i]->m_pAfterBurner->SetPlaneScale(m_fBurnerElapsed / 100);
			m_pRight_AfterBurner[i]->m_pAfterBurner->SetPlaneScale(m_fBurnerElapsed / 100);
		}*/
	}
	if (m_pLeft_AfterBurner)
	{
		if (m_fBurnerElapsed / 150 <= 1.0f)
		{
			m_pLeft_AfterBurner->m_pAfterBurner->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
			m_pRight_AfterBurner->m_pAfterBurner->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
		}
		m_pLeft_AfterBurner->m_pAfterBurner->UpdateTransform(&m_pLeft_AfterBurner->m_xmf4x4World);
		m_pRight_AfterBurner->m_pAfterBurner->UpdateTransform(&m_pRight_AfterBurner->m_xmf4x4World);
		/*if (m_fBurnerElapsed / 150 <= 0.4f)
		{
			m_pLeft_AfterBurnerEX->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
			m_pLeft_AfterBurnerIN->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
			m_pRight_AfterBurnerEX->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
			m_pRight_AfterBurnerIN->m_fBurnerBlendAmount = m_fBurnerElapsed / 150;
		}*/

		/*m_pLeft_AfterBurner[i]->m_pAfterBurner->UpdateTransform(&m_pLeft_AfterBurner[i]->m_xmf4x4World);
		m_pRight_AfterBurner[i]->m_pAfterBurner->UpdateTransform(&m_pRight_AfterBurner[i]->m_xmf4x4World);
		if (m_fBurnerElapsed / 100 < 0.8)
		{
			m_pLeft_AfterBurner[i]->m_pAfterBurner->SetPlaneScale(m_fBurnerElapsed / 100);
			m_pRight_AfterBurner[i]->m_pAfterBurner->SetPlaneScale(m_fBurnerElapsed / 100);
		}*/
	}
}

void CAirplanePlayer::SetEngineRefractionPos()
{
	if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"EngineRefractionObj", OBJ_EFFECT) && m_pEngineRefraction != NULL)
	{
		GET_MANAGER<ObjectManager>()->GetObjFromTag(L"EngineRefractionObj", OBJ_EFFECT)->SetPosition(m_pEngineRefraction->GetPosition());
		//GET_MANAGER<ObjectManager>()->GetObjFromTag(L"EngineRefractionObj", OBJ_EFFECT)->m_xmf4x4ToParent = m_pEngineRefraction->m_xmf4x4World;
		//GET_MANAGER<ObjectManager>()->GetObjFromTag(L"EngineRefractionObj", OBJ_EFFECT)->SetScale(0.1, 0.1, 0);
		//GET_MANAGER<ObjectManager>()->GetObjFromTag(L"EngineRefractionObj", OBJ_EFFECT)->SetPlaneScale(100);
	}
}

void CAirplanePlayer::SetNaviPosition()
{
	if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui16_navigator", OBJ_NAVIGATOR) != NULL)
		if (m_pNaviPos != NULL)
			GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player_ui16_navigator", OBJ_NAVIGATOR)->SetPosition(m_pNaviPos->GetPosition());
}

void CAirplanePlayer::OnPrepareRender()
{
	CPlayer::OnPrepareRender();
}

CCamera* CAirplanePlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		SetFriction(2.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(2.5f);
		SetMaxVelocityY(40.0f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->OrthogonalProjectionMatrix(1.01f, 5000.0f, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case SPACESHIP_CAMERA:
		SetFriction(1200);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(1000.0f);
		SetMaxVelocityY(1000.0f);
		m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.6f, -4.1f));
		m_pCamera->GenerateProjectionMatrix(1.01f, m_fFarPlaneDistance, ASPECT_RATIO, 60.0f);
		m_pCamera->OrthogonalProjectionMatrix(-5000.f, m_fFarPlaneDistance, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case THIRD_PERSON_CAMERA:
		SetFriction(20.5f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(25.5f);
		SetMaxVelocityY(20.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 1.0f, -5.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->OrthogonalProjectionMatrix(1.01f, 5000.0f, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	default:
		break;
	}

	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
	Animate(fTimeElapsed);

	return(m_pCamera);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
CTerrainPlayer::CTerrainPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, void* pContext)
{
	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);

	CLoadedModelInfo* pAngrybotModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Model/water.bin", NULL, MODEL_STD);
	SetChild(pAngrybotModel->m_pModelRootObject, true);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	SetPlayerUpdatedContext(pContext);
	SetCameraUpdatedContext(pContext);
}

CTerrainPlayer::~CTerrainPlayer()
{
}

CCamera* CTerrainPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return(m_pCamera);
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, -400.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case SPACESHIP_CAMERA:
		SetFriction(125.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 10000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case THIRD_PERSON_CAMERA:
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, -50.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	default:
		break;
	}

	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
	Update(fTimeElapsed);

	return(m_pCamera);
}

void CTerrainPlayer::OnPlayerUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)m_pPlayerUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3PlayerPosition = GetPosition();
	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) + 0.0f;
	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		SetPosition(xmf3PlayerPosition);
	}
}

void CTerrainPlayer::OnCameraUpdateCallback(float fTimeElapsed)
{
	CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)m_pCameraUpdatedContext;
	XMFLOAT3 xmf3Scale = pTerrain->GetScale();
	XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();
	int z = (int)(xmf3CameraPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z, bReverseQuad) + 5.0f;
	if (xmf3CameraPosition.y <= fHeight)
	{
		xmf3CameraPosition.y = fHeight;
		m_pCamera->SetPosition(xmf3CameraPosition);
		if (m_pCamera->GetMode() == THIRD_PERSON_CAMERA)
		{
			CThirdPersonCamera* p3rdPersonCamera = (CThirdPersonCamera*)m_pCamera;
			p3rdPersonCamera->SetLookAt(GetPosition());
		}
	}
}

int CTerrainPlayer::Update(float fTimeElapsed)
{
	CPlayer::Animate(fTimeElapsed);

	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z * m_xmf3Velocity.z);
	SetTrackAnimationSet(0, ::IsZero(fLength) ? 0 : 1);

	return 0;
}