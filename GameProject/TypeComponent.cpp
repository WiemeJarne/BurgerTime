#include "TypeComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "TypeCommand.h"

TypeComponent::TypeComponent(dae::GameObject* pOwner, std::shared_ptr<dae::Font> font, dae::Scene* sceneToAddCommandsTo)
	: Component(pOwner)
{
	auto textComponent{ std::make_unique<dae::TextComponent>(pOwner, "_", font) };

	using namespace commands;

	auto commandA = std::make_unique<TypeCommand>('A', textComponent.get());
	auto commandB = std::make_unique<TypeCommand>('B', textComponent.get());
	auto commandC = std::make_unique<TypeCommand>('C', textComponent.get());
	auto commandD = std::make_unique<TypeCommand>('D', textComponent.get());
	auto commandE = std::make_unique<TypeCommand>('E', textComponent.get());
	auto commandF = std::make_unique<TypeCommand>('F', textComponent.get());
	auto commandG = std::make_unique<TypeCommand>('G', textComponent.get());
	auto commandH = std::make_unique<TypeCommand>('H', textComponent.get());
	auto commandI = std::make_unique<TypeCommand>('I', textComponent.get());
	auto commandJ = std::make_unique<TypeCommand>('J', textComponent.get());
	auto commandK = std::make_unique<TypeCommand>('K', textComponent.get());
	auto commandL = std::make_unique<TypeCommand>('L', textComponent.get());
	auto commandM = std::make_unique<TypeCommand>('M', textComponent.get());
	auto commandN = std::make_unique<TypeCommand>('N', textComponent.get());
	auto commandO = std::make_unique<TypeCommand>('O', textComponent.get());
	auto commandP = std::make_unique<TypeCommand>('P', textComponent.get());
	auto commandQ = std::make_unique<TypeCommand>('Q', textComponent.get());
	auto commandR = std::make_unique<TypeCommand>('R', textComponent.get());
	auto commandS = std::make_unique<TypeCommand>('S', textComponent.get());
	auto commandT = std::make_unique<TypeCommand>('T', textComponent.get());
	auto commandU = std::make_unique<TypeCommand>('U', textComponent.get());
	auto commandV = std::make_unique<TypeCommand>('V', textComponent.get());
	auto commandW = std::make_unique<TypeCommand>('W', textComponent.get());
	auto commandX = std::make_unique<TypeCommand>('X', textComponent.get());
	auto commandY = std::make_unique<TypeCommand>('Y', textComponent.get());
	auto commandZ = std::make_unique<TypeCommand>('Z', textComponent.get());
	auto commandDash = std::make_unique<TypeCommand>('-', textComponent.get());
	auto commandBackSpace = std::make_unique<TypeCommand>('\b', textComponent.get());
	auto commandSpace = std::make_unique<TypeCommand>(' ', textComponent.get());

	using KeyState = dae::KeyState;
	using KeyboardKey = dae::InputManager::KeyboardKey;
	
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandA), KeyState::down, KeyboardKey::A);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandB), KeyState::down, KeyboardKey::B);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandC), KeyState::down, KeyboardKey::C);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandD), KeyState::down, KeyboardKey::D);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandE), KeyState::down, KeyboardKey::E);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandF), KeyState::down, KeyboardKey::F);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandG), KeyState::down, KeyboardKey::G);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandH), KeyState::down, KeyboardKey::H);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandI), KeyState::down, KeyboardKey::I);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandJ), KeyState::down, KeyboardKey::J);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandK), KeyState::down, KeyboardKey::K);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandL), KeyState::down, KeyboardKey::L);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandM), KeyState::down, KeyboardKey::M);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandN), KeyState::down, KeyboardKey::N);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandO), KeyState::down, KeyboardKey::O);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandP), KeyState::down, KeyboardKey::P);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandQ), KeyState::down, KeyboardKey::Q);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandR), KeyState::down, KeyboardKey::R);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandS), KeyState::down, KeyboardKey::S);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandT), KeyState::down, KeyboardKey::T);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandU), KeyState::down, KeyboardKey::U);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandV), KeyState::down, KeyboardKey::V);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandW), KeyState::down, KeyboardKey::W);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandX), KeyState::down, KeyboardKey::X);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandY), KeyState::down, KeyboardKey::Y);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandZ), KeyState::down, KeyboardKey::Z);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandDash), KeyState::down, KeyboardKey::Minus);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandBackSpace), KeyState::down, KeyboardKey::BackSpace);
	sceneToAddCommandsTo->AddKeyboardCommand(std::move(commandSpace), KeyState::down, KeyboardKey::Space);

	pOwner->AddComponent(std::move(textComponent));
}