#include "GameClear.h"

using namespace KamataEngine;

GameClear::~GameClear() {

	delete sprite_;
	sprite_ = nullptr;
}

void GameClear::Initialize() {

	textureHandle_ = TextureManager::Load("Background/GameClear.png");

	sprite_ = Sprite::Create(textureHandle_, {0.0f, 0.0f});

	fade_ = new Fade();
	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 1.0f);
	phase_ = Phase::kFadeIn;

	BGMHandle_ = Audio::GetInstance()->LoadWave("Sound/BGM/GameClearBGM.mp3");

	Audio::GetInstance()->PlayWave(BGMHandle_, true, 1.0f);
}

void GameClear::Update() {
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;

	case Phase::kMain:
		// メイン処理
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;

	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {

			Audio::GetInstance()->StopWave(BGMHandle_);
			isFinished_ = true;
		}
		break;
	}


}

void GameClear::Draw() {
	
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	if (sprite_) {
		sprite_->Draw();
	}

	Sprite::PostDraw();

	fade_->Draw();
}