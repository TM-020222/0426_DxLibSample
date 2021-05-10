//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使うときに必要
#define GAME_TITLE "ゲームタイトル"
#define GAME_WIDTH 1280
#define GAME_HEIGHT 720
#define GAME_COLOR 32
#define GAME_ICON_ID 333		//ゲームのアイコンのID
#define GAME_WINDOW_BAR 0

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

	while (1)
	{
		//キーが押されたとき
		/*if (CheckHitKeyAll() != 0)
		{
			break;
		}*/

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0) { break; }

		//画面消去
		if (ClearDrawScreen() != 0)	break;
		
		
		ScreenFlip();	//ダブルバッファリングした画面を描画;
	}

	
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}