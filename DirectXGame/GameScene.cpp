#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	for (int i = 0; i < 4; i++) {
		delete sprites_[i];
		sprites_[i] = nullptr;
	}
	delete obstacles_;
	obstacles_ = nullptr;
}

void GameScene::Initialize() {
	// 背景テクスチャの読み込み
	// スプライトの生成
	for (int i = 0; i < 4; i++) {
		bgTextureHandle_[i] = TextureManager::Load("BackGround/backGround.png");
		// 1280pxずつ横に並べる
		sprites_[i] = Sprite::Create(bgTextureHandle_[i], Vector2(1280.0f * i, 0.0f));
	}

	// 障害物の初期化
	obstacles_ = new Obstacles();
	obstacles_->Initialize();
}

void GameScene::Update() {
	// 背景を右に移動
	//sprite_->SetPosition({sprite_->GetPosition().x - bgSpeed_, 0});

	//sprite2_->SetPosition({sprite2_->GetPosition().x - bgSpeed_, 0});

	//// sprite_ が左に消えたら
	//// sprite2_ の右側へ
	//if (sprite_->GetPosition().x <= -1280.0f) {
	//	sprite_->SetPosition({sprite2_->GetPosition().x + 1280.0f, 0});
	//}

	//// sprite2_ が左に消えたら
	//// sprite_ の右側へ
	//if (sprite2_->GetPosition().x <= -1280.0f) {
	//	sprite2_->SetPosition({sprite_->GetPosition().x + 1280.0f, 0});
	//}

	// スクロール量を増やす
	scrollX_ += bgSpeed_;

	if (scrollX_ >= 1280.0f * 4.0f) {
		scrollX_ -= 1280.0f * 4.0f;
	}

	// 障害物の更新
	// 障害物の更新
	if (obstacles_) {
		obstacles_->Update();
	}
}

void GameScene::Draw() {
	// 背景スプライトの描画
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	// 背景
	for (int i = 0; i < 4; i++) {

		float x = (1280.0f * i) - scrollX_;

		sprites_[i]->SetPosition({x, 0.0f});

		sprites_[i]->Draw();
	}
	//sprite2_->Draw();

	Sprite::PostDraw();
	// 障害物の描画
	obstacles_->Draw();
}