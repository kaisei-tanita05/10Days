#pragma once
#include "KamataEngine.h"
#include <vector>

// 1つの破片（パーティクル）のデータ構造
struct Particle {
	KamataEngine::Vector2 position; // 座標
	KamataEngine::Vector2 velocity; // 速度
	KamataEngine::Vector4 color;    // 色 (RGBA)
	float size;                     // サイズ
	float lifetime;                 // 生存時間（秒）
	float maxLifetime;              // 最大生存時間
};

// パーティクル管理クラス
class ParticleManager {
public:
	void Initialize(uint32_t textureHandle);
	void Update(float deltaTime = 1.0f / 60.0f);
	void Draw(float scrollX, float offsetY = 0.0f);

	// 破片を拡散生成する関数
	void SpawnBurst(const KamataEngine::Vector2& position, int count = 20);

private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	std::vector<Particle> particles_;
};