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
}

void Quiz::Update(bool isActivePlayer2, const Vector2& player2Pos) {
	(void)player2Pos;

	Input* input = Input::GetInstance();
	int selectedIndex = -1;

	// クリアしていない時だけ入力チェックを行う
	if (!isCleared_ && isActivePlayer2) {

		if (input->TriggerKey(DIK_1)) {
			selectedIndex = 0;
		}
		if (input->TriggerKey(DIK_2) || input->TriggerKey(DIK_RETURN)) {
			selectedIndex = 1;
		}
		if (input->TriggerKey(DIK_3)) {
			selectedIndex = 2;
		}

		if (selectedIndex == correctAnswerIndex_) {
			isCleared_ = true;
		}
	}

	// ★クリア状態に関わらず、毎フレーム座標を更新してスクロール追従させる
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
