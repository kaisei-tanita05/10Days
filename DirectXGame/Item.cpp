#include "Item.h"

using namespace KamataEngine;

Item::~Item() { delete sprite_; }

void Item::Initialize(const Vector2& position) {
	worldPosition_ = position;
	// アイテムのテクスチャを読み込む
	itemTextureHandle_ = TextureManager::Load("item/item1.png");
	// スプライトの生成
	sprite_ = Sprite::Create(itemTextureHandle_, worldPosition_);
	isOnStand_ = false;
}

void Item::Update() {
	// 落としている場合
	if (isDropped_) {

		// 横方向に進む
		worldPosition_.x += velocityX_;
		// 重力
		velocityY_ += gravity_;
		if (velocityY_ > maxFallSpeed_) {
			velocityY_ = maxFallSpeed_;
		}
		worldPosition_.y += velocityY_;
		// 地面に着いたら止める
		if (worldPosition_.y >= groundY_) {
			worldPosition_.y = groundY_;
			velocityY_ = 0.0f;
			// 落下終了
			isDropped_ = false;
		}
	}
	if (sprite_) {
		// ステージ座標 → 画面座標
		float screenX = worldPosition_.x - scrollX_;
		sprite_->SetPosition({screenX, worldPosition_.y});
	}
}

void Item::PickUp() {

	isHeld_ = true;

	// 持ったので落下状態を解除
	isDropped_ = false;
	isOnStand_ = false;
	velocityY_ = 0.0f;
}

void Item::Drop() {
	isHeld_ = false;
	isDropped_ = true; 
	isOnStand_ = false;
	// 少し上に投げる
	velocityY_ = -8.0f;

	// 前に飛ばす
	velocityX_ = 5.0f;
}


void Item::Stop() {

	isDropped_ = false;
	isHeld_ = false;
	isOnStand_ = true;
	velocityX_ = 0.0f;
	velocityY_ = 0.0f;
}

void Item::Draw() {
	if (sprite_) {
		sprite_->Draw();
	}
}