#include "stand.h"

using namespace KamataEngine;

Stand::~Stand() {

	delete sprite_;
	sprite_ = nullptr;
}

void Stand::Initialize(const Vector2& position) {

	// ワールド座標
	position_ = position;

	// Standの画像
	textureHandle_ = TextureManager::Load("stand/stand.png");

	// スプライト生成
	sprite_ = Sprite::Create(textureHandle_, position_);

	// 左上を基準にする
	sprite_->SetAnchorPoint({0.0f, 0.0f});
}

bool Stand::IsCollision(const Vector2& itemPosition, float itemWidth, float itemHeight) const {

	//========================================
	// Stand
	//========================================

	float standLeft = position_.x;
	float standRight = position_.x + width_;

	float standTop = position_.y;
	float standBottom = position_.y + height_;

	//========================================
	// Item
	//========================================

	float itemLeft = itemPosition.x;
	float itemRight = itemPosition.x + itemWidth;

	float itemTop = itemPosition.y;
	float itemBottom = itemPosition.y + itemHeight;

	//========================================
	// AABB判定
	//========================================

	if (itemRight <= standLeft) {
		return false;
	}

	if (itemLeft >= standRight) {
		return false;
	}

	if (itemBottom <= standTop) {
		return false;
	}

	if (itemTop >= standBottom) {
		return false;
	}

	return true;
}

Vector2 Stand::GetItemPosition() const {

	// ItemをStandの上に置く位置
	return {position_.x, position_.y - 64.0f};
}

void Stand::Draw() {

	if (sprite_) {

		float screenX = position_.x - scrollX_;

		sprite_->SetPosition({screenX, position_.y});

		sprite_->Draw();
	}
}