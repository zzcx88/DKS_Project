#include "stdafx.h"
#include "CPlayer.h"
#include "CCamera.h"

CCamera::CCamera()
{
	m_xmf4x4View = Matrix4x4::Identity();
	m_xmf4x4Identity = Matrix4x4::Identity();
	m_xmf4x4Projection = Matrix4x4::Identity();
	m_xmf4x4OrthogonalProjection = Matrix4x4::Identity();
	

	m_d3dViewport = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	m_d3dScissorRect = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;
	m_xmf3Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fTimeLag = 0.0f;
	m_xmf3LookAtWorld = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_nMode = 0x00;
	m_pPlayer = NULL;
}

CCamera::CCamera(CCamera* pCamera)
{
	if (pCamera)
	{
		*this = *pCamera;
	}
	else
	{
		m_xmf4x4View = Matrix4x4::Identity();
		m_xmf4x4Identity = Matrix4x4::Identity();
		m_xmf4x4Projection = Matrix4x4::Identity();
		m_xmf4x4OrthogonalProjection = Matrix4x4::Identity();
		m_d3dViewport = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
		m_d3dScissorRect = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
		m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
		m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = 0.0f;
		m_xmf3Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_fTimeLag = 0.0f;
		m_xmf3LookAtWorld = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_nMode = 0x00;
		m_pPlayer = NULL;
	}
}

CCamera::~CCamera()
{
}

void CCamera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
}

void CCamera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
{
	m_d3dScissorRect.left = xLeft;
	m_d3dScissorRect.top = yTop;
	m_d3dScissorRect.right = xRight;
	m_d3dScissorRect.bottom = yBottom;
}

void CCamera::SetLookVector(XMFLOAT3 xmf3Look)
{
	m_xmf3Look = xmf3Look;
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	m_xmf4x4Projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);


	//XMMATRIX xmmtxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
		//XMStoreFloat4x4(&m_xmf4x4Projection, xmmtxProjection);
}
void CCamera::OrthogonalProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fViewW, float fViewH)
{
	m_xmf4x4OrthogonalProjection = Matrix4x4::OrthoLH(fViewW, fViewH, fNearPlaneDistance, fFarPlaneDistance);

		//XMMATRIX xmmtxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
			//XMStoreFloat4x4(&m_xmf4x4Projection, xmmtxProjection);
}
void CCamera::GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up)
{
	m_xmf3Position = xmf3Position;
	m_xmf3LookAtWorld = xmf3LookAt;
	m_xmf3Up = xmf3Up;

	GenerateViewMatrix();
}

void CCamera::GenerateViewMatrix()
{
	m_xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, m_xmf3LookAtWorld, m_xmf3Up);
}

void CCamera::RegenerateViewMatrix()
{
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);

	m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;
	m_xmf4x4View._41 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Right);
	m_xmf4x4View._42 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Up);
	m_xmf4x4View._43 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Look);

	//카메라 변환 행렬이 바뀔 때마다 절두체를 다시 생성한다(절두체는 월드 좌표계로 생성한다).
	GenerateFrustum();
}

void CCamera::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_CAMERA_INFO) + 255) & ~255); //256의 배수
	m_pd3dcbCamera = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbCamera->Map(0, NULL, (void**)&m_pcbMappedCamera);
}

void CCamera::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4View, &xmf4x4View, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Identity;
	XMStoreFloat4x4(&xmf4x4Identity, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Identity)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4Identity, &xmf4x4Identity, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4Projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));
	
	XMFLOAT4X4 xmf4x4OrthogonalProjection;
	XMStoreFloat4x4(&xmf4x4OrthogonalProjection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4OrthogonalProjection)));
	::memcpy(&m_pcbMappedCamera->m_xmf4x4OrthogonalProjection, &xmf4x4OrthogonalProjection, sizeof(XMFLOAT4X4));
	
	::memcpy(&m_pcbMappedCamera->m_xmf3Position, &m_xmf3Position, sizeof(XMFLOAT3));
	//if (GET_MANAGER<ObjectManager>()->GetObjFromTag(L"player", OBJ_PLAYER) != NULL)
	//{
	m_fTestFloat = 0.5f;
	::memcpy(&m_pcbMappedCamera->m_fTestFloat, &m_fTestFloat, sizeof(float));
	//}

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbCamera->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(0, d3dGpuVirtualAddress);
}

void CCamera::ReleaseShaderVariables()
{
	if (m_pd3dcbCamera)
	{
		m_pd3dcbCamera->Unmap(0, NULL);
		m_pd3dcbCamera->Release();
	}
}

void CCamera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSpaceShipCamera

CSpaceShipCamera::CSpaceShipCamera(CCamera* pCamera) : CCamera(pCamera)
{
	m_nMode = SPACESHIP_CAMERA;
}

void CSpaceShipCamera::Rotate(float x, float y, float z)
{
	if (m_pPlayer && (x != 0.0f))
	{
		XMFLOAT3 xmf3Right = m_pPlayer->GetRightVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(x));
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Position = Vector3::Subtract(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Position = Vector3::TransformCoord(m_xmf3Position, xmmtxRotate);
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_pPlayer->GetPosition());
	}
	if (m_pPlayer && (y != 0.0f))
	{
		XMFLOAT3 xmf3Up = m_pPlayer->GetUpVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Position = Vector3::Subtract(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Position = Vector3::TransformCoord(m_xmf3Position, xmmtxRotate);
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_pPlayer->GetPosition());
	}
	if (m_pPlayer && (z != 0.0f))
	{
		XMFLOAT3 xmf3Look = m_pPlayer->GetLookVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));

		/*XMVECTOR DestRight = XMLoadFloat3(&m_pPlayer->GetRightVector());
		XMVECTOR DestUp = XMLoadFloat3(&m_pPlayer->GetUpVector());
		XMVECTOR DestLook = XMLoadFloat3(&m_pPlayer->GetLookVector());

		XMVECTOR StartRight = XMLoadFloat3(&m_xmf3Right);
		XMVECTOR StartUp = XMLoadFloat3(&m_xmf3Up);
		XMVECTOR StartLook = XMLoadFloat3(&m_xmf3Look);

		XMStoreFloat3(&m_xmf3Right, XMVectorLerp(StartRight, DestRight, 0.01 * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
		XMStoreFloat3(&m_xmf3Up, XMVectorLerp(StartUp, DestUp, 0.01 * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
		XMStoreFloat3(&m_xmf3Look, XMVectorLerp(StartLook, DestLook, 0.01 * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));*/

		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);

		m_xmf3Position = Vector3::Subtract(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Position = Vector3::TransformCoord(m_xmf3Position, xmmtxRotate);
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_pPlayer->GetPosition());
	}
}

void CSpaceShipCamera::Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed)
{
	if (m_pPlayer)
	{
		XMFLOAT4X4 xmf4x4Rotate = Matrix4x4::Identity();
		XMFLOAT3 xmf3Right = m_pPlayer->GetRightVector();
		XMFLOAT3 xmf3Up = m_pPlayer->GetUpVector();
		XMFLOAT3 xmf3Look = m_pPlayer->GetLookVector();
		xmf4x4Rotate._11 = xmf3Right.x; xmf4x4Rotate._21 = xmf3Up.x; xmf4x4Rotate._31 = xmf3Look.x;
		xmf4x4Rotate._12 = xmf3Right.y; xmf4x4Rotate._22 = xmf3Up.y; xmf4x4Rotate._32 = xmf3Look.y;
		xmf4x4Rotate._13 = xmf3Right.z; xmf4x4Rotate._23 = xmf3Up.z; xmf4x4Rotate._33 = xmf3Look.z;

		XMFLOAT3 xmf3Offset = Vector3::TransformCoord(m_xmf3Offset, xmf4x4Rotate);
		XMFLOAT3 xmf3Position = Vector3::Add(m_pPlayer->GetPosition(), xmf3Offset);
		XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, m_xmf3Position);
		float fLength = Vector3::Length(xmf3Direction);
		xmf3Direction = Vector3::Normalize(xmf3Direction);
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;
		if (fDistance > fLength) fDistance = fLength;
		if (fLength < 0.01f) fDistance = fLength;
		if (fDistance > 0)
		{
			m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Direction, fDistance);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFirstPersonCamera

CFirstPersonCamera::CFirstPersonCamera(CCamera* pCamera) : CCamera(pCamera)
{
	m_nMode = FIRST_PERSON_CAMERA;
	if (pCamera)
	{
		if (pCamera->GetMode() == SPACESHIP_CAMERA)
		{
			m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_xmf3Right.y = 0.0f;
			m_xmf3Look.y = 0.0f;
			m_xmf3Right = Vector3::Normalize(m_xmf3Right);
			m_xmf3Look = Vector3::Normalize(m_xmf3Look);
		}
	}
}

void CFirstPersonCamera::Rotate(float x, float y, float z)
{
	if (x != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}
	if (m_pPlayer && (y != 0.0f))
	{
		XMFLOAT3 xmf3Up = m_pPlayer->GetUpVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}
	if (m_pPlayer && (z != 0.0f))
	{
		XMFLOAT3 xmf3Look = m_pPlayer->GetLookVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));
		m_xmf3Position = Vector3::Subtract(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Position = Vector3::TransformCoord(m_xmf3Position, xmmtxRotate);
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CThirdPersonCamera

CThirdPersonCamera::CThirdPersonCamera(CCamera* pCamera) : CCamera(pCamera)
{
	m_nMode = THIRD_PERSON_CAMERA;
	if (pCamera)
	{
		if (pCamera->GetMode() == SPACESHIP_CAMERA)
		{
			m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_xmf3Right.y = 0.0f;
			m_xmf3Look.y = 0.0f;
			m_xmf3Right = Vector3::Normalize(m_xmf3Right);
			m_xmf3Look = Vector3::Normalize(m_xmf3Look);
		}
	}
}

void CThirdPersonCamera::Rotate(float x, float y, float z)
{
	if (m_pPlayer && (x != 0.0f))
	{
		XMFLOAT3 xmf3Right = m_pPlayer->GetRightVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(x));
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Position = Vector3::Subtract(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Position = Vector3::TransformCoord(m_xmf3Position, xmmtxRotate);
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_pPlayer->GetPosition());
	}
	if (m_pPlayer && (y != 0.0f))
	{
		XMFLOAT3 xmf3Up = m_pPlayer->GetUpVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Up), XMConvertToRadians(y));
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Position = Vector3::Subtract(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Position = Vector3::TransformCoord(m_xmf3Position, xmmtxRotate);
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_pPlayer->GetPosition());
	}
	if (m_pPlayer && (z != 0.0f))
	{
		XMFLOAT3 xmf3Look = m_pPlayer->GetLookVector();
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Look), XMConvertToRadians(z));
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
		m_xmf3Position = Vector3::Subtract(m_xmf3Position, m_pPlayer->GetPosition());
		m_xmf3Position = Vector3::TransformCoord(m_xmf3Position, xmmtxRotate);
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_pPlayer->GetPosition());
	}
}

void CThirdPersonCamera::Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed)
{
	if (m_pPlayer)
	{
		XMFLOAT4X4 xmf4x4Rotate = Matrix4x4::Identity();
		XMFLOAT3 xmf3Right = m_pPlayer->GetRightVector();
		XMFLOAT3 xmf3Up = m_pPlayer->GetUpVector();
		XMFLOAT3 xmf3Look = m_pPlayer->GetLookVector();
		xmf4x4Rotate._11 = xmf3Right.x; xmf4x4Rotate._21 = xmf3Up.x; xmf4x4Rotate._31 = xmf3Look.x;
		xmf4x4Rotate._12 = xmf3Right.y; xmf4x4Rotate._22 = xmf3Up.y; xmf4x4Rotate._32 = xmf3Look.y;
		xmf4x4Rotate._13 = xmf3Right.z; xmf4x4Rotate._23 = xmf3Up.z; xmf4x4Rotate._33 = xmf3Look.z;

		XMFLOAT3 xmf3Offset = Vector3::TransformCoord(m_xmf3Offset, xmf4x4Rotate);
		XMFLOAT3 xmf3Position = Vector3::Add(m_pPlayer->GetPosition(), xmf3Offset);
		XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, m_xmf3Position);
		float fLength = Vector3::Length(xmf3Direction);
		xmf3Direction = Vector3::Normalize(xmf3Direction);
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;
		if (fDistance > fLength) fDistance = fLength;
		if (fLength < 0.01f) fDistance = fLength;
		if (fDistance < 0)
		{
			m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Direction, fDistance);
			SetLookAt(xmf3LookAt);
		}
	}
}

void CThirdPersonCamera::SetLookAt(XMFLOAT3& xmf3LookAt)
{
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, m_pPlayer->GetUpVector());
	m_xmf3Right = XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_xmf3Up = XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_xmf3Look = XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}

void CCamera::SetLookAt(XMFLOAT3& xmf3LookAt)
{
	XMFLOAT4X4 mtxLookAt = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, m_pPlayer->GetUpVector());
	XMVECTOR DestRight = XMLoadFloat3(&XMFLOAT3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31));
	XMVECTOR DestUp = XMLoadFloat3(&XMFLOAT3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._33));
	XMVECTOR DestLook = XMLoadFloat3(&XMFLOAT3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33));
	
	XMVECTOR StartRight = XMLoadFloat3(&m_xmf3Right);
	XMVECTOR StartUp = XMLoadFloat3(&m_xmf3Up);
	XMVECTOR StartLook = XMLoadFloat3(&m_xmf3Look);

	XMStoreFloat3(&m_xmf3Right, XMVectorLerp(StartRight,DestRight, 10.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
	XMStoreFloat3(&m_xmf3Up, XMVectorLerp(StartUp, DestUp, 10.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
	XMStoreFloat3(&m_xmf3Look, XMVectorLerp(StartLook, DestLook, 10.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
}

void CCamera::SetLookPlayer(bool bLerpOpt)
{
	if (bLerpOpt == true)
	{
		if (m_bDefaultCameraMode == false)
		{
			XMVECTOR DestRight = XMLoadFloat3(&m_pPlayer->GetRightVector());
			XMVECTOR DestUp = XMLoadFloat3(&m_pPlayer->GetUpVector());

			XMFLOAT3 Look = m_pPlayer->GetLookVector();
			Look.x = Look.x + m_pPlayer->GetUpVector().x * 0.15;
			Look.y = Look.y + m_pPlayer->GetUpVector().y * 0.15;
			Look.z = Look.z + m_pPlayer->GetUpVector().z * 0.15;
			XMVECTOR DestLook = XMLoadFloat3(&Look);

			XMVECTOR StartRight = XMLoadFloat3(&m_xmf3Right);
			XMVECTOR StartUp = XMLoadFloat3(&m_xmf3Up);
			XMVECTOR StartLook = XMLoadFloat3(&m_xmf3Look);

			XMStoreFloat3(&m_xmf3Right, XMVectorLerp(StartRight, DestRight, 5.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
			XMStoreFloat3(&m_xmf3Up, XMVectorLerp(StartUp, DestUp, 5.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
			XMStoreFloat3(&m_xmf3Look, XMVectorLerp(StartLook, DestLook, 5.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
		}
		else
		{
			XMVECTOR DestRight = XMLoadFloat3(&m_pPlayer->GetRightVector());
			XMVECTOR DestUp = XMLoadFloat3(&m_pPlayer->GetUpVector());
			XMVECTOR DestLook = XMLoadFloat3(&m_pPlayer->GetLookVector());

			XMVECTOR StartRight = XMLoadFloat3(&m_xmf3Right);
			XMVECTOR StartUp = XMLoadFloat3(&m_xmf3Up);
			XMVECTOR StartLook = XMLoadFloat3(&m_xmf3Look);

			XMStoreFloat3(&m_xmf3Right, XMVectorLerp(StartRight, DestRight, 5.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
			XMStoreFloat3(&m_xmf3Up, XMVectorLerp(StartUp, DestUp, 5.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
			XMStoreFloat3(&m_xmf3Look, XMVectorLerp(StartLook, DestLook, 5.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
		}

		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		XMVECTOR DestPosition;
		if(m_bDefaultCameraMode == false)
			xmf3Shift = Vector3::Add(xmf3Shift, m_pPlayer->GetUpVector(), 0.3);
		else
			xmf3Shift = Vector3::Add(xmf3Shift, m_pPlayer->GetUpVector(), 0.6);
		DestPosition = XMLoadFloat3(&Vector3::Add(m_pPlayer->GetPosition(), xmf3Shift));
		xmf3Shift = Vector3::Add(xmf3Shift, m_pPlayer->GetLookVector(), -4.1);
		DestPosition = XMLoadFloat3(&Vector3::Add(m_pPlayer->GetPosition(), xmf3Shift));

		//XMVECTOR DestPosition = XMLoadFloat3(&m_pPlayer->GetPosition());
		XMVECTOR StartPosition = XMLoadFloat3(&GetPosition());
		XMVECTOR Result = XMVectorLerp(StartPosition, DestPosition, 20.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed());
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, Result);
		SetPosition(xmf3Result);
	}
	else
	{
		SetPosition(m_pPlayer->GetPosition());
		m_xmf3Right = m_pPlayer->GetRightVector();
		m_xmf3Up = m_pPlayer->GetUpVector();
		m_xmf3Look = m_pPlayer->GetLookVector();
	}
}

void CCamera::ShakingCamera()
{
	m_fShakingTimeElapsed += 10.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed();

	XMFLOAT3 xmf3Right = m_pPlayer->GetRightVector();
	XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Right), XMConvertToRadians(m_nShakeAmplitude));

	XMFLOAT3 Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
	XMFLOAT3 Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
	XMFLOAT3 Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);

	XMVECTOR DestRight = XMLoadFloat3(&Right);
	XMVECTOR DestUp = XMLoadFloat3(&Up);
	XMVECTOR DestLook = XMLoadFloat3(&Look);

	XMVECTOR StartRight = XMLoadFloat3(&m_xmf3Right);
	XMVECTOR StartUp = XMLoadFloat3(&m_xmf3Up);
	XMVECTOR StartLook = XMLoadFloat3(&m_xmf3Look);

	XMStoreFloat3(&m_xmf3Right, XMVectorLerp(StartRight, DestRight, 10.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
	XMStoreFloat3(&m_xmf3Up, XMVectorLerp(StartUp, DestUp, 10.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));
	XMStoreFloat3(&m_xmf3Look, XMVectorLerp(StartLook, DestLook, 10.f * GET_MANAGER<CDeviceManager>()->GetGameTimer().GetTimeElapsed()));

	if (m_bEneShake == false)
	{
		if (m_fShakingTimeElapsed > m_fShakingTimeFrequncy)
		{
			if (m_nShakeAmplitude < 0)
				m_nShakeAmplitude += 5;
			else
				m_nShakeAmplitude -= 5;
			m_nShakeAmplitude *= -1;
			m_fShakingTimeElapsed = 0;
		}
	}
	else
	{
		if (m_fShakingTimeElapsed > 0.25f)
		{
			if (m_nShakeAmplitude < 0)
				m_nShakeAmplitude += 5;
			else
				m_nShakeAmplitude -= 5;
			m_nShakeAmplitude *= -1;
			m_fShakingTimeElapsed = 0;
		}
	}

	if (m_nShakeAmplitude == 0)
	{
		m_bShakeSwitch = false;
		m_nShakeAmplitude = 50;
		m_bEneShake = false;
	}
}


bool CCamera::IsInFrustum(BoundingSphere& xmBoundingSphere)
{
	return(m_xmFrustum.Intersects(xmBoundingSphere));
}

void CCamera::GenerateFrustum()
{
	//원근 투영 변환 행렬에서 절두체를 생성한다(절두체는 카메라 좌표계로 표현된다).
	m_xmFrustum.CreateFromMatrix(m_xmFrustum, XMLoadFloat4x4(&m_xmf4x4Projection));
	//카메라 변환 행렬의 역행렬을 구한다.
	XMMATRIX xmmtxInversView = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4View));
	//절두체를 카메라 변환 행렬의 역행렬로 변환한다(이제 절두체는 월드 좌표계로 표현된다).
	m_xmFrustum.Transform(m_xmFrustum, xmmtxInversView);
}

bool CCamera::IsInFrustum(BoundingOrientedBox& xmBoundingBox)
{
	return(m_xmFrustum.Intersects(xmBoundingBox));
}

bool CCamera::IsInFrustum(BoundingBox& xmBoundingBox)
{
	return(m_xmFrustum.Intersects(xmBoundingBox));
}

