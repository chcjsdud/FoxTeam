#pragma once

struct Status
{
	Status()
		: Stat_AttackPower_(0.f)
		, Stat_Health_(0.f)
		, Stat_HealthRecovery_(0.f)
		, Stat_Stamina_(0.f)
		, Stat_StaminaRecovery_(0.f)
		, Stat_Defense_(0.f)
		, Stat_AttackSpeed_(0.f)
		, Stat_CriticalHit_(0.f)
		, Stat_MoveSpeed_(0.f)
		, Stat_Eyesight_(0.f)
		, Stat_IsMult_(false)
	{}

	Status(bool Mult_true)
		: Stat_AttackPower_(1.f)
		, Stat_Health_(1.f)
		, Stat_HealthRecovery_(1.f)
		, Stat_Stamina_(1.f)
		, Stat_StaminaRecovery_(1.f)
		, Stat_Defense_(1.f)
		, Stat_AttackSpeed_(1.f)
		, Stat_CriticalHit_(1.f)
		, Stat_MoveSpeed_(1.f)
		, Stat_Eyesight_(1.f)
		, Stat_IsMult_(true)
	{}

	Status(const Status& _value)
	{
		Stat_AttackPower_ = _value.Stat_AttackPower_;
		Stat_Health_ = _value.Stat_Health_;
		Stat_HealthRecovery_ = _value.Stat_HealthRecovery_;
		Stat_Stamina_ = _value.Stat_Stamina_;
		Stat_StaminaRecovery_ = _value.Stat_StaminaRecovery_;
		Stat_Defense_ = _value.Stat_Defense_;
		Stat_AttackSpeed_ = _value.Stat_AttackSpeed_;
		Stat_CriticalHit_ = _value.Stat_CriticalHit_;
		Stat_MoveSpeed_ = _value.Stat_MoveSpeed_;
		Stat_Eyesight_ = _value.Stat_Eyesight_;
		Stat_IsMult_ = _value.Stat_IsMult_;
	}

	bool Stat_IsMult_;

	float	Stat_AttackPower_;				// 공격력
	int		Stat_Health_;					// 체력
	float	Stat_HealthRecovery_;			// 체력재생
	int		Stat_Stamina_;					// 스태미너
	float	Stat_StaminaRecovery_;			// 스태미너 재생
	float	Stat_Defense_;					// 방어력
	float	Stat_AttackSpeed_;				// 공격속도
	float	Stat_CriticalHit_;				// 치명타
	float	Stat_MoveSpeed_;				// 이동속도
	float	Stat_Eyesight_;					// 시야

	void Reset()
	{
		Stat_AttackPower_ = 0.f;
		Stat_Health_ = 0;
		Stat_HealthRecovery_ = 0.f;
		Stat_Stamina_ = 0;
		Stat_StaminaRecovery_ = 0.f;
		Stat_Defense_ = 0.f;
		Stat_AttackSpeed_ = 0.f;
		Stat_CriticalHit_ = 0.f;
		Stat_MoveSpeed_ = 0.f;
		Stat_Eyesight_ = 0.f;

		Stat_IsMult_ = false;
	}

	void Reset_mult()
	{
		Stat_AttackPower_ = 1.f;
		Stat_Health_ = 1;
		Stat_HealthRecovery_ = 1.f;
		Stat_Stamina_ = 1;
		Stat_StaminaRecovery_ = 1.f;
		Stat_Defense_ = 1.f;
		Stat_AttackSpeed_ = 1.f;
		Stat_CriticalHit_ = 1.f;
		Stat_MoveSpeed_ = 1.f;
		Stat_Eyesight_ = 1.f;

		Stat_IsMult_ = true;
	}

#pragma region 오퍼레이터

	const Status& operator+(const Status _value) const
	{
		Status ret = *this;
		ret.Stat_AttackPower_ += _value.Stat_AttackPower_;
		ret.Stat_Health_ += _value.Stat_Health_;
		ret.Stat_HealthRecovery_ += _value.Stat_HealthRecovery_;
		ret.Stat_Stamina_ += _value.Stat_Stamina_;
		ret.Stat_StaminaRecovery_ += _value.Stat_StaminaRecovery_;
		ret.Stat_Defense_ += _value.Stat_Defense_;
		ret.Stat_AttackSpeed_ += _value.Stat_AttackSpeed_;
		ret.Stat_CriticalHit_ += _value.Stat_CriticalHit_;
		ret.Stat_MoveSpeed_ += _value.Stat_MoveSpeed_;
		ret.Stat_Eyesight_ += _value.Stat_Eyesight_;
		return Status(ret);
	}

	const Status operator-(const Status _value) const
	{
		Status ret = *this;
		ret.Stat_AttackPower_ -= _value.Stat_AttackPower_;
		ret.Stat_Health_ -= _value.Stat_Health_;
		ret.Stat_HealthRecovery_ -= _value.Stat_HealthRecovery_;
		ret.Stat_Stamina_ -= _value.Stat_Stamina_;
		ret.Stat_StaminaRecovery_ -= _value.Stat_StaminaRecovery_;
		ret.Stat_Defense_ -= _value.Stat_Defense_;
		ret.Stat_AttackSpeed_ -= _value.Stat_AttackSpeed_;
		ret.Stat_CriticalHit_ -= _value.Stat_CriticalHit_;
		ret.Stat_MoveSpeed_ -= _value.Stat_MoveSpeed_;
		ret.Stat_Eyesight_ -= _value.Stat_Eyesight_;

		return Status(ret);
	}

	const Status operator*(const Status _value) const
	{
		Status ret = *this;
		ret.Stat_AttackPower_ *= _value.Stat_AttackPower_;
		ret.Stat_Health_ *= _value.Stat_Health_;
		ret.Stat_HealthRecovery_ *= _value.Stat_HealthRecovery_;
		ret.Stat_Stamina_ *= _value.Stat_Stamina_;
		ret.Stat_StaminaRecovery_ *= _value.Stat_StaminaRecovery_;
		ret.Stat_Defense_ *= _value.Stat_Defense_;
		ret.Stat_AttackSpeed_ *= _value.Stat_AttackSpeed_;
		ret.Stat_CriticalHit_ *= _value.Stat_CriticalHit_;
		ret.Stat_MoveSpeed_ *= _value.Stat_MoveSpeed_;
		ret.Stat_Eyesight_ *= _value.Stat_Eyesight_;

		return Status(ret);
	}

	const Status& operator*(const float _value) const
	{
		Status ret = *this;
		ret.Stat_AttackPower_ *= _value;
		ret.Stat_Health_ *= _value;
		ret.Stat_HealthRecovery_ *= _value;
		ret.Stat_Stamina_ *= _value;
		ret.Stat_StaminaRecovery_ *= _value;
		ret.Stat_Defense_ *= _value;
		ret.Stat_AttackSpeed_ *= _value;
		ret.Stat_CriticalHit_ *= _value;
		ret.Stat_MoveSpeed_ *= _value;
		ret.Stat_Eyesight_ *= _value;

		return Status(ret);
	}

	Status& operator+=(const Status _value)
	{
		Stat_AttackPower_ += _value.Stat_AttackPower_;
		Stat_Health_ += _value.Stat_Health_;
		Stat_HealthRecovery_ += _value.Stat_HealthRecovery_;
		Stat_Stamina_ += _value.Stat_Stamina_;
		Stat_StaminaRecovery_ += _value.Stat_StaminaRecovery_;
		Stat_Defense_ += _value.Stat_Defense_;
		Stat_AttackSpeed_ += _value.Stat_AttackSpeed_;
		Stat_CriticalHit_ += _value.Stat_CriticalHit_;
		Stat_MoveSpeed_ += _value.Stat_MoveSpeed_;
		Stat_Eyesight_ += _value.Stat_Eyesight_;

		return *this;
	}

	Status& operator-=(const Status _value)
	{
		Stat_AttackPower_ -= _value.Stat_AttackPower_;
		Stat_Health_ -= _value.Stat_Health_;
		Stat_HealthRecovery_ -= _value.Stat_HealthRecovery_;
		Stat_Stamina_ -= _value.Stat_Stamina_;
		Stat_StaminaRecovery_ -= _value.Stat_StaminaRecovery_;
		Stat_Defense_ -= _value.Stat_Defense_;
		Stat_AttackSpeed_ -= _value.Stat_AttackSpeed_;
		Stat_CriticalHit_ -= _value.Stat_CriticalHit_;
		Stat_MoveSpeed_ -= _value.Stat_MoveSpeed_;
		Stat_Eyesight_ -= _value.Stat_Eyesight_;

		return *this;
	}

	Status& operator*=(const Status _value)
	{
		Stat_AttackPower_ *= _value.Stat_AttackPower_;
		Stat_Health_ *= _value.Stat_Health_;
		Stat_HealthRecovery_ *= _value.Stat_HealthRecovery_;
		Stat_Stamina_ *= _value.Stat_Stamina_;
		Stat_StaminaRecovery_ *= _value.Stat_StaminaRecovery_;
		Stat_Defense_ *= _value.Stat_Defense_;
		Stat_AttackSpeed_ *= _value.Stat_AttackSpeed_;
		Stat_CriticalHit_ *= _value.Stat_CriticalHit_;
		Stat_MoveSpeed_ *= _value.Stat_MoveSpeed_;
		Stat_Eyesight_ *= _value.Stat_Eyesight_;

		return *this;
	}
	Status& operator*=(const float _value)
	{
		Stat_AttackPower_ *= _value;
		Stat_Health_ *= _value;
		Stat_HealthRecovery_ *= _value;
		Stat_Stamina_ *= _value;
		Stat_StaminaRecovery_ *= _value;
		Stat_Defense_ *= _value;
		Stat_AttackSpeed_ *= _value;
		Stat_CriticalHit_ *= _value;
		Stat_MoveSpeed_ *= _value;
		Stat_Eyesight_ *= _value;

		return *this;
	}

	Status& operator=(Status _value)
	{
		Stat_AttackPower_ = _value.Stat_AttackPower_;
		Stat_Health_ = _value.Stat_Health_;
		Stat_HealthRecovery_ = _value.Stat_HealthRecovery_;
		Stat_Stamina_ = _value.Stat_Stamina_;
		Stat_StaminaRecovery_ = _value.Stat_StaminaRecovery_;
		Stat_Defense_ = _value.Stat_Defense_;
		Stat_AttackSpeed_ = _value.Stat_AttackSpeed_;
		Stat_CriticalHit_ = _value.Stat_CriticalHit_;
		Stat_MoveSpeed_ = _value.Stat_MoveSpeed_;
		Stat_Eyesight_ = _value.Stat_Eyesight_;
		Stat_IsMult_ = _value.Stat_IsMult_;

		return *this;
	}


	bool operator==(const Status _value)const
	{
		return (
			Stat_AttackPower_ == _value.Stat_AttackPower_ &&
			Stat_Health_ == _value.Stat_Health_ &&
			Stat_HealthRecovery_ == _value.Stat_HealthRecovery_ &&
			Stat_Stamina_ == _value.Stat_Stamina_ &&
			Stat_StaminaRecovery_ == _value.Stat_StaminaRecovery_ &&
			Stat_Defense_ == _value.Stat_Defense_ &&
			Stat_AttackSpeed_ == _value.Stat_AttackSpeed_ &&
			Stat_CriticalHit_ == _value.Stat_CriticalHit_ &&
			Stat_MoveSpeed_ == _value.Stat_MoveSpeed_ &&
			Stat_Eyesight_ == _value.Stat_Eyesight_ &&
			Stat_IsMult_ == _value.Stat_IsMult_
			);
	}

	bool operator!=(const Status _value)const
	{
		return (
			Stat_AttackPower_ != _value.Stat_AttackPower_ ||
			Stat_Health_ != _value.Stat_Health_ ||
			Stat_HealthRecovery_ != _value.Stat_HealthRecovery_ ||
			Stat_Stamina_ != _value.Stat_Stamina_ ||
			Stat_StaminaRecovery_ != _value.Stat_StaminaRecovery_ ||
			Stat_Defense_ != _value.Stat_Defense_ ||
			Stat_AttackSpeed_ != _value.Stat_AttackSpeed_ ||
			Stat_CriticalHit_ != _value.Stat_CriticalHit_ ||
			Stat_MoveSpeed_ != _value.Stat_MoveSpeed_ ||
			Stat_Eyesight_ != _value.Stat_Eyesight_ ||
			Stat_IsMult_ != _value.Stat_IsMult_
			);
	}

#pragma endregion 
};