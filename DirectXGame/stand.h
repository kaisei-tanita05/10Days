#pragma once

#include <KamataEngine.h>

class Stand {

public:
	~Stand();

	// 初期化
	void Initialize(const KamataEngine::Vector2& position);

	// 描画
	void Draw();

	// Itemとの当たり判定
	bool IsCollision(const KamataEngine::Vector2& itemPosition, float itemWidth, float itemHeight) const;

	// Itemを置く位置を取得
	KamataEngine::Vector2 GetItemPosition() const;

	// スクロール
	void SetScrollX(float scrollX) { scrollX_ = scrollX; }

private:
	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t textureHandle_ = 0;

	// ワールド座標
	KamataEngine::Vector2 position_{};

	// スクロール量
	float scrollX_ = 0.0f;

	// Standのサイズ
	float width_ = 128.0f;
	float height_ = 128.0f;
};