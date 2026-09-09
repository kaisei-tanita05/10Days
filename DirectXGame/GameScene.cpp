#include "GameScene.h"
#include <algorithm> // std::min / std::max に必要

#ifndef NOMINMAX
#define NOMINMAX // Windows.h の min/max マクロ定義を防ぐ
#endif
#include <Windows.h>

using namespace KamataEngine;

GameScene::~GameScene() {

	if (voiceHandle_ != 0) {
        Audio::GetInstance()->StopWave(voiceHandle_);
    }

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

	// 壁1・壁2の解放
	delete wall1Sprite_;
	wall1Sprite_ = nullptr;

	delete wall2Sprite_;
	wall2Sprite_ = nullptr;

	// Fadeの解放
	delete fade_;
	fade_ = nullptr;

	// ボタンの解放
	delete botom_;
	botom_ = nullptr;

	delete timeDisplay_;

	delete particleManager_;
	particleManager_ = nullptr;
}

void GameScene::Initialize() {
	// 背景テクスチャの読み込み
	// スプライトの生成
	for (int i = 0; i < 4; i++) {
		bgTextureHandle_[i] = TextureManager::Load("BackGround/backGround.png");
		// 1280pxずつ横に並べる
		sprites_[i] = Sprite::Create(bgTextureHandle_[i], Vector2(1280.0f * i, 0.0f));
	}

	particleTextureHandle_ = TextureManager::Load("particle/particle.png");
	particleManager_ = new ParticleManager();
	particleManager_->Initialize(particleTextureHandle_);

	//==================================================
	// 障害物 (背景2枚目のエリア X: 1280〜2560)
	//==================================================
	obstacles_ = new Obstacles();
	obstacles_->Initialize({2800.0f, 256.0f});

	//==================================================
	// ボタン・扉ギミック (背景最後のエリア X: 3800前後)
	// ※扉はボタン位置から+300px(4100.0f)に配置されます
	//==================================================
	botom_ = new Botom();
	botom_->Initialize({4500.0f, 620.0f});

	// プレイヤーの初期化
	// 画像の読み込み
	player1TextureHandle_ = TextureManager::Load("player/player1.png");
	player2TextureHandle_ = TextureManager::Load("player/player2.png");

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
	player1_->Initialize(player1TextureHandle_, {100.0f, 192.0f});
	player1_->SetMoveLimitY(0.0f, 192.0f); // プレイヤー1の移動範囲を設定

	player2_ = new Player();
	player2_->Initialize(player2TextureHandle_, {100.0f, 542.0f});
	player2_->SetMoveLimitY(350.0f, 550.0f); // プレイヤー2の移動範囲を設定

	// 紐のスプライトを生成
	chainSprite_ = Sprite::Create(chainTextureHandle_, {0.0f, 0.0f});
	chainSprite_->SetAnchorPoint({0.0f, 0.5f}); // 左端を基準にする
	// 紐の最大長を設定
	maxChainLength_ = 400.0f;

	// Standの初期化
	stand_ = new Stand();
	// Standの位置
	stand_->Initialize({3400.0f, 606.0f});


	fade_ = new Fade();
	fade_->Initialize();
	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 2.0f);
	phase_ = Phase::kFadeIn;

	//サウンド
	/// BGMの読み込み
	BGMHandle_ = Audio::GetInstance()->LoadWave("Sound/BGM/GameSceneBGM.mp3");
	voiceHandle_ = Audio::GetInstance()->PlayWave(BGMHandle_, true, 1.0f); // ループ再生

	//殴る音
	SEPunchHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/PunchSE.mp3");

	//itemを投げる音
	SEThrowHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/throwSE.mp3");

	//扉を開ける音
	SEOpendoor_ = Audio::GetInstance()->LoadWave("Sound/SE/doorOpenSE.mp3");

	SEOpenGate_ = Audio::GetInstance()->LoadWave("Sound/SE/openGateSE.mp3");

	// ボタンを押す音
	SEButtonHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/pushButtonSE.mp3");

	// 扉が壊れる音
	SEdoorCrushHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/doorCrushSE.mp3");

	// 成功した音
	SEConnectHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/connectSE.mp3");

	uint32_t quizTex = TextureManager::Load("Quiz/mondai1.png");
	uint32_t quizTex2 = TextureManager::Load("Quiz/mondai2.png");

	uint32_t btnTexs[3] = {TextureManager::Load("Quiz/button1.png"), TextureManager::Load("Quiz/button2.png"), TextureManager::Load("Quiz/button3.png")};

	Vector2 boardWorldPosition = {1900.0f, 380.0f};  // 下画面側の問題板
	Vector2 boardWorldPosition2 = {1900.0f, 100.0f}; // 上画面側の問題板

	// ステージ（下画面）内に配置する3つのボタンのワールド座標
	Vector2 btnWorldPositions[3] = {
	    {1800.0f, 550.0f}, // ボタン1
	    {1950.0f, 550.0f}, // ボタン2
	    {2100.0f, 550.0f}  // ボタン3
	};

	quiz_ = new Quiz();
	quiz_->Initialize(quizTex, quizTex2, btnTexs, boardWorldPosition, boardWorldPosition2, btnWorldPositions);

	// 石壁の初期化
	// 壁1 (背景2枚目の出口 X: 2400)
	wallTextureHandle_ = TextureManager::Load("wall.png");
	wall1Position_ = {2400.0f, 0.0f};
	wall1MinY_ = -800.0f;
	wall1Sprite_ = Sprite::Create(wallTextureHandle_, wall1Position_);

	// 壁2 (背景3枚目の出口 X: 3700)
	wall2Position_ = {3700.0f, 0.0f};
	wall2MinY_ = -800.0f;
	wall2Sprite_ = Sprite::Create(wallTextureHandle_, wall2Position_);

	// 時間表示
	timeDisplay_ = new Time();
	timeDisplay_->Initialize();
}

void GameScene::Update() {

	//========================================
	// TitleSceneと同様のswitch文によるPhase制御
	//========================================
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		return;

	case Phase::kMain:
		// メインのゲームロジック実行（下部で処理）
		//制限時間のカウントダウン (60fps想定)
		timer_ -= 1;
		// Timeクラスに現在の残り時間を渡して計算させる
		timeDisplay_->UpDate(timer_);
		//1分経過したらGameOverにしてフェードアウト開始
		if (timer_ <= 0) {
			timer_ = 0;
			isGameOver_ = true; // フラグを立てる

			fade_->Start(Fade::Status::FadeOut, 2.0f);
			phase_ = Phase::kFadeOut;
			return;
		}
		break;

	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			Audio::GetInstance()->StopWave(BGMHandle_);
			isFinished_ = true;
		}
		return; // フェードアウト中もゲーム処理をスキップ
	}

	// FadeIn中もゲーム操作を受け付けない場合はリターン
	if (phase_ == Phase::kFadeIn) {
		return;
	}


	// 画面シェイクの更新処理
	if (shakeDuration_ > 0.0f) {
		shakeDuration_ -= 1.0f / 60.0f; // フレーム経過

		// ランダムな方向に揺らす (-1.0 ～ 1.0 の乱数 * 強度)
		float offsetX = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * shakeIntensity_;
		float offsetY = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * shakeIntensity_;
		shakeOffset_ = Vector2{offsetX, offsetY};

		if (shakeDuration_ <= 0.0f) {
			shakeDuration_ = 0.0f;
			shakeOffset_ = Vector2{0.0f, 0.0f};
		}
	} else {
		shakeOffset_ = Vector2{0.0f, 0.0f};
	}

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
	if (wall1Position_.y > wall1MinY_) {
		if (player1_)
			player1_->SetWallLimitX(wall1Position_.x);
		if (player2_)
			player2_->SetWallLimitX(wall1Position_.x);
	}
	if (wall2Position_.y > wall2MinY_) {
		if (player1_)
			player1_->SetWallLimitX(wall2Position_.x);
		if (player2_)
			player2_->SetWallLimitX(wall2Position_.x);
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

	//==================================================
	// ボタン・扉ギミック更新
	//==================================================

	if (botom_) {

		// スクロール量を渡す
		botom_->SetScrollX(scrollX_);

		// ギミックを更新
		botom_->Update();
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

	//==================================================
	// ボタン・扉ギミック
	//==================================================

	if (botom_) {

		//==================================================
		// 下側：ガリリ
		// ボタンを押して下の扉を開ける
		//==================================================

		if (player2_) {

			// ガリリを操作中
			if (activePlayer_ == ActivePlayer::Player2) {

				// ボタンに触れているか
				if (botom_->IsCollision(player2_->GetPosition(), player2_->kWidth, player2_->kHeight)) {

					// Enterでボタンを押す
					if (input->TriggerKey(DIK_RETURN)) {
						// ボタン音を再生
						Audio::GetInstance()->PlayWave(SEButtonHandle_, false, 2.0f);
						Audio::GetInstance()->PlayWave(SEOpendoor_, false, 3.0f);
						botom_->Push();
					}
				}
			}
		}

		//==================================================
		// 上側：デブブ
		// 扉を破壊する
		//==================================================

		if (player1_) {

			// デブブが上の扉に当たっているか
			if (botom_->IsDoorCollision(player1_->GetPosition(), player1_->kWidth, player1_->kHeight)) {

				// デブブを操作中
				if (activePlayer_ == ActivePlayer::Player1) {

					// Enterで扉を破壊
					if (input->TriggerKey(DIK_RETURN)) {

						// パンチ音を再生
						Audio::GetInstance()->PlayWave(SEPunchHandle_, false, 2.0f);
						botom_->BreakDoor();
						// 扉が壊れる音を再生
						Audio::GetInstance()->PlayWave(SEdoorCrushHandle_, false, 2.0f);
						// 画面シェイク発動
						StartShake(15.0f, 0.3f);
					}
				}
			}
		}

		//==================================================
		// 下側：ガリリ用の扉
		//==================================================

		if (player2_) {

			// 下の扉がまだ開いていない場合
			if (botom_->IsBottomDoorCollision(player2_->GetPosition(), player2_->kWidth, player2_->kHeight)) {

				// 扉にぶつかったら止める
				player2_->ResolveCollision(botom_->GetBottomDoorPosition(), 80.0f, 120.0f);
			}
		}

		//==================================================
		// 上側：デブブ用の扉
		//==================================================

		if (player1_) {

			// 上の扉がまだ壊れていない場合
			if (botom_->IsDoorCollision(player1_->GetPosition(), player1_->kWidth, player1_->kHeight)) {

				// 扉にぶつかったら止める
				player1_->ResolveCollision(botom_->GetDoorPosition(), 80.0f, 120.0f);
			}
		}

		//==================================================
		//扉開通＆両プレイヤー通過によるクリア判定
		//==================================================
		// 上の扉が壊れ、下の扉が開いている時
		if (botom_->IsDoorBroken() && botom_->IsBottomDoorOpened()) {

			// 扉の右端のX座標
			float doorRightX = botom_->GetDoorPosition().x + 80.0f;

			// 両方のプレイヤーが扉の位置を通過したかチェック
			if (player1_ && player2_ && player1_->GetPosition().x > doorRightX && player2_->GetPosition().x > doorRightX) {
				fade_->Start(Fade::Status::FadeOut, 2.0f);

				// FadeOut状態へ
				phase_ = Phase::kFadeOut;
			}
		}
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
					// パンチ音を再生
					Audio::GetInstance()->PlayWave(SEPunchHandle_, false, 2.0f);
					obstacles_->Hit();

					Vector2 obstacleCenter = {obstacles_->GetPosition().x + obstacleWidth * 0.5f, obstacles_->GetPosition().y + obstacleHeight * 0.5f};
					if (particleManager_) {
						particleManager_->SpawnBurst(obstacleCenter, 25); // 25粒生成
					}

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

	if (particleManager_) {
		particleManager_->Update();
	}

	if (item_ && !item_->IsHeld() && !item_->IsDropped() && !item_->IsOnStand()) {

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

		Audio::GetInstance()->PlayWave(SEThrowHandle_, false, 3.0f);
		// Itemを落とす
		item_->Drop();
		// 持ち主を解除
		itemHolder_ = nullptr;
	}
	//========================================
	// 投げたItemとStandの当たり判定
	//========================================

	if (phase_ == Phase::kMain &&item_ && stand_ && item_->IsDropped() && !isFinished_) {

		const float itemWidth = 64.0f;
		const float itemHeight = 64.0f;

		if (stand_->IsCollision(item_->GetPosition(), itemWidth, itemHeight)) {
			Audio::GetInstance()->PlayWave(SEConnectHandle_, false, 3.0f);
			// Standの上にItemを置く
			item_->SetPosition(stand_->GetItemPosition());

			// Itemを停止
			item_->Stop();
			// 背景3枚目のギミッククリア（壁2を開ける）
			isWall2Cleared_ = true;
		}
	}

	float currentWallLimitX = 99999.0f;

	if (quiz_ && !quiz_->IsCleared()) {
		currentWallLimitX = (std::min)(currentWallLimitX, wall1Position_.x);
	}
	if (!isWall2Cleared_) {
		currentWallLimitX = (std::min)(currentWallLimitX, wall2Position_.x);
	}

	if (currentWallLimitX < 99999.0f) {
		if (player1_)
			player1_->SetWallLimitX(currentWallLimitX);
		if (player2_)
			player2_->SetWallLimitX(currentWallLimitX);
	} else {
		if (player1_)
			player1_->ClearWallLimitX();
		if (player2_)
			player2_->ClearWallLimitX();
	}


	// --- ギミック1: クイズ更新および壁1上昇処理 ---
	if (quiz_) {
		quiz_->SetScrollX(scrollX_);
		Vector2 p2Pos = player2_ ? player2_->GetPosition() : Vector2{0, 0};
		quiz_->Update(activePlayer_ == ActivePlayer::Player2, p2Pos);

		// 間違えた選択肢を選んだ場合に時間を10秒減らす
		if (quiz_->IsIncorrectTriggered()) {
			timer_ -= 10;
			if (timer_ < 0) {
				timer_ = 0;
			}
			// 誤答時のフィードバックとして画面を少し揺らす（任意）
			StartShake(10.0f, 0.2f);
		}
	}

	if (quiz_ && quiz_->IsCleared()) {
		if (wall1Position_.y > wall1MinY_) {
			if (!isPlaySEGate1_) {
				Audio::GetInstance()->PlayWave(SEOpenGate_, false, 2.0f);
				isPlaySEGate1_ = true; // 再生済みフラグを立てる
			}
			wall1Position_.y -= 3.0f;
			if (wall1Position_.y < wall1MinY_) {
				wall1Position_.y = wall1MinY_;
			}
		}
	}

	// --- ギミック2: Stand（アイテム載せ）成功で壁2上昇処理 ---
	if (isWall2Cleared_) {
		if (wall2Position_.y > wall2MinY_) {
			if (!isPlaySEGate2_) {
				Audio::GetInstance()->PlayWave(SEOpenGate_, false, 2.0f);
				isPlaySEGate2_ = true; // 再生済みフラグを立てる
			}
			wall2Position_.y -= 3.0f;
			if (wall2Position_.y < wall2MinY_) {
				wall2Position_.y = wall2MinY_;
			}
		}
	}

	// 壁スプライトの位置（スクロール反映）
	if (wall1Sprite_) {
		wall1Sprite_->SetPosition({wall1Position_.x - scrollX_, wall1Position_.y});
	}
	if (wall2Sprite_) {
		wall2Sprite_->SetPosition({wall2Position_.x - scrollX_, wall2Position_.y});
	}
}

void GameScene::Draw() {
	// 背景スプライトの描画
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// シェイク分を加味したスクロール値
	float renderScrollX = scrollX_ - shakeOffset_.x;
	float renderOffsetY = shakeOffset_.y;

	// 背景
	for (int i = 0; i < 4; i++) {
		float x = (1280.0f * i) - renderScrollX;
		sprites_[i]->SetPosition({x, renderOffsetY});
		sprites_[i]->Draw();
	}

	// 壁1・壁2の描画
	if (wall1Sprite_) {
		wall1Sprite_->Draw();
	}
	if (wall2Sprite_) {
		wall2Sprite_->Draw();
	}
	// 床

	// 床1 (上エリアの床)
	if (floor1Sprite_) {
		// 背景と同じように1280px刻みで並べて描画する場合
		for (int i = 0; i < 4; i++) {
			float x = (1280.0f * i) - renderScrollX;
			floor1Sprite_->SetPosition({x, 320.0f + renderOffsetY});
			floor1Sprite_->Draw();
		}
	}

	// 床2 (下エリアの床)
	if (floor2Sprite_) {
		for (int i = 0; i < 4; i++) {
			float x = (1280.0f * i) - renderScrollX;
			floor2Sprite_->SetPosition({x, 670.0f + renderOffsetY});
			floor2Sprite_->Draw();
		}
	}

	// ボタン・扉ギミック
	if (botom_) {
		botom_->SetScrollX(renderScrollX);
		botom_->Draw();
	}

	// 紐（チェーン）描画
	if (chainSprite_) {
		Vector2 pos1 = player1_->GetPosition();
		Vector2 pos2 = player2_->GetPosition();

		Vector2 p1 = {pos1.x + 32.0f - renderScrollX, pos1.y + 32.0f + renderOffsetY};
		Vector2 p2 = {pos2.x + 32.0f - renderScrollX, pos2.y + 32.0f + renderOffsetY};

		Vector2 diff = {p2.x - p1.x, p2.y - p1.y};
		float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		float angle = std::atan2(diff.y, diff.x);

		chainSprite_->SetPosition(p1);
		chainSprite_->SetRotation(angle);
		chainSprite_->SetSize({distance, 4.0f});
		chainSprite_->Draw();
	}

	// プレイヤー
	if (player1_) {
		player1_->SetScrollX(renderScrollX);
		player1_->Draw();
	}
	if (player2_) {
		player2_->SetScrollX(renderScrollX);
		player2_->Draw();
	}

	// Stand
	if (stand_) {
		stand_->SetScrollX(renderScrollX);
		stand_->Draw();
	}

	if (item_) {
		item_->SetScrollX(renderScrollX);
		item_->Draw();
	}

	// クイズの描画
	if (quiz_) {
		quiz_->Draw();
	}

	if (obstacles_) {
		obstacles_->SetScrollX(renderScrollX);
		obstacles_->Draw();
	}

	// 最後に非操作領域の暗転オーバーレイを描画
	if (overlaySprite_) {
		overlaySprite_->Draw();
	}

	// 時間表示
	if (timeDisplay_) {
		timeDisplay_->Draw();
	}

	if (particleManager_) {
		particleManager_->Draw(renderScrollX, renderOffsetY);
	}
	Sprite::PostDraw();
	
	fade_->Draw();
	// 障害物の描画
}