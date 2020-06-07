#pragma once
#include "stdafx.h"
#include "CBlur.h"
#include "CBlurFilter.h"


CBlurFilter::CBlurFilter(ID3D12Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	md3dDevice = device;

	mWidth = width;
	mHeight = height;
	mFormat = format;

	BuildResources();
}
CBlurFilter::CBlurFilter()
{
}
CBlurFilter::~CBlurFilter()
{

}
ID3D12Resource* CBlurFilter::Output()
{
	return mBlurMap0.Get();
}

void CBlurFilter::BuildDescriptors(CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor,
	CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor, UINT descriptorSize)
{
	//�����ڿ� ���� �������� �����صд�.
	mBlur0CpuSrv = hCpuDescriptor;
	mBlur0CpuUav = hCpuDescriptor.Offset(1, descriptorSize);
	mBlur1CpuSrv = hCpuDescriptor.Offset(1, descriptorSize);
	mBlur1CpuUav = hCpuDescriptor.Offset(1, descriptorSize);

	mBlur0GpuSrv = hGpuDescriptor;
	mBlur0GpuUav = hGpuDescriptor.Offset(1, descriptorSize);
	mBlur1GpuSrv = hGpuDescriptor.Offset(1, descriptorSize);
	mBlur1GpuUav = hGpuDescriptor.Offset(1, descriptorSize);

	BuildDescriptors();
}

void CBlurFilter::OnResize(UINT newWidth, UINT newHeight)
{
	if ((mWidth != newWidth) || (mHeight != newHeight))
	{
		mWidth = newWidth;
		mHeight = newHeight;
		
		//�� ũ��� ȭ�� �� �ؽ�ó �ڿ����� �ٽ� �����.
		BuildResources();

		// �ڿ����� ���ŵǾ����Ƿ� �ش� �����ڵ鵵 �ٽ� �����.
		BuildDescriptors();
	}
}

void CBlurFilter::Execute(ID3D12GraphicsCommandList* pd3dCommandList,
	ID3D12RootSignature* pd3dRootSignature,
	ID3D12PipelineState* horzBlurPSO,
	ID3D12PipelineState* vertBlurPSO,
	ID3D12Resource* input,
	int blurCount)
{
	auto weights = CalcGaussWeights(3.5f);    // ǥ�� ������ 2.5�� �ϰ� ���콺 �Լ��� ����ġ�� ����Ѵ�.
	int blurRadius = (int)weights.size() / 2;     // ����(m = n �� ��� ũ�� / 2)

	pd3dCommandList->SetComputeRootSignature(pd3dRootSignature);
	
	pd3dCommandList->SetComputeRoot32BitConstants(17, 1, &blurRadius, 0);
	pd3dCommandList->SetComputeRoot32BitConstants(17, (UINT)weights.size(), weights.data(), 1);

	// GPU�� �غ� �ȵ� ���¿��� �ڿ� ���� ���̽� �ڿ� ���� ��Ȳ�� ���ϱ� ���ؼ� ���¸� �����ϰ�
	// �������α׷��� ���� ���̸� D3D���� ���������ν� GPU�� �ڿ� ������ ���ϴµ� ��ó�� �� �� �ִ�.
	// ���� �ڿ� �庮���� �迭�� �����Ͽ� �ѹ��� API ȣ��� �������� �ڿ��� ������ �� �ִ�.

	// D3D12_RESOURCE_STATE_RENDER_TARGET : �ڿ��� ���� ������� ��� �ȴ�. 
	// �̰��� ���� ���� �����̴�. 
	// D3D12_RESOURCE_STATE_COPY_SOURCE : �ڿ��� ���� �۾����� �ҽ��� ���ȴ�. 
	// �̰��� �б� ���� �����̴�. 
	
	// D3D12_RESOURCE_STATE_RENDER_TARGET ���� 
	// D3D12_RESOURCE_STATE_COPY_SOURCE �� ���� ����
	// �ĸ� ���� �ڿ��� 
	// ���� ���(����)���� 
	// ���� �۾� �ҽ�(�б�)�� ���� ����
	pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(input,
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE));

	// D3D12_RESOURCE_STATE_COMMON : �ٸ� �׷��� ���� ������ �ڿ��� �׼��� �� ��쿡�� ���� ���α׷��� �� ���·� ��ȯ�Ǿ���Ѵ�.
	// Ư��, �ڿ��� COPY ť���� ���Ǳ� ����(������ DIRECT / COMPUTE���� ��� �� ���) �� DIRECT / COMPUTE���� ���Ǳ� ����(������ COPY���� ��� �� ���) COMMON ���¿� �־���Ѵ�.
	// D3D12_RESOURCE_STATE_COPY_DEST : �ڿ��� ���� �۾����� ������� ���ȴ�. 
	// �̰��� ���� ���� �����̴�.
	
	// ȭ�� �� �ؽ�ó �ڿ� �ؽ�ó A�� 
	// COMMON���� 
	// ���� �۾� ���(����)�� ���� ����
	pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

	// �ĸ� ���۸� BlurMap0�� �����Ѵ�.
	pd3dCommandList->CopyResource(mBlurMap0.Get(), input);

	// D3D12_RESOURCE_STATE_GENERIC_READ : �ٸ� �б� ���� ��Ʈ�� ���� OR �����̴�. ���ε� ���� �ʼ� ���� �����̴�. 
	
	// ȭ�� �� �ؽ�ó �ڿ� �ؽ�ó A�� 
	// ���� �۾� ���(����)���� 
	// ���ε� �� ���� ���·� ���� ����
	pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	// D3D12_RESOURCE_STATE_UNORDERED_ACCESS : �ڿ��� �������� �׼����� ���ȴ�.
	// ���ĵ��� ���� �׼������⸦ ���� GPU�� ���� �ڿ��� �׼��� �� �� ���� �ڿ��� �� ���¿� �־���Ѵ�. 
	// �̰��� �б�/���� �����̴�.

	// ȭ�� �� �ؽ�ó �ڿ� �ؽ�ó B��
	// COMMON���� 
	// ���ĵ��� ���� �׼������⸦ ���� GPU�� ���� �ڿ��� �׼��� �� �� �ֵ���(�б�/����) ���� ����
	pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap1.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

	for (int i = 0; i < blurCount; ++i)
	{
		//
		// ���� ��
		//
	
		pd3dCommandList->SetPipelineState(horzBlurPSO);

		pd3dCommandList->SetComputeRootDescriptorTable(1, mBlur0GpuSrv);
		pd3dCommandList->SetComputeRootDescriptorTable(2, mBlur1GpuUav);

		// numGroupsX�� �� ��(row)�� �̹��� �ȼ����� ó���ϴ� �� �ʿ���
		// ������ �׷��� �����̴�. �� �׷��� 256���� �ȼ��� ó���ؾ� �Ѵ�.
		// (256�� ��� ���̴��� ���ǵǾ� �ִ� ��ġ�̴�.)
		UINT numGroupsX = (UINT)ceilf(mWidth / 256.0f);
		pd3dCommandList->Dispatch(numGroupsX, mHeight, 1); // ��ǻƮ ���̴� ����

		pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

		pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap1.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ));

		//
		// ���� ��
		//
		
		pd3dCommandList->SetPipelineState(vertBlurPSO);

		pd3dCommandList->SetComputeRootDescriptorTable(1, mBlur1GpuSrv);
		pd3dCommandList->SetComputeRootDescriptorTable(2, mBlur0GpuUav);

		// numGroupsY�� �� �� (column)�� �̹��� �ȼ����� ó���ϴµ� �ʿ���
		// ������ ũ���� �����̴�. �� �׷��� 256���� �ȼ��� ó���ؾ� �Ѵ�.
		// (256�� ��� ���̴��� ���ǵǾ� �ִ� ��ġ�̴�.)
		UINT numGroupsY = (UINT)ceilf(mHeight / 256.0f);
		pd3dCommandList->Dispatch(mWidth, numGroupsY, 1);

		pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ));

		pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap1.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
	}
}

std::vector<float> CBlurFilter::CalcGaussWeights(float sigma)
{
	float twoSigma2 = 2.0f * sigma * sigma;  // �ñ׸�(ǥ������)�� Ŀ�� ���� �̿� �ȼ��鿡 �� ū ����ġ�� �ο��ȴ�.

	int blurRadius = (int)ceil(2.0f * sigma);  // ���� �ٻ�� (�� ������ = �ø�(2*�ñ׸�(ǥ������)))

	assert(blurRadius <= MaxBlurRadius);  // true �ϰ�� �ƹ��� ����, false �� ��� ����

	std::vector<float> weights;
	weights.resize(2 * blurRadius + 1);  // ��� m x n    m = 2a + 1, n = 2b + 1 ( a = b ��� �����Ѵ� )

	float weightSum = 0.0f;  //  ����ġ�� �� �ʱ�ȭ

	for (int i = -blurRadius; i <= blurRadius; ++i)
	{
		float x = (float)i;

		weights[i + blurRadius] = expf(-x * x / twoSigma2);

		weightSum += weights[i + blurRadius];
	}

	//  ����ġ ����ȭ
	for (int i = 0; i < weights.size(); ++i)
	{
		weights[i] /= weightSum; 
	}

	return weights;
}

void CBlurFilter::BuildDescriptors()
{
	//���� ����
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = mFormat;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

	uavDesc.Format = mFormat;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	// ���� ����

	md3dDevice->CreateShaderResourceView(mBlurMap0.Get(), &srvDesc, mBlur0CpuSrv);
	md3dDevice->CreateUnorderedAccessView(mBlurMap0.Get(), NULL, &uavDesc, mBlur0CpuUav);

	md3dDevice->CreateShaderResourceView(mBlurMap1.Get(), &srvDesc, mBlur1CpuSrv);
	md3dDevice->CreateUnorderedAccessView(mBlurMap1.Get(), NULL, &uavDesc, mBlur1CpuUav);
}

void CBlurFilter::BuildResources()
{
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = mFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	HRESULT hresult = md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&mBlurMap0));

	hresult = md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&mBlurMap1));
}

