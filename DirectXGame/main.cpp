#include <Windows.h>

#include "GameScene.h"
#include "KamataEngine.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"ガリリとデブブ");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// GameScene
	GameScene gameScene;

	// GameSceneの初期化
	gameScene.Initialize();

	// メインループ
	while (true) {

		// エンジンの更新
		if (Update()) {
			break;
		}

		// GameSceneの更新
		gameScene.Update();

		// 描画開始
		dxCommon->PreDraw();

		// スプライトの描画前処理
		Sprite::PreDraw();

		// GameSceneの描画
		gameScene.Draw();

		// スプライトの描画後処理
		Sprite::PostDraw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// エンジンの終了処理
	Finalize();

	return 0;
}