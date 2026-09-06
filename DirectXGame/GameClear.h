#pragma once

#include <KamataEngine.h>

class GameClear {
public:
	~GameClear();

	void Initialize();

	void Update();

	void Draw();

private:
	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t textureHandle_ = 0;
};