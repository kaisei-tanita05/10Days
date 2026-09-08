#pragma once
#include <KamataEngine.h>
#include "Obstacles.h"
#include "Player.h"
#include "Item.h"
#include "stand.h"
#include "Quiz.h"

enum class ActivePlayer { Player1, Player2 };

class GameScene {
public:

	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// ゲームクリアしたか
	bool IsFinished() const { return isFinished_; }

	private:

	KamataEngine::Sprite* sprites_[4] = {};

	
	uint32_t bgTextureHandle_[4];

	// 背景のスクロール量
	float scrollX_ = 0.0f;

	// プレイヤーがここまで来たらスクロール開始
	float scrollStartX_ = 600.0f;

	// スクロールできる最大値
	float maxScrollX_ = 3840.0f;

	// 背景の移動速度
	float bgSpeed_ = 5.0f;

	// 障害物のインスタンス
	Obstacles* obstacles_ = nullptr;

	// アイテムのインスタンス
	Item* item_ = nullptr;

	Stand* stand_ = nullptr;

	// プレイヤーのインスタンス
	Player* player1_ = nullptr;
	Player* player2_ = nullptr;

	// 床用の変数
	uint32_t floor1TextureHandle_ = 0;             // 床のテクスチャハンドル
	KamataEngine::Sprite* floor1Sprite_ = nullptr; // 床のスプライト

	uint32_t floor2TextureHandle_ = 0;             // 床のテクスチャハンドル
	KamataEngine::Sprite* floor2Sprite_ = nullptr; // 床のスプライト

	uint32_t player1TextureHandle_ = 0;                 // プレイヤー1のテクスチャハンドル
	uint32_t player2TextureHandle_ = 0;                 // プレイヤー2のテクスチャハンドル
	ActivePlayer activePlayer_ = ActivePlayer::Player1; // 現在アクティブなプレイヤー

	float maxChainLength_ = 400.0f; // 紐の最大長（ピクセル）

	// 紐の描画用スプライト
	uint32_t chainTextureHandle_ = 0;
	KamataEngine::Sprite* chainSprite_ = nullptr;

	Player* itemHolder_ = nullptr;

	// ゲームクリアフラグ
	bool isFinished_ = false;

	// 半透明暗転用画像
	uint32_t overlayTextureHandle_ = 0;
	KamataEngine::Sprite* overlaySprite_ = nullptr;
	Quiz* quiz_ = nullptr;

	// 石壁用
	uint32_t wallTextureHandle_ = 0;
	KamataEngine::Sprite* wallSprite_ = nullptr;
	KamataEngine::Vector2 wallPosition_ = {3000.0f, 320.0f}; // Y座標を修正！
	float wallMinY_ = -200.0f;
};
