#ifndef CCHARACTER_H__
#define CCHARACTER_H__



//▼ヘッダーをインクルード

//▼構造体の定義
//キャラクターの座標を扱う
//テンプレート
template < typename T >
struct TPOINT{
	T	x;
	T	y;
};


//▼ キャラクタークラスの定義
class CCharacter{

private:


protected:

	//画像の要素を扱う
	int		m_ImgNum;
	
public:

	TPOINT<float>	m_Fpos;		//座標を扱う
	TPOINT<int>		m_Ipic;		//座標元を扱う
	TPOINT<float>	m_Fmove;	//移動量を扱う
	TPOINT<int>		m_Isize;	//サイズを扱う
	bool	m_existFlag;		//生存フラグ
	float	m_scale;			//拡縮の倍率
	float	m_alpha;			//透過率
	int		m_direction;		//方向
	int		m_kindNum;			//種類番号

	//▼プロトタイプ宣言

	//コンストラクタ
	CCharacter();
	//ディストラクタ
	~CCharacter();

	//各種設定用関数
	void	SetPosition( TPOINT<float> Tpos ){ this->m_Fpos = Tpos; }	//座標
	void	SetMove( TPOINT<float> Tmove ){ this->m_Fmove = Tmove; }	//移動量
	void	SetSize( TPOINT<int> Tsize ){ this->m_Isize = Tsize; }		//サイズ
	//画像の要素番号
	void	SetCharaImg( int ImgNum ){ this->m_ImgNum = ImgNum; }

	//各種取得用関数
	TPOINT<float>	GetPosition() const { return this->m_Fpos; }		//座標
	TPOINT<float>	GetMove() const { return this->m_Fmove; }			//移動量
	TPOINT<int>		GetSize() const { return this->m_Isize; }			//サイズ
	//画像の要素番号
	int		GetImageNum() const { return this->m_ImgNum; }

	/*
	***************************************************************************************************
	引数のCCharacterクラスをアップキャストする
	確保したらtrueを返す
	***************************************************************************************************
	*/
	bool	SetCharacter( CCharacter *lpcChara );
	
	
	//▼ 純粋仮想関数の定義
	virtual void	InitChara() = 0;				//初期化
	virtual void	UpdateChara() = 0;				//更新	
	virtual void	DrawChara() = 0;				//表示
	virtual void	ReleaseChara() = 0;				//解放

};


#endif