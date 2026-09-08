#pragma once
#include "Fade.h"
#include <KamataEngine.h>

class GameClear {
public:

	// 02_12 27枚目 シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	~GameClear();

	void Initialize();

	void Update();

	void Draw();

	// ゲームクリアしたか
	bool IsFinished() const { return isFinished_; }

private:
	KamataEngine::Sprite* sprite_ = nullptr;
	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kMain;

	// ゲームクリアフラグ
	bool isFinished_ = false;

	uint32_t textureHandle_ = 0;

	// サウンドハンドル
	uint32_t BGMHandle_ = 0;
};