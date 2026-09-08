#pragma once
#include <KamataEngine.h>
#include "Obstacles.h"
#include "Player.h"
#include "Item.h"
#include "stand.h"
#include "Quiz.h"
#include "Botom.h"
#include "Fade.h"
#include "Time.h"

enum class ActivePlayer { Player1, Player2 };

class GameScene {
public:

	// 02_12 27枚目 シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

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

	// ゲームオーバーになったかどうかのフラグを取得
	bool IsGameOver() const { return isGameOver_; }

	//シェイク開始関数
	void StartShake(float intensity, float duration) {
		shakeIntensity_ = intensity;
		shakeDuration_ = duration;
	}

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

	// ボタン
	Botom* botom_ = nullptr;

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


	int timer_ = 4200;     // 60秒の制限時間
	bool isGameOver_ = false; // ゲームオーバー判定用フラグ

	// サウンドハンドル
	uint32_t BGMHandle_ = 0;

	uint32_t voiceHandle_ = 0;

	//SE
	uint32_t SEPunchHandle_ = 0;

	uint32_t SEThrowHandle_ = 0;

	uint32_t SEOpendoor_ = 0;

	uint32_t SEOpenGate_ = 0;

	uint32_t SEButtonHandle_ = 0;

	uint32_t SEdoorCrushHandle_ = 0;

	//成功した音
	uint32_t SEConnectHandle_ = 0;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kMain;

	float shakeIntensity_ = 0.0f;        // シェイクの強度（ピクセル）
	float shakeDuration_ = 0.0f;         // シェイクの残り時間（秒）
	KamataEngine::Vector2 shakeOffset_ = {0.0f, 0.0f}; // 計算された揺れ幅

	// 半透明暗転用画像
	uint32_t overlayTextureHandle_ = 0;
	KamataEngine::Sprite* overlaySprite_ = nullptr;
	Quiz* quiz_ = nullptr;

	// 石壁用
	uint32_t wallTextureHandle_ = 0;
	// 壁1（Quiz用：背景2枚目の終わり）
    KamataEngine::Sprite* wall1Sprite_ = nullptr;
    KamataEngine::Vector2 wall1Position_ = { 0.0f, 0.0f };
    float wall1MinY_ = -800.0f;

    // 壁2（新規追加：背景3枚目の終わり）
    KamataEngine::Sprite* wall2Sprite_ = nullptr;
    KamataEngine::Vector2 wall2Position_ = { 0.0f, 0.0f };
    float wall2MinY_ = -800.0f;
    bool isWall2Cleared_ = false; // 壁2が開いたかのフラグ

	bool isPlaySEGate1_ = false; // 壁1の音再生済みフラグ
	bool isPlaySEGate2_ = false; // 壁2の音再生済みフラグ

	Time* timeDisplay_ = nullptr;
};
