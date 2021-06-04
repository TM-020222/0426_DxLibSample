//ヘッダファイル読み込み
#include"game.h"		//ゲーム全体のヘッダファイル
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

//キャラクタの構造体
typedef struct CHARACTOR
{
	int handle = -1;		//画像のハンドル(管理番号)
	char path[255];			//画像の場所(パス)
	int x;					//X位置
	int y;					//Y位置
	int width;				//幅
	int height;				//高さ

	int speed = 1;			//移動速度

	RECT coll;				//当たり判定の領域(四角)
	BOOL IsDraw = FALSE;	//画像が描画できる？
}CHARA;

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

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回のゲームのシーン
GAME_SCENE NextGameScene;	// 次 のゲームのシーン

//プレイヤー
CHARA player;

//ゴール
CHARA goal;

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
	DeleteGraph(player.handle);	//画像をメモリ上から削除
	DeleteGraph(goal.handle);	//画像をメモリ上から削除
	DeleteGraph(playmovie.handle);	//動画をメモリ上から削除

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
	strcpyDx(player.path, TEXT(".\\image\\player.png"));
	player.handle = LoadGraph(player.path);	//画像の読み込み
	//ゴールの画像を読み込み
	strcpyDx(goal.path, TEXT(".\\image\\goal.png"));
	goal.handle = LoadGraph(goal.path);	//画像の読み込み
	//プレイ動画の背景読み込み
	strcpyDx(playmovie.path, TEXT(".\\movie\\playmovie.mp4"));
	playmovie.handle = LoadGraph(playmovie.path);	//動画の読み込み

	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メッセージのウィンドウハンドル
			TEXT(player.path),				//メッセージ本文
			TEXT("画像読み込みエラー！"),	//メッセージタイトル
			MB_OK);							//ボタン

		return FALSE;
	}
	if (goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//メッセージのウィンドウハンドル
			TEXT(goal.path),				//メッセージ本文
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

	//画像の幅と高さを取得
	GetGraphSize(player.handle, &player.width, &player.height);
	//画像の幅と高さを取得
	GetGraphSize(goal.handle, &goal.width, &goal.height);
	//動画の幅と高さを取得
	GetGraphSize(playmovie.handle, &playmovie.width, &playmovie.height);

	return TRUE;
}

/// <summary>
/// ゲームデータを初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//プレイヤーを初期化
	player.x = GAME_WIDTH / 2 - player.width / 2;		//中央寄せ
	player.y = GAME_HEIGHT / 2 - player.height / 2;		//中央寄せ
	player.speed = 500;									//移動速度
	player.IsDraw = TRUE;								//描画できる

	//ゴールを初期化
	goal.x = GAME_WIDTH - goal.width;				//右端寄せ
	goal.y = GAME_HEIGHT / 2 - goal.height / 2;		//中央寄せ
	goal.speed = 500;								//移動速度
	goal.IsDraw = TRUE;								//描画できる

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
		//シーンを切り替え
		//次のシーンの初期化をここで行うと楽

		//ゲームの初期化
		GameInit();

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}
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
	//エンドシーンへ切り替える
	if (KeyClick(KEY_INPUT_RETURN) == TRUE && GAME_DEBUG == TRUE)
	{
		//シーンを切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}

	

	if (KeyDown(KEY_INPUT_UP) == TRUE)
		player.y = player.y - player.speed * fps.DeltaTime;
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		player.y = player.y + player.speed * fps.DeltaTime;
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
		player.x = player.x - player.speed * fps.DeltaTime;
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		player.x = player.x + player.speed * fps.DeltaTime;
	
	//当たり判定を更新する
	CollUpdatePlayer(&player);
	CollUpdateGoal(&goal);

	if (CubeCollision(player.coll, goal.coll) == TRUE)	//プレイヤーがゴールにあたったとき
	{
		ChangeScene(GAME_SCENE_END);					//エンド画面に切り替え
		return;											//処理を強制終了
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
	if (player.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(player.x, player.y, player.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	//ゴールを描画
	if (goal.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(goal.x, goal.y, goal.handle, TRUE);

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
		//シーンを切り替え
		//次のシーンの初期化をここで行うと楽

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}
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
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdateGoal(CHARA* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

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