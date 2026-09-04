#pragma once
#include <KamataEngine.h>

class Player {
public:
	void Initialize(uint32_t textureHandle, const KamataEngine::Vector2& initialPos);
	void Update(bool isActive); // プレイヤーの更新処理
	void Draw();

	// プレイヤーの位置
	const KamataEngine::Vector2& GetPosition() const { return position_; }

	// プレイヤーの位置を設定する関数
	void SetPosition(const KamataEngine::Vector2& pos);

	// 移動範囲（Y軸）を設定する関数
	void SetMoveLimitY(float minY, float maxY) {
		minY_ = minY;
		maxY_ = maxY;
	}

private:
	uint32_t textureHandle_=0; // テクスチャハンドル
	KamataEngine::Sprite* sprite_ = nullptr; // スプライト
	KamataEngine::Vector2 position_={0.0f, 0.0f}; // プレイヤーの位置
	float speed_ = 5.0f;             // プレイヤーの移動速度


	// 移動範囲（Y軸）
	float minY_ = 0.0f; // 最小Y座標
	float maxY_ = 600.0f; // 最大Y座標

	float velocityY_ = 0.0f;             // Y方向の速度
	float gravity_ = 0.8f;               // 重力
	float jumpInitialVelocity_ = -15.0f; // ジャンプ力（負の値で上方向）
	bool isGrounded_ = false;            // 着地フラグ
	

};
