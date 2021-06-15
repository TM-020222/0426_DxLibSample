#define _CRT_SECURE_NO_WARNINGS
//�w�b�_�t�@�C���ǂݍ���
#include"game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//����̍\����
struct MOVIE
{
	int handle = -1;		//����̃n���h��(�Ǘ��ԍ�)
	char path[255];			//����̏ꏊ(�p�X)

	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	int volume = 255;		//�{�����[�� 0-255

	//BOOL IsDraw = FALSE;	//���悪�`��ł���H
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;		//����̃n���h��(�Ǘ��ԍ�)
	char path[255];			//����̏ꏊ(�p�X)

	int volume = -1;		//�{�����[�� 0-255
	int playtype = -1;		//BGM or SE
};

//�摜�̍\����
struct IMAGE
{
	int handle = -1;		//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];			//�摜�̏ꏊ(�p�X)
	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	BOOL IsDraw = FALSE;	//�摜���`��ł���H
};

//�L�����N�^�̍\����
typedef struct CHARACTOR
{
	IMAGE img;

	int speed = 1;			//�ړ����x

	RECT coll;				//�����蔻��̗̈�(�l�p)
}CHARA;

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	// �� �̃Q�[���̃V�[��

//�v���C���[
CHARA player;

//�S�[��
CHARA goal;

//�G
CHARA enemy;

//�摜��ǂݍ���
IMAGE titlelogo;
IMAGE titleenter;
IMAGE endclear;
IMAGE endover;
IMAGE asiato1;
IMAGE asiato2;

//���y
AUDIO titleBGM;
AUDIO playBGM;
AUDIO endBGM;

//���ʉ�
AUDIO playerSE;

//�v���C��ʂ̔w�i
MOVIE playmovie;

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsFadeIn  = FALSE;	//�t�F�[�h�C��

//�V�[���؂�ւ�
int fadeTimeMill = 2000;						//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�C���̃J�E���^
int fadeInCntMax = 0;				//�t�F�[�h�C���̃J�E���^MAX 0?

//TitleLogo�̃t�F�[�h�C��
int TitleLogoCnt = 0;				//�J�E���^
const int TitleLogoCntMax = 60;		//�J�E���^MAX
BOOL TitleLogoIn = FALSE;			//�t�F�[�h�C��������

//PushEnter�̓_��
int PushEnterCnt = 0;				//�J�E���^
const int PushEnterCntMax = 60;		//�J�E���^MAX
BOOL PushEnterBrink = FALSE;		//�_�ł��Ă��邩

//EndClear�̃t�F�[�h�C��
int EndClearCnt = 0;				//�J�E���^
const int EndClearCntMax = 60;		//�J�E���^MAX
BOOL EndClearIn = FALSE;			//�t�F�[�h�C��������

//EndOver�̃t�F�[�h�C��
int EndOverCnt = 0;					//�J�E���^
const int EndOverCntMax = 60;		//�J�E���^MAX
BOOL EndOverIn = FALSE;				//�t�F�[�h�C��������

//���Ղ̕`��p�֐�
int oldasiato = 0;
int playerplaceX = 0;
int playerplaceY = 0;

//���Ղ̃t�F�[�h�A�E�g
int asiatoCnt = 0;					//�J�E���^
const int asiatoCntMax = 30;		//�J�E���^MAX
BOOL asiatoOut = FALSE;				//�t�F�[�h�A�E�g������

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(����)
VOID TitleDraw(VOID);	//�^�C�g�����(�`��)

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C���(����)
VOID PlayDraw(VOID);	//�v���C���(�`��)

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h���(����)
VOID EndDraw(VOID);		//�G���h���(�`��)

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ����(�`��)

VOID Over(VOID);		//�Q�[���I�[�o�[���
VOID OverProc(VOID);	//�Q�[���I�[�o�[���(����)
VOID OverDraw(VOID);	//�Q�[���I�[�o�[���(�`��)

BOOL GameLoad(VOID);	//�Q�[���f�[�^�̃��[�h

//���y�̃��[�h
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playtype);

//�摜�̃��[�h
BOOL LoadImg(IMAGE* image, const char* path);

VOID GameInit(VOID);	//�Q�[���̏�����
VOID TitleInit(VOID);	//�^�C�g���̏�����

VOID CollUpdate(CHARA* chara);	//�����蔻��̗̈���X�V

BOOL CubeCollision(RECT A, RECT B);		//��`�Ƌ�`�̓����蔻��

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���
//DxLib��DirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	//Log.txt���o�͂��Ȃ�

	ChangeWindowMode(TRUE);			//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);	//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//�E�B���h�E�̉𑜓x��ݒ�

	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//�E�B���h�E�̑傫����ݒ�

	SetBackgroundColor(255, 255, 255);
	//�f�t�H���g�̔w�i�̐F

	SetWindowIconID(GAME_ICON_ID);
	//�A�C�R���t�@�C����ǂݍ���

	SetWindowStyleMode(GAME_WINDOW_BAR);
	//�E�B���h�E�o�[�̏��

	SetWaitVSyncFlag(TRUE);
	//�f�B�X�v���C�̐���������L���ɂ���

	SetAlwaysRunFlag(TRUE);
	//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	
	//DrawPixel(320, 240, GetColor(255, 255, 255));	// �_��ł�

	//WaitKey();				// �L�[���͑҂�

	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���̓ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();	//DxLib�I��
		return -1;		//�����I��
	}

	if (!LoadAudio(
		&titleBGM,
		".\\audio\\���.mp3",
		255,
		DX_PLAYTYPE_LOOP))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadAudio(
		&playBGM,
		".\\audio\\�����铲��.mp3",
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
		".\\audio\\SE\\����.mp4",
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

	if (!LoadImg(&endover,".\\image\\GameOver.png"))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadImg(&asiato1,".\\image\\asiato1.png"))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadImg(&asiato2,".\\image\\asiato2.png"))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadImg(&player.img,".\\image\\player.png"))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadImg(&goal.img,".\\image\\goal.png"))
	{
		DxLib_End();
		return -1;
	}

	if (!LoadImg(&enemy.img,".\\image\\dokuro.png"))
	{
		DxLib_End();
		return -1;
	}

	//�Q�[���̏�����
	GameInit();

	//�^�C�g���̏�����
	TitleInit();

	while (1)
	{
		//�L�[�������ꂽ�Ƃ�
		/*if (CheckHitKeyAll() != 0) { break; }*/

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0) { break; }

		//��ʏ���
		if (ClearDrawScreen() != 0) break;
		
		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) break;

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
			OldGameScene = GameScene;

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		case GAME_SCENE_OVER:	//�Q�[���I�[�o�[���
			Over();
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();
		
		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��;
	}

	//�I���Ƃ��̏���
	DeleteGraph(player.img.handle);			//�摜���������ォ��폜
	DeleteGraph(goal.img.handle);			//�摜���������ォ��폜
	DeleteGraph(enemy.img.handle);			//�摜���������ォ��폜
	DeleteGraph(playmovie.handle);		//������������ォ��폜
	
	DeleteGraph(titlelogo.handle);			//�摜���������ォ��폜
	DeleteGraph(titleenter.handle);			//�摜���������ォ��폜
	DeleteGraph(endclear.handle);			//�摜���������ォ��폜
	DeleteGraph(endover.handle);			//�摜���������ォ��폜

	DeleteSoundMem(titleBGM.handle);	//���y���������ォ��폜
	DeleteSoundMem(playBGM.handle);		//���y���������ォ��폜
	DeleteSoundMem(endBGM.handle);		//���y���������ォ��폜

	DeleteSoundMem(playerSE.handle);	//���y���������ォ��폜

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE,�ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	//�v���C����̔w�i�ǂݍ���
	strcpyDx(playmovie.path, TEXT(".\\movie\\playmovie.mp4"));
	playmovie.handle = LoadGraph(playmovie.path);	//����̓ǂݍ���

	if (playmovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���b�Z�[�W�̃E�B���h�E�n���h��
			TEXT(playmovie.path),				//���b�Z�[�W�{��
			TEXT("����ǂݍ��݃G���[�I"),	//���b�Z�[�W�^�C�g��
			MB_OK);							//�{�^��

		return FALSE;
	}

	//����̕��ƍ������擾
	GetGraphSize(playmovie.handle, &playmovie.width, &playmovie.height);

	return TRUE;
}

/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playtype">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playtype)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path, path);					//�p�X�̃R�s�[
	audio->handle = LoadSoundMem(audio->path);	//���y�̓ǂݍ���

	//���y���ǂݍ��߂Ȃ��������́A�G���[������
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���C���̃E�B���h�E�n���h��
			audio->path,					//���b�Z�[�W�{��
			TEXT("���y�ǂݍ��݃G���[�I"),	//���b�Z�[�W�^�C�g��
			MB_OK							//�{�^��
		);

		return FALSE;
	}
	//���̑��̐ݒ�
	audio->volume = volume;
	audio->playtype = playtype;

	return TRUE;
}

/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name=""></param>
BOOL LoadImg(IMAGE* image, const char* path)
{
	//�摜�̓ǂݍ���
	strcpyDx(image->path, path);					//�p�X�̃R�s�[
	image->handle = LoadGraph(image->path);	//���y�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ��������́A�G���[������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),			//���C���̃E�B���h�E�n���h��
			image->path,					//���b�Z�[�W�{��
			TEXT("�摜�ǂݍ��݃G���[�I"),	//���b�Z�[�W�^�C�g��
			MB_OK							//�{�^��
		);

		return FALSE;
	}
	//���̑��̐ݒ�
	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	return TRUE;
}



/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�v���C���[��������
	player.img.x = GAME_WIDTH / 2 - player.img.width / 2;		//������
	player.img.y = GAME_HEIGHT / 2 - player.img.height / 2;		//������
	player.speed = 500;									//�ړ����x
	player.img.IsDraw = TRUE;								//�`��ł���

	//�S�[����������
	goal.img.x = GAME_WIDTH - goal.img.width;				//�E�[��
	goal.img.y = GAME_HEIGHT / 2 - goal.img.height / 2;		//������
	goal.speed = 500;								//�ړ����x
	goal.img.IsDraw = TRUE;								//�`��ł���

	//�^�C�g�����S��������
	titlelogo.x = GAME_WIDTH / 2 - titlelogo.width / 2;		//������
	titlelogo.y = titlelogo.height * 2;						//������Ə�

	//�G���^�[��������
	titleenter.x = GAME_WIDTH / 2 - titleenter.width / 2;	//������
	titleenter.y = GAME_HEIGHT - titleenter.height * 1.2;		//���Ȃ艺

	//�N���A��������
	endclear.x = GAME_WIDTH / 2 - endclear.width / 2;	//������
	endclear.y = GAME_HEIGHT / 2 - endclear.height / 2;	//������Ɖ�

	//EndClear�̃t�F�[�h�C��
	EndClearCnt = 0;
	EndClearIn = FALSE;	//���Ă��Ȃ�

	//�G��������
	enemy.img.x = rand() % (GAME_WIDTH - enemy.img.width);	//�����_��
	enemy.img.y = rand() % (GAME_HEIGHT - enemy.img.height);//�����_��
	enemy.img.IsDraw = TRUE;	//�`��ł���

	//�Q�[���I�[�o�[��������
	endover.x = GAME_WIDTH / 2 - endover.width / 2;	//������
	endover.y = GAME_HEIGHT / 2 - endover.height / 2;	//������Ɖ�

	//���Ղ̏�����
	oldasiato = 0;

	//����1�̏�����
	asiato1.x = 0;
	asiato1.y = 0;
	asiato1.IsDraw = FALSE;

	//����2�̏�����
	asiato2.x = 0;
	asiato2.y = 0;
	asiato2.IsDraw = FALSE;

	//�����������
	playmovie.volume = 255;		//���ʍő�

	//�����蔻����X�V����
	CollUpdate(&player);	//�v���C���[�̓����蔻��̃A�h���X
	CollUpdate(&goal);	//�S�[���̓����蔻��̃A�h���X
}

/// <summary>
/// �^�C�g���̉摜��������
/// </summary>
/// <param name=""></param>
VOID TitleInit(VOID)
{
	//PushEnter�̓_��
	PushEnterCnt = 0;
	PushEnterBrink = FALSE;

	//TitleLogo�̃t�F�[�h�C��
	TitleLogoCnt = 0;
	TitleLogoIn = FALSE;	//���Ă��Ȃ�
}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	//�v���C�V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(titleBGM.handle);
		
		//�V�[����؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(titleBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(titleBGM.handle,titleBGM.playtype);
	}
	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	//�t�F�[�h�C��
	if (TitleLogoIn == FALSE && TitleLogoCntMax >TitleLogoCnt)
	{
		//������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			((float)TitleLogoCnt / TitleLogoCntMax) * 255);

		DrawGraph(titlelogo.x, titlelogo.y,
			titlelogo.handle, TRUE);		//���S��`��

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		TitleLogoCnt++;
	}
	else if(TitleLogoCntMax == TitleLogoCnt)
	{
		TitleLogoIn == TRUE;	//����
		DrawGraph(titlelogo.x, titlelogo.y,
			titlelogo.handle, TRUE);		//���S��`��
	}

	//MAX�l�܂ő҂�
	if (PushEnterCnt < PushEnterCntMax) { PushEnterCnt++; }
	else
	{
		PushEnterCnt = 0;
		if (PushEnterBrink == TRUE) { PushEnterBrink = FALSE; PushEnterCnt = 40; }	//20f
		else if (PushEnterBrink == FALSE) { PushEnterBrink = TRUE; }				//60f
	}

	//PushEnter��_��
	if (PushEnterBrink == TRUE)
	{
		//PushEnter�̕`��
		DrawGraph(titleenter.x, titleenter.y,
			titleenter.handle, TRUE);		//PUSHENTER��`��
	}
	
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//����
	PlayDraw();	//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	//�G���h�V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE && GAME_DEBUG == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(playBGM.handle);
		StopSoundMem(playerSE.handle);

		//�V�[����؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		return;
	}

	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.img.y = player.img.y - player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ���ǉ�
		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			playerplaceX = player.img.x;	//�v���C���[��X���W���L�^
			playerplaceY = player.img.y;	//�v���C���[��Y���W���L�^
			//BGM�𗬂�
			PlaySoundMem(playerSE.handle, playerSE.playtype);
			asiatoOut = FALSE;				//�t�F�[�h�A�E�g���Ă��邩
			if (oldasiato != 1)
			{
				//����1�`��\
				asiato1.IsDraw = TRUE;
			}
			else
			{
				//����2�`��\
				asiato2.IsDraw = TRUE;
			}
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.img.y = player.img.y + player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ���ǉ�
		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			playerplaceX = player.img.x;	//�v���C���[��X���W���L�^
			playerplaceY = player.img.y;	//�v���C���[��Y���W���L�^
			//BGM�𗬂�
			PlaySoundMem(playerSE.handle, playerSE.playtype);
			asiatoOut = FALSE;				//�t�F�[�h�A�E�g���Ă��邩
			if (oldasiato != 1)
			{
				//����1�`��\
				asiato1.IsDraw = TRUE;
			}
			else
			{
				//����2�`��\
				asiato2.IsDraw = TRUE;
			}
		}
	}
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.img.x = player.img.x - player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ���ǉ�
		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			playerplaceX = player.img.x;	//�v���C���[��X���W���L�^
			playerplaceY = player.img.y;	//�v���C���[��Y���W���L�^
			//BGM�𗬂�
			PlaySoundMem(playerSE.handle, playerSE.playtype);
			asiatoOut = FALSE;				//�t�F�[�h�A�E�g���Ă��邩
			if (oldasiato != 1)
			{
				//����1�`��\
				asiato1.IsDraw = TRUE;
			}
			else
			{
				//����2�`��\
				asiato2.IsDraw = TRUE;
			}
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.img.x = player.img.x + player.speed * fps.DeltaTime;

		//�����Ƃ��̌��ʉ���ǉ�
		//BGM������Ă��Ȃ��Ƃ�
		if (CheckSoundMem(playerSE.handle) == 0)
		{
			playerplaceX = player.img.x;	//�v���C���[��X���W���L�^
			playerplaceY = player.img.y;	//�v���C���[��Y���W���L�^
			//BGM�𗬂�
			PlaySoundMem(playerSE.handle, playerSE.playtype);
			asiatoOut = FALSE;				//�t�F�[�h�A�E�g���Ă��邩
			if (oldasiato != 1)
			{
				//����1�`��\
				asiato1.IsDraw = TRUE;
			}
			else
			{
				//����2�`��\
				asiato2.IsDraw = TRUE;
			}
		}
	}
	
	//�����蔻����X�V����
	CollUpdate(&player);
	CollUpdate(&goal);
	CollUpdate(&enemy);

	if (CubeCollision(player.coll, goal.coll) == TRUE)	//�v���C���[���S�[���ɂ��������Ƃ�
	{
		//BGM���~�߂�
		StopSoundMem(playBGM.handle);
		StopSoundMem(playerSE.handle);

		ChangeScene(GAME_SCENE_END);					//�G���h��ʂɐ؂�ւ�
		return;											//�����������I��
	}

	//�L�[�������Ă���Ƃ�����
	if (CubeCollision(player.coll, enemy.coll) == TRUE 
		&& (KeyDown(KEY_INPUT_UP)
		|| KeyDown(KEY_INPUT_DOWN)
		|| KeyDown(KEY_INPUT_LEFT)
		|| KeyDown(KEY_INPUT_RIGHT))
		)	//�v���C���[���G�ɂ��������Ƃ�
	{
		//BGM���~�߂�
		StopSoundMem(playBGM.handle);
		StopSoundMem(playerSE.handle);

		ChangeScene(GAME_SCENE_OVER);					//�Q�[���I�[�o�[��ʂɐ؂�ւ�
		return;											//�����������I��
	}
	//�����Ă��Ȃ��Ȃ�
	else if (CubeCollision(player.coll, enemy.coll) == TRUE)
	{
		//������
		GameInit();
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(playBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(playBGM.handle,playBGM.playtype);
	}
	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�w�i�����`��

	//�����A���悪�Đ�����Ă��Ȃ��Ƃ�
	if (GetMovieStateToGraph(playmovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playmovie.handle, 0);	//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playmovie.handle);		//������Đ�
	}
	//�����`��
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playmovie.handle, TRUE);

	//�v���C���[��`��
	if (player.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//�f�o�b�O�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	//�S�[����`��
	if (goal.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(goal.img.x, goal.img.y, goal.img.handle, TRUE);

		//�f�o�b�O�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(goal.coll.left, goal.coll.top, goal.coll.right, goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}
	
	//�G��`��
	if (enemy.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(enemy.img.x, enemy.img.y, enemy.img.handle, TRUE);

		//�f�o�b�O�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(enemy.coll.left, enemy.coll.top, enemy.coll.right, enemy.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}
	if (asiato1.IsDraw == TRUE)
	{
		//�t�F�[�h�C��
		if (asiatoOut == FALSE && asiatoCntMax > asiatoCnt)
		{
			//������
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,
				((float)(asiatoCntMax - asiatoCnt) / asiatoCntMax) * 255);
		
			DrawGraph(playerplaceX +(player.img.width/2)-asiato1.width, playerplaceY+player.img.height-asiato1.height/2,
				asiato1.handle, TRUE);		//���S��`��
		
			//�������I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
			asiatoCnt++;
		}
		else if (asiatoCntMax <= asiatoCnt)
		{
			asiatoOut = TRUE;	//����
			asiatoCnt = 0;
			asiato1.IsDraw = FALSE;
			oldasiato = 1;
		}
	}
	if (asiato2.IsDraw == TRUE)
	{
		//�t�F�[�h�C��
		if (asiatoOut == FALSE && asiatoCntMax > asiatoCnt)
		{
			//������
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,
				((float)(asiatoCntMax - asiatoCnt) / asiatoCntMax) * 255);
		
			DrawGraph(playerplaceX+(player.img.width/2)+asiato2.width, playerplaceY+player.img.height - asiato2.height / 2,
				asiato2.handle, TRUE);		//���S��`��
		
			//�������I��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
			asiatoCnt++;
		}
		else if (asiatoCntMax <= asiatoCnt)
		{
			asiatoOut = TRUE;	//����
			asiatoCnt = 0;
			asiato2.IsDraw = FALSE;
			oldasiato = 2;
		}
	}

	DrawString(0, 0, "�v���C���", GetColor(255, 255, 255));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	//����
	EndDraw();	//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	//�^�C�g���V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(endBGM.handle);

		//�V�[����؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g���̏�����
		TitleInit();

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}
	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(endBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(endBGM.handle, endBGM.playtype);
	}
	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	//�t�F�[�h�C��
	if (EndClearIn == FALSE && EndClearCntMax > EndClearCnt)
	{
		//������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			((float)EndClearCnt / EndClearCntMax) * 255);

		DrawGraph(endclear.x, endclear.y,
			endclear.handle, TRUE);		//GameClear��`��

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		EndClearCnt++;
	}
	else if (EndClearCntMax == EndClearCnt)
	{
		EndClearIn == TRUE;	//����
		DrawGraph(endclear.x, endclear.y,
			endclear.handle, TRUE);		//GameClear��`��
	}
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//����
	ChangeDraw();	//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else	//�t�F�[�h�C���������I�������
		{
			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C���I��
		}
	}
	
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else	//�t�F�[�h�A�E�g�������I�������
		{
			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;			//�t�F�[�h�A�E�g�I��
		}
	}


	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			// �v���C ��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();			// �G���h ��ʂ̕`��
		break;
	case GAME_SCENE_OVER:
		OverDraw();
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntInit) * 255);	//0-255
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);	//0-255
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0),TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 16, "�؂�ւ����", GetColor(128, 128, 128));
	return;
}

/// <summary>
/// �Q�[���I�[�o�[���
/// </summary>
VOID Over(VOID)
{
	OverProc();	//����
	OverDraw();	//�`��

	return;
}

/// <summary>
/// �Q�[���I�[�o�[��ʂ̏���
/// </summary>
VOID OverProc(VOID)
{
	//�^�C�g���V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(endBGM.handle);

		//�V�[����؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g���̏�����
		TitleInit();

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}
	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(endBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(endBGM.handle, endBGM.playtype);
	}
	return;
}

/// <summary>
/// �Q�[���I�[�o�[��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID OverDraw(VOID)
{
	DrawString(0, 0, "�Q�[���I�[�o�[���", GetColor(0, 0, 0));
	//�t�F�[�h�C��
	if (EndOverIn == FALSE && EndOverCntMax > EndOverCnt)
	{
		//������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,
			((float)EndOverCnt / EndOverCntMax) * 255);

		DrawGraph(endover.x, endover.y,
			endover.handle, TRUE);		//GameClear��`��

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		EndOverCnt++;
	}
	else if (EndOverCntMax == EndOverCnt)
	{
		EndOverIn == TRUE;	//����
		DrawGraph(endover.x, endover.y,
			endover.handle, TRUE);		//GameOver��`��
	}
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdate(CHARA* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

BOOL CubeCollision(RECT A,RECT B)
{
	if (A.left<B.right			//��`A�̍���X���W < ��`B�̉E��X���W����
		&& A.right>B.left		//��`A�̉E��X���W > ��`B�̍���X���W����
		&& A.top<B.bottom		//��`A�̏��Y���W < ��`B�̉���Y���W����
		&& A.bottom>B.top)		//��`A�̉���Y���W > ��`B�̏��Y���W
	{
		//�������Ă���Ƃ�
		return TRUE;
	}
	else
	{
		//�������Ă��Ȃ��Ƃ�
		return FALSE;
	}
}