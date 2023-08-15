// Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "Timer.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "Renderer.h"
#include "InputManager.h"
#include "HealthComponent.h"
#include <iostream>
#include "LevelManager.h"
#include "SDLSoundSystem.h"
#include "ButtonComponent.h"
#include "TexturedGameObjectPrefab.h"
#include "SoundServiceLocator.h"
#include "SDLSoundSystem.h"

void load()
{
	auto scene = dae::SceneManager::GetInstance().CreateScene("startMenu", true);

	const float windowWidth = dae::SceneManager::GetInstance().GetScenesWidth();

	std::cout << "controls: \n";
	std::cout << "\tmoving: wasd or left joystick (controller)\n";
	std::cout << "\tuse pepper: E or A (controller)\n";
	std::cout << "\tskip level: F1\n";

	dae::ServiceLocator::registerSoundSystem<dae::SDLSoundSystem>();

	//logo
	auto logo{ std::make_unique<TexturedGameObjectPrefab>(scene, "burgerTimeLogo.png")->GetGameObject() };
	auto logoSize{ logo->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
	logo->SetLocalPosition(windowWidth / 2.f - logoSize.x / 2.f, 0.f);
	scene->Add(std::move(logo));

	//create font to use for buttons
	auto font{ dae::ResourceManager::GetInstance().LoadFont("PressStart2P-vaV7.ttf", 24) };

	//Single player button
	auto button1{ std::make_unique<dae::GameObject>(scene) };
	button1->AddComponent(std::make_unique<dae::TextComponent>(button1.get(), "Single player", font));
	auto onclickButton1 =
		[&]()
	{
		LevelManager::GetInstance().LoadLevel(1, GameMode::singlePlayer);
	};
	const auto button1Size{ button1->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
	glm::vec2 button1Pos{ windowWidth / 2.f - button1Size.x / 2.f, 150.f };
	button1->AddComponent(std::make_unique<dae::ButtonComponent>(button1.get(), button1Pos, static_cast<float>(button1Size.x), static_cast<float>(button1Size.y), onclickButton1));
	button1->SetLocalPosition(button1Pos.x, button1Pos.y);
	scene->Add(std::move(button1), true);

	//Co-op button
	auto button2{ std::make_unique<dae::GameObject>(scene) };
	button2->AddComponent(std::make_unique<dae::TextComponent>(button2.get(), "Co-op", font));
	auto onclickButton2 =
		[&]()
	{
		LevelManager::GetInstance().LoadLevel(1, GameMode::coOp);
	};
	const auto button2Size{ button2->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
	const glm::vec2 button2Pos{ windowWidth / 2.f - button2Size.x / 2.f, 250.f };
	button2->AddComponent(std::make_unique<dae::ButtonComponent>(button2.get(), button2Pos, static_cast<float>(button2Size.x), static_cast<float>(button2Size.y), onclickButton2));
	button2->SetLocalPosition(button2Pos.x, button2Pos.y);
	scene->Add(std::move(button2), true);

	//Versus button
	auto button3{ std::make_unique<dae::GameObject>(scene) };
	button3->AddComponent(std::make_unique<dae::TextComponent>(button3.get(), "Versus", font));
	auto onclickButton3 =
		[&]()
	{
		LevelManager::GetInstance().LoadLevel(1, GameMode::versus);
	};
	const auto button3Size{ button3->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
	glm::vec2 button3Pos{ windowWidth / 2.f - button3Size.x / 2.f, 350.f };
	button3->AddComponent(std::make_unique<dae::ButtonComponent>(button3.get(), button3Pos, static_cast<float>(button3Size.x), static_cast<float>(button3Size.y), onclickButton3));
	button3->SetLocalPosition(button3Pos.x, button3Pos.y);
	scene->Add(std::move(button3), true);

	//create pauseScreen
	auto pauseScreen{ dae::SceneManager::GetInstance().CreateScene("pauseScreen", false) };

	//logo
	logo = std::make_unique<TexturedGameObjectPrefab>(pauseScreen, "burgerTimeLogo.png")->GetGameObject();
	logoSize = logo->GetComponent<dae::TextureComponent>()->GetSize();
	logo->SetLocalPosition(windowWidth / 2.f - logoSize.x / 2.f, 0.f);
	pauseScreen->Add(std::move(logo));

	//"paused" text
	auto scoreObject{ std::make_unique<dae::GameObject>(pauseScreen) };
	scoreObject->AddComponent(std::make_unique<dae::TextComponent>(scoreObject.get(), "paused", font));
	auto textureSize{ scoreObject->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
	scoreObject->SetLocalPosition(windowWidth / 2.f - textureSize.x / 2.f, 100.f);
	pauseScreen->Add(std::move(scoreObject));

	//continue button
	auto continueButton{ std::make_unique<dae::GameObject>(scene) };
	continueButton->AddComponent(std::make_unique<dae::TextComponent>(continueButton.get(), "continue", font));
	auto onclickcontinueButton =
		[&]()
	{
		dae::SceneManager::GetInstance().SetActiveSceneByName("levelScene");
	};
	const auto continueButtonSize{ continueButton->GetComponent<dae::RenderComponent>()->GetTextureComponent()->GetSize() };
	glm::vec2 continueButtonPos{ windowWidth / 2.f - continueButtonSize.x / 2.f, 350.f };
	continueButton->AddComponent(std::make_unique<dae::ButtonComponent>(continueButton.get(), continueButtonPos, static_cast<float>(continueButtonSize.x), static_cast<float>(continueButtonSize.y), onclickcontinueButton));
	continueButton->SetLocalPosition(continueButtonPos.x, button3Pos.y);
	pauseScreen->Add(std::move(continueButton), true);
}

int main(int, char* []) {

	dae::Engine engine("../Data/");
	engine.Run(load);
	return 0;
}