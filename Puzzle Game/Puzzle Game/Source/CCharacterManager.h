#ifndef		CCHARACTERMANAGER_H__
#define		CCHARACTERMANAGER_H__

#include	<windows.h>
#include	"CCharacter.h"


//CCharacterクラスを管理する
class	CCharacterManager{

private:

	//CCharacterクラスを扱うデータテーブル
	CCharacter	**m_lplpCharaTbl;

	//CCharacterクラスを継承したキャラクターの最大数
	int	m_CharaMax;

protected:

public:

	//for get setting
	static TPOINT<float>		m_GetPos;
	static TPOINT<float>		m_GetMove;
	static TPOINT<int>			m_GetSize;


	CCharacterManager() :
		m_lplpCharaTbl(NULL) ,
		m_CharaMax(0)
	{}
	~CCharacterManager();

	virtual void	InitManagerCharacter() = 0;				
	virtual void	UpdateManagerCharacter() = 0;
	virtual void	DrawManagerCharacter() = 0;				
	virtual void	ReleaseManagerCharacter() = 0;			

	/*
	**************************************************************************************************
	引数の値だけCCharacterクラスを確保する
	**************************************************************************************************
	*/
	void	SetCharacter( int charaMax );		
	/*
	***************************************************************************************************
	引数のCCharacterクラスをアップキャストする
	確保したらtrueを返す
	***************************************************************************************************
	*/
	bool	SetUpCastCharacter( CCharacter *lpcChara );

	//各種取得用関数
	TPOINT<float>	GetCharacterManagerPosition() const { return this->m_GetPos; }		//座標
	TPOINT<float>	GetCharacterManagerMove() const { return this->m_GetMove; }			//移動量
	TPOINT<int>		GetCharacterManagerSize() const { return this->m_GetSize; }			//サイズ

};


#endif