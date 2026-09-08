#include "Player.h"

using namespace KamataEngine;

Player::~Player() {
	delete sprite_;
	sprite_ = nullptr;
}

void Player::Initialize(uint32_t textureHandle, const KamataEngine::Vector2& initialPos) {
	textureHandle_ = textureHandle;
	position_ = initialPos;

	// スプライト生成
	sprite_ = Sprite::Create(textureHandle_, position_);

	SEJumpHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/jumpSE.mp3");
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

	// 【壁の制限処理】
	// プレイヤーのサイズ（kWidth = 64.0f）を考慮して壁の「左端」でピタッとストップさせる
	if (position_.x + kWidth > wallLimitX_) {
		position_.x = wallLimitX_ - kWidth;
	}
}

bool Player::IsCollision(const Vector2& objectPos, float objectWidth, float objectHeight) {
	// プレイヤーの左右上下の端（ワールド座標）
	float pLeft = position_.x;
	float pRight = position_.x + kWidth;
	float pTop = position_.y;
	float pBottom = position_.y + kHeight;

	// 対象オブジェクト（壁など）の左右上下の端（ワールド座標）
	float oLeft = objectPos.x;
	float oRight = objectPos.x + objectWidth;
	float oTop = objectPos.y;
	float oBottom = objectPos.y + objectHeight;

	// 重なっているかチェック
	if (pRight > oLeft && pLeft < oRight && pBottom > oTop && pTop < oBottom) {
		return true; // 衝突している
	}

	return false; // 衝突していない
}

// =========================================================
// 押し戻し処理（壁めり込み解消）
// =========================================================
void Player::ResolveCollision(const Vector2& objectPos, float objectWidth, float objectHeight) {
	// プレイヤーとオブジェクトの中心座標を計算
	float pCenterX = position_.x + kWidth / 2.0f;
	float pCenterY = position_.y + kHeight / 2.0f;

	float oCenterX = objectPos.x + objectWidth / 2.0f;
	float oCenterY = objectPos.y + objectHeight / 2.0f;

	// 中心同士の差分
	float diffX = pCenterX - oCenterX;
	float diffY = pCenterY - oCenterY;

	// 重なっている幅（めり込み量）を計算
	float overlapX = (kWidth / 2.0f + objectWidth / 2.0f) - std::abs(diffX);
	float overlapY = (kHeight / 2.0f + objectHeight / 2.0f) - std::abs(diffY);

	// めり込み量が少ない軸の方向に押し戻す
	if (overlapX < overlapY) {
		// 横方向からの衝突
		if (diffX > 0.0f) {
			// オブジェクトの右側に押し戻す
			position_.x += overlapX;
		} else {
			// オブジェクトの左側に押し戻す
			position_.x -= overlapX;
		}
	} else {
		// 縦方向からの衝突
		if (diffY > 0.0f) {
			// オブジェクトの下側に押し戻す
			position_.y += overlapY;
		} else {
			// オブジェクトの上側に押し戻す
			position_.y -= overlapY;
		}
	}
}

void Player::Draw() {
	if (sprite_) {
		// 描画時にスクリーン座標へ変換
		sprite_->SetPosition({position_.x - scrollX_, position_.y});
		sprite_->Draw();
	}
}
