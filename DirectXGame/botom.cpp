#include "Botom.h"

using namespace KamataEngine;

//==================================================
// デストラクタ
//==================================================

Botom::~Botom() {

	delete buttonSprite_;
	delete doorSprite_;
	delete brokenDoorSprite_;
	delete bottomDoorSprite_;
}

//==================================================
// 初期化
//==================================================

void Botom::Initialize(const Vector2& position) {

	//==================================================
	// 下側：ボタン
	//==================================================

	// ボタンの位置を保存
	worldPosition_ = position;

	// ボタン画像を読み込む
	buttonTextureHandle_ = TextureManager::Load("button.png");

	// ボタンのスプライトを作成
	buttonSprite_ = Sprite::Create(buttonTextureHandle_, worldPosition_);

	// ボタンのサイズ
	buttonSprite_->SetSize({kButtonWidth, kButtonHeight});

	//==================================================
	// 上側：デブブ用の扉
	//==================================================

	// 上の扉を配置
	// 上階の床はY=320なので、
	// 上側：デブブ用の扉
	doorPosition_ = {position.x + 300.0f, 80.0f};

	// 通常の扉画像
	doorTextureHandle_ = TextureManager::Load("door.png");

	// 壊れた扉画像
	brokenDoorTextureHandle_ = TextureManager::Load("brokenDoor.png");

	// 通常の扉を作成
	doorSprite_ = Sprite::Create(doorTextureHandle_, doorPosition_);

	// 壊れた扉を作成
	brokenDoorSprite_ = Sprite::Create(brokenDoorTextureHandle_, doorPosition_);

	// 扉を2倍の大きさにする
	doorSprite_->SetSize({kDoorWidth, kDoorHeight});

	// 壊れた扉も同じ大きさにする
	brokenDoorSprite_->SetSize({kDoorWidth, kDoorHeight});

	//==================================================
	// 下側：ガリリ用の扉
	//==================================================

	// 下の扉を配置
	// 下階の床はY=670なので、
	// 下側：ガリリ用の扉
	bottomDoorPosition_ = {position.x + 300.0f, 430.0f};

	// 下の扉を作成
	bottomDoorSprite_ = Sprite::Create(doorTextureHandle_, bottomDoorPosition_);

	// 下の扉も2倍サイズ
	bottomDoorSprite_->SetSize({kDoorWidth, kDoorHeight});
}

//==================================================
// 更新
//==================================================

void Botom::Update() {

	//==================================================
	// 下側：ボタン
	//==================================================

	if (buttonSprite_) {

		// ワールド座標から画面座標に変換
		float screenX = worldPosition_.x - scrollX_;

		buttonSprite_->SetPosition({screenX, worldPosition_.y});
	}

	//==================================================
	// 上側：デブブ用の扉
	//==================================================

	if (doorSprite_) {

		float screenX = doorPosition_.x - scrollX_;

		doorSprite_->SetPosition({screenX, doorPosition_.y});
	}

	if (brokenDoorSprite_) {

		float screenX = doorPosition_.x - scrollX_;

		brokenDoorSprite_->SetPosition({screenX, doorPosition_.y});
	}

	//==================================================
	// 下側：ガリリ用の扉
	//==================================================

	if (bottomDoorSprite_) {

		float screenX = bottomDoorPosition_.x - scrollX_;

		bottomDoorSprite_->SetPosition({screenX, bottomDoorPosition_.y});
	}
}

//==================================================
// スクロール
//==================================================

void Botom::SetScrollX(float scrollX) { scrollX_ = scrollX; }

//==================================================
// 下側：ボタンの位置を取得
//==================================================

const Vector2& Botom::GetPosition() const { return worldPosition_; }

//==================================================
// 下側：ボタンの当たり判定
//==================================================

bool Botom::IsCollision(const Vector2& playerPosition, float playerWidth, float playerHeight) const {

	// プレイヤーの範囲
	float playerLeft = playerPosition.x;

	float playerRight = playerPosition.x + playerWidth;

	float playerTop = playerPosition.y;

	float playerBottom = playerPosition.y + playerHeight;

	// ボタンの範囲
	float buttonLeft = worldPosition_.x;

	float buttonRight = worldPosition_.x + kButtonWidth;

	float buttonTop = worldPosition_.y;

	float buttonBottom = worldPosition_.y + kButtonHeight;

	// AABBによる当たり判定
	if (playerRight > buttonLeft && playerLeft < buttonRight && playerBottom > buttonTop && playerTop < buttonBottom) {

		return true;
	}

	return false;
}

//==================================================
// 下側：ボタンを押す
//==================================================

void Botom::Push() {

	// すでに押されていたら何もしない
	if (isPressed_) {
		return;
	}

	// ボタンを押した状態にする
	isPressed_ = true;

	// 下の扉だけを開く
	isBottomDoorOpened_ = true;
}

//==================================================
// 上側：扉の位置を取得
//==================================================

const Vector2& Botom::GetDoorPosition() const { return doorPosition_; }

//==================================================
// 上側：デブブ用の扉の当たり判定
//==================================================

bool Botom::IsDoorCollision(const Vector2& playerPosition, float playerWidth, float playerHeight) const {

	// すでに壊れていたら当たり判定なし
	if (isDoorBroken_) {
		return false;
	}

	// プレイヤーの範囲
	float playerLeft = playerPosition.x;

	float playerRight = playerPosition.x + playerWidth;

	float playerTop = playerPosition.y;

	float playerBottom = playerPosition.y + playerHeight;

	// 扉の範囲
	float doorLeft = doorPosition_.x;

	float doorRight = doorPosition_.x + kDoorWidth;

	float doorTop = doorPosition_.y;

	float doorBottom = doorPosition_.y + kDoorHeight;

	// AABBによる当たり判定
	if (playerRight > doorLeft && playerLeft < doorRight && playerBottom > doorTop && playerTop < doorBottom) {

		return true;
	}

	return false;
}

//==================================================
// 上側：デブブが扉を破壊
//==================================================

void Botom::BreakDoor() {

	// すでに壊れていたら何もしない
	if (isDoorBroken_) {
		return;
	}

	// 扉を破壊
	isDoorBroken_ = true;
}

//==================================================
// 下側：扉の位置を取得
//==================================================

const Vector2& Botom::GetBottomDoorPosition() const { return bottomDoorPosition_; }

//==================================================
// 下側：ガリリ用の扉の当たり判定
//==================================================

bool Botom::IsBottomDoorCollision(const Vector2& playerPosition, float playerWidth, float playerHeight) const {

	// ボタンで開いていたら当たり判定なし
	if (isBottomDoorOpened_) {
		return false;
	}

	// プレイヤーの範囲
	float playerLeft = playerPosition.x;

	float playerRight = playerPosition.x + playerWidth;

	float playerTop = playerPosition.y;

	float playerBottom = playerPosition.y + playerHeight;

	// 下の扉の範囲
	float doorLeft = bottomDoorPosition_.x;

	float doorRight = bottomDoorPosition_.x + kDoorWidth;

	float doorTop = bottomDoorPosition_.y;

	float doorBottom = bottomDoorPosition_.y + kDoorHeight;

	// AABBによる当たり判定
	if (playerRight > doorLeft && playerLeft < doorRight && playerBottom > doorTop && playerTop < doorBottom) {

		return true;
	}

	return false;
}

//==================================================
// 描画
//==================================================

void Botom::Draw() {

	//==================================================
	// 下側：ボタン
	//==================================================

	// 押されていないときだけ表示
	if (!isPressed_) {

		if (buttonSprite_) {
			buttonSprite_->Draw();
		}
	}

	//==================================================
	// 上側：デブブ用の扉
	//==================================================

	// 壊れている場合
	if (isDoorBroken_) {

		if (brokenDoorSprite_) {
			brokenDoorSprite_->Draw();
		}

	} else {

		// 通常の扉
		if (doorSprite_) {
			doorSprite_->Draw();
		}
	}

	//==================================================
	// 下側：ガリリ用の扉
	//==================================================

	// ボタンが押されていない場合だけ表示
	if (!isBottomDoorOpened_) {

		if (bottomDoorSprite_) {
			bottomDoorSprite_->Draw();
		}
	}
}