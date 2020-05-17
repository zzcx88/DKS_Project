#pragma once

class CBlurFilter
{
public:
	CBlurFilter(ID3D12Device* device, UINT width, UINT height, DXGI_FORMAT format);
	CBlurFilter();
	CBlurFilter(const CBlurFilter& rhs) = delete;
	CBlurFilter& operator=(const CBlurFilter& rhs) = delete;
	~CBlurFilter();

	ID3D12Resource* Output();

	void BuildDescriptors(															// �ڿ����� ����Ǿ��� �� �ش� �����ڵ��� �ٽ� �����ϱ� ���� �Լ�
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor,
		CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor,
		UINT descriptorSize);
	void OnResize(UINT newWidth, UINT newHeight);					// Ŭ���̾�Ʈ â�� �ٲ� ��� ����ϴ� �Լ�

	void Execute(																// �� �������� ����� ������ �׷� ������ �ľ��ϰ�, �帮�� ������ ���� ��� ���̴� ȣ�� ����� ����ϴ� �Լ�
		ID3D12GraphicsCommandList* pd3dCommandList,
		ID3D12RootSignature* pd3dRootSignature,
		ID3D12PipelineState* horzBlurPSO,
		ID3D12PipelineState* vertBlurPSO,
		ID3D12Resource* input,
		int blurCount);


	CShader* m_pShader = NULL;
private:
	std::vector<float> CalcGaussWeights(float sigma);					//  ���콺 �Լ��� ����ġ ���ϴ� �Լ� 2���� �帮�⺸�� 1���� �帮���� ǥ�� ������ �ξ� ����.

	void BuildDescriptors();															// �׷��Ƚ� ���������ο� Bind �� ������ ��ü�� ����
	void BuildResources();															// �����ڰ� ������ �ڿ��� ����

private:


	const int MaxBlurRadius = 10;															// �ִ� �� ������

	ID3D12Device* md3dDevice = nullptr;

	UINT mWidth = FRAME_BUFFER_WIDTH;
	UINT mHeight = FRAME_BUFFER_HEIGHT;
	DXGI_FORMAT mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur0CpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur0CpuUav;

	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur1CpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur1CpuUav;

	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuSrv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuUav;

	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur1GpuSrv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur1GpuUav;

	// Two for ping-ponging the textures.
	Microsoft::WRL::ComPtr<ID3D12Resource> mBlurMap0 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mBlurMap1 = nullptr;
};