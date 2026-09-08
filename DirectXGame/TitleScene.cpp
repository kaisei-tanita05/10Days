#include "TitleScene.h"

using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete fade_;
	delete sprite_;
	sprite_ = nullptr;
}

void TitleScene::Initialize() {

	textureHandle_ = TextureManager::Load("Background/Title.png");

	sprite_ = Sprite::Create(textureHandle_, {0.0f, 0.0f});

	BGMHandle_ = Audio::GetInstance()->LoadWave("Sound/BGM/TitleSceneBGM.mp3");
	Audio::GetInstance()->PlayWave(BGMHandle_, true, 1.0f);

	fade_ = new Fade();
	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 2.0f);
}

void TitleScene::Update() {

	// 今はSPACEによるシーン切り替えはmain側で行う
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;

	case Phase::kMain:

		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
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

void TitleScene::Draw() {

	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	if (sprite_) {
		sprite_->Draw();
	}

	Sprite::PostDraw();
	fade_->Draw();

}