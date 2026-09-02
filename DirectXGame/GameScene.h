#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;

class GameScene {

public:
	//==================================================
	// シーンの種類
	//==================================================
	enum class Scene {
		Title,     // タイトル画面
		Operation, // 操作説明画面
		GamePlay,  // ゲーム本編
		GameClear, // ゲームクリア画面
		GameOver   // ゲームオーバー画面
	};

public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// デストラクタ
	~GameScene();

	// 現在のシーンを取得
	Scene GetScene() const { return scene_; }

private:
	//==================================================
	// タイトル
	//==================================================
	void UpdateTitle();
	void DrawTitle();

	//==================================================
	// 操作説明
	//==================================================
	void UpdateOperation();
	void DrawOperation();

	//==================================================
	// ゲーム本編
	//==================================================
	void UpdateGamePlay();
	void DrawGamePlay();

	//==================================================
	// ゲームクリア
	//==================================================
	void UpdateGameClear();
	void DrawGameClear();

	//==================================================
	// ゲームオーバー
	//==================================================
	void UpdateGameOver();
	void DrawGameOver();

private:
	// 現在のシーン
	Scene scene_ = Scene::Title;

	// 次に移動するシーン
	Scene nextScene_ = Scene::Title;

	//==================================================
	// テクスチャハンドル
	//==================================================

	// タイトル画面のテクスチャ
	uint32_t titleTextureHandle_ = 0;

	// 操作説明画面のテクスチャ
	uint32_t operationTextureHandle_ = 0;

	// ゲーム本編のテクスチャ
	uint32_t gamePlayTextureHandle_ = 0;

	// ゲームクリア画面のテクスチャ
	uint32_t gameClearTextureHandle_ = 0;

	// ゲームオーバー画面のテクスチャ
	uint32_t gameOverTextureHandle_ = 0;

	//==================================================
	// スプライト
	//==================================================

	// 各シーンの画像を表示するスプライト
	Sprite* titleSprite_ = nullptr;
	Sprite* operationSprite_ = nullptr;
	Sprite* gamePlaySprite_ = nullptr;
	Sprite* gameClearSprite_ = nullptr;
	Sprite* gameOverSprite_ = nullptr;
};