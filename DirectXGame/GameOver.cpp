#include "GameOver.h"

using namespace KamataEngine;

GameOver::~GameOver() {

	delete sprite_;
	sprite_ = nullptr;
	delete fade_;
}

void GameOver::Initialize() {

	textureHandle_ = TextureManager::Load("Background/GameOver.png");

	sprite_ = Sprite::Create(textureHandle_, {0.0f, 0.0f});

	BGMHandle_ = Audio::GetInstance()->LoadWave("Sound/BGM/GameOverBGM.mp3");
	Audio::GetInstance()->PlayWave(BGMHandle_, true, 1.0f);


	SEHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/choice.mp3");

	fade_ = new Fade();
	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 2.0f);
}

void GameOver::Update() {

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
			Audio::GetInstance()->PlayWave(SEHandle_, false, 5.0f);
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;

	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			Audio::GetInstance()->StopWave(BGMHandle_);
			finished_ = true;
		}
		break;
	}
}

void GameOver::Draw() {

	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	if (sprite_) {
		sprite_->Draw();
	}

	Sprite::PostDraw();
	fade_->Draw();
}