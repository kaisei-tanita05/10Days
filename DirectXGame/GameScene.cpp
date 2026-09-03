#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene(){}

// デストラクタで動的に確保したメモリを解放
GameScene::~GameScene() {
	delete player1_;
	delete player2_;
}

void GameScene::Initialize() {
	// 画像の読み込み
	player1TextureHandle_ = TextureManager::Load("hito.png");
	player2TextureHandle_ = TextureManager::Load("hito2.png");

	// 床のテクスチャを読み込む
	floor1TextureHandle_ = TextureManager::Load("floor.png");
	floor2TextureHandle_ = TextureManager::Load("floor.png");

	// 床のスプライトを生成
	floor1Sprite_ = Sprite::Create(floor1TextureHandle_, {0.0f, 320.0f});
	floor1Sprite_->SetAnchorPoint({0.0f, 0.0f}); // 左上を基準にする

	floor2Sprite_ = Sprite::Create(floor2TextureHandle_, {0.0f, 670.0f});
	floor2Sprite_->SetAnchorPoint({0.0f, 0.0f}); // 左上を基準にする

	// プレイヤーの初期化
	player1_ = new Player();
	player1_->Initialize(player1TextureHandle_, {100.0f, 110.0f});
	player1_->SetMoveLimitY(0.0f, 190.0f); // プレイヤー1の移動範囲を設定

	player2_ = new Player();
	player2_->Initialize(player2TextureHandle_, {100.0f, 500.0f});
	player2_->SetMoveLimitY(350.0f, 550.0f); // プレイヤー2の移動範囲を設定

}

void GameScene::Update() {
	// ゲームロジックや入力処理を記述
	Input* input = Input::GetInstance();

	// プレイヤーの更新
	if (input->TriggerKey(DIK_1)) {
		if(activePlayer_ == ActivePlayer::Player1) 
		{
			activePlayer_ = ActivePlayer::Player2;
		} else 
		{
			activePlayer_ = ActivePlayer::Player1;
		}
	}

	// それぞれに「自分が操作中か」を渡してUpdate
	if (player1_) {
		player1_->Update(activePlayer_ == ActivePlayer::Player1);
	}
	if (player2_) {
		player2_->Update(activePlayer_ == ActivePlayer::Player2);
	}
}

void GameScene::Draw() {
	//DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 描画前処理
	Sprite::PreDraw();

	if (floor1Sprite_) {
		floor1Sprite_->Draw();
	}
	if (floor2Sprite_) {
		floor2Sprite_->Draw();
	}

	// スプライトの描画 (PreDraw と PostDraw の間に記述)
	if (player1_) {
		player1_->Draw();
	}
	if (player2_) {
		player2_->Draw();
	}

	// 描画後処理
	Sprite::PostDraw();
}