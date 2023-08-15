#pragma once
#include "Component.h"
#include "EventListener.h"
#include "TextComponent.h"
#include <glm/vec2.hpp>
#include "BurgerPartEvents.h"
#include "EnemyDiedEvent.h"

class PointsComponent final : public dae::Component, public dae::EventListener<BurgerPartDropped1LevelEvent>, public dae::EventListener<BurgerPartReachedPlateEvent>, public dae::EventListener<BurgerPartDroppedWithEnemiesOnEvent>, public dae::EventListener<EnemyDiedEvent>
{
public:
	PointsComponent(dae::GameObject* owner, const glm::vec2& middlePos = {0.f, 0.f});
	~PointsComponent();
	PointsComponent(const PointsComponent& other) = delete;
	PointsComponent(PointsComponent&& other) = delete;
	PointsComponent& operator=(const PointsComponent& other) = delete;
	PointsComponent& operator=(PointsComponent&& other) = delete;

	void Update() override {}
	void Render() const override {}
	void RenderImGui() override {}
	void OnNotify(const BurgerPartDropped1LevelEvent* pEvent) override;
	void OnNotify(const BurgerPartReachedPlateEvent* pEvent) override;
	void OnNotify(const BurgerPartDroppedWithEnemiesOnEvent* pEvent) override;
	void OnNotify(const EnemyDiedEvent* pEvent) override;
	int GetPoints() const { return m_AmountOfPoints; }
	void AddPoints(int amount);

private:
	int m_AmountOfPoints{};
	dae::TextComponent* m_pTextComponent{};
	const glm::vec2 m_MiddlePos{};

	void Recenter();
};