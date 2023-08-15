#include "SoundServiceLocator.h"

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_spSoundSystem{ new dae::DefaultSoundSystem() };