#include "Particle.h"
#include <cstdlib>

using namespace KamataEngine;

void ParticleManager::Initialize(uint32_t textureHandle) {
	textureHandle_ = textureHandle;
	// 汎用スプライト生成（アンカーポイントを中心に設定）
	sprite_ = Sprite::Create(textureHandle_, {0.0f, 0.0f});
	sprite_->SetAnchorPoint({0.5f, 0.5f});
}

void ParticleManager::SpawnBurst(const Vector2& position, int count) {
	for (int i = 0; i < count; ++i) {
		Particle p;
		p.position = position;

		// X方向: -8.0 ～ 8.0 のランダムな速度（左右に飛び散る）
		float vx = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 8.0f;
		// Y方向: -12.0 ～ -3.0 のランダムな速度（上に跳ね上がる）
		float vy = -((float)rand() / RAND_MAX * 9.0f + 3.0f);
		p.velocity = {vx, vy};

		// 色（白～薄いグレー、ランダム感）
		p.color = {1.0f, 1.0f, 1.0f, 1.0f};

		// サイズ: 8px ～ 20px
		p.size = (float)rand() / RAND_MAX * 12.0f + 8.0f;

		// 生存時間: 0.3 ～ 0.6 秒
		p.maxLifetime = (float)rand() / RAND_MAX * 0.3f + 0.3f;
		p.lifetime = p.maxLifetime;

		particles_.push_back(p);
	}
}

void ParticleManager::Update(float deltaTime) {
	const float gravity = 0.8f; // 重力加速度

	for (auto it = particles_.begin(); it != particles_.end();) {
		it->lifetime -= deltaTime;

		if (it->lifetime <= 0.0f) {
			// 寿命切れで削除
			it = particles_.erase(it);
		} else {
			// 物理移動（速度加算 ＋ 重力影響）
			it->position.x += it->velocity.x;
			it->position.y += it->velocity.y;
			it->velocity.y += gravity; // 徐々に落下

			// 時間経過に伴うフェードアウト
			it->color.w = it->lifetime / it->maxLifetime;

			++it;
		}
	}
}

void ParticleManager::Draw(float scrollX, float offsetY) {
	if (!sprite_)
		return;

	for (const auto& p : particles_) {
		// ワールド座標からスクロール量を引き算して画面描画座標へ
		Vector2 screenPos = {p.position.x - scrollX, p.position.y + offsetY};

		sprite_->SetPosition(screenPos);
		sprite_->SetSize({p.size, p.size});
		sprite_->SetColor(p.color);
		sprite_->Draw();
	}
}