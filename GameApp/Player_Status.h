#pragma once


struct PlayerStatus
{
	PlayerStatus()
		: Stat_Hp_(0)
		, Stat_MaxHp_(0)
		, Stat_Stamina_(0.f)
		, Stat_MaxStamina_(0.f)
		, Stat_AttackPower_(0)
		, Stat_Speed_(0.f)
		, Stat_RunSpeed_(0.f)
		, Stat_Attack_Stamina_(0.f)
		, Stat_Stamina_RecoverRate_(0.f)
		, Stat_IsMult_(false)
	{}

	PlayerStatus(const PlayerStatus& _value)
	{
		Stat_Hp_ = _value.Stat_Hp_;
		Stat_MaxHp_ = _value.Stat_MaxHp_;
		Stat_Stamina_ = _value.Stat_Stamina_;
		Stat_MaxStamina_ = _value.Stat_MaxStamina_;
		Stat_AttackPower_ = _value.Stat_AttackPower_;
		Stat_Speed_ = _value.Stat_Speed_;
		Stat_RunSpeed_ = _value.Stat_RunSpeed_;
		Stat_Attack_Stamina_ = _value.Stat_Attack_Stamina_;
		Stat_Stamina_RecoverRate_ = _value.Stat_Stamina_RecoverRate_;
		Stat_IsMult_ = _value.Stat_IsMult_;
	}

	bool Stat_IsMult_;

	int Stat_Hp_;
	int Stat_MaxHp_;
	float Stat_Stamina_;
	float Stat_MaxStamina_;

	// 공격 빠와
	int Stat_AttackPower_;
	// 이동 속도
	float Stat_Speed_;
	float Stat_RunSpeed_;
	// 공격 스테미나 소모량
	float Stat_Attack_Stamina_;
	// Idle 스테미나 회복량
	float Stat_Stamina_RecoverRate_;

	void Reset()
	{
		Stat_Hp_ = 0;
		Stat_MaxHp_ = 0;
		Stat_Stamina_ = 0.f;
		Stat_MaxStamina_ = 0.f;
		Stat_AttackPower_ = 0;
		Stat_Speed_ = 0.f;
		Stat_RunSpeed_ = 0.f;
		Stat_Attack_Stamina_ = 0.f;
		Stat_Stamina_RecoverRate_ = 0.f;
		Stat_IsMult_ = false;
	}

#pragma region 오퍼레이터

	const PlayerStatus& operator+(const PlayerStatus _value) const
	{
		PlayerStatus ret = *this;
		ret.Stat_Hp_ += _value.Stat_Hp_;
		ret.Stat_MaxHp_ += _value.Stat_MaxHp_;
		ret.Stat_Stamina_ += _value.Stat_Stamina_;
		ret.Stat_MaxStamina_ += _value.Stat_MaxStamina_;
		ret.Stat_AttackPower_ += _value.Stat_AttackPower_;
		ret.Stat_Speed_ += _value.Stat_Speed_;
		ret.Stat_RunSpeed_ += _value.Stat_RunSpeed_;
		ret.Stat_Attack_Stamina_ += _value.Stat_Attack_Stamina_;
		ret.Stat_Stamina_RecoverRate_ += _value.Stat_Stamina_RecoverRate_;

		return PlayerStatus(ret);
	}

	const PlayerStatus operator-(const PlayerStatus _value) const
	{
		PlayerStatus ret = *this;
		ret.Stat_Hp_ -= _value.Stat_Hp_;
		ret.Stat_MaxHp_ -= _value.Stat_MaxHp_;
		ret.Stat_Stamina_ -= _value.Stat_Stamina_;
		ret.Stat_MaxStamina_ -= _value.Stat_MaxStamina_;
		ret.Stat_AttackPower_ -= _value.Stat_AttackPower_;
		ret.Stat_Speed_ -= _value.Stat_Speed_;
		ret.Stat_RunSpeed_ -= _value.Stat_RunSpeed_;
		ret.Stat_Attack_Stamina_ -= _value.Stat_Attack_Stamina_;
		ret.Stat_Stamina_RecoverRate_ -= _value.Stat_Stamina_RecoverRate_;

		return PlayerStatus(ret);
	}

	const PlayerStatus operator*(const PlayerStatus _value) const
	{
		PlayerStatus ret = *this;
		ret.Stat_Hp_ *= _value.Stat_Hp_;
		ret.Stat_MaxHp_ *= _value.Stat_MaxHp_;
		ret.Stat_Stamina_ *= _value.Stat_Stamina_;
		ret.Stat_MaxStamina_ *= _value.Stat_MaxStamina_;
		ret.Stat_AttackPower_ *= _value.Stat_AttackPower_;
		ret.Stat_Speed_ *= _value.Stat_Speed_;
		ret.Stat_RunSpeed_ *= _value.Stat_RunSpeed_;
		ret.Stat_Attack_Stamina_ *= _value.Stat_Attack_Stamina_;
		ret.Stat_Stamina_RecoverRate_ *= _value.Stat_Stamina_RecoverRate_;

		return PlayerStatus(ret);
	}

	const PlayerStatus& operator*(const float _value) const
	{
		PlayerStatus ret = *this;
		ret.Stat_Hp_ *= _value;
		ret.Stat_MaxHp_ *= _value;
		ret.Stat_Stamina_ *= _value;
		ret.Stat_MaxStamina_ *= _value;
		ret.Stat_AttackPower_ *= _value;
		ret.Stat_Speed_ *= _value;
		ret.Stat_RunSpeed_ *= _value;
		ret.Stat_Attack_Stamina_ *= _value;
		ret.Stat_Stamina_RecoverRate_ *= _value;

		return PlayerStatus(ret);
	}

	PlayerStatus& operator+=(const PlayerStatus _value)
	{
		Stat_Hp_ += _value.Stat_Hp_;
		Stat_MaxHp_ += _value.Stat_MaxHp_;
		Stat_Stamina_ += _value.Stat_Stamina_;
		Stat_MaxStamina_ += _value.Stat_MaxStamina_;
		Stat_AttackPower_ += _value.Stat_AttackPower_;
		Stat_Speed_ += _value.Stat_Speed_;
		Stat_RunSpeed_ += _value.Stat_RunSpeed_;
		Stat_Attack_Stamina_ += _value.Stat_Attack_Stamina_;
		Stat_Stamina_RecoverRate_ += _value.Stat_Stamina_RecoverRate_;

		return *this;
	}

	PlayerStatus& operator-=(const PlayerStatus _value)
	{
		Stat_Hp_ -= _value.Stat_Hp_;
		Stat_MaxHp_ -= _value.Stat_MaxHp_;
		Stat_Stamina_ -= _value.Stat_Stamina_;
		Stat_MaxStamina_ -= _value.Stat_MaxStamina_;
		Stat_AttackPower_ -= _value.Stat_AttackPower_;
		Stat_Speed_ -= _value.Stat_Speed_;
		Stat_RunSpeed_ -= _value.Stat_RunSpeed_;
		Stat_Attack_Stamina_ -= _value.Stat_Attack_Stamina_;
		Stat_Stamina_RecoverRate_ -= _value.Stat_Stamina_RecoverRate_;

		return *this;
	}

	PlayerStatus& operator*=(const PlayerStatus _value)
	{
		Stat_Hp_ *= _value.Stat_Hp_;
		Stat_MaxHp_ *= _value.Stat_MaxHp_;
		Stat_Stamina_ *= _value.Stat_Stamina_;
		Stat_MaxStamina_ *= _value.Stat_MaxStamina_;
		Stat_AttackPower_ *= _value.Stat_AttackPower_;
		Stat_Speed_ *= _value.Stat_Speed_;
		Stat_RunSpeed_ *= _value.Stat_RunSpeed_;
		Stat_Attack_Stamina_ *= _value.Stat_Attack_Stamina_;
		Stat_Stamina_RecoverRate_ *= _value.Stat_Stamina_RecoverRate_;

		return *this;
	}
	PlayerStatus& operator*=(const float _value)
	{
		Stat_Hp_ *= _value;
		Stat_MaxHp_ *= _value;
		Stat_Stamina_ *= _value;
		Stat_MaxStamina_ *= _value;
		Stat_AttackPower_ *= _value;
		Stat_Speed_ *= _value;
		Stat_RunSpeed_ *= _value;
		Stat_Attack_Stamina_ *= _value;
		Stat_Stamina_RecoverRate_ *= _value;

		return *this;
	}

	PlayerStatus& operator=( PlayerStatus _value)
	{
		Stat_Hp_ = _value.Stat_Hp_;
		Stat_MaxHp_ = _value.Stat_MaxHp_;
		Stat_Stamina_ = _value.Stat_Stamina_;
		Stat_MaxStamina_ = _value.Stat_MaxStamina_;
		Stat_AttackPower_ = _value.Stat_AttackPower_;
		Stat_Speed_ = _value.Stat_Speed_;
		Stat_RunSpeed_ = _value.Stat_RunSpeed_;
		Stat_Attack_Stamina_ = _value.Stat_Attack_Stamina_;
		Stat_Stamina_RecoverRate_ = _value.Stat_Stamina_RecoverRate_;
		Stat_IsMult_ = _value.Stat_IsMult_;

		return *this;
	}


	bool operator==(const PlayerStatus _value)const
	{
		return (Stat_Hp_ == _value.Stat_Hp_ &&
			Stat_MaxHp_ == _value.Stat_MaxHp_ &&
			Stat_Stamina_ == _value.Stat_Stamina_ &&
			Stat_MaxStamina_ == _value.Stat_MaxStamina_ &&
			Stat_AttackPower_ == _value.Stat_AttackPower_ &&
			Stat_Speed_ == _value.Stat_Speed_ &&
			Stat_RunSpeed_ == _value.Stat_RunSpeed_ &&
			Stat_Attack_Stamina_ == _value.Stat_Attack_Stamina_ &&
			Stat_Stamina_RecoverRate_ == _value.Stat_Stamina_RecoverRate_ &&
			Stat_IsMult_ == _value.Stat_IsMult_);
	}

	bool operator!=(const PlayerStatus _value)const
	{
		return (Stat_Hp_ != _value.Stat_Hp_ ||
			Stat_MaxHp_ != _value.Stat_MaxHp_ ||
			Stat_Stamina_ != _value.Stat_Stamina_ ||
			Stat_MaxStamina_ != _value.Stat_MaxStamina_ ||
			Stat_AttackPower_ != _value.Stat_AttackPower_ ||
			Stat_Speed_ != _value.Stat_Speed_ ||
			Stat_RunSpeed_ != _value.Stat_RunSpeed_ ||
			Stat_Attack_Stamina_ != _value.Stat_Attack_Stamina_ ||
			Stat_Stamina_RecoverRate_ != _value.Stat_Stamina_RecoverRate_ ||
			Stat_IsMult_ != _value.Stat_IsMult_);
	}

#pragma endregion 
};