#pragma once
#include "Fade.h"
#include <KamataEngine.h>

class GameOver {
public:
	// 02_12 27枚目 シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	~GameOver();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

private:
	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t textureHandle_ = 0;

	Fade* fade_ = nullptr;

	// 02_13 27枚目 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	bool finished_ = false;

	uint32_t BGMHandle_ = 0;

	uint32_t voiceHandle_ = 0;

	uint32_t SEHandle_ = 0;
};