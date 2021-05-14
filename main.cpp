//ヘッダーファイルの読み込み
#include "DxLib.h"		//DxLibを使うときに必要
#include "keyboard.h"	//キーボードの処理

#define GAME_TITLE		"ゲームタイトル"
#define GAME_WIDTH		1280
#define GAME_HEIGHT		720
#define GAME_COLOR		32
#define GAME_ICON_ID	333		//ゲームのアイコンのID
#define GAME_WINDOW_BAR	0

//列挙型
enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};	//ゲームのシーン

//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回のゲームのシーン
GAME_SCENE NextGameScene;	// 次 のゲームのシーン

//画面の切り替え
BOOL IsFadeOut = FALSE;	//フェードアウト
BOOL IsFadeIn  = FALSE;	//フェードイン

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面(処理)
VOID TitleDraw(VOID);	//タイトル画面(描画)
VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面(処理)
VOID PlayDraw(VOID);	//プレイ画面(描画)
VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面(処理)
VOID EndDraw(VOID);		//エンド画面(描画)
VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面(処理)
VOID ChangeDraw(VOID);	//切り替え画面(描画)



// プログラムは WinMain から始まります
//Windowsのプログラミング方法 = (WinAPI)で動いている
//DxLibはDirectXという、ゲームプログラミングを簡単に使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	//Log.txtを出力しない

	ChangeWindowMode(TRUE);			//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);	//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//ウィンドウの解像度を設定

	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//ウィンドウの大きさを設定

	SetBackgroundColor(255, 255, 255);
	//デフォルトの背景の色

	SetWindowIconID(GAME_ICON_ID);
	//アイコンファイルを読み込み

	SetWindowStyleMode(GAME_WINDOW_BAR);
	//ウィンドウバーの状態

	SetWaitVSyncFlag(TRUE);
	//ディスプレイの垂直同期を有効にする

	SetAlwaysRunFlag(TRUE);
	//ウィンドウをずっとアクティブにする

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	
	//DrawPixel(320, 240, GetColor(255, 255, 255));	// 点を打つ

	//WaitKey();				// キー入力待ち

	SetDrawScreen(DX_SCREEN_BACK);

	//円の中心点
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	//円の半径
	int radius = 50;

	//速度
	int spd = 4;

	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	while (1)
	{
		//キーが押されたとき
		/*if (CheckHitKeyAll() != 0) { break; }*/

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0) { break; }

		//画面消去
		if (ClearDrawScreen() != 0) break;
		
		//キーボード入力の更新
		AllKeyUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) break;

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
			OldGameScene = GameScene;

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}

		//キー入力
		if (KeyDown(KEY_INPUT_UP) == TRUE)
			Y -= spd;
		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
			Y += spd;
		if (KeyDown(KEY_INPUT_LEFT) == TRUE)
			X -= spd;
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
			X += spd;

		if (X < radius)
			X = radius;
		if (Y < radius)
			Y = radius;
		if (X > GAME_WIDTH - radius)
			X = GAME_WIDTH - radius;
		if (Y > GAME_HEIGHT - radius)
			Y = GAME_HEIGHT - radius;

		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);
		
		ScreenFlip();	//ダブルバッファリングした画面を描画;
	}

	
	//DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{
	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//処理
	PlayDraw();	//描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();	//処理
	EndDraw();	//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//処理
	ChangeDraw();	//描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{
	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

