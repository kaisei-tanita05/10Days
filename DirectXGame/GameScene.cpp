#include "GameScene.h"

//==================================================
// フェード時間
//==================================================

// フェードにかける時間
// 1.0f = 1秒
const float GameScene::kFadeDuration = 1.0f;

//==================================================
// デストラクタ
//==================================================

GameScene::~GameScene() {

	// 作成したスプライトを削除する
	delete titleSprite_;
	delete operationSprite_;
	delete gamePlaySprite_;
	delete gameClearSprite_;
	delete gameOverSprite_;
}

//==================================================
// 初期化
//==================================================

void GameScene::Initialize() {

	// 最初はタイトル画面から開始する
	scene_ = Scene::Title;
	nextScene_ = Scene::Title;

	//==================================================
	// フェードの初期化
	//==================================================

	// フェード用スプライトを初期化する
	fade_.Initialize();

	// 最初はフェードインから開始する
	// ゲームを起動したときに
	// 黒画面からタイトルが表示される
	fade_.Start(Fade::Status::FadeIn, kFadeDuration);

	// シーン切り替え中として扱う
	isSceneChanging_ = true;

	// 最初はフェードインなのでfalse
	isFadeOut_ = false;

	//==================================================
	// テクスチャの読み込み
	//==================================================

	// Resourcesフォルダにある画像を読み込む
	titleTextureHandle_ = TextureManager::Load("title.png");
	operationTextureHandle_ = TextureManager::Load("operation.png");
	gamePlayTextureHandle_ = TextureManager::Load("gameplay.png");
	gameClearTextureHandle_ = TextureManager::Load("gameclear.png");
	gameOverTextureHandle_ = TextureManager::Load("gameover.png");

	//==================================================
	// スプライトの作成
	//==================================================

	// 読み込んだテクスチャを使ってスプライトを作成する
	// {0, 0} は画面左上を基準にしている
	titleSprite_ = Sprite::Create(titleTextureHandle_, {0, 0});
	operationSprite_ = Sprite::Create(operationTextureHandle_, {0, 0});
	gamePlaySprite_ = Sprite::Create(gamePlayTextureHandle_, {0, 0});
	gameClearSprite_ = Sprite::Create(gameClearTextureHandle_, {0, 0});
	gameOverSprite_ = Sprite::Create(gameOverTextureHandle_, {0, 0});

	//==================================================
	// スプライトのサイズ設定
	//==================================================

	// 画面いっぱいに表示するため1280×720に設定する
	titleSprite_->SetSize({1280.0f, 720.0f});
	operationSprite_->SetSize({1280.0f, 720.0f});
	gamePlaySprite_->SetSize({1280.0f, 720.0f});
	gameClearSprite_->SetSize({1280.0f, 720.0f});
	gameOverSprite_->SetSize({1280.0f, 720.0f});

	//==================================================
	// 制限時間の初期化
	//==================================================

	// ゲーム開始時の残り時間を10秒に戻す
	gameTime_ = kGameTime;

	// タイマーのフレーム数を0に戻す
	timerFrame_ = 0;
}

//==================================================
// 更新
//==================================================

void GameScene::Update() {

	//==================================================
	// フェードの更新
	//==================================================

	// フェードの時間を進める
	fade_.Update();

	//==================================================
	// シーン切り替え中の処理
	//==================================================

	if (isSceneChanging_) {

		// フェードが終了したか確認する
		if (fade_.IsFinished()) {

			//==================================================
			// フェードアウトが終了した場合
			//==================================================

			if (isFadeOut_) {

				// 次のシーンへ切り替える
				scene_ = nextScene_;

				//==================================================
				// GamePlayに入ったときの処理
				//==================================================

				if (scene_ == Scene::GamePlay) {

					// 制限時間を10秒に戻す
					gameTime_ = kGameTime;

					// タイマーのフレーム数もリセットする
					timerFrame_ = 0;
				}

				//==================================================
				// フェードイン開始
				//==================================================

				// 黒画面から新しいシーンを表示する
				fade_.Start(Fade::Status::FadeIn, kFadeDuration);

				// フェードイン中なのでfalse
				isFadeOut_ = false;
			}

			//==================================================
			// フェードインが終了した場合
			//==================================================

			else {

				// フェードを停止する
				fade_.Stop();

				// シーン切り替えが終了した
				isSceneChanging_ = false;
			}
		}

		// シーン切り替え中は通常のシーン更新を行わない
		return;
	}

	//==================================================
	// 現在のシーンに応じて更新処理を切り替える
	//==================================================

	switch (scene_) {

	case Scene::Title:
		UpdateTitle();
		break;

	case Scene::Operation:
		UpdateOperation();
		break;

	case Scene::GamePlay:
		UpdateGamePlay();
		break;

	case Scene::GameClear:
		UpdateGameClear();
		break;

	case Scene::GameOver:
		UpdateGameOver();
		break;
	}

	//==================================================
	// シーン切り替え開始
	//==================================================

	// 現在のシーンと次のシーンが違う場合
	// フェードアウトを開始する
	if (scene_ != nextScene_) {

		// シーン切り替え中にする
		isSceneChanging_ = true;

		// フェードアウト中にする
		isFadeOut_ = true;

		// 現在の画面を黒くしていく
		fade_.Start(Fade::Status::FadeOut, kFadeDuration);
	}
}

//==================================================
// 描画
//==================================================

void GameScene::Draw() {

	//==================================================
	// 現在のシーンに応じて描画する画像を切り替える
	//==================================================

	switch (scene_) {

	case Scene::Title:
		DrawTitle();
		break;

	case Scene::Operation:
		DrawOperation();
		break;

	case Scene::GamePlay:
		DrawGamePlay();
		break;

	case Scene::GameClear:
		DrawGameClear();
		break;

	case Scene::GameOver:
		DrawGameOver();
		break;
	}

	//==================================================
	// フェードを描画
	//==================================================

	// シーンの上に黒いフェードを重ねる
	fade_.Draw();
}

//==================================================
// Title
//==================================================

void GameScene::UpdateTitle() {

	// SPACEキーを押した瞬間に操作説明画面へ移動する
	// TriggerKeyなので、キーを押している間ではなく
	// 押した瞬間に1回だけ反応する
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		nextScene_ = Scene::Operation;
	}
}

void GameScene::DrawTitle() {

	// タイトル画面の画像を描画する
	titleSprite_->Draw();
}

//==================================================
// Operation
//==================================================

void GameScene::UpdateOperation() {

	// SPACEキーを押した瞬間にゲーム本編へ移動する
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		nextScene_ = Scene::GamePlay;
	}
}

void GameScene::DrawOperation() {

	// 操作説明画面の画像を描画する
	operationSprite_->Draw();
}

//==================================================
// GamePlay
//==================================================

void GameScene::UpdateGamePlay() {

	//==================================================
	// 制限時間
	//==================================================

	// 1フレーム経過したのでカウントする
	timerFrame_++;

	// 60フレーム経過したら1秒減らす
	// ※60FPSで動作する想定
	if (timerFrame_ >= 60) {

		// 残り時間を1秒減らす
		gameTime_--;

		// フレーム数をリセットする
		timerFrame_ = 0;
	}

	//==================================================
	// 時間切れ
	//==================================================

	// 残り時間が0秒以下になったら
	// ゲームオーバー画面へ移動する
	if (gameTime_ <= 0) {
		nextScene_ = Scene::GameOver;
	}

	//==================================================
	// ゲームクリア
	//==================================================

	// Cキーを押した瞬間にゲームクリア画面へ移動する
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		nextScene_ = Scene::GameClear;
	}

	// NOTE:
	// Gキーによるゲームオーバーへのテスト遷移は追加していない
}

void GameScene::DrawGamePlay() {

	// ゲーム本編の画像を描画する
	gamePlaySprite_->Draw();
}

//==================================================
// GameClear
//==================================================

void GameScene::UpdateGameClear() {

	// SPACEキーを押した瞬間にタイトル画面へ戻る
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		nextScene_ = Scene::Title;
	}
}

void GameScene::DrawGameClear() {

	// ゲームクリア画面の画像を描画する
	gameClearSprite_->Draw();
}

//==================================================
// GameOver
//==================================================

void GameScene::UpdateGameOver() {

	// SPACEキーを押した瞬間にタイトル画面へ戻る
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		nextScene_ = Scene::Title;
	}
}

void GameScene::DrawGameOver() {

	// ゲームオーバー画面の画像を描画する
	gameOverSprite_->Draw();
}