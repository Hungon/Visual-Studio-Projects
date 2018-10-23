#ifndef CFPS_H__
#define CFPS_H__


//▼ヘッダーファイルの読み込み
#include "graphics.h"


//FPSクラス
class CFps{

private:

	int m_Cnt;				//経過回数計算用
	double m_Old;			//前回の経過回数用
	int m_Show;				//表示用
	double m_Wait;			//プログラムを開いた際の初期時間を取得

protected:

public:

	static const double FPS_CONSTRAIN_TIME;
	CFps();
	~CFps();
	void DrawFps( int x , int y );
	bool WaitFps();

};

#endif
