#pragma once
#include <KamataEngine.h>
#include "Obstacles.h"

class GameScene {
public:

	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	private:

	KamataEngine::Sprite* sprites_[4] = {};

	
	uint32_t bgTextureHandle_[4];

	// 背景のスクロール量
	float scrollX_ = 0.0f;

	// 背景の移動速度
	float bgSpeed_ = 2.0f;

	Obstacles* obstacles_ = nullptr;
};
