#pragma once
#include <KamataEngine.h>

class Botom {
public:
	//==================================================
	// 基本処理
	//==================================================

	// デストラクタ
	~Botom();

	// 初期化
	void Initialize(const KamataEngine::Vector2& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	// スクロール
	void SetScrollX(float scrollX);

	//==================================================
	// 下側：ボタン
	//==================================================

	// ボタンの位置を取得
	const KamataEngine::Vector2& GetPosition() const;

	// プレイヤーとボタンが当たっているか
	bool IsCollision(const KamataEngine::Vector2& playerPosition, float playerWidth, float playerHeight) const;

	// ボタンを押す
	void Push();

	// ボタンが押されているか
	bool IsPressed() const { return isPressed_; }

	//==================================================
	// 上側：デブブ用の扉
	//==================================================

	// 上の扉とプレイヤーが当たっているか
	bool IsDoorCollision(const KamataEngine::Vector2& playerPosition, float playerWidth, float playerHeight) const;

	// 上の扉を破壊する
	void BreakDoor();

	// 上の扉の位置を取得
	const KamataEngine::Vector2& GetDoorPosition() const;

	// 上の扉が壊れているか
	bool IsDoorBroken() const { return isDoorBroken_; }

	//==================================================
	// 下側：ガリリ用の扉
	//==================================================

	// 下の扉とプレイヤーが当たっているか
	bool IsBottomDoorCollision(const KamataEngine::Vector2& playerPosition, float playerWidth, float playerHeight) const;

	// 下の扉が開いているか
	bool IsBottomDoorOpened() const { return isBottomDoorOpened_; }

	// 下の扉の位置を取得
	const KamataEngine::Vector2& GetBottomDoorPosition() const;

private:
	//==================================================
	// ボタン
	//==================================================

	// ボタンのスプライト
	KamataEngine::Sprite* buttonSprite_ = nullptr;

	// ボタンのテクスチャ
	uint32_t buttonTextureHandle_ = 0;

	// ボタンのワールド座標
	KamataEngine::Vector2 worldPosition_{};

	// ボタンが押されたか
	bool isPressed_ = false;

	// ボタンの大きさ
	const float kButtonWidth = 20.0f;
	const float kButtonHeight = 20.0f;

	//==================================================
	// 上側：デブブ用の扉
	//==================================================

	// 上の通常扉
	KamataEngine::Sprite* doorSprite_ = nullptr;

	// 上の壊れた扉
	KamataEngine::Sprite* brokenDoorSprite_ = nullptr;

	// 上の扉のテクスチャ
	uint32_t doorTextureHandle_ = 0;

	// 壊れた扉のテクスチャ
	uint32_t brokenDoorTextureHandle_ = 0;

	// 上の扉のワールド座標
	KamataEngine::Vector2 doorPosition_{};

	// 上の扉が壊れたか
	bool isDoorBroken_ = false;

	//==================================================
	// 下側：ガリリ用の扉
	//==================================================

	// 下の扉
	KamataEngine::Sprite* bottomDoorSprite_ = nullptr;

	// 下の扉のワールド座標
	KamataEngine::Vector2 bottomDoorPosition_{};

	// 下の扉が開いたか
	bool isBottomDoorOpened_ = false;

	//==================================================
	// 扉の大きさ
	//==================================================

	// 元画像は40×60
	// 2倍の大きさで表示する
	const float kDoorWidth = 160.0f;
	const float kDoorHeight = 240.0f;

	//==================================================
	// スクロール
	//==================================================

	float scrollX_ = 0.0f;
};