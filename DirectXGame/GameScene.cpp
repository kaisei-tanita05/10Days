#include "GameScene.h"

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
}

//==================================================
// 更新
//==================================================

void GameScene::Update() {

	// 現在のシーンに応じて更新処理を切り替える
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

	// Update○○()で決めた次のシーンへ移動する
	if (scene_ != nextScene_) {
		scene_ = nextScene_;
	}
}

//==================================================
// 描画
//==================================================

void GameScene::Draw() {

	// 現在のシーンに応じて描画する画像を切り替える
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

	// Cキーを押した瞬間にゲームクリア画面へ移動する
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		nextScene_ = Scene::GameClear;
	}

	// Gキーを押した瞬間にゲームオーバー画面へ移動する
	if (Input::GetInstance()->TriggerKey(DIK_G)) {
		nextScene_ = Scene::GameOver;
	}
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