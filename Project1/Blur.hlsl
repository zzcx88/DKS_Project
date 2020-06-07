//=============================================================================
// �ִ� 5�ȼ������� �帮�� ���������� �и� ���� ���콺 �帮�⸦ �����Ѵ�.
//=============================================================================

cbuffer cbSettings : register(b0)
{ 
	// ��Ʈ ����鿡 �����Ǵ� ��� ���ۿ� �迭�� �� ���� �����Ƿ�,
	// ����ó�� �迭 ���ҵ��� ���� �����μ� �����ؾ� �Ѵ�.
	int gBlurRadius;

	// �ִ� 11���� �帮�� ����ġ�� �����Ѵ�.
	float w0;
	float w1;
	float w2;
	float w3;
	float w4;
	float w5;
	float w6;
	float w7;
	float w8;
	float w9;
	float w10;
	float w11;
	float w12;
	float w13;
	float w14;
	float w15;
	float w16;
	float w17;
	float w18;
	float w19;
	float w20;
};

static const int gMaxBlurRadius = 10;


Texture2D gInput            : register(t4);
RWTexture2D<float4> gOutput : register(u0);

#define N 256
#define CacheSize (N + 2*gMaxBlurRadius)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void HorzBlurCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
	// ����ġ���� �迭�� �ִ´�(�������� ������ �� �ֵ���)
	float weights[21] = { w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12, w13, w14, w15, w16, w17, w18, w19, w20 };

	//
	// �뿪���� ���̱� ���� ���� ������ ����Ҹ� ä���.
	// �帮�� ������ ������ �ȼ� N���� �帮���� N + 2*BlurRadius����
	// �ȼ��� �о���Ѵ�.
	//
	// �� ������ �׷��� N���� �����带 �����Ѵ�. ������ �ȼ� 2*BlurRadius ����
	// ����, 2*BlurRadius ���� �����尡 �ȼ��� �ϳ� �� �����ϰ� �Ѵ�.
	if(groupThreadID.x < gBlurRadius)
	{
		// �̹��� �����ڸ� �ٱ��� ǥ���� �̹��� �����ڸ� ǥ������ �����Ѵ�.
		int x = max(dispatchThreadID.x - gBlurRadius, 0);
		gCache[groupThreadID.x] = gInput[int2(x, dispatchThreadID.y)];
	}
	if(groupThreadID.x >= N-gBlurRadius)
	{
		// �̹��� �����ڸ� �ٱ��� ǥ���� �̹��� �����ڸ� ǥ������ �����Ѵ�.
		int x = min(dispatchThreadID.x + gBlurRadius, gInput.Length.x-1);
		gCache[groupThreadID.x+2*gBlurRadius] = gInput[int2(x, dispatchThreadID.y)];
	}

	// �̹��� �����ڸ� �ٱ��� ǥ���� �̹��� �����ڸ� ǥ������ �����Ѵ�.
	gCache[groupThreadID.x+gBlurRadius] = gInput[min(dispatchThreadID.xy, gInput.Length.xy-1)];

	// ��� �����尡 �� ������ ������ ������ ��ٸ���.
	GroupMemoryBarrierWithGroupSync();
	
	//
	// ���� �� �ȼ��� �� ó���Ѵ�.
	//

	float4 blurColor = float4(0, 0, 0, 0);
	
	for(int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.x + gBlurRadius + i;
		
		blurColor += weights[i+gBlurRadius]*gCache[k];
	}
	
	gOutput[dispatchThreadID.xy] = blurColor;
}

[numthreads(1, N, 1)]
void VertBlurCS(int3 groupThreadID : SV_GroupThreadID,
				int3 dispatchThreadID : SV_DispatchThreadID)
{
	// ����ġ���� �迭�� �ִ´�(�������� ���� �����ϵ���)
	float weights[11] = { w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10 };

	//
	// �뿪���� ���̱� ���� ���� ������ ����Ҹ� ä���.
	// �帮�� ������ ������ �ȼ� N���� �帮���� N + 2*BlurRadius ����
	// �ȼ��� �о���Ѵ�.
	//
	
	// �� ������ �׷��� N���� �����带 �����Ѵ�.������ �ȼ� 2 * BlurRadius ����
	// ����, 2*BlurRadius ���� �����尡 �ȼ��� �ϳ� �� �����ϰ� �Ѵ�.
	if(groupThreadID.y < gBlurRadius)
	{
		// �̹��� �����ڸ� �ٱ��� ǥ���� �̹��� �����ڸ� ǥ������ �����Ѵ�.
		int y = max(dispatchThreadID.y - gBlurRadius, 0);
		gCache[groupThreadID.y] = gInput[int2(dispatchThreadID.x, y)];
	}
	if(groupThreadID.y >= N-gBlurRadius)
	{
		// �̹��� �����ڸ� �ٱ��� ǥ���� �̹��� �����ڸ� ǥ������ �����Ѵ�.
		int y = min(dispatchThreadID.y + gBlurRadius, gInput.Length.y-1);
		gCache[groupThreadID.y+2*gBlurRadius] = gInput[int2(dispatchThreadID.x, y)];
	}
	
	// �̹��� �����ڸ� �ٱ��� ǥ���� �̹��� �����ڸ� ǥ������ �����Ѵ�.
	gCache[groupThreadID.y+gBlurRadius] = gInput[min(dispatchThreadID.xy, gInput.Length.xy-1)];


	// ��� �����尡 �� ������ ������ ������ ��ٸ���.
	GroupMemoryBarrierWithGroupSync();
	
	// 
	// ���� �ȼ��� �� ó���Ѵ�.
	//
	
	float4 blurColor = float4(0, 0, 0, 0);
	
	for(int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.y + gBlurRadius + i;
		
		blurColor += weights[i+ gBlurRadius]*gCache[k];
	}
	
	gOutput[dispatchThreadID.xy] = blurColor;
}