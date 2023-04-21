#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

GameScene::GameScene() {
	delete sprite_;
	delete model_;
	delete DebugCamera_;
}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ライン描画
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);


	//デバックカメラ
	DebugCamera_ = new DebugCamera
	(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸表示
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection
	(&DebugCamera_->GetViewProjection());

	// 読み込み処理
	textureHandle_ = TextureManager::Load("sample.png");
	// 生成
	sprite_ = Sprite::Create(textureHandle_, {10, 10});

	// モデルの画像読み込み
	modeltextureHandle_ = TextureManager::Load("sample.png");
	// 生成
	model_ = Model::Create();

	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// ビュープロジェクション初期化
	viewProjection_.Initialize();

	//音声の読み込み
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	//
	audio_->PlayWave(soundDataHandle_, true);
}

void GameScene::Update() {
	Vector2 pos = sprite_->GetPosition();
	pos.x += 1;
	pos.y += 1;
	sprite_->SetPosition(pos);

	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(soundDataHandle_);
	}

	ImGui::ShowDemoWindow();

	ImGui::Begin("Test");
	ImGui::Text("Kamata Tarou %d.%d.%d", 2023, 10, 23);
	ImGui::InputFloat3("inputFloat3", inputFloat);
	ImGui::SliderFloat3("SliderFloat3", inputFloat, 0.0f, 1.0f);
	ImGui::End();

	// デバックカメラ
	DebugCamera_->Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	//　スプライト
	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// モデル
	model_->Draw(worldTransform_, DebugCamera_->GetViewProjection(), textureHandle_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	//ライン描画
	PrimitiveDrawer::GetInstance()->DrawLine3d
	({0,0,0,},{0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
