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

	//四角の位置
	int x = GAME_WIDTH / 2;
	int y = GAME_HEIGHT / 2;

	//正負(速度)
	int i = 16;
	int j = 16;

	int speed = 1;
	int xspeed = speed;
	int yspeed = speed;

	//半径
	int radius = 100;

	//初手端かの確認
	int z = 1;

	//四角の大きさ
	int width = 32;
	int height = 32;

	SetDrawScreen(DX_SCREEN_BACK);

	while (1)
	{
		//キーが押されたとき
		if (CheckHitKeyAll() != 0)
		{
			break;
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)
		{
			break;
		}

		if (ClearDrawScreen() != 0)
			break;
		
		/*if (x >= GAME_WIDTH - width)
			i *= -1;
		if (y >= GAME_HEIGHT - height)
			j *= -1;
		if (x <= 0 && y <= 0 && z == 0)
			z = 1;
		else if (x <= 0 && y <= 0)
		{
			i *= -1;
			j *= -1;
		}
		else if (x <= 0)
			i *= -1;
		else if (y <= 0)
			j *= -1;*/

		if (x >= GAME_WIDTH - radius)
			i *= -1;
		if (y >= GAME_HEIGHT - radius)
			j *= -1;
		if (x - radius <= 0 && y - radius <= 0 && z == 0)
			z = 1;
		else if (x - radius <= 0 && y - radius <= 0)
		{
			i *= -1;
			j *= -1;
		}
		else if (x - radius <= 0)
			i *= -1;
		else if (y - radius <= 0)
			j *= -1;
		
		/*if(x < 0 || x + width > GAME_WIDTH)
		{
			xspeed = -xspeed;
			if (xspeed > 0) xspeed++;
			else if (xspeed < 0) xspeed--;
		}
		if(y < 0 || y + height > GAME_HEIGHT)
		{
			yspeed = -yspeed;
			if (yspeed > 0) yspeed++;
			else if (yspeed < 0) yspeed--;
		}*/
		

		/*DrawBox(
			x, y, x + width, y + height,
			GetColor(255, 0, 0),	//色取得
			true					//塗りつぶし
		);*/

		DrawCircle
		(
			x, y, radius,
			GetColor(0, 255, 0),
			FALSE, 5
		);
		
		x+=i;
		y+=j;
		
		/*
		x += xspeed;
		y += yspeed;
		*/
		ScreenFlip();	//ダブルバッファリングした画面を描画;
	}

	
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}