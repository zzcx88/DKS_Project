#include "Shaders.hlsl"

cbuffer cbGameObjectInfo : register(b2)
{
    matrix		gmtxWorld : packoffset(c0);

};

//float4x4 m_WVPNew; // ���� ���� ������ ���� ������ �� ����
//float4x4 m_WVPOld; // ������ ���� ������ ���� ������ �� ����
//float4x4 m_ROnly; // ���� ���͸� ���� �������� ��ȯ�ϴ� ���. ���� �����̹Ƿ� ȸ�� ���� ��.

Texture2D tex0            : register(t4); // ����� ������ �̹���. Pass1���� ����Ѵ�.

//sampler tex0 : register (s0);
sampler tex1 : register (s0); // �ӵ� ��. Pass1���� ����Ѵ�.

// �ӵ� ���� �ۼ�
struct VS_CreateVMap_INPUT
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_CreateVMap_OUTPUT
{
    float4 position : SV_POSITION;
    float4 uv : TEXCOORD; // �ӵ� ����
};

VS_CreateVMap_OUTPUT VS_CreateVMap(VS_CreateVMap_INPUT input)
{
    VS_CreateVMap_OUTPUT Out;

    // ���� ���ʹ� ȸ�� ���� ���� ����� ���� ��
    float3 N = mul(input.normal.xyz, m_ROnly);

    // ������ ������ ��ǥ
    float4 NewPos = mul(input.position, m_WVPNew);

    // ������ ������ ��ǥ
    float4 OldPos = mul(input.position, m_WVPOld);

    // ������ �̵� ���� ����
    float3 Dir = NewPos.xyz - OldPos.xyz;

    // ������ �̵� ���� ���Ϳ� ���� ������ ������ ���
    float a = dot(normalize(Dir), normalize(N));

    // ������ �������� ���� ����
    if (a < 0.0f)
        Out.position = OldPos;
    else
        Out.position = NewPos;

    // �ӵ� ������ ���
    //-1.0f���� 1.0f�� ������ �ؽ�ó ��ǥ���� 0.0f ~ 1.0f�� �ؼ� ��ġ�� �����ϱ� ���� �Ÿ��� ������
    Out.uv.xy = (NewPos.xy / NewPos.w - OldPos.xy / OldPos.w) * 0.5f;

    // ���������� �ؼ��� ������ ���̵Ǳ� ������ Y ������ �ݴ� ���������ϴ�
    Out.uv.y* = -1.0f;

    // ����� Z ���� ����ϱ����� �Ű� ����
    Out.uv.z = Out.position.z;
    Out.uv.w = Out.position.w;

    return Out;
}

float4 PS_CreateVMap(VS_CreateVMap_OUTPUT In) : SV_TARGET
{
   float4 Out;

    // �ӵ� ����
    Out.xy = In.uv.xy;

    // �̻��
    Out.z = 1.0f;

    // Z ���� ���
    Out.w = In.uv.z / In.uv.w;

    return Out;
}

// �ӵ� ���� ���� �� ó��
struct VS_Final_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;

};

struct VS_Final_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_Final_OUTPUT VS_Final(VS_Final_INPUT input)
{
    VS_Final_OUTPUT Out;

    Out.position = input.position;
    Out.uv = input.uv;

    return Out;
}

float4 PS_Final(VS_Final_OUTPUT In) : SV_TARGET
{

    // ����� �ε巯��. ��ġ�� ũ���ϸ� �Ų����Եȴ�.
    int Blur = 10;

    // �ӵ� �ʿ��� �ӵ� ���� �� Z ���� ����
    float4 Velocity = tex2D(tex1, In.uv);

    Velocity.xy / = (float)Blur;

    int cnt = 1;
    float4 BColor;

    // ����� ������ �̹����� ����. a ���п� Z ���� ����ȴ�.
    float4 Out = tex2D(tex0, In.uv);

    for (int i = cnt; i < Blur; i++)
    {
        // �ӵ� ������ ���� �ؼ� ��ġ�� ���� ����� ������ �̹����� ���� ������ ���� �� �ֽ��ϴ�.
        BColor = tex2D(tex0, In.uv + Velocity.xy * (float)i);

        // �ӵ� ���� Z ���� �ӵ� ���� ���⿡�ִ� �ؼ� ��ġ�� ���ø� �� ����� ������ �̹����� Z ���� ���Ѵ�. (���� 1)
        if (Velocity.a < BColor.a + 0.04f)
        {
           cnt++;
           Out + = BColor;
        }
    }

     Out / = (float)cnt;

     return Out;
}
//
//technique TShader
//{
//    // �ӵ� �� �ۼ�
//    pass P0
//    {
//        VertexShader = compile vs_1_1 VS_CreateVMap();
//        PixelShader = compile ps_2_0 PS_CreateVMap();
//    }
//
//    // �� ó��
//    pass P1
//    {
//        VertexShader = compile vs_1_1 VS_Final();
//        PixelShader = compile ps_3_0 PS_Final();
//    }
//}