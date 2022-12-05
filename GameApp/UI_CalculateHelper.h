#pragma once
#include "GameEngine/GameEngineUIRenderer.h"

using namespace std;

//

class UI_CalculateHelper : public GameEngineActor
{

public:
	UI_CalculateHelper();
	~UI_CalculateHelper();

public:
	//인자로 들어오는건 3D좌표의 LocalPosition
	float4 Cal3Dto2D(float4 _Position)
	{
		//3D공간의 좌표를 2D화면좌표로 변환
		

		DirectX::XMVECTOR Position = _Position.DirectVector;
		D3D11_VIEWPORT ViewPort_;

		UINT Number = 1;

		GameEngineDevice::GetContext()->RSGetViewports(&Number, &ViewPort_);

		float4x4 Pro = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
		float4x4 View = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
		DirectX::XMMATRIX ViewProjectionMatrix = View.DirectMatrix * Pro.DirectMatrix;
		Position = DirectX::XMVector3TransformCoord(Position, ViewProjectionMatrix);
		//로컬좌표에 월드 뷰 투영 행렬을 곱해서 -1과 1사이의 클립 스페이스로 이동


		float4 ReturnPosition = { 0.0f, 0.0f };
		ReturnPosition.DirectVector = Position;


		ReturnPosition.DxXmfloat3 = DirectX::XMFLOAT3(ViewPort_.Width * (ReturnPosition.x + 1.0f) / 2.0f + ViewPort_.TopLeftX, ViewPort_.Height * (2.0f - (ReturnPosition.y + 1.0f)) / 2.0f + ViewPort_.TopLeftY, 0.0f);
		//클립스페이스의 -1~1 사이에 있는 좌표를 비율로 바꾼다
		//그 이후 뷰포트를 곱해서 2D좌표를 구함


		ReturnPosition.x -= (ViewPort_.Width / 2);
		ReturnPosition.y = (ReturnPosition.y - (ViewPort_.Height / 2)) * -1.0f;
		// 프로젝트의 0,0의 위치는 화면 한가운데로 되어 있으므로 그것에 맞춰서 좌표를 변환
		// 좌표의 x값 = 뷰포트의 절반만큼 뺀 값
		// 좌표의 y값 = 뷰포트의 절반만큼 뺀 값 * -1
		// 윈도우 좌표계는 화면 밑으로 내려갈수록 y값이 커지므로 -1을 곱해서 반전시켜줘야한다

		return ReturnPosition;
	}

	float4 CalRound(float4 _origin)
	{
		float4 returnfloat4 = { round(_origin.x), round(_origin.y), _origin.z, _origin.w };
		return returnfloat4;
	}

private:
	UI_CalculateHelper(const UI_CalculateHelper& _Other) = delete;
	UI_CalculateHelper(UI_CalculateHelper&& _Other) noexcept = delete;
	UI_CalculateHelper& operator=(const UI_CalculateHelper& _Other) = delete;
	UI_CalculateHelper& operator=(UI_CalculateHelper&& _Other) noexcept = delete;
};

