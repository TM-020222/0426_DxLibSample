#define _CRT_SECURE_NO_WARNINGS
//ヘッダファイル読み込み
#include"game.h"		//ゲーム全体のヘッダファイル
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

//動画の構造体
struct MOVIE
{
	int handle = -1;		//動画のハンドル(管理番号)
	char path[255];			//動画の場所(パス)

	int x;					//X位置
	int y;					//Y位置
	int width;				//幅
	int height;				//高さ

	int volume = 255;		//ボリューム 0-255

	//BOOL IsDraw = FALSE;	//動画が描画できる？
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;		//動画のハンドル(管理番号)
	char path[255];			//動画の場所(パス)

	int volume = -1;		//ボリューム 0-255
	int playtype = -1;		//BGM or SE
};

//画像の構造体
struct IMAGE
{
	int handle = -1;		//画像のハンドル(管理番号)
	char path[255];			//画像の場所(パス)
	int x;					//X位置
	int y;					//Y位置
	int width;				//幅
	int height;				//高さ

	BOOL IsDraw = FALSE;	//画像が描画できる？
};

//キャラクタの構造体
typedef struct CHARACTOR
{
	IMAGE img;

	int speed = 1;			//移動速度

	RECT coll;				//当たり判定の領域(四角)
}CHARA;

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回のゲームのシーン
GAME_SCENE NextGameScene;	// 次 のゲームのシーン

//プレイヤー
CHARA player;

//ゴール
CHARA goal;

//画像を読み込む
IMAGE titlelogo;
IMAGE titleenter;
IMAGE endclear;

//音楽
AUDIO titleBGM;
AUDIO playBGM;
AUDIO endBGM;

//効果音
AUDIO playerSE;

//プレイ画面の背景
MOVIE playmovie;

//画面の切り替え
BOOL IsFadeOut = FALSE;	//フェードアウト
BOOL IsFadeIn  = FALSE;	//フェードイン

//シーン切り替え
int fadeTimeMill = 2000;						//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードインのカウンタ
int fadeInCntMax = 0;				//フェードインのカウンタMAX 0?

//TitleLogoのフェードイン
int TitleLogoCnt = 0;
const int TitleLogoCntMax = 60;
BOOL TitleLogoIn = FALSE;

//PushEnterの点滅
int PushEnterCnt = 0;			//カウンタ
const int PushEnterCntMax = 60;		//カウンタMAX
BOOL PushEnterBrink = FALSE;	//点滅しているか

//EndClearのフェードイン
int EndClearCnt = 0;
const int EndClearCntMax = 60;
BOOL EndClearIn = FALSE;

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

BOOL GameLoad(VOID);	//ゲームデータのロード

//音楽のロード
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playtype);

//画像のロード
BOOL LoadImg(IMAGE* image, const char* path);

VOID GameInit(VOID);	//ゲームの初期化

VOID CollUpdatePlayer(CHARA* chara);	//当たり判定の領域を更新
VOID CollUpdateGoal(CHARA* chara);		//当たり判定の領域を更新

BOOL CubeCollision(RECT A, RECT B);		//矩形と矩形の当たり判定

VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

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

	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//ゲームの読み込み
	if (!GameLoad())
	{
		//データの読み込みに失敗したとき
		DxLib_End();	//DxLib終了
		return -1;		//強制終了
	}

	if (!LoadAudio(
		&titleBGM,
		".\\audio\\雪空.mp3",
		255,
		DX_PLAYTYPE_LOOP))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadAudio(
		&playBGM,
		".\\audio\\夢見る憧憬.mp3",
		255,
		DX_PLAYTYPE_LOOP))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadAudio(
		&endBGM,
		".\\audio\\NOIR.mp3",
		255,
		DX_PLAYTYPE_LOOP))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadAudio(
		&playerSE,
		".\\audio\\SE\\足音.mp4",
		255,
		DX_PLAYTYPE_BACK))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadImg(&titlelogo,".\\image\\Arknights_logo.png"))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadImg(&titleenter,".\\image\\PushEnter.png"))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadImg(&endclear,".\\image\\GameClear.png"))
	{
		DxLib_End();
		return -1;
	}



	//ゲームの初期化
	GameInit();

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

		//FPS値の更新
		FPSUpdate();

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

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//FPSを描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();
		
		ScreenFlip();	//ダブルバッファリングした画面を描画;
	}

	//終わるときの処理
	DeleteGraph(player.img.handle);			//画像をメモリ上から削除
	DeleteGraph(goal.img.handle);			//画像をメモリ上から削除
	DeleteGraph(playmovie.handle);		//動画をメモリ上から削除
	
	DeleteGraph(titlelogo.handle);			//画像をメモリ上から削除
	DeleteGraph(titleenter.handle);			//画像をメモリ上から削除
	DeleteGraph(endclear.handle);			//画像をメモリ上から削除

	DeleteSoundMem(titleBGM.handle);	//音楽をメモリ上から削除
	DeleteSoundMem(playBGM.handle);		//音楽をメモリ上から削除
	DeleteSoundMem(endBGM.handle);		//音楽をメモリ上から削除

	DeleteSoundMem(playerSE.handle);	//音楽をメモリ上から削除

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

/// <summary>
/// ゲームデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE,読み込めなかったらFALSE</returns>
BOOL GameLoad(VOID)
{
	//プレイヤーの画像を読み込み
	strcpyDx(player.img.path, TEXT(".\\image\\player.png"));
	player.img.handle = LoadGraph(player.img.path);	//画像の読み込み
	//ゴールの画像を読み込み
	strcpyDx(goal.img.path, TEXT(".\\image\\goal.png"));
	goal.img.handle = LoadGraph(goal.img.path);	//画像の読み込み
	//プレイ動画の背景読み込み
	strcpyDx(playmovie.path, TEXT(".\\movie\\playmovie.mp4"));
	playmovie.handle = LoadGraph(playmovie.path);	//動画の読み込み
	////タイトル音楽の読み込み
	//strcpyDx(titleBGM.path, TEXT(".\\audio\\雪空.mp3"));
	//titleBGM.handle = LoadSoundMem(titleBGM.path);	//動画の読み込み
	////プレイ音楽の読み込み
	//strcpyDx(playBGM.path, TEXT(".\\audio\\夢見る憧憬.mp3"));
	//playBGM.handle = LoadSoundMem(playBGM.path);	//動画の読み込み
	////エンド音楽の読み込み
	//strcpyDx(endBGM.path, TEXT(".\\audio\\NOIR.mp3"));
	//endBGM.handle = LoadSoundMem(endBGM.path);	//動画の読み込み

	if (player.img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メッセージのウィンドウハンドル
			TEXT(player.img.path),				//メッセージ本文
			TEXT("画像読み込みエラー！"),	//メッセージタイトル
			MB_OK);							//ボタン

		return FALSE;
	}
	if (goal.img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メッセージのウィンドウハンドル
			TEXT(goal.img.path),				//メッセージ本文
			TEXT("画像読み込みエラー！"),	//メッセージタイトル
			MB_OK);							//ボタン

		return FALSE;
	}
	if (playmovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メッセージのウィンドウハンドル
			TEXT(playmovie.path),				//メッセージ本文
			TEXT("動画読み込みエラー！"),	//メッセージタイトル
			MB_OK);							//ボタン

		return FALSE;
	}
	//if (titleBGM.handle == -1)
	//{
	//	MessageBox(
	//		GetMainWindowHandle(),			//メッセージのウィンドウハンドル
	//		TEXT(titleBGM.path),				//メッセージ本文
	//		TEXT("動画読み込みエラー！"),	//メッセージタイトル
	//		MB_OK);							//ボタン
	//
	//	return FALSE;
	//}
	//if (playBGM.handle == -1)
	//{
	//	MessageBox(
	//		GetMainWindowHandle(),			//メッセージのウィンドウハンドル
	//		TEXT(playBGM.path),				//メッセージ本文
	//		TEXT("動画読み込みエラー！"),	//メッセージタイトル
	//		MB_OK);							//ボタン
	//
	//	return FALSE;
	//}
	//if (endBGM.handle == -1)
	//{
	//	MessageBox(
	//		GetMainWindowHandle(),			//メッセージのウィンドウハンドル
	//		TEXT(endBGM.path),				//メッセージ本文
	//		TEXT("動画読み込みエラー！"),	//メッセージタイトル
	//		MB_OK);							//ボタン
	//
	//	return FALSE;
	//}
	//
	//titleBGM.playtype = DX_PLAYTYPE_LOOP;
	//titleBGM.volume = 255;
	//
	//playBGM.playtype = DX_PLAYTYPE_LOOP;
	//playBGM.volume = 255;
	//
	//endBGM.playtype = DX_PLAYTYPE_LOOP;
	//endBGM.volume = 255;

	//画像の幅と高さを取得
	GetGraphSize(player.img.handle, &player.img.width, &player.img.height);
	//画像の幅と高さを取得
	GetGraphSize(goal.img.handle, &goal.img.width, &goal.img.height);
	//動画の幅と高さを取得
	GetGraphSize(playmovie.handle, &playmovie.width, &playmovie.height);

	return TRUE;
}

/// <summary>
/// 音楽をメモリに読み込み
/// </summary>
/// <param name="audio">Audio構造体変数のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playtype">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playtype)
{
	//音楽の読み込み
	strcpyDx(audio->path, path);					//パスのコピー
	audio->handle = LoadSoundMem(audio->path);	//音楽の読み込み

	//音楽が読み込めなかった時は、エラーが入る
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メインのウィンドウハンドル
			audio->path,					//メッセージ本文
			TEXT("音楽読み込みエラー！"),	//メッセージタイトル
			MB_OK							//ボタン
		);

		return FALSE;
	}
	//その他の設定
	audio->volume = volume;
	audio->playtype = playtype;

	return TRUE;
}

/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name=""></param>
BOOL LoadImg(IMAGE* image, const char* path)
{
	//画像の読み込み
	strcpyDx(image->path, path);					//パスのコピー
	image->handle = LoadGraph(image->path);	//音楽の読み込み

	//画像が読み込めなかった時は、エラーが入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メインのウィンドウハンドル
			image->path,					//メッセージ本文
			TEXT("画像読み込みエラー！"),	//メッセージタイトル
			MB_OK							//ボタン
		);

		return FALSE;
	}
	//その他の設定
	//画像の幅と高さを取得
	GetGraphSize(image->handle, &image->width, &image->height);

	return TRUE;
}



/// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//プレイヤーを初期化
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;		//中央寄せ
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;		//中央寄せ
	player.speed = 500;									//移動速度
	player.img.IsDraw = TRUE;								//描画できる

	//ゴールを初期化
	goal.img.x = GAME_WIDTH - goal.img.width;				//右端寄せ
	goal.img.y = GAME_HEIGHT / 2 - goal.img.height / 2;		//中央寄せ
	goal.speed = 500;								//移動速度
	goal.img.IsDraw = TRUE;								//描画できる

	//タイトルロゴを初期化
	titlelogo.x = GAME_WIDTH / 2 - titlelogo.width / 2;		//中央寄せ
	titlelogo.y = titlelogo.height * 2;						//ちょっと上

	//エンターを初期化
	titleenter.x = GAME_WIDTH / 2 - titleenter.width / 2;	//中央寄せ
	titleenter.y = GAME_HEIGHT - titleenter.height * 1.2;		//かなり下

	//クリアを初期化
	endclear.x = GAME_WIDTH / 2 - endclear.width / 2;	//中央寄せ
	endclear.y = GAME_HEIGHT / 2 - endclear.height / 2;	//ちょっと下

	//EndClearのフェードイン
	EndClearCnt = 0;
	EndClearIn = FALSE;	//していない

	//動画を初期化
	//playmovie.x = 0;			//左端寄せ
	//playmovie.y = 0;			//左端寄せ
	playmovie.volume = 255;		//音量最大
	//playmovie.IsDraw = TRUE;	//描画できる

	//当たり判定を更新する
	CollUpdatePlayer(&player);	//プレイヤーの当たり判定のアドレス
	//CollUpdateGoal(&goal);	//ゴールの当たり判定のアドレス
}

/// <summary>
/// タイトルの画像を初期化
/// </summary>
/// <param name=""></param>
VOID TitleInit(VOID)
{
	//PushEnterの点滅
	PushEnterCnt = 0;
	PushEnterBrink = FALSE;

	//TitleLogoのフェードイン
	TitleLogoCnt = 0;
	TitleLogoIn = FALSE;	//していない
}

/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//シーンを切り替え
	IsFadeIn = FALSE;	//フェードインしない
	IsFadeOut = TRUE;	//フェードアウトする

	return;
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
	//プレイシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(titleBGM.handle);
		
		//シーンを切り替え
		//次のシーンの初期化をここで行うと楽

		//ゲームの初期化
		GameInit();

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(titleBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(titleBGM.handle,titleBGM.playtype);
	}
	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{
	//フェードイン
	if (TitleLogoIn == FALSE && TitleLogoCntMax >TitleLogoCnt)
	{
		//半透明
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			((float)TitleLogoCnt / TitleLogoCntMax) * 255);

		DrawGraph(titlelogo.x, titlelogo.y,
			titlelogo.handle, TRUE);		//ロゴを描画

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		TitleLogoCnt++;
	}
	else if(TitleLogoCntMax == TitleLogoCnt)
	{
		TitleLogoIn == TRUE;	//完了
		DrawGraph(titlelogo.x, titlelogo.y,
			titlelogo.handle, TRUE);		//ロゴを描画
	}

	//MAX値まで待つ
	if (PushEnterCnt < PushEnterCntMax) { PushEnterCnt++; }
	else
	{
		PushEnterCnt = 0;
		if (PushEnterBrink == TRUE) { PushEnterBrink = FALSE; PushEnterCnt = 40; }	//20f
		else if (PushEnterBrink == FALSE) { PushEnterBrink = TRUE; }				//60f
	}

	//PushEnterを点滅
	if (PushEnterBrink == TRUE)
	{
		//PushEnterの描画
		DrawGraph(titleenter.x, titleenter.y,
			titleenter.handle, TRUE);		//PUSHENTERを描画
	}
	
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
	//エンドシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE && GAME_DEBUG == TRUE)
	{
		//BGMを止める
		StopSoundMem(playBGM.handle);
		StopSoundMem(playerSE.handle);

		//シーンを切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);

		return;
	}

	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.img.y = player.img.y - player.speed * fps.DeltaTime;

		//動くときの効果音を追加
		//BGMが流れていないとき
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			//BGMを流す
			PlaySoundMem(playerSE.handle, playerSE.playtype);
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.img.y = player.img.y + player.speed * fps.DeltaTime;

		//動くときの効果音を追加
		//BGMが流れていないとき
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			//BGMを流す
			PlaySoundMem(playerSE.handle, playerSE.playtype);
		}
	}
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.img.x = player.img.x - player.speed * fps.DeltaTime;

		//動くときの効果音を追加
		//BGMが流れていないとき
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			//BGMを流す
			PlaySoundMem(playerSE.handle, playerSE.playtype);
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.img.x = player.img.x + player.speed * fps.DeltaTime;

		//動くときの効果音を追加
		//BGMが流れていないとき
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			//BGMを流す
			PlaySoundMem(playerSE.handle, playerSE.playtype);
		}
	}
	
	//当たり判定を更新する
	CollUpdatePlayer(&player);
	CollUpdateGoal(&goal);

	if (CubeCollision(player.coll, goal.coll) == TRUE)	//プレイヤーがゴールにあたったとき
	{
		//BGMを止める
		StopSoundMem(playBGM.handle);
		StopSoundMem(playerSE.handle);

		ChangeScene(GAME_SCENE_END);					//エンド画面に切り替え
		return;											//処理を強制終了
	}
	//BGMが流れていないとき
	if (CheckSoundMem(playBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(playBGM.handle,playBGM.playtype);
	}
	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	//背景動画を描画

	//もし、動画が再生されていないとき
	if (GetMovieStateToGraph(playmovie.handle) == 0)
	{
		//再生する
		SeekMovieToGraph(playmovie.handle, 0);	//シークバーを最初に戻す
		PlayMovieToGraph(playmovie.handle);		//動画を再生
	}
	//動画を描画
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playmovie.handle, TRUE);

	//プレイヤーを描画
	if (player.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	//ゴールを描画
	if (goal.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(goal.img.x, goal.img.y, goal.img.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(255, 255, 255));
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
	//タイトルシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(endBGM.handle);

		//シーンを切り替え
		//次のシーンの初期化をここで行うと楽

		//タイトルの初期化
		TitleInit();

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}
	//BGMが流れていないとき
	if (CheckSoundMem(endBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(endBGM.handle, endBGM.playtype);
	}
	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	//フェードイン
	if (EndClearIn == FALSE && EndClearCntMax > EndClearCnt)
	{
		//半透明
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			((float)EndClearCnt / EndClearCntMax) * 255);

		DrawGraph(endclear.x, endclear.y,
			endclear.handle, TRUE);		//GameClearを描画

		//半透明終了
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		EndClearCnt++;
	}
	else if (EndClearCntMax == EndClearCnt)
	{
		EndClearIn == TRUE;	//完了
		DrawGraph(endclear.x, endclear.y,
			endclear.handle, TRUE);		//GameClearを描画
	}
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
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウンタを減らす
		}
		else	//フェードイン処理が終わったら
		{
			fadeInCnt = fadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン終了
		}
	}
	
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウンタを増やす
		}
		else	//フェードアウト処理が終わったら
		{
			fadeOutCnt = fadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;			//フェードアウト終了
		}
	}


	//切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のゲームシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			// プレイ 画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();			// エンド 画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntInit) * 255);	//0-255
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);	//0-255
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0),TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 16, "切り替え画面", GetColor(128, 128, 128));
	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARA* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdateGoal(CHARA* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

BOOL CubeCollision(RECT A,RECT B)
{
	if (A.left<B.right			//矩形Aの左辺X座標 < 矩形Bの右辺X座標かつ
		&& A.right>B.left		//矩形Aの右辺X座標 > 矩形Bの左辺X座標かつ
		&& A.top<B.bottom		//矩形Aの上辺Y座標 < 矩形Bの下辺Y座標かつ
		&& A.bottom>B.top)		//矩形Aの下辺Y座標 > 矩形Bの上辺Y座標
	{
		//当たっているとき
		return TRUE;
	}
	else
	{
		//当たっていないとき
		return FALSE;
	}
}