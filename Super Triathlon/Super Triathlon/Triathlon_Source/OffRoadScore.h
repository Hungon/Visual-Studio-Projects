#ifndef COFFROADSCORE_H__
#define COFFROADSCORE_H__


//▼ヘッダーをインクルード
#include	"CCharacter.h"

//▼構造体の定義

//▼ オフロードスコアクラスの定義
class COffRoadScore : public CCharacter{

private:


protected:

public:


	//▼プロトタイプ宣言

	//コンストラクタ
	COffRoadScore();
	//ディストラクタ
	~COffRoadScore();
	
	
	//▼ 純粋仮想関数の定義
	void	InitChara();				//初期化
	void	UpdateChara();				//更新	
	void	DrawChara();				//表示
	void	ReleaseChara();				//解放

};


#endif