#pragma once

#include "geometria.h"
#include <stdio.h>
#include "nlohmann/json.hpp"
#include "./theactualshit.h"
#include "../MiniAudio/MiniAudioManager/MiniAudioManager.h"

using json = nlohmann::json;

struct GameMain
{
	static void Init()
	{
		Empty* script = new Empty();
		script->AddScript<BadApple>();
		MiniAudioManager::StartEngine();
		script->AddScript<MiniAudioSource>();
		script->GetScript<MiniAudioSource>()->LoadAudioFile("Game/bad_apple.ogg");
		script->GetScript<MiniAudioSource>()->PlayOnStart();
		SceneManager::MainScene().MainDrawCall()->sort = DrawCall::Sorting::Static;
	}
};