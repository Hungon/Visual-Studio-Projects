#include "animation.h"

//▼グローバル変数


//***************************************************************************************************
//キャラクターにアニメーション情報を設定
//***************************************************************************************************
void SetAnimation(CHARACTER* ch , ANIMATION* ani , int sx , int sy , int max , int fram , int type , bool flag){
	
	//優先度に応じた処理or強制的な変更
	if((ani -> type < type) || (flag == true)){

		//引数をもとにアニメーション情報を設定
		ani->sx = sx;
		ani->sy = sy;
		ani->max = max;
		ani->fram = fram;
		ani->time = 0;
		ani->cnt = 0;
		//キャラクターの表示元を更新
		ch->picx = sx;
		ch->picy = sy +(ch->direction * ch->h);
		//種類を初期化
		ani->type = type;
	}	
}

//***************************************************************************************************	
//キャラクターのアニメーションを更新
//***************************************************************************************************
bool UpdateAnimation( CHARACTER* ch , ANIMATION* ani ){

	bool ret = true;
	
	//経過時間を加算
	ani->time++;

	//経過時間が１コマ目の時間を越えたかを確認
	if( ani->time > ani->fram ){
	
		ani->cnt++;		//次のコマへ
		ani->time = 0;	//時間をリセット
	}

	//最大のコマを超えたかを確認
	if( ani->cnt >= ani->max ){

		ani->cnt = 0;	//1コマ目へ戻る
		ret = false;	//アニメーションの最後のコマまで到達したらfalseを返す
	}	

	//表示元座標の更新
	ch->picx = ani->sx + ( ani->cnt * ch->w );
	ch->picy = ani->sy + ( ch->direction * ch->h );

	return ret;	//最終的なフラグを返す

}

/*
**************************************************************************************************	
キャラクターのアニメーションを更新
**************************************************************************************************
*/
bool UpdateReverseAnimation( CHARACTER* ch , ANIMATION* ani ){

	//戻り値用
	bool ret = true;
	
	//経過時間を加算
	ani->time++;

	//加算アニメーション
	if( ani->reverseF == false ){
		//加算アニメーション処理
		//経過時間が１コマ目の時間を越えたかを確認
		if( ani->time > ani->fram ){
			
			ani->cnt++;		//次のコマへ
			ani->time = 0;	//時間をリセット
		}

		//最大のコマを超えたら反対アニメーションフラグにする
		if( ani->cnt >= ani->max )	ani->reverseF = true;

		//表示元座標の更新
		ch->picx = ani->sx + ( ani->cnt * ch->w );
		ch->picy = ani->sy + ( ch->direction * ch->h );

		return ret;

	}
	
	//減算アニメーション処理
	if( ani->reverseF ){
		if( ani->time > ani->fram ){
			
			ani->cnt--;		//次のコマへ
			ani->time = 0;	//時間をリセット
		}

		//反対アニメーション中で、アニメーションの始めのコマまで到達したらfalseを返す
		if( ani->cnt <= 0 )  ret = false;

		//表示元座標の更新
		ch->picx = ani->sx + ( ani->cnt * ch->w );
		ch->picy = ani->sy + ( ch->direction * ch->h );
	}

	//反対フラグを更新
	ani->reverseF = ret;
	return ret;	//最終的なフラグを返す

}

	