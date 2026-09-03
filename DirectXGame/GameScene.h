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
	//==================================================
	// 基本処理
	//==================================================

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
	// Title
	//==================================================

	// タイトル画面の更新
	void UpdateTitle();

	// タイトル画面の描画
	void DrawTitle();

	//==================================================
	// Operation
	//==================================================

	// 操作説明画面の更新
	void UpdateOperation();

	// 操作説明画面の描画
	void DrawOperation();

	//==================================================
	// GamePlay
	//==================================================

	// ゲーム本編の更新
	void UpdateGamePlay();

	// ゲーム本編の描画
	void DrawGamePlay();

	//==================================================
	// GameClear
	//==================================================

	// ゲームクリア画面の更新
	void UpdateGameClear();

	// ゲームクリア画面の描画
	void DrawGameClear();

	//==================================================
	// GameOver
	//==================================================

	// ゲームオーバー画面の更新
	void UpdateGameOver();

	// ゲームオーバー画面の描画
	void DrawGameOver();

private:
	//==================================================
	// シーン管理
	//==================================================

	// 現在のシーン
	Scene scene_ = Scene::Title;

	// 次に移動するシーン
	Scene nextScene_ = Scene::Title;

	//==================================================
	// 制限時間
	//==================================================

	// 動作確認用の制限時間
	// 現在は10秒に設定している
	static const int kGameTime = 10;

	// 現在の残り時間
	int gameTime_ = kGameTime;

	// 制限時間を数えるためのフレーム数
	int timerFrame_ = 0;

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