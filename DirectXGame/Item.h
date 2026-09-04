#pragma once
#include <KamataEngine.h>

class Item {

public:
	~Item();
	/// <summary> 
	/// 初期化
	/// </summary>
	void Initialize(const KamataEngine::Vector2& position);

	/// <summary> 
	/// 更新 
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// アイテムを持つ
	/// </summary>
	void PickUp();
	/// <summary>
	/// アイテムを落とす
	/// </summary>
	void Drop();

	/// <summary>
	/// アイテムを持っているか
	/// </summary>
	bool IsHeld() const { return isHeld_; }

	/// <summary>
	/// アイテムが落ちているか
	/// </summary>
	bool IsDropped() const { return isDropped_; }

	/// <summary> 
	/// スクロール量を設定
	/// </summary>
	void SetScrollX(float scrollX) { scrollX_ = scrollX; }
	/// <summary>
	/// アイテムの位置を設定
	/// /// </summary>
	void SetPosition(const KamataEngine::Vector2& position) { worldPosition_ = position; }
	/// <summary>
	/// アイテムの位置を取得
	/// </summary>

	const KamataEngine::Vector2& GetPosition()

	    const {
		return worldPosition_;
	}
	

	/// <summary>
	/// 落下する地面のY座標を設定
	/// </summary>
	void SetGroundY(float groundY) { groundY_ = groundY; }

private:
	KamataEngine::Sprite* sprite_ = nullptr;
	uint32_t itemTextureHandle_ = 0;
	// ステージ上の座標
	KamataEngine::Vector2 worldPosition_{};
	// 背景のスクロール量
	float scrollX_ = 0.0f;
	// 持っているか
	bool isHeld_ = false;
	// 落ちているか
	bool isDropped_ = false;
	// 落下速度
	float velocityY_ = 0.0f;
	// 重力
	const float gravity_ = 0.5f;
	/// 落下速度の上限
	const float maxFallSpeed_ = 10.0f;
	// 地面
	float groundY_ = 550.0f;
};
