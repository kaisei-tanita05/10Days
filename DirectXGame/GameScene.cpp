#include "GameScene.h"
#include "Player.h"
#include <cmath>
#include <algorithm>

using namespace KamataEngine;

GameScene::GameScene(){}

// デストラクタで動的に確保したメモリを解放
GameScene::~GameScene() {
	delete player1_;
	delete player2_;
	delete chainSprite_;
}

void GameScene::Initialize() {
	// 画像の読み込み
	player1TextureHandle_ = TextureManager::Load("hito.png");
	player2TextureHandle_ = TextureManager::Load("hito2.png");

	// 床のテクスチャを読み込む
	floor1TextureHandle_ = TextureManager::Load("floor.png");
	floor2TextureHandle_ = TextureManager::Load("floor.png");

	// 紐のテクスチャを読み込む
	chainTextureHandle_ = TextureManager::Load("chain.png");

	// 床のスプライトを生成
	floor1Sprite_ = Sprite::Create(floor1TextureHandle_, {0.0f, 320.0f});
	floor1Sprite_->SetAnchorPoint({0.0f, 0.0f}); // 左上を基準にする

	floor2Sprite_ = Sprite::Create(floor2TextureHandle_, {0.0f, 670.0f});
	floor2Sprite_->SetAnchorPoint({0.0f, 0.0f}); // 左上を基準にする

	// 紐のスプライトを生成
	chainSprite_ = Sprite::Create(chainTextureHandle_, {0.0f, 0.0f});
	chainSprite_->SetAnchorPoint({0.0f, 0.5f}); // 左端を基準にする



	// プレイヤーの初期化
	player1_ = new Player();
	player1_->Initialize(player1TextureHandle_, {100.0f, 110.0f});
	player1_->SetMoveLimitY(0.0f, 190.0f); // プレイヤー1の移動範囲を設定

	player2_ = new Player();
	player2_->Initialize(player2TextureHandle_, {100.0f, 500.0f});
	player2_->SetMoveLimitY(350.0f, 550.0f); // プレイヤー2の移動範囲を設定

	// 紐の最大長を設定 (※必要に応じて調整してください)
	maxChainLength_ = 400.0f;

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

	// 紐の長さ制限の処理
	if (player1_ && player2_) {
		Vector2 pos1 = player1_->GetPosition();
		Vector2 pos2 = player2_->GetPosition();

		float heightDiff = std::abs(pos1.y - pos2.y);

		if (heightDiff < maxChainLength_) {
			float maxDx = std::sqrt(maxChainLength_ * maxChainLength_ - heightDiff * heightDiff);
			float currentDx = pos2.x - pos1.x; // P1から見たP2の位置差分
			float absDx = std::abs(currentDx);

			// 限界距離を超えて移動しようとした場合
			if (absDx > maxDx) {
				float sign = (currentDx > 0.0f) ? 1.0f : -1.0f;

				if (activePlayer_ == ActivePlayer::Player1) {
					// 【Player 1 操作中】
					// P1に引っぱられてP2が限界位置まで引き寄せられる
					pos2.x = pos1.x + sign * maxDx;
					player2_->SetPosition(pos2);
				} else if (activePlayer_ == ActivePlayer::Player2) {
					// 【Player 2 操作中】
					// P2は限界位置でそのままストップする（これ以上遠くに行けない）
					pos2.x = pos1.x + sign * maxDx;
					player2_->SetPosition(pos2);
				}
			}
		}
	}

	// 紐（ひも）スプライトの変形更新処理
	if (player1_ && player2_ && chainSprite_) {
		Vector2 pos1 = player1_->GetPosition();
		Vector2 pos2 = player2_->GetPosition();

		// キャラクターの中心座標
		Vector2 p1 = {pos1.x + 32.0f, pos1.y + 32.0f};
		Vector2 p2 = {pos2.x + 32.0f, pos2.y + 32.0f};

		// 2点間の差分と距離を計算
		Vector2 diff = {p2.x - p1.x, p2.y - p1.y};
		float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

		// 2点間の角度（ラジアン）を計算
		float angle = std::atan2(diff.y, diff.x);

		// 紐スプライトの座標、角度、スケールを設定
		chainSprite_->SetPosition(p1);    // 始点はP1の中心
		chainSprite_->SetRotation(angle); // P2へ向くように回転

		// スケール（X方向=紐の長さ, Y方向=紐の太さ）
		float chainThickness = 4.0f; // 紐の太さ（ピクセル）
		chainSprite_->SetSize({distance, chainThickness});
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

	
	// 紐をプレイヤーの背後に描画
	if (chainSprite_) {
		chainSprite_->Draw();
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