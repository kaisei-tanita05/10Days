#pragma once

#include <KamataEngine.h>

class TitleScene {
public:
	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

private:
	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t textureHandle_ = 0;
};