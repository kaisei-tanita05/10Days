#pragma once

#include <KamataEngine.h>
#include "Player.h"


enum class ActivePlayer { Player1, Player2 };

class GameScene 
{
public:
	GameScene();
	~GameScene();

	void Initialize();

	void Update();

	void Draw();


private:
	// プレイヤーのインスタンス
	Player* player1_ = nullptr;
	Player* player2_ = nullptr;

	//床用の変数
	uint32_t floor1TextureHandle_ = 0; // 床のテクスチャハンドル
	KamataEngine::Sprite* floor1Sprite_ = nullptr; // 床のスプライト

	uint32_t floor2TextureHandle_ = 0; // 床のテクスチャハンドル
	KamataEngine::Sprite* floor2Sprite_ = nullptr; // 床のスプライト

	uint32_t player1TextureHandle_ = 0; // プレイヤー1のテクスチャハンドル
	uint32_t player2TextureHandle_ = 0; // プレイヤー2のテクスチャハンドル
	ActivePlayer activePlayer_ = ActivePlayer::Player1; // 現在アクティブなプレイヤー

};
