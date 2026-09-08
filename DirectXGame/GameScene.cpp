#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() {

	// 背景スプライトの解放
	for (int i = 0; i < 4; i++) {
		delete sprites_[i];
		sprites_[i] = nullptr;
	}

	// 障害物の解放
	delete obstacles_;
	obstacles_ = nullptr;

	// アイテムの解放
	delete item_;
	item_ = nullptr;

	// Standの解放
	delete stand_;
	stand_ = nullptr;

	// プレイヤーの解放
	delete player1_;
	player1_ = nullptr;
	delete player2_;
	player2_ = nullptr;

	// 床
	delete floor1Sprite_;
	floor1Sprite_ = nullptr;

	delete floor2Sprite_;
	floor2Sprite_ = nullptr;

	// 鎖モデルの解放
	delete chainSprite_;
	chainSprite_ = nullptr;

	delete quiz_;
	quiz_ = nullptr;

	delete wallSprite_;
	wallSprite_ = nullptr;
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

	// 紐のテクスチャを読み込む
	chainTextureHandle_ = TextureManager::Load("chain.png");

	// 床のスプライトを生成
	floor1Sprite_ = Sprite::Create(floor1TextureHandle_, {0.0f, 320.0f});
	floor1Sprite_->SetAnchorPoint({0.0f, 0.0f}); // 左上を基準にする

	floor2Sprite_ = Sprite::Create(floor2TextureHandle_, {0.0f, 670.0f});
	floor2Sprite_->SetAnchorPoint({0.0f, 0.0f}); // 左上を基準にする

	// 暗転用画像の読み込みとスプライト生成
	overlayTextureHandle_ = TextureManager::Load("black.png");
	overlaySprite_ = Sprite::Create(overlayTextureHandle_, {0.0f, 0.0f});

	// 色と透明度の設定 (R, G, B, A) -> アルファ値 0.2f で20%透過
	overlaySprite_->SetColor({1.0f, 1.0f, 1.0f, 0.99f});

	// プレイヤーの初期化
	player1_ = new Player();
	player1_->Initialize(player1TextureHandle_, {100.0f, 110.0f});
	player1_->SetMoveLimitY(0.0f, 190.0f); // プレイヤー1の移動範囲を設定

	player2_ = new Player();
	player2_->Initialize(player2TextureHandle_, {100.0f, 500.0f});
	player2_->SetMoveLimitY(350.0f, 550.0f); // プレイヤー2の移動範囲を設定

	// 紐のスプライトを生成
	chainSprite_ = Sprite::Create(chainTextureHandle_, {0.0f, 0.0f});
	chainSprite_->SetAnchorPoint({0.0f, 0.5f}); // 左端を基準にする
	// 紐の最大長を設定
	maxChainLength_ = 400.0f;

	// Standの初期化
	stand_ = new Stand();

	// Standの位置
	stand_->Initialize({2600.0f, 606.0f});

	uint32_t quizTex = TextureManager::Load("Quiz/mondai1.png");
	uint32_t quizTex2 = TextureManager::Load("Quiz/mondai2.png");

	uint32_t btnTexs[3] = {TextureManager::Load("Quiz/button1.png"), TextureManager::Load("Quiz/button2.png"), TextureManager::Load("Quiz/button3.png")};

	Vector2 boardScreenPosition = {600.0f, 380.0f};
	Vector2 boardScreenPosition2 = {600.0f, 100.0f};

	// ステージ（下画面）内に配置する3つのボタンのワールド座標
	Vector2 btnWorldPositions[3] = {
	    {600.0f, 550.0f}, // ボタン1
	    {750.0f, 550.0f}, // ボタン2
	    {900.0f, 550.0f}  // ボタン3
	};

	quiz_ = new Quiz();
	quiz_->Initialize(quizTex, quizTex2, btnTexs, boardScreenPosition, boardScreenPosition2, btnWorldPositions);

	// 石壁の初期化
	wallTextureHandle_ = TextureManager::Load("wall.png");
	wallPosition_ = {1150.0f, 0.0f};
	wallMinY_ = -800.0f; // 画面外（画面上端より上）まで引き上げる目標値
	wallSprite_ = Sprite::Create(wallTextureHandle_, wallPosition_);
}

void GameScene::Update() {

	// ゲームロジックや入力処理を記述
	Input* input = Input::GetInstance();

	// プレイヤー切替
	if (input->TriggerKey(DIK_1)) {
		if (activePlayer_ == ActivePlayer::Player1) {
			activePlayer_ = ActivePlayer::Player2;
		} else {
			activePlayer_ = ActivePlayer::Player1;
		}
	}

	// 1. 【準備】プレイヤーに壁の制限座標をあらかじめ設定する（移動処理の前！）
	if (wallPosition_.y > wallMinY_) {
		if (player1_)
			player1_->SetWallLimitX(wallPosition_.x);
		if (player2_)
			player2_->SetWallLimitX(wallPosition_.x);
	} else {
		if (player1_)
			player1_->ClearWallLimitX();
		if (player2_)
			player2_->ClearWallLimitX();
	}

	// 2. 【移動】プレイヤーのUpdateを実行（1フレームに1回だけ実行）
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

		// プレイヤーのワールド座標（すでに壁でブロックされた後の座標）
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

	// 暗転オーバーレイの位置更新
	if (overlaySprite_) {
		if (activePlayer_ == ActivePlayer::Player1) {
			// Player1 操作中 -> Player2 の領域（下段）に被せる
			overlaySprite_->SetPosition({0.0f, 320.0f}); // 床2の高さなどに合わせる
		} else {
			// Player2 操作中 -> Player1 の領域（上段）に被せる
			overlaySprite_->SetPosition({0.0f, 0.0f}); // 上段エリアの先頭
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
	// アイテム
	//========================================

	if (obstacles_ && obstacles_->IsDestroyed() && item_ == nullptr) {

		// 瓶が壊れた場所
		Vector2 itemPosition = obstacles_->GetPosition();

		item_ = new Item();
		item_->Initialize(itemPosition);
	}

	if (item_) {

		item_->SetScrollX(scrollX_);
		item_->Update();
	}

	if (stand_) {

		stand_->SetScrollX(scrollX_);
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

		Vector2 p1 = {pos1.x + 32.0f - scrollX_, pos1.y + 32.0f};

		Vector2 p2 = {pos2.x + 32.0f - scrollX_, pos2.y + 32.0f};

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

					// 瓶が壊れた
					if (obstacles_->IsDestroyed()) {

						// Itemがまだ存在していなければ生成
						if (!item_) {

							item_ = new Item();

							// 瓶の位置にItemを出す
							item_->Initialize(obstacles_->GetPosition());
						}
					}
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

	if (item_ && !item_->IsHeld() && !item_->IsDropped()) {

		const float itemWidth = 64.0f;
		const float itemHeight = 64.0f;

		if (player1_ && player1_->IsCollision(item_->GetPosition(), itemWidth, itemHeight)) {

			item_->PickUp();
			itemHolder_ = player1_;
		} else if (player2_ && player2_->IsCollision(item_->GetPosition(), itemWidth, itemHeight)) {

			item_->PickUp();
			itemHolder_ = player2_;
		}
	}

	//========================================
	// Itemを持っている間
	//========================================

	if (item_ && item_->IsHeld() && itemHolder_) {

		Vector2 playerPos = itemHolder_->GetPosition();

		// プレイヤーの頭上に持つ
		item_->SetPosition({playerPos.x, playerPos.y - 50.0f});
	}

	//========================================
	// Itemを落とす
	//========================================

	if (item_ && item_->IsHeld() && itemHolder_ && input->TriggerKey(DIK_RETURN)) {

		// 誰が持っているかで落下する地面を変更
		if (itemHolder_ == player1_) {

			// Player1側の地面
			item_->SetGroundY(606.0f);

		} else if (itemHolder_ == player2_) {

			// Player2側の地面
			item_->SetGroundY(606.0f);
		}

		// Itemを落とす
		item_->Drop();

		// 持ち主を解除
		itemHolder_ = nullptr;
	}
	//========================================
	// 投げたItemとStandの当たり判定
	//========================================

	if (item_ && stand_ && item_->IsDropped() && !isFinished_) {

		const float itemWidth = 64.0f;
		const float itemHeight = 64.0f;

		if (stand_->IsCollision(item_->GetPosition(), itemWidth, itemHeight)) {

			// Standの上にItemを置く
			item_->SetPosition(stand_->GetItemPosition());

			// Itemを停止
			item_->Stop();

			// ゲームクリア
			isFinished_ = true;
		}
	}

	// 1. クイズの更新
	if (quiz_) {
		quiz_->SetScrollX(scrollX_);
		// P2が操作中の場合のみ解答可能なように P2 の情報を渡す
		Vector2 p2Pos = player2_ ? player2_->GetPosition() : Vector2{0, 0};
		quiz_->Update(activePlayer_ == ActivePlayer::Player2, p2Pos);
	}

	// 2. クイズ正解時の石壁の移動処理
	if (quiz_ && quiz_->IsCleared()) {
		if (wallPosition_.y > wallMinY_) {
			wallPosition_.y -= 3.0f; // 上に昇る速度
			if (wallPosition_.y < wallMinY_) {
				wallPosition_.y = wallMinY_;
			}
		}
	}

	// 壁スプライトの位置（スクロール反映）
	if (wallSprite_) {
		wallSprite_->SetPosition({wallPosition_.x - scrollX_, wallPosition_.y});
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
	// 石壁の描画
	if (wallSprite_) {
		wallSprite_->Draw();
	}
	// 床
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

	// Stand
	if (stand_) {
		stand_->Draw();
	}

	if (item_) {
		item_->Draw();
	}

	// クイズの描画
	if (quiz_) {
		quiz_->Draw();
	}

	if (obstacles_) {
		obstacles_->Draw();
	}

	// 最後に非操作領域の暗転オーバーレイを描画
	if (overlaySprite_) {
		overlaySprite_->Draw();
	}

	Sprite::PostDraw();
}
