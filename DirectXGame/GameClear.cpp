#include "GameClear.h"

using namespace KamataEngine;

GameClear::~GameClear() {

	delete sprite_;
	sprite_ = nullptr;
}

void GameClear::Initialize() {

	textureHandle_ = TextureManager::Load("Background/GameClear.png");

	sprite_ = Sprite::Create(textureHandle_, {0.0f, 0.0f});
}

void GameClear::Update() {}

void GameClear::Draw() {
	
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	if (sprite_) {
		sprite_->Draw();
	}

	Sprite::PostDraw();
}