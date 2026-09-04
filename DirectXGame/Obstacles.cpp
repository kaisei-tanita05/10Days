#include "Obstacles.h"

using namespace KamataEngine;

Obstacles::~Obstacles() { delete sprite_; }

void Obstacles::Initialize(const Vector2& position) {
	worldPosition_ = position;

	// 障害物テクスチャの読み込み
	obstacleTextureHandle_ = TextureManager::Load("Obstacles/bottle.png");
	// スプライトの生成
	sprite_ = Sprite::Create(obstacleTextureHandle_, worldPosition_);
}

void Obstacles::Update() {
	if (sprite_ && !isDestroyed_) {

		// ステージ座標 - スクロール量
		float screenX = worldPosition_.x - scrollX_;

		sprite_->SetPosition({screenX, worldPosition_.y});
	}
}

void Obstacles::SetScrollX(float scrollX) { scrollX_ = scrollX; }

const Vector2& Obstacles::GetPosition() const { return worldPosition_; }

void Obstacles::Hit() {

	// すでに壊れていたら何もしない
	if (isDestroyed_) {
		return;
	}

	// Enterを押した回数を増やす
	hitCount_++;

	// 3回押したら壊す
	if (hitCount_ >= kDestroyCount) {
		isDestroyed_ = true;
	}
}

void Obstacles::Draw() {
	// 壊れていたら描画しない
	if (isDestroyed_) {
		return;
	}

	if (sprite_) {
		sprite_->Draw();
	}
}