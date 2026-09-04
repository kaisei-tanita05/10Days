#pragma once
#include <KamataEngine.h>


class Obstacles {
public:

	~Obstacles();

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
	/// スクロール量を設定
	/// </summary>
	void SetScrollX(float scrollX);

	/// <summary>
	/// ステージ上の座標を取得
	/// </summary>
	const KamataEngine::Vector2& GetPosition() const;

	/// <summary>
	/// Enterキーで攻撃
	/// </summary>
	void Hit();

	/// <summary>
	/// 壊れているか
	/// </summary>
	bool IsDestroyed() const { return isDestroyed_; }

private:
	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t obstacleTextureHandle_;
	

	// ステージ上の座標
	KamataEngine::Vector2 worldPosition_{};

	// 背景のスクロール量
	float scrollX_ = 0.0f;

	// Enterを押した回数
	int hitCount_ = 0;

	// 壊れるまでに必要な回数
	static constexpr int kDestroyCount = 3;

	// 壊れたか
	bool isDestroyed_ = false;
};
