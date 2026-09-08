#pragma once
#include <KamataEngine.h>

class Quiz {
public:
	~Quiz();
	void Initialize(
	    uint32_t quizTex, uint32_t quizTex2, const uint32_t btnTexs[3], const KamataEngine::Vector2& boardWorldPos, const KamataEngine::Vector2& boardWorldPos2,
	    const KamataEngine::Vector2 btnWorldPositions[3]);
	void Update(bool isActivePlayer2, const KamataEngine::Vector2& player2Pos);

	void Draw();

	void SetScrollX(float scrollX) { scrollX_ = scrollX; }

	// ギミックをクリアしたか
	bool IsCleared() const { return isCleared_; }

	// 誤答ボタンが押されたか
	bool IsIncorrectTriggered() const { return isIncorrectTriggered_; }

private:
	// クイズボード（ワールド配置・スクロール移動）
	uint32_t quizTextureHandle_ = 0;
	KamataEngine::Sprite* quizSprite_ = nullptr;
	KamataEngine::Vector2 boardPosition_ = {0.0f, 0.0f};
	uint32_t quizTextureHandle2_ = 0;
	KamataEngine::Sprite* quizSprite2_ = nullptr;
	KamataEngine::Vector2 boardPosition2_ = {0.0f, 0.0f};

	// ボタン3つ（ワールド配置・スクロール移動）
	uint32_t buttonTextureHandles_[3] = {};
	KamataEngine::Sprite* buttonSprites_[3] = {};
	KamataEngine::Vector2 buttonPositions_[3] = {};

	int correctAnswerIndex_ = 1; // 0:ボタン1, 1:ボタン2, 2:ボタン3 が正解
	bool isCleared_ = false;     // クリアフラグ
	bool isIncorrectTriggered_ = false; // 誤答が押された瞬間フラグ
	float scrollX_ = 0.0f;       // スクロール量

	// 当たり判定の幅・高さ設定
	const float buttonWidth_ = 64.0f;
	const float buttonHeight_ = 128.0f;

	// 成功した音
	uint32_t SEConnectHandle_ = 0;

	// 不正解の音
	uint32_t SEUnconnectHandle_ = 0;
};
