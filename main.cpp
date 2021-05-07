//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g���Ƃ��ɕK�v
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

	//�l�p�̈ʒu
	int x = GAME_WIDTH / 2;
	int y = GAME_HEIGHT / 2;

	//����(���x)
	int i = 16;
	int j = 16;

	int speed = 1;
	int xspeed = speed;
	int yspeed = speed;

	//���a
	int radius = 100;

	//����[���̊m�F
	int z = 1;

	//�l�p�̑傫��
	int width = 32;
	int height = 32;

	SetDrawScreen(DX_SCREEN_BACK);

	while (1)
	{
		//�L�[�������ꂽ�Ƃ�
		if (CheckHitKeyAll() != 0)
		{
			break;
		}

		//���b�Z�[�W���󂯎�葱����
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
			GetColor(255, 0, 0),	//�F�擾
			true					//�h��Ԃ�
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
		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��;
	}

	
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}