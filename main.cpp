//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"		//DxLib���g���Ƃ��ɕK�v
#include "keyboard.h"	//�L�[�{�[�h�̏���

#define GAME_TITLE "�Q�[���^�C�g��"
#define GAME_WIDTH 1280
#define GAME_HEIGHT 720
#define GAME_COLOR 32
#define GAME_ICON_ID 333		//�Q�[���̃A�C�R����ID
#define GAME_WINDOW_BAR 0

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

	//�~�̒��S�_
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	//�~�̔��a
	int radius = 50;

	//���x
	int spd = 4;

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

		//�L�[����
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
		
		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��;
	}

	
	//DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}