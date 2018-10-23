#include	"CCharacter.h"
#include	"MemoryLeakCheck.h"
#include	<math.h>



/*
**************************************************************************************************
コンストラクタ
**************************************************************************************************
*/
CCharacter::CCharacter(){

	this->m_Fpos.x = 0.0f;			//表示先座標X
	this->m_Fpos.y = 0.0f;			//表示先座標Y
	this->m_StartFpos.x = 0.0f;		//starting coordinateX
	this->m_StartFpos.y = 0.0f;		//starting coordinateY
	this->m_Ipic.x = 0;				//表示元座標X
	this->m_Ipic.y = 0;				//表示元座標Y
	this->m_Fmove.x = 0.0f;			//移動量X
	this->m_Fmove.y = 0.0f;			//移動量Y
	this->m_existFlag = false;		//生存フラグ
	this->m_Isize.x = 0;			//幅
	this->m_Isize.y = 0;			//高さ
	this->m_alpha = 0.0f;			//透過倍率
	this->m_scale = 0.0f;			//拡縮率
	this->m_kindNum = 0;			//種類番号
}

/*
**************************************************************************************************
ディストラクタ
**************************************************************************************************
*/
CCharacter::~CCharacter(){

}
/*
***********************************************************************************************
Culculate character's center position and rotate process
************************************************************************************************
*/
void CCharacter::CenterCharacter(int len, int angle) {

	//回転中心を算出
	int cx = (int)this->m_StartFpos.x + (this->m_Isize.x >> 1);
	int cy = (int)this->m_StartFpos.y + (this->m_Isize.y >> 1);

	//回転後の座標を求める
	this->m_Fpos.x = (float)cos(angle * 3.14 / 180.0) * len + cx - (this->m_Isize.x >> 1);
	this->m_Fpos.y = (float)sin(angle * 3.14 / 180.0) * len + cy - (this->m_Isize.y >> 1);
}


/*
************************************************************************************************
Update bezier drawing
************************************************************************************************
*/
void CCharacter::SetBezier(TPOINT<int> *lpPos, int bezier, double time) {

	int set = bezier - 1;
	//▼X座標
	//始点座標を求める
	float sx = lpPos[0].x * (float)(pow((1 - time), set));
	//中間座標
	float cx = 0.0f;
	//終点座標を求める
	float ex = lpPos[set].x * (float)(pow(time, set));

	//▼Y座標
	//始点座標を求める
	float sy = lpPos[0].y * (float)(pow((1 - time), set));
	//中間座標
	float cy = 0.0f;
	//終点座標を求める
	float ey = lpPos[set].y * (float)(pow(time, set));

	//始点を代入
	this->m_Fpos.x = sx;			//X
	this->m_Fpos.y = sy;			//Y

	//中心座標を求める
	for (int i = 1; i < set; i++) {
		//X座標
		this->m_Fpos.x += lpPos[i].x * set * (float)(pow((1 - time), (set - i)))
			* (float)(pow(time, i));
		//Y座標
		this->m_Fpos.y += lpPos[i].y * set * (float)(pow((1 - time), (set - i)))
			* (float)(pow(time, i));
	}
	//終点を最後に加算
	this->m_Fpos.x += ex;		//X
	this->m_Fpos.y += ey;		//Y
}