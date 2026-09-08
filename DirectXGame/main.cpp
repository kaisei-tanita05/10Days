#include "GameClear.h"
#include "GameOver.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "Operate.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

enum class Scene { kUnknown, kTitle, kOperate, kGame, kGameClear, kGameOver };

// シーン
Scene scene = Scene::kUnknown;

// 各シーン
TitleScene* titleScene = nullptr;
Operate* operateScene = nullptr;
GameScene* gameScene = nullptr;
GameClear* gameClearScene = nullptr;
GameOver* gameOverScene = nullptr;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// TitleSceneを削除
			delete titleScene;
			titleScene = nullptr;

			// OperateSceneを生成
			operateScene = new Operate();
			operateScene->Initialize();

			// シーン変更
			scene = Scene::kOperate;
		}

		break;

	case Scene::kOperate:
		if (operateScene->IsFinished()) {
			delete operateScene;
			operateScene = nullptr;
			gameScene = new GameScene();
			gameScene->Initialize();
			scene = Scene::kGame;
		}

		break;

	case Scene::kGame:
		if (gameScene->IsFinished()) {

			// GameSceneを削除
			delete gameScene;
			gameScene = nullptr;

			// GameClearを生成
			gameClearScene = new GameClear();
			gameClearScene->Initialize();

			// シーン変更
			scene = Scene::kGameClear;
		}

		break;
	case Scene::kGameClear:
		break;

	case Scene::kGameOver:
		
		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene) {
			titleScene->Update();
		}
		break;

	case Scene::kOperate:
		if (operateScene) {
			operateScene->Update();
		}
		break;

	case Scene::kGame:
		if (gameScene) {
			gameScene->Update();
		}
		break;

	case Scene::kGameClear:
		if (gameClearScene) {
			gameClearScene->Update();
		}
		break;

	case Scene::kGameOver:
		if (gameOverScene) {
			gameOverScene->Update();
		}
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kOperate:
		operateScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	case Scene::kGameClear:
		gameClearScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"3065_ガリリとデブブ");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		// シーン切り替え
		ChangeScene();

		// 現在シーン更新
		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();

		DrawScene();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	titleScene = nullptr;
	delete gameScene;
	gameScene = nullptr;
	delete gameOverScene;
	gameOverScene = nullptr;
	delete gameClearScene;
	gameClearScene = nullptr;
	delete operateScene;
	operateScene = nullptr;
	// エンジンの終了処理
	Finalize();

	return 0;
}