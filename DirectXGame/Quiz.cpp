#include "Quiz.h"

using namespace KamataEngine;

Quiz::~Quiz() {
	delete quizSprite_;
	quizSprite_ = nullptr;
	delete quizSprite2_;
	quizSprite2_ = nullptr;

	for (int i = 0; i < 3; i++) {
		delete buttonSprites_[i];
		buttonSprites_[i] = nullptr;
	}
}

void Quiz::Initialize(uint32_t quizTex, uint32_t quizTex2, const uint32_t btnTexs[3], const Vector2& boardWorldPos, const Vector2& boardWorldPos2, const Vector2 btnWorldPositions[3]) {

	quizTextureHandle_ = quizTex;
	boardPosition_ = boardWorldPos;
	quizTextureHandle2_ = quizTex2;
	boardPosition2_ = boardWorldPos2;

	// クイズボードの生成
	quizSprite_ = Sprite::Create(quizTextureHandle_, boardPosition_);
	quizSprite2_ = Sprite::Create(quizTextureHandle2_, boardPosition2_);

	// ボタン3つの初期化
	for (int i = 0; i < 3; i++) {
		buttonTextureHandles_[i] = btnTexs[i];
		buttonPositions_[i] = btnWorldPositions[i];
		buttonSprites_[i] = Sprite::Create(buttonTextureHandles_[i], buttonPositions_[i]);
	}

	correctAnswerIndex_ = 1; // ボタン2（インデックス1）を正解に設定
	isCleared_ = false;
	isIncorrectTriggered_ = false;

	// 成功した音
	SEConnectHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/connectSE.mp3");
	// 不正解の音
	SEUnconnectHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/unconnectSE.mp3");
}

void Quiz::Update(bool isActivePlayer2, const Vector2& player2Pos) {
	(void)player2Pos;

	// 毎フレーム誤答トリガーをリセット
	isIncorrectTriggered_ = false;

	Input* input = Input::GetInstance();
	//int selectedIndex = -1;

	// クリアしていない時だけ入力チェックを行う
	// クリアしていない ＆ Player2（下画面キャラクター）を操作中のみ処理
	if (!isCleared_ && isActivePlayer2) {

		// Enterキーが押された瞬間に判定
		if (input->TriggerKey(DIK_RETURN)) {
			// Player2のサイズ（おおよその当たり判定サイズ）
			const float playerWidth = 64.0f;
			const float playerHeight = 64.0f;

			for (int i = 0; i < 3; i++) {
				// Player2と各ボタンの矩形当たり判定 (AABB)
				bool isColliding =
				    (player2Pos.x < buttonPositions_[i].x + buttonWidth_ && player2Pos.x + playerWidth > buttonPositions_[i].x && player2Pos.y < buttonPositions_[i].y + buttonHeight_ &&
				     player2Pos.y + playerHeight > buttonPositions_[i].y);

				// 接触しているボタンの上で Enter を押した場合
				if (isColliding) {
					if (i == correctAnswerIndex_) {
						isCleared_ = true; // 正解
						Audio::GetInstance()->PlayWave(SEConnectHandle_, false, 1.0f); // 正解音再生
					} else {
						isIncorrectTriggered_ = true; // 不正解
						Audio::GetInstance()->PlayWave(SEUnconnectHandle_, false, 1.0f); // 不正解音再生
					}
					break; // 1つのボタンに判定したら抜ける
				}
			}
		}
	}

	// クリア状態に関わらず、毎フレーム座標を更新してスクロール追従させる
	if (quizSprite_) {
		quizSprite_->SetPosition({boardPosition_.x - scrollX_, boardPosition_.y});
	}
	if (quizSprite2_) {
		quizSprite2_->SetPosition({boardPosition2_.x - scrollX_, boardPosition2_.y});
	}

	for (int i = 0; i < 3; i++) {
		if (buttonSprites_[i]) {
			buttonSprites_[i]->SetPosition({buttonPositions_[i].x - scrollX_, buttonPositions_[i].y});
		}
	}
}

void Quiz::Draw() {
	if (quizSprite_) {
		quizSprite_->Draw();
	}
	if (quizSprite2_) {
		quizSprite2_->Draw();
	}

	for (int i = 0; i < 3; i++) {
		if (buttonSprites_[i]) {
			buttonSprites_[i]->Draw();
		}
	}
}
