#include"easing.h"
#include <math.h>

const float PI = 3.141592f;

XMFLOAT3 easeOutSineVec3(XMFLOAT3 startpos, XMFLOAT3 endpos, float time)
{
	XMFLOAT3 gomikasu;

	gomikasu.x = startpos.x + (sin((time * PI) / 2)) * (endpos.x - startpos.x);
	gomikasu.y = startpos.y + (sin((time * PI) / 2)) * (endpos.y - startpos.y);
	gomikasu.z = startpos.z + (sin((time * PI) / 2)) * (endpos.z - startpos.z);

	return gomikasu;
}

XMFLOAT3 easeOutQuintVec3(XMFLOAT3 start, XMFLOAT3 end, double time)
{
	XMFLOAT3 gomikasu;
	gomikasu.x = start.x + (1 - pow(1 - time, 5)) * (end.x - start.x);
	gomikasu.y = start.y + (1 - pow(1 - time, 5)) * (end.y - start.y);
	gomikasu.z = start.z + (1 - pow(1 - time, 5)) * (end.z - start.z);

	return gomikasu;
}

//a����b�܂ł̎���(0�`1)�̈ʒu��m�邱�Ƃ��ł���֐�
double lerp(double a, double b, double t)
{
	return a + t * (b - a);
}

double easeInSine(double start, double end, double time)
{
	return start + (1 - cos((time * PI) / 2)) * (end - start);
}

double easeOutSine(double start, double end, double time)
{
	return start + (sin((time * PI) / 2)) * (end - start);
}

double easeInOutSine(double start, double end, double time)
{
	return start + (-(cos(PI * time) - 1) / 2) * (end - start);
}

double easeOutQuad(double start, double end, double time)
{
	return start + (1 - (1 - time) * (1 - time)) * (end - start);
}

//start�͎n�߂���W end�͈�ԍŌ�̍��W time�͌o�ߎ���(�����͎����ɏ���Ă�)
//��{�I�� �C�[�W���O���������l = �Ŏg��
//���̌������g�������ꍇ�� return start + (����) * (end - start); �Ŏg����
double easeInQuint(double start, double end, double time)
{
	return start + (time * time * time * time * time) * (end - start);
}

double easeOutQuint(double start, double end, double time)
{
	return start + (1 - pow(1 - time, 5)) * (end - start);
}
double easeInOutQuint(double start, double end, double time)
{
	return start + (time < 0.5 ? 16 * pow(time, 5) : 1 - pow(-2 * time + 2, 5) / 2) * (end - start);
}
double easeInBack(double start, double end, double time)
{
	const double c1 = 1.70158;
	const double c3 = c1 + 1;

	return start + (c3 * time * time * time - c1 * time * time) * (end - start);
}
double easeOutBack(double start, double end, double time)
{
	const double c1 = 1.70158;
	const double c3 = c1 + 1;

	return start + (1 + c3 * pow(time - 1, 3) + c1 * pow(time - 1, 2)) * (end - start);
}

double easeOutElastic(double start, double end, double time)
{
	const double c4 = (2 * PI) / 3;

	return start + (time == 0
		? 0
		: time == 1
		? 1
		: pow(2, -10 * time) * sin((time * 8 - 0.75) * c4) + 1) * (end - start);
}
