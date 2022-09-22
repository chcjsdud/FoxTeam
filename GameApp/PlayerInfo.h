#pragma once

/// <summary>
// ���� �������̽��� ���� ������ ��� ���� ĳ���� �����Դϴ�.
// �� ��Ŷ���� ������ Execute() ���� �ش� PlayerInfo �� ������ ä�� �ִ� ������ ����� �����Դϴ�.
/// </summary>
struct PlayerInfo
{
	int playerNumber_;
	int character_;
	int startPoint_;
	int isReady_;
	std::string playerNickname_;

	// Lumia Level Only
	float4 curPos_;
	float4 curDir_;
	std::string curAnimation_;
};