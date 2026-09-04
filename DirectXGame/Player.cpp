#include "Player.h"

using namespace KamataEngine;

void Player::Initialize(uint32_t textureHandle, const KamataEngine::Vector2& initialPos) {
	textureHandle_ = textureHandle;
	position_ = initialPos;

	// スプライト生成
	sprite_ = Sprite::Create(textureHandle_, position_);
}

void Player::Update(bool isActive) {

		// 移動前の座標を保存
		previousPosition_ = position_;

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
		velocityY_ = 0.0f;  // 着地したら速度をリセット
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
		sprite_->SetPosition({position_.x - scrollX_, position_.y});
	}
}


bool Player::IsCollision(const Vector2& obstaclePosition, float obstacleWidth, float obstacleHeight) const {

	// プレイヤーの矩形
	float playerLeft = position_.x;
	float playerRight = position_.x + kWidth;
	float playerTop = position_.y;
	float playerBottom = position_.y + kHeight;

	// 障害物の矩形
	float obstacleLeft = obstaclePosition.x;
	float obstacleRight = obstaclePosition.x + obstacleWidth;
	float obstacleTop = obstaclePosition.y;
	float obstacleBottom = obstaclePosition.y + obstacleHeight;

	// AABB判定
	if (playerRight <= obstacleLeft) {
		return false;
	}

	if (playerLeft >= obstacleRight) {
		return false;
	}

	if (playerBottom <= obstacleTop) {
		return false;
	}

	if (playerTop >= obstacleBottom) {
		return false;
	}

	return true;
}

void Player::ResolveCollision(const Vector2& obstaclePosition, float obstacleWidth, float obstacleHeight) {

	float playerLeft = position_.x;
	float playerRight = position_.x + kWidth;
	float playerTop = position_.y;
	float playerBottom = position_.y + kHeight;

	float obstacleLeft = obstaclePosition.x;
	float obstacleRight = obstaclePosition.x + obstacleWidth;
	float obstacleTop = obstaclePosition.y;
	float obstacleBottom = obstaclePosition.y + obstacleHeight;

	//========================================
	// 上から乗った
	//========================================

	if (previousPosition_.y + kHeight <= obstacleTop && playerBottom > obstacleTop) {

		position_.y = obstacleTop - kHeight;

		velocityY_ = 0.0f;
		isGrounded_ = true;
	}
	//========================================
	// 下からぶつかった
	//========================================
	else if (previousPosition_.y >= obstacleBottom && playerTop < obstacleBottom) {

		position_.y = obstacleBottom;

		velocityY_ = 0.0f;
	}
	//========================================
	// 横からぶつかった
	//========================================
	else {

		// 左から右へ進んでぶつかった
		if (previousPosition_.x + kWidth <= obstacleLeft && playerRight > obstacleLeft) {

			position_.x = obstacleLeft - kWidth;
		}
		// 右から左へ進んでぶつかった
		else if (previousPosition_.x >= obstacleRight && playerLeft < obstacleRight) {

			position_.x = obstacleRight;
		}
	}

	// スプライト位置を更新
	if (sprite_) {
		sprite_->SetPosition({position_.x - scrollX_, position_.y});
	}
}

void Player::Draw() {
	if (sprite_) {
		sprite_->Draw();
	}
}