#include "Player.h"

using namespace KamataEngine;

void Player::Initialize(uint32_t textureHandle, const KamataEngine::Vector2& initialPos) {
	textureHandle_ = textureHandle;
	position_ = initialPos;
	
	//スプライト生成
	sprite_ = Sprite::Create(textureHandle_, position_);
}

void Player::Update(bool isActive) {
	if (isActive) {

		Input* input = Input::GetInstance();

		// キー入力に応じてプレイヤーの位置を更新
		
		if (input->PushKey(DIK_A)) {
			position_.x -= speed_;
		}
		if (input->PushKey(DIK_D)) {
			position_.x += speed_;
		}

		// ジャンプ処理
		if (isGrounded_ && input->TriggerKey(DIK_W)) {
			velocityY_ = jumpInitialVelocity_;
			isGrounded_ = false;
		}
	}

	// 重力の適用
	velocityY_ += gravity_;
	position_.y += velocityY_;

	// y座標の移動範囲の制限
	if (position_.y >= maxY_) {
		position_.y = maxY_;
		velocityY_ = 0.0f; // 着地したら速度をリセット
		isGrounded_ = true; // 着地フラグを立てる
	} else {
		isGrounded_ = false; // 空中にいる場合は着地フラグを下ろす
	}
	if (position_.y < minY_) {
		position_.y = minY_;
		velocityY_ = 0.0f; // 着地したら速度をリセット
	}

	// スプライトの位置を更新
	if (sprite_) {
		sprite_->SetPosition(position_);
	}
}

void Player::Draw() {
	if (sprite_) {
		sprite_->Draw();
	}
}

void Player::SetPosition(const KamataEngine::Vector2& pos) {
	position_ = pos;
	if (sprite_) {
		sprite_->SetPosition(position_);
	}
}