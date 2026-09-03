#include "GameScene.h" // GameSceneのヘッダーをインクルード
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化 ("ガリリとデブブ" というタイトルでウィンドウ作成)
	Initialize(L"ガリリとデブブ");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ゲームシーンの生成と初期化
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新 (ウィンドウの閉じるボタン等で脱出)
		if (Update()) {
			break;
		}

		// シーンの更新処理 (キー入力やキャラクターの移動など)
		gameScene->Update();

		// 描画開始
		dxCommon->PreDraw();

		// シーンの描画処理 (スプライトや3Dモデルの描画)
		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの破棄
	delete gameScene;
	gameScene = nullptr;

	// エンジンの終了処理
	Finalize();

	return 0;
}