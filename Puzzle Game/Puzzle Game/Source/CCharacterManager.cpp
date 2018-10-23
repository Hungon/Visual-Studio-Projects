#include	"CCharacterManager.h"
#include	"MemoryLeakCheck.h"
#include	"Likelihood.h"

//for get setting
TPOINT<float>		CCharacterManager::m_GetPos = { 0 };
TPOINT<float>		CCharacterManager::m_GetMove = { 0 };
TPOINT<int>			CCharacterManager::m_GetSize = { 0 };


CCharacterManager::~CCharacterManager() {}

/*
**************************************************************************************************
引数の値だけCCharacterクラスを確保する
**************************************************************************************************
*/
void	CCharacterManager::SetCharacter( int charaMax ){

	//引数の値を取得
	this->m_CharaMax = charaMax;
	//引数の値だけCCharacterクラスのメモリを確保
	this->m_lplpCharaTbl = new CCharacter*[ charaMax ];
	//0で初期化
	ZeroMemory( this->m_lplpCharaTbl , sizeof( CCharacter* ) * charaMax );

}

/*
***************************************************************************************************
引数のCCharacterクラスをアップキャストする
確保したらtrueを返す
***************************************************************************************************
*/
bool	CCharacterManager::SetUpCastCharacter( CCharacter *lpcChara ){
	
	//戻り値用
	bool ret = false;
	//キャラクターの数だけループを回す
	for( int i = 0; i < this->m_CharaMax; i ++ ){

		//初期化されていたら確保する
		if( this->m_lplpCharaTbl[i] == NULL ){	
			//アップキャスト
			this->m_lplpCharaTbl[i] = lpcChara; 
			ret = true; 
		}
	}
	return ret;
}


