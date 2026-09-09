#include "Operate.h"

using namespace KamataEngine;


Operate::~Operate() {

	delete sprite_;
	sprite_ = nullptr;
	delete fade_;
	fade_ = nullptr;
}

void Operate::Initialize() {
	fade_ = new Fade();
	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 3.0f);

	textureHandle_ = TextureManager::Load("BackGround/operation.png");

	sprite_ = Sprite::Create(textureHandle_, {0.0f, 0.0f});

	SEHandle_ = Audio::GetInstance()->LoadWave("Sound/SE/choice.mp3");
}


void Operate::Update() {
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
			Audio::GetInstance()->PlayWave(SEHandle_, false, 3.0f);
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

void Operate::Draw() { 

	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	if (sprite_) {
		sprite_->Draw();
	}

	Sprite::PostDraw();

	fade_->Draw();
}