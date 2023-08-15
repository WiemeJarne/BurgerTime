#pragma once
class EnemyState
{
public:
	EnemyState() = default;

	virtual ~EnemyState() = default;
	EnemyState(const EnemyState&) = delete;
	EnemyState(EnemyState&&) = delete;
	EnemyState& operator=(const EnemyState&) = delete;
	EnemyState& operator=(EnemyState&&) = delete;


	virtual EnemyState* Update() = 0;
};