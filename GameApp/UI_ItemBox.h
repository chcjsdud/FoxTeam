#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// ���� :
class GameEngineUIRenderer;
class Player;
class UI_ItemBox : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_ItemBox();
	~UI_ItemBox();

	// delete Function
	UI_ItemBox(const UI_ItemBox& _Other) = delete;
	UI_ItemBox(UI_ItemBox&& _Other) noexcept = delete;
	UI_ItemBox& operator=(const UI_ItemBox& _Other) = delete;
	UI_ItemBox& operator=(UI_ItemBox&& _Other) noexcept = delete;

	bool UIOn;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}
	
protected:
	GameEngineUIRenderer* ItemBoxBackGround_Renderer;
	
	//�ڽ��� ��ü�� �����̳Ŀ� ���� �޶����� ��
	//������ � ��� �� �̹����� ��ü�� ���� ����
	GameEngineUIRenderer* BoxtypeFont_Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//����
private:
	Player* Player_;

#pragma region ������ ��¿� ������ ����

//	GameEngineUIRenderer* DefenseValue_Renderer;
//	GameEngineUIRenderer* AttackSpeedValue_Renderer;
//	GameEngineUIRenderer* MoveSpeedValue_Renderer;
//	GameEngineUIRenderer* CriticalValue_Renderer;

#pragma endregion 

	/*
	�������
	������ �ڽ��� ��ġ�� �׻� ����, ���� ������ �ڽ��� �׻� �����̴�


	�޾ƾ� �ϴ� �͵�

	*������ �ڽ��� ������ �ƴϳ��� ����

	*������ �ڽ��� ��� �ִ� �����۵��� ����
	-> ������ ���� �������� ���� �������� ����ִ��� üũ

	*������ �ڽ��� ����->�⺻����/��ü/����
	->��ü�� ��� ���ĭ 8ĭ+�κ��丮 8ĭ�ؼ� 16ĭ¥�� ������ ������ ��� �ڽ��� ����
	-> �켱���� ����, �ϴ��� 8ĭ¥�� �ʵ忡�� �����ϴ� 8ĭ ¥�� �ڽ����� ����



	�� ���

	* UI_ItemBox�� ItemRemove() �Լ�
	�ڽ����� �����۷������� Off�Ѵ�


	* UI_Inventory�� ItemAttach() �Լ�
	�κ��丮�� ������ �������� On�Ѵ�


	

	* Box_to_Inventory() �Լ�?
	�ڽ����� �������� �̵������� 
	



	
	*/

};

