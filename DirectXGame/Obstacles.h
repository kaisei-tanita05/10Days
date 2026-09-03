#pragma once
#include <KamataEngine.h>


class Obstacles {
public:

	~Obstacles();

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
	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t obstacleTextureHandle_;
	

};
