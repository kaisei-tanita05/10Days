#pragma once
#include <KamataEngine.h>

class Player {
public:
	~Player();

	void Initialize(uint32_t textureHandle, const KamataEngine::Vector2& position);
	void Update(bool isActive);
	void Draw();

	// ゲッター・セッター
	const KamataEngine::Vector2& GetPosition() const { return position_; }
	void SetPosition(const KamataEngine::Vector2& pos) { position_ = pos; }
	void SetScrollX(float scrollX) { scrollX_ = scrollX; }
	void SetMoveLimitY(float minY, float maxY) {
		minY_ = minY;
		maxY_ = maxY;
	}

	// 当たり判定 (AABB)
	bool IsCollision(const KamataEngine::Vector2& objectPos, float objectWidth, float objectHeight);

	// 押し戻し処理（壁との衝突判定）
	void ResolveCollision(const KamataEngine::Vector2& objectPos, float objectWidth, float objectHeight);

	// 壁の制限X座標を設定するメソッド
	void SetWallLimitX(float wallX) { wallLimitX_ = wallX; }

	// 壁制限を解除するメソッド
	void ClearWallLimitX() { wallLimitX_ = 99999.0f; }

private:
	KamataEngine::Sprite* sprite_ = nullptr;
	uint32_t textureHandle_ = 0;

	// 座標関連
	KamataEngine::Vector2 position_ = {0.0f, 0.0f};
	KamataEngine::Vector2 previousPosition_ = {0.0f, 0.0f};
	float scrollX_ = 0.0f;

	// プレイヤーのサイズ（定数）
	static inline const float kWidth = 64.0f;
	static inline const float kHeight = 64.0f;

	// 移動・ジャンプパラメータ
	float speed_ = 5.0f;
	float velocityY_ = 0.0f;
	float gravity_ = 0.8f;
	float jumpInitialVelocity_ = -12.0f;
	bool isGrounded_ = false;

	// Y移動制限
	float minY_ = 0.0f;
	float maxY_ = 0.0f;


	float velocityY_ = 0.0f;             // Y方向の速度
	float gravity_ = 0.8f;               // 重力
	float jumpInitialVelocity_ = -15.0f; // ジャンプ力（負の値で上方向）
	bool isGrounded_ = false;            // 着地フラグ

	// ジャンプSEのハンドル
	uint32_t SEJumpHandle_ = 0;
};
	float wallLimitX_ = 99999.0f;
};