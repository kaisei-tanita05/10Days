#include "Obstacles.h"

using namespace KamataEngine;

Obstacles::~Obstacles() { delete sprite_; }

void Obstacles::Initialize() {
	// 障害物テクスチャの読み込み
	obstacleTextureHandle_ = TextureManager::Load("Obstacles/bottle.png");
	// スプライトの生成
	sprite_ = Sprite::Create(obstacleTextureHandle_, Vector2(1100, 320));
}

void Obstacles::Update() {
	
}

void Obstacles::Draw() {
	// 障害物スプライトの描画
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();
}