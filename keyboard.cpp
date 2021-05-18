#include"keyboard.h"



KEYBOARD keyboard;

//�֐�

/// <summary>
/// �L�[�̓��͏�Ԃ��X�V����
/// </summary>
VOID AllKeyUpdate(VOID)
{
	//���O�̃L�[���͂�����Ă���
	for (int i = 0; i < KEY_KIND_MAX; i++)
		keyboard.OldAllKeyState[i] = keyboard.AllKeyState[i];

	//���ׂẴL�[�̓��͏�Ԃ𓾂�
	GetHitKeyStateAll(&keyboard.TempKeyState[0]);

	//������Ă���L�[�̎��Ԃ��X�V����
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		//�L�[�R�[�h�������Ă���Ƃ�
		if (keyboard.TempKeyState[i] != 0)
			keyboard.AllKeyState[i]++;		//�����Ă��鎞�Ԃ����Z
		else
			keyboard.AllKeyState[i] = 0;	//�����Ă��鎞�Ԃ�0�ɖ߂�
	}

	return;
}

/// <summary>
/// �L�[�������Ă���̂��A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT_">�L�[�R�[�h</param>
/// <returns>�����Ă�����TRUE</returns>
BOOL KeyDown(int KEY_INPUT_)
{
	if (keyboard.AllKeyState[KEY_INPUT_] != 0)
		return TRUE;
	else
		return FALSE;
}

/// <summary>
/// �L�[���グ�Ă���̂��A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT_">�L�[�R�[�h</param>
/// <returns>�����Ă�����TRUE</returns>
BOOL KeyUp(int KEY_INPUT_)
{
	if (
		keyboard.OldAllKeyState[KEY_INPUT_] != 0	//�ȑO�����Ă���
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//���݉����ĂȂ�
		)
		return TRUE;
	else
		return FALSE;
}

/// <summary>
/// �L�[���N���b�N�������A�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="KEY_INPUT_">�L�[�R�[�h</param>
/// <returns>�N���b�N������TRUE</returns>
BOOL KeyClick(int KEY_INPUT_)
{
	if (
		keyboard.OldAllKeyState[KEY_INPUT_] != 0	//�ȑO�����Ă���
		&& keyboard.AllKeyState[KEY_INPUT_] == 0	//���݉����ĂȂ�
		)
		return TRUE;
	else
		return FALSE;
}

/// <summary>
/// �L�[�����������Ă��邩
/// </summary>
/// <param name="KEY_INPUT_">�L�[�R�[�h</param>
/// <param name="MilliTime">�L�[�����������Ă���~���b</param>
/// <returns></returns>
BOOL KeyDownKeep(int KEY_INPUT_, int MilliTime)
{
	//1�b��1000�~���b
	float MilliSec = 1000.0f;

	//���������Ă��鎞�Ԃ́A�~���b��*FPS�l
	//��j1500�~���b����/1000�~�� �� 1.5�b
	int UpdateTime = (MilliTime / MilliSec) * 60;

	if (keyboard.AllKeyState[KEY_INPUT_] > UpdateTime)
		return TRUE;	//���������Ă���
	else
		return FALSE;	//���������ĂȂ�

}




