#include "TitleScene.h"

using namespace KamataEngine;

TitleScene::~TitleScene() {

	delete sprite_;
	sprite_ = nullptr;
}

void TitleScene::Initialize() {

	textureHandle_ = TextureManager::Load("Title/title.png");

	sprite_ = Sprite::Create(textureHandle_, {0.0f, 0.0f});
}

void TitleScene::Update() {

	// 今はSPACEによるシーン切り替えはmain側で行う
}

void TitleScene::Draw() {

	if (sprite_) {
		sprite_->Draw();
	}
}