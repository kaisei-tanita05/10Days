#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	for (int i = 0; i < 4; i++) {
		delete sprites_[i];
		sprites_[i] = nullptr;
	}
	delete obstacles_;
	obstacles_ = nullptr;

	delete player1_;
	delete player2_;
}

void GameScene::Initialize() {
	// 背景テクスチャの読み込み
	// スプライトの生成
	for (int i = 0; i < 4; i++) {
		bgTextureHandle_[i] = TextureManager::Load("BackGround/backGround.png");
		// 1280pxずつ横に並べる
		sprites_[i] = Sprite::Create(bgTextureHandle_[i], Vector2(1280.0f * i, 0.0f));
	}

	// 障害物の初期化
	obstacles_ = new Obstacles();
	// ステージ上の座標
	obstacles_->Initialize({2000.0f, 256.0f});

	// プレイヤーの初期化
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
		if (activePlayer_ == ActivePlayer::Player1) {
			activePlayer_ = ActivePlayer::Player2;
		} else {
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

	//========================================
	// 背景スクロール
	//========================================

	// 操作中のプレイヤーを取得
	Player* activePlayer = nullptr;

	if (activePlayer_ == ActivePlayer::Player1) {
		activePlayer = player1_;
	} else {
		activePlayer = player2_;
	}

	if (activePlayer) {

		// プレイヤーのワールド座標
		float playerWorldX = activePlayer->GetPosition().x;

		// プレイヤーの画面上の座標
		float playerScreenX = playerWorldX - scrollX_;

		// Dキーを押している
		if (input->PushKey(DIK_D)) {

			// 画面上のプレイヤーが800pxに到達したらスクロール
			if (playerScreenX >= scrollStartX_) {

				// プレイヤーと同じ速度で背景をスクロール
				scrollX_ += bgSpeed_;

				// 最大スクロール量
				if (scrollX_ > maxScrollX_) {
					scrollX_ = maxScrollX_;
				}
			}
		}
	}

	// プレイヤーにスクロール量を渡す
	if (player1_) {
		player1_->SetScrollX(scrollX_);
	}

	if (player2_) {
		player2_->SetScrollX(scrollX_);
	}

	// 障害物の更新
	if (obstacles_) {
		obstacles_->SetScrollX(scrollX_);
		obstacles_->Update();
	}

	//========================================
	// 障害物との当たり判定
	//========================================

	if (obstacles_ && !obstacles_->IsDestroyed()) {

		const float obstacleWidth = 64.0f;
		const float obstacleHeight = 64.0f;

		// Player1
		if (player1_) {

			if (player1_->IsCollision(obstacles_->GetPosition(), obstacleWidth, obstacleHeight)) {

				// 通常の衝突処理
				player1_->ResolveCollision(obstacles_->GetPosition(), obstacleWidth, obstacleHeight);

				// Enterキー
				if (input->TriggerKey(DIK_RETURN)) {

					obstacles_->Hit();
				}
			}
		}

		// Player2
		if (player2_) {

			if (player2_->IsCollision(obstacles_->GetPosition(), obstacleWidth, obstacleHeight)) {

				// 通常の衝突処理
				player2_->ResolveCollision(obstacles_->GetPosition(), obstacleWidth, obstacleHeight);

				// Enterキー
				if (input->TriggerKey(DIK_RETURN)) {

					obstacles_->Hit();
				}
			}
		}
	}
}

void GameScene::Draw() {
	// 背景スプライトの描画
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	// 背景
	for (int i = 0; i < 4; i++) {

		float x = (1280.0f * i) - scrollX_;

		sprites_[i]->SetPosition({x, 0.0f});

		sprites_[i]->Draw();
	}
	//sprite2_->Draw();

	//プレイヤー
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
	obstacles_->Draw();


	Sprite::PostDraw();
	// 障害物の描画
}