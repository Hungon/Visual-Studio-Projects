#include	"SelectStage.h"
#include	"wipe.h"
#include	"input.h"
#include	"map.h"
#include	"play.h"
#include	"sound.h"
#include	"swimer.h"
#include	"CRunner.h"
#include	"OffRoadRacer.h"

//▼グローバル変数
//画像の要素数を扱う
int		*g_lpSelectStageImg = NULL;

//画像の設定を扱う
CHARACTER	*g_lpSelectCh = NULL;

//説明画面用設定
CHARACTER	g_SelectBriefCh = {0};

//説明画面用設定
CHARACTER	g_SelectCursorCh = {0};

//効果音の要素を扱う
int		*g_lpSelectStageEffectSound = NULL;

//クリア済ステージを扱う
COMPLETESTAGE	g_CompleteStage[ STAGE_MAX ] = {0};

//選択したステージ番号を扱う
tagSELECTSTAGE	g_SelectStageNum = STAGE_VACANT;

//treat scene's flag in SelectStage
tagSELECTSTAGESCENE		g_SelectStageSceneNow = SELECTSTAGE_SCENE_SELECT_STAGE;
tagSELECTSTAGESCENE		g_SelectStageSceneNext = SELECTSTAGE_SCENE_SELECT_STAGE;
//treat each character for the brief
CHARACTER	g_SelectChForBrief = { 0 };
ANIMATION	g_SelectAniForBrief = { 0 };
//each character's words of action or technique
CHARACTER	*g_lpWordsImageCh;

//************************************************************************************************
//初期化
//************************************************************************************************
void InitSelectStage( HWND hWnd ){

	//get present language
	tagGAMELANGUAGE language = GetGameLanguage();
	//word's image of action for each languages
	char	*lpWordImage[GAME_KIND_OF_LANGUAGE][STAGE_MAX] = {
		//Japanese
		 "Image\\Sea\\AttackWords1.bmp" ,					//kind of attack for swimer
		 "Image\\Road\\ActionWords1.bmp" ,					//kind of action for runner
		 "Image\\OffRoad\\TechniqueWords1.bmp", 			//kind of technique for rider
		//English
		 "Image\\Sea\\AttackWords2.bmp" ,					//kind of attack for swimer
		 "Image\\Road\\ActionWords2.bmp" ,					//kind of action for runner
		 "Image\\OffRoad\\TechniqueWords2.bmp" ,			//kind of technique for rider
	};
	//使用する画像データ
	char	*lpImageTbl[ SELECTSTAGE_IMAGE_MAX ] = {

		{ "Image\\SelectStageBackImage.bmp" } ,				//背景
		{ "Image\\SelectStage.bmp" } ,						//各種ステージ文字列イメージ
		{ "Image\\cursor.bmp" } ,							//カーソル
		//各種ステージの操作説明画面	
		{ "Image\\BriefScene.bmp" } ,
		{ "Image\\Sea\\swimer.bmp"},						//swimer in the sea's stage
		{ "Image\\Road\\PlayerRunner.bmp"},					//runner in the road's stage
		{ "Image\\OffRoad\\cyclist.bmp"},					//rider in the off-road's stage
		lpWordImage[language][STAGE_SEA],					//kind of attack for swimer
		lpWordImage[language][STAGE_ROAD],					//kind of action for runner
		lpWordImage[language][STAGE_OFF_ROAD],				//kind of technique for rider
	};

	//使用する効果音データ
	char	*lpSoundTbl[ SELECTSTAGE_EFFECT_SOUND_MAX ] = {

		{ "SE\\cursor.wav" }	,				//カーソル音
		{ "SE\\ok.wav" }		,				//決定音
		{ "SE\\cancel.wav" }	,				//キャンセル音	
	};

	//▼ 各種メモリの確保
	//画像の要素
	g_lpSelectStageImg = new int [ SELECTSTAGE_IMAGE_MAX ];

	//　▼画像の読み込み
	for( int i = 0 ; i < SELECTSTAGE_IMAGE_MAX ; i++ ){
		g_lpSelectStageImg[i] = CreateDDImage( lpImageTbl[i] , hWnd );
	}

	//treat scene's flag in SelectStage
	g_SelectStageSceneNow = SELECTSTAGE_SCENE_SELECT_STAGE;			//the first scene is select stage
	g_SelectStageSceneNext = SELECTSTAGE_SCENE_SELECT_STAGE;

	//画像設定用
	g_lpSelectCh = new CHARACTER [ SELECTSTAGE_SCENE_SELECT_STAGE_KIND_OF_IMAGE ];
	//0で初期化
	ZeroMemory( g_lpSelectCh , sizeof( CHARACTER ) * SELECTSTAGE_SCENE_SELECT_STAGE_KIND_OF_IMAGE );

	//効果音の要素
	g_lpSelectStageEffectSound = new int [ SELECTSTAGE_EFFECT_SOUND_MAX ];
	//効果音の読み込み
	for( int i = 0 ; i < SELECTSTAGE_EFFECT_SOUND_MAX ; i++ ){

		g_lpSelectStageEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}

	//▼ BGMの読み込み
	CreateStreaming( "BGM\\option.wav" );


	//▼ 表示設定
	//背景
	g_lpSelectCh[0].h = SELECTSTAGE_BACKGROUND_IMAGE_HEIGHT;			//高さ
	g_lpSelectCh[0].w = SELECTSTAGE_BACKGROUND_IMAGE_WIDTH;				//幅
	g_lpSelectCh[0].flag = true;

	//ステージ選択画面タイトル画像
	g_lpSelectCh[4].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH
		( SELECTSTAGE_STAGE_TOP_TITLE_IMAGE_WIDTH );
	g_lpSelectCh[4].y = 20.0f;
	g_lpSelectCh[4].w = SELECTSTAGE_STAGE_TOP_TITLE_IMAGE_WIDTH;
	g_lpSelectCh[4].h = SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;						//高さ
	g_lpSelectCh[4].picy = ( 3 * SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT );			//表示元Y
	g_lpSelectCh[4].scale = 1.0f;													//拡縮倍率
	g_lpSelectCh[4].flag = true;													//表示フラグ

	//ステージ文字列イメージ
	//SEA
	g_lpSelectCh[1].x = ( STAGE_WIDTH - ( SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH * 
		SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE ) ) / 2 - 
		( ( SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH * SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE ) + 
		SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_SPACE_X );							//表示先X
	g_lpSelectCh[1].y = SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_Y;					//表示先Y
	g_lpSelectCh[1].h = SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;						//高さ
	g_lpSelectCh[1].w = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;						//幅
	g_lpSelectCh[1].scale = SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE;			//拡縮倍率
	g_lpSelectCh[1].flag = true;													//表示フラグ
	//ROAD
	g_lpSelectCh[2].x = ( g_lpSelectCh[1].x + SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH * 
		SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE ) +
		SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_SPACE_X;								//表示先X
	g_lpSelectCh[2].y = SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_Y;					//表示先Y
	g_lpSelectCh[2].h = SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;						//高さ
	g_lpSelectCh[2].w = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;						//幅
	g_lpSelectCh[2].picy = STAGE_ROAD * SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;		//表示元Y
	g_lpSelectCh[2].scale = SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE;			//拡縮倍率
	g_lpSelectCh[2].flag = true;													//表示フラグ
	//OFFROAD
	g_lpSelectCh[3].x = ( g_lpSelectCh[2].x + SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH * 
		SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE ) + 
		SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_SPACE_X;								//表示先X
	g_lpSelectCh[3].y = SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_Y;					//表示先Y
	g_lpSelectCh[3].h = SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;						//高さ
	g_lpSelectCh[3].w = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;						//幅
	g_lpSelectCh[3].picy = STAGE_OFF_ROAD * SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;	//表示元Y
	g_lpSelectCh[3].scale = SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE;			//拡縮倍率
	g_lpSelectCh[3].flag = true;													//表示フラグ

	//クリア済のステージなら画像の差し替え
	for( int i = 0 ; i < STAGE_MAX ; i++ ){
		if( g_CompleteStage[i] == SELECTSTAGE_STAGE_COMPLETE ){
 			g_lpSelectCh[ i + 1 ].picx = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;		//表示元X
		}
	}

	//カーソル
	g_SelectCursorCh.x = g_lpSelectCh[1].x;											//表示先X
	g_SelectCursorCh.y = g_lpSelectCh[1].y;											//表示先Y
	g_SelectCursorCh.mx = ( SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH * 
		SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE ) + 
		SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_SPACE_X;								//移動量X
	g_SelectCursorCh.h = (int)( SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT * 
		SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE );								//高さ
	g_SelectCursorCh.w = (int)( SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH * 
		SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE );								//幅
	g_SelectCursorCh.alpha = SELECTSTAGE_CURSOR_DEFAULT_ALPHA;						//拡縮倍率
	g_SelectCursorCh.flag = true;													//表示フラグ
	g_SelectCursorCh.time = 0;

	//各種ステージの説明画面
	g_SelectBriefCh.x = 0.0f;
	g_SelectBriefCh.h = SELECTSTAGE_BRIEF_IMAGE_HEIGHT;			//高さ
	g_SelectBriefCh.w = SELECTSTAGE_BRIEF_IMAGE_WIDTH;			//幅
	g_SelectBriefCh.mx = SELECTSTAGE_BRIEF_MOVE_SPEED_X;
	g_SelectBriefCh.flag = false;
	g_SelectBriefCh.picy = 0;
	g_SelectBriefCh.picx = 0;

	//initialize each character for explain the brief in the stage
	g_SelectChForBrief.flag = false;
}

//************************************************************************************************
//更新
//************************************************************************************************
void UpdateSelectStage() {

	//play back ground music
	PlayStreaming(true);
	
	//difer character's action acoording to selected stage
	void(*lpUpdateCharacter[STAGE_MAX])(void) = {
		UpdateCharacterForBriefInSea,				//sea
		UpdateCharacterForBriefInRoad,				//road
		UpdateCharacterForBriefInOffRoad,			//off-road
	};

	//transition scene from select stage to explain how to play in the stage
	if (g_SelectStageSceneNow == g_SelectStageSceneNext) {
		switch (g_SelectStageSceneNow) {
		case SELECTSTAGE_SCENE_SELECT_STAGE:				//select stage to play
			SelectStageChoiceStage();						//choice stage
			SelectStageControlCursor();						//control cursor
			break;
		case SELECTSTAGE_SCENE_STAGE_BRIEF1:				//how to control in the stage
			SelectStageExplainStageBrief1();
			break;
		case SELECTSTAGE_SCENE_STAGE_BRIEF2:				//explain actions in the stage
			lpUpdateCharacter[g_SelectStageNum]();			//execute character's action for the brief the stage
			SelectStageExplainStageBrief2();				//execute transition scene as player press key or button
			break;
		case SELECTSTAGE_SCENE_STAGE_BRIEF3:				//explain others in the stage. these are items or effects or scores and so on.
			SelectStageExplainStageBrief3();				//execute transition scene into play as player press key or button
			break;
		default:
			break;
		}
	}

	//constrain time of transition scene
	if (g_SelectStageSceneNow != g_SelectStageSceneNext) {
		g_SelectCursorCh.time++;				//count time
		if (g_SelectCursorCh.time >= SELECTSTAGE_CURSOR_DONT_ACCEPT_CONTROL_TIME) {		//limit time is 20
			g_SelectStageSceneNow = g_SelectStageSceneNext;			//subsutitute next flag for now flag
			g_SelectCursorCh.time = 0;								//reset time
		}
	}

	//each stages' brief move to each default coordinateX
	if (g_SelectBriefCh.flag &&
		g_SelectStageSceneNow == g_SelectStageSceneNext) {
		//each default positionX
		float briefPosX[SELECTSTAGE_KIND_OF_BRIEF] = {
			0,										//as first brief
			SELECTSTAGE_BRIEF_IMAGE_WIDTH,			//as second brief
			(SELECTSTAGE_BRIEF_IMAGE_WIDTH * 2),	//as third brief
		};
		//kind of brief
		tagSELECTSTAGESCENE briefScene[SELECTSTAGE_KIND_OF_BRIEF] = {
			SELECTSTAGE_SCENE_STAGE_BRIEF1,
			SELECTSTAGE_SCENE_STAGE_BRIEF2,
			SELECTSTAGE_SCENE_STAGE_BRIEF3,
		};
		for (int i = 0; i < SELECTSTAGE_KIND_OF_BRIEF; i++) {			//loop to kind of brief
			//add move speed into coordinateX
			if (g_SelectStageSceneNow == briefScene[i] &&
				g_SelectBriefCh.picx != briefPosX[i]) {
				//when present stage is off-road, don't explain the third brief
				if (g_SelectStageNum == STAGE_OFF_ROAD && g_SelectStageSceneNow == SELECTSTAGE_SCENE_STAGE_BRIEF2 && 
					g_SelectBriefCh.picx >= SELECTSTAGE_BRIEF_IMAGE_WIDTH) break;
				if (g_SelectBriefCh.picx >= briefPosX[i]) {									//if present coordinateX more than default position
					g_SelectBriefCh.picx -= g_SelectBriefCh.mx;									//as subtract coordinateX from present coordinateY
					if (g_SelectBriefCh.picx <= briefPosX[i]) g_SelectBriefCh.picx = briefPosX[i];		//limit and subsutitute
					break;
				}
				if (g_SelectBriefCh.picx <= briefPosX[i]) {									//if present coordinateX less than default position
					g_SelectBriefCh.picx += g_SelectBriefCh.mx;													//as add coordinateX
					if (g_SelectBriefCh.picx >= briefPosX[i]) g_SelectBriefCh.picx = briefPosX[i];		//limit and subsutitute
					break;
				}
			}
		}
	}
}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawSelectStage(){

	//背景画像の表示
	if( g_lpSelectCh[0].flag ){
		DDBlt(						
			(int)g_lpSelectCh[0].x ,
			(int)g_lpSelectCh[0].y ,
			g_lpSelectCh[0].w ,
			g_lpSelectCh[0].h ,
			g_lpSelectStageImg[0] ,
			g_lpSelectCh[0].picx , 
			g_lpSelectCh[0].picy 
			);
	}

	//説明画像の表示
	if( g_SelectBriefCh.flag ){
		DDBlt(						
			(int)g_SelectBriefCh.x ,
			(int)g_SelectBriefCh.y ,
			g_SelectBriefCh.w ,
			g_SelectBriefCh.h ,
			g_lpSelectStageImg[3] ,
			g_SelectBriefCh.picx , 
			g_SelectBriefCh.picy 
			);
	}

	//各種ステージ文字列イメージ画像の表示
	for( int i = 1 ; i <  SELECTSTAGE_SCENE_SELECT_STAGE_KIND_OF_IMAGE ; i++ ){
		if( g_lpSelectCh[i].flag ){

			//拡縮表示
			DDScaleBltSelectIndicate(						
			g_lpSelectCh[i].x ,
			g_lpSelectCh[i].y ,
			g_lpSelectCh[i].w ,
			g_lpSelectCh[i].h ,
			g_lpSelectStageImg[1] ,
			g_lpSelectCh[i].picx , 
			g_lpSelectCh[i].picy ,
			g_lpSelectCh[i].scale ,
			0
			);
		}
	}
	//draw character for the brief
	if (g_SelectChForBrief.flag) {
		//kind of character in the stage
		int	image[STAGE_MAX] = { g_lpSelectStageImg[4], g_lpSelectStageImg[5], g_lpSelectStageImg[6] };
		DDBlt(
			(int)g_SelectChForBrief.x,
			(int)g_SelectChForBrief.y,
			g_SelectChForBrief.w,
			g_SelectChForBrief.h,
			image[g_SelectStageNum],
			g_SelectChForBrief.picx,
			g_SelectChForBrief.picy
			);
		//draw words of action by character's animation
		int kindTbl[STAGE_MAX] = { 
			SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_SWIMER ,
			SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RUNNER ,
			SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RIDER 
		};
		//each material of drawing
		int wordImage[STAGE_MAX] = { g_lpSelectStageImg[7],g_lpSelectStageImg[8] ,g_lpSelectStageImg[9] };
		for (int i = 0; i < kindTbl[g_SelectStageNum]; i++) {
			if (g_lpWordsImageCh[i].flag) {
				DDBlt(
					(int)g_lpWordsImageCh[i].x,
					(int)g_lpWordsImageCh[i].y,
					g_lpWordsImageCh[i].w,
					g_lpWordsImageCh[i].h,
					wordImage[g_SelectStageNum],
					g_lpWordsImageCh[i].picx,
					g_lpWordsImageCh[i].picy
					);
				break;
			}
		}
	}
	//カーソルの表示
	if( g_SelectCursorCh.flag ){

		//拡縮表示
		DDAlphaBlt(						
		(int)g_SelectCursorCh.x ,
		(int)g_SelectCursorCh.y ,
		g_SelectCursorCh.w ,
		g_SelectCursorCh.h ,
		g_lpSelectStageImg[2] ,
		g_SelectCursorCh.picx , 
		g_SelectCursorCh.picy ,
		g_SelectCursorCh.alpha 
		);
	}

}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseSelectStage(){

	//画像の解放
	for( int i = 0; i < SELECTSTAGE_IMAGE_MAX ; i++ ){
		ReleaseDDImage( g_lpSelectStageImg[i] );
	}
	//効果音の解放
	for( int i = 0 ; i < SELECTSTAGE_EFFECT_SOUND_MAX ; i++ ){
		ReleaseSound( g_lpSelectStageEffectSound[i] );
	}
	//確保した要素の解放
	delete []g_lpSelectCh;				//画像設定用
	g_lpSelectCh = NULL;
	delete []g_lpSelectStageImg;		//画像の要素
	g_lpSelectStageImg = NULL;
	delete []g_lpSelectStageEffectSound;
	g_lpSelectStageEffectSound = NULL;	//効果音の要素

	//曲の解放
	ReleaseStreaming();

}

/*
**************************************************************************************************
クリア済ステージを設定する
**************************************************************************************************
*/
void	SetCompleteStage( COMPLETESTAGE complete , tagSELECTSTAGE stage ) { g_CompleteStage[stage] = complete; }

/*
**************************************************************************************************
クリア済みステージを取得
**************************************************************************************************
*/
COMPLETESTAGE	*GetCompleteStage() { return g_CompleteStage; }


/*
control cursor
*/
void	SelectStageControlCursor() {

	//▼ カーソルの移動
	if (g_SelectCursorCh.flag) {
		//右移動
		if (JustKey(VK_RIGHT) || JustLever(LEVER_RIGHT)) {
			PlaySound(g_lpSelectStageEffectSound[0], 0, false);
			g_SelectCursorCh.x += g_SelectCursorCh.mx;
		}
		//左移動
		if (JustKey(VK_LEFT) || JustLever(LEVER_LEFT)) {
			PlaySound(g_lpSelectStageEffectSound[0], 0, false);
			g_SelectCursorCh.x -= g_SelectCursorCh.mx;
		}
		//移動制限
		//右側
		if (g_SelectCursorCh.x > g_lpSelectCh[3].x) g_SelectCursorCh.x = g_lpSelectCh[1].x;
		//左側
		if (g_SelectCursorCh.x < g_lpSelectCh[1].x) g_SelectCursorCh.x = g_lpSelectCh[3].x;

	}
}

/*
Select stage to play
*/
void	SelectStageChoiceStage() {

	//▼ セレクトステージの座標
	float	StagePosXTbl[STAGE_MAX] = {
		g_lpSelectCh[1].x ,				//Sea
		g_lpSelectCh[2].x ,				//Road
		g_lpSelectCh[3].x ,				//OffRoad
	};

	//▼ ステージ番号
	tagSELECTSTAGE	StageTbl[STAGE_MAX] = {
		STAGE_SEA		,
		STAGE_ROAD		,
		STAGE_OFF_ROAD	,
	};

	//▼ ステージ選択中
	if (g_lpSelectCh[0].flag) {
		//▼ キー入力で次のシーンを変更
		for (int i = 0; i < STAGE_MAX; i++) {
			if (JustKey('Z') || JustButton(BUTTON_1)) {
				if (g_SelectCursorCh.x == StagePosXTbl[i]) {
					//選択したステージ番号を代入
					g_SelectStageNum = StageTbl[i];
					//背景、各種画像を非表示に
					for (int i = 0; i < 5; i++) { g_lpSelectCh[i].flag = false; }
					//説明画面を表示
					g_SelectBriefCh.flag = true;
					g_SelectBriefCh.picx = 0;			//starting point of default coordinateX
					//don't draw cursor
					g_SelectCursorCh.flag = false;
					//origin indication Y for present language
					int changeHeight = 0;
					//if present language is English, change the origin indication Y
					tagGAMELANGUAGE language = GetGameLanguage();
					if (language == GAME_LANGUAGE_ENGLISH) changeHeight = SELECTSTAGE_BRIEF_IMAGE_HEIGHT * STAGE_MAX;
					//ステージ番号に応じて説明画面を表示
					g_SelectBriefCh.picy = (g_SelectStageNum * SELECTSTAGE_BRIEF_IMAGE_HEIGHT) + changeHeight;
					//効果音
					PlaySound(g_lpSelectStageEffectSound[1], 0, false);
					g_SelectStageSceneNext = SELECTSTAGE_SCENE_STAGE_BRIEF1;			//subsutitute next scene for next flag
					return;
				}
			}
		}
		//"X"でオープニングへ戻る
		if (JustKey('X') || JustButton(BUTTON_2)) {
			//効果音
			PlaySound(g_lpSelectStageEffectSound[2], 0, false);
			//オープニングへ
			CreateWipe(SCENE_OPENING, CHANGE_SCENE, WIPE_CONTRACTION);
		}
	}
}

/*
explain each stages' brief
*/
void	SelectStageExplainStageBrief1() {

	//press accept key or button to next explanation
	if (JustKey('Z') || JustButton(BUTTON_1)) {
		//effect sound
		PlaySound(g_lpSelectStageEffectSound[1], 0, false);
		g_SelectStageSceneNext = SELECTSTAGE_SCENE_STAGE_BRIEF2;			//subsutitute transition scene flag for next flag
		//initialize each value for the character
		InitializeCharacterForBrief();
		return;
	}
	//back to select stage
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//draw cursor
		g_SelectCursorCh.flag = true;
		//draw back ground and each stages' title
		for (int i = 0; i < 5; i++) { g_lpSelectCh[i].flag = true; }
		//don't draw brief's image
		g_SelectBriefCh.flag = false;
		//fit cursor's coordinateY with sea's title
		g_SelectCursorCh.x = g_lpSelectCh[1].x;
		//effect sound
		PlaySound(g_lpSelectStageEffectSound[2], 0, false);
		g_SelectStageSceneNext = SELECTSTAGE_SCENE_SELECT_STAGE;			//subsutitute transition scene flag for next flag
		return;
	}
}
/*
explain each stages' brief
*/
void	SelectStageExplainStageBrief2() {

	//press accept key or button to next explanation
	if (JustKey('Z') || JustButton(BUTTON_1)) {
		//don't draw character
		g_SelectChForBrief.flag = false;
		//effect sound
		PlaySound(g_lpSelectStageEffectSound[1], 0, false);
		//delete memory for word of action
		delete[]g_lpWordsImageCh;
		g_lpWordsImageCh = NULL;
		if (g_SelectStageNum == STAGE_OFF_ROAD) {							//when present stage is off-road, transition scene into play
			ChangeStage(g_SelectStageNum);									//subsutitute present stage for global variable for playing stage
			CreateWipe(SCENE_PLAY, CHANGE_SCENE, WIPE_CONTRACTION); 
			return;
		}
		g_SelectStageSceneNext = SELECTSTAGE_SCENE_STAGE_BRIEF3;			//subsutitute transition scene flag for next flag		
		return;
	}
	//press cancel key or button to last scene that is first brief
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//don't draw character
		g_SelectChForBrief.flag = false;
		//effect sound
		PlaySound(g_lpSelectStageEffectSound[2], 0, false);
		g_SelectStageSceneNext = SELECTSTAGE_SCENE_STAGE_BRIEF1;			//subsutitute transition scene flag for next flag
		delete[]g_lpWordsImageCh;											//delete memory for word of action
		g_lpWordsImageCh = NULL;
		return;
	}
}

/*
explain each stages' brief
*/
void	SelectStageExplainStageBrief3() {

	//press accept key or button to play scene
	if (JustKey('Z') || JustButton(BUTTON_1)) {
		//effect sound
		PlaySound(g_lpSelectStageEffectSound[1], 0, false);
		//subsutitute present stage for global variable for playing stage
		ChangeStage(g_SelectStageNum);
		//transition scene to play scene
		CreateWipe(SCENE_PLAY, CHANGE_SCENE, WIPE_CONTRACTION);
		return;
	}
	//press cancel key or button to last scene that is first brief
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//initialize character
		InitializeCharacterForBrief();
		//effect sound
		PlaySound(g_lpSelectStageEffectSound[2], 0, false);
		g_SelectStageSceneNext = SELECTSTAGE_SCENE_STAGE_BRIEF2;			//subsutitute transition scene flag for next flag
		return;
	}
}

/*
Initialize character for explain the brief in the stage
*/
void	InitializeCharacterForBrief() {

	ZeroMemory(&g_SelectChForBrief, sizeof(CHARACTER));			//for indication
	ZeroMemory(&g_SelectAniForBrief, sizeof(ANIMATION));		//for animation

	//coordinate position X and Y
	FPOSITION chPos[STAGE_MAX] = {
		{ CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(SWIMER_DEFAULT_IMAGE_WIDTH), SELECTSTAGE_IMAGE_SEA_BRIEF_CHARACTER_INDICATE_Y },			//swimer
		{ CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(RUNNER_DEFAULT_IMAGE_WIDTH), SELECTSTAGE_IMAGE_ROAD_BRIEF_CHARACTER_INDICATE_Y },			//runner
		{ CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OFFROADRACER_IMAGE_RACER_WIDTH), SELECTSTAGE_IMAGE_OFFROAD_BRIEF_CHARACTER_INDICATE_Y },	//rider
	};
	//each character's size
	POINT	chSize[STAGE_MAX] = {
		{ SWIMER_DEFAULT_IMAGE_WIDTH, SWIMER_DEFAULT_IMAGE_HEIGHT },							//swimer
		{ RUNNER_DEFAULT_IMAGE_WIDTH, RUNNER_DEFAULT_IMAGE_HEIGHT },							//runner
		{ OFFROADRACER_IMAGE_RACER_WIDTH, OFFROADRACER_IMAGE_RACER_HEIGHT },					//rider
	};
	//set each value for animation
	//each frams
	int	chFram[STAGE_MAX] = {
		SWIMER_TYPE_NORMAL_ANIMATION_FRAM,				//swimer
		RUNNER_ANIMATION_TYPE_NORMAL_FRAM,				//runner
		OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM,		//rider
	};
	//each animation max
	int chMax[STAGE_MAX] = {
		SWIMER_TYPE_NORMAL_ANIMATION_MAX,				//swimer
		RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX,			//runner
		OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX,		//rider
	};
	//subsutitute each value for global variable
	g_SelectChForBrief.x = chPos[g_SelectStageNum].fX;
	g_SelectChForBrief.y = chPos[g_SelectStageNum].fY;
	g_SelectChForBrief.w = chSize[g_SelectStageNum].x;
	g_SelectChForBrief.h = chSize[g_SelectStageNum].y;
	//for animation
	g_SelectAniForBrief.fram = chFram[g_SelectStageNum];
	g_SelectAniForBrief.max = chMax[g_SelectStageNum];

	//set words of actions
	//each kind
	int kindTbl[STAGE_MAX] = {
		SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_SWIMER,		//swimer
		SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RUNNER,		//runner
		SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RIDER,		//rider
	};
	//each sizes
	POINT	wordSize[STAGE_MAX] = {
		{ SELECTSTAGE_IMAGE_WORDS_FOR_SWIMER_WIDTH, SELECTSTAGE_IMAGE_WORDS_FOR_SWIMER_HEIGHT },
		{ SELECTSTAGE_IMAGE_WORDS_FOR_RUNNER_WIDTH, SELECTSTAGE_IMAGE_WORDS_FOR_RUNNER_HEIGHT },
		{ SELECTSTAGE_IMAGE_WORDS_FOR_RIDER_WIDTH, SELECTSTAGE_IMAGE_WORDS_FOR_RIDER_HEIGHT },
	};
	//allot memory for describe words of action
	g_lpWordsImageCh = new CHARACTER[kindTbl[g_SelectStageNum]];
	ZeroMemory(g_lpWordsImageCh, sizeof(CHARACTER) * kindTbl[g_SelectStageNum]);
	//intialize indication setting
	for (int i = 0; i < kindTbl[g_SelectStageNum]; i++) {
		g_lpWordsImageCh[i].w = wordSize[g_SelectStageNum].x;
		g_lpWordsImageCh[i].h = wordSize[g_SelectStageNum].y;
		g_lpWordsImageCh[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(wordSize[g_SelectStageNum].x);
		g_lpWordsImageCh[i].y = chPos[g_SelectStageNum].fY - wordSize[g_SelectStageNum].y;
		g_lpWordsImageCh[i].picy = (i * wordSize[g_SelectStageNum].y);
	}
}


/*
Update character's action for the explanation to sea's stage
*/
void	UpdateCharacterForBriefInSea() {

	//kind of animation
	tagSWIMERATTACKTYPE presentType[SWIMER_ATTACK_TYPE_MAX] = {
		SWIMER_ATTACK_NORMAL ,				//normal
		SWIMER_ATTACK_WAVE_ATTACK ,			//wave
		SWIMER_ATTACK_CIRCLE_ATTACK ,		//rotate
		SWIMER_ATTACK_BUMP_ATTACK ,			//bump

	};
	//kind of animation to next
	tagSWIMERATTACKTYPE nextType[SWIMER_ATTACK_TYPE_MAX] = {
		SWIMER_ATTACK_WAVE_ATTACK ,			//wave
		SWIMER_ATTACK_CIRCLE_ATTACK ,		//rotate
		SWIMER_ATTACK_BUMP_ATTACK ,			//bump
		SWIMER_ATTACK_NORMAL ,				//normal
	};
	//each difference size to next
	int sizeTbl[SWIMER_ATTACK_TYPE_MAX] = {
		SWIMER_DEFAULT_IMAGE_HEIGHT,		//wave
		SWIMER_ATTACK_IMAGE_HEIGHT,			//rotate
		SWIMER_ATTACK_IMAGE_HEIGHT,			//bump
		SWIMER_DEFAULT_IMAGE_HEIGHT,		//normal
	};
	//each difference origin coordinateY to next
	int picYTbl[SWIMER_ATTACK_TYPE_MAX] = {
		SWIMER_DEFAULT_IMAGE_HEIGHT * 2,			//wave
		SWIMER_ANIMATION_TYPE_CIRCLE_ATTACK_PIC_Y,	//rotate
		SWIMER_ANIMATION_TYPE_BUMP_ATTACK_PIC_Y,	//bump
		0,											//normal
	};
	//each max fram to next
	int		cntMaxTbl[SWIMER_ATTACK_TYPE_MAX] = {
		SWIMER_TYPE_NORMAL_ANIMATION_MAX,			//wave
		SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_MAX,	//rotate
		SWIMER_TYPE_BUMP_ATTACK_ANIMATION_MAX,		//bump
		SWIMER_TYPE_NORMAL_ANIMATION_MAX,			//normal
	};
	//each fram speed to next
	int		framTbl[SWIMER_ATTACK_TYPE_MAX] = {
		SWIMER_TYPE_NORMAL_ANIMATION_FRAM,			//wave
		SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_FRAM,	//rotate
		SWIMER_TYPE_BUMP_ATTACK_ANIMATION_FRAM * 3,	//bump
		SWIMER_TYPE_NORMAL_ANIMATION_FRAM,			//normal
	};
	//kind of word for swimer's action
	tagSWIMERATTACKTYPE wordType[SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_SWIMER] = {
		SWIMER_ATTACK_WAVE_ATTACK ,			//wave
		SWIMER_ATTACK_CIRCLE_ATTACK ,		//rotate
		SWIMER_ATTACK_BUMP_ATTACK ,			//bump
	};

	//draw character
	if (g_SelectChForBrief.flag == false) {
		if (g_SelectBriefCh.picx == SELECTSTAGE_BRIEF_IMAGE_WIDTH) g_SelectChForBrief.flag = true;
		g_SelectAniForBrief.type = SWIMER_ATTACK_NORMAL;
	}
	if (g_SelectChForBrief.flag) {										//when swimer is drawing
		for (int i = 0; i < SWIMER_ATTACK_TYPE_MAX; i++) {				//loop to kind of animation
			if (g_SelectAniForBrief.type == presentType[i]) {				//update character's animation
			//character's animation for the explanation of the stage
				if (UpdateAnimation(&g_SelectChForBrief, &g_SelectAniForBrief) == false) {
					g_SelectChForBrief.h = sizeTbl[i];						//next size
					g_SelectAniForBrief.sy = picYTbl[i];					//next origin coordinateY
					g_SelectAniForBrief.type = nextType[i];					//next type
					g_SelectAniForBrief.fram = framTbl[i];					//fram speed
					g_SelectAniForBrief.max = cntMaxTbl[i];					//animation count max
					g_SelectAniForBrief.time = 0;							//reset time
					break;
				}
			}
		}
		for (int i = 0; i < SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_SWIMER; i++) {
			if (g_SelectAniForBrief.type == wordType[i]) {				//describe word of action
				g_lpWordsImageCh[wordType[i]].flag = true;
			}
			else {
				g_lpWordsImageCh[wordType[i]].flag = false;
			}
		}
	}
}

/*
Update character's action for the explanation to road's stage
*/
void	UpdateCharacterForBriefInRoad() {

	//kind of animation
	BYTE presentType[RUNNER_ANIMATION_MAX] = {
		RUNNER_ANIMATION_ROUND,			//round
		RUNNER_ANIMATION_JUMP,			//jump
		RUNNER_ANIMATION_ATTACK,		//attack
	};
	//kind of animation to next
	BYTE nextType[RUNNER_ANIMATION_MAX] = {
		RUNNER_ANIMATION_JUMP,			//jump
		RUNNER_ANIMATION_ATTACK,		//attack
		RUNNER_ANIMATION_ROUND,			//round
	};
	//each max fram to next
	int		cntMaxTbl[RUNNER_ANIMATION_MAX] = {
		RUNNER_ANIMATION_TYPE_JUMP_CNT_MAX,		//jump
		RUNNER_ANIMATION_TYPE_ATTACK_CNT_MAX,	//attack
		RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX,	//round
	};
	//each fram speed to next
	int		framTbl[RUNNER_ANIMATION_MAX] = {
		RUNNER_ANIMATION_TYPE_JUMP_FRAM,		//jump
		RUNNER_ANIMATION_TYPE_ATTACK_FRAM,		//attack
		RUNNER_ANIMATION_TYPE_NORMAL_FRAM,		//round
	};

	//kind of word for action
	BYTE wordType[SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RUNNER] = {
		RUNNER_ANIMATION_JUMP,			//jump
		RUNNER_ANIMATION_ATTACK,		//attack
	};

	//draw character
	if (g_SelectChForBrief.flag == false) {
		if (g_SelectBriefCh.picx == SELECTSTAGE_BRIEF_IMAGE_WIDTH) g_SelectChForBrief.flag = true;
		g_SelectAniForBrief.type = RUNNER_ANIMATION_ROUND;				//starting animation type
	}
	if (g_SelectChForBrief.flag) {										//when runner is drawing
		for (int i = 0; i < RUNNER_ANIMATION_MAX; i++) {				//loop to kind of animation
			if (g_SelectAniForBrief.type == presentType[i]) {
				//character's animation for the explanation of the stage
				if (UpdateAnimation(&g_SelectChForBrief, &g_SelectAniForBrief) == false) {
					g_SelectAniForBrief.sy = nextType[i] * g_SelectChForBrief.h;	//next origin coordinateY
					g_SelectAniForBrief.type = nextType[i];							//next type
					g_SelectAniForBrief.fram = framTbl[i];							//fram speed
					g_SelectAniForBrief.max = cntMaxTbl[i];							//animation count max
					g_SelectAniForBrief.time = 0;									//reset time
					break;
				}
			}
		}
		for (int i = 0; i < SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RUNNER; i++) {
			if (g_SelectAniForBrief.type == wordType[i]) {				//describe word of action
				g_lpWordsImageCh[i].flag = true;
			}
			else {
				g_lpWordsImageCh[i].flag = false;
			}
		}
	}
}

/*
Update character's action for the explanation to off-road's stage
*/
void	UpdateCharacterForBriefInOffRoad() {

	//kind of animation
	tagRACERTECHNIQUE presentType[RACERTECHNIQUE_KIND_OF_TYPE] = {
		RACERTECHNIQUE_TYPE_NORMAL,					//run on the road
		RACERTECHNIQUE_TYPE_RIGHT_HALF_ROTATE,		//half turn right in the sky
		RACERTECHNIQUE_TYPE_LEFT_HALF_ROTATE,		//half trun left in the sky
		RACERTECHNIQUE_TYPE_RIGHT_ROTATE,			//once turn right in the sky
		RACERTECHNIQUE_TYPE_LEFT_ROTATE,			//once turn left in the sky
	};
	//kind of animation to next
	tagRACERTECHNIQUE nextType[RACERTECHNIQUE_KIND_OF_TYPE] = {
		RACERTECHNIQUE_TYPE_RIGHT_HALF_ROTATE,		//half turn right in the sky
		RACERTECHNIQUE_TYPE_LEFT_HALF_ROTATE,		//half trun left in the sky
		RACERTECHNIQUE_TYPE_RIGHT_ROTATE,			//once turn right in the sky
		RACERTECHNIQUE_TYPE_LEFT_ROTATE,			//once turn left in the sky
		RACERTECHNIQUE_TYPE_NORMAL,					//run on the road
	};
	//each difference size, that is width
	int	sizeTbl[RACERTECHNIQUE_KIND_OF_TYPE] = {
		OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH,
		OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH,
		OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH,
		OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH,
		OFFROADRACER_IMAGE_RACER_WIDTH,
	};
	//each origin coordinateY for indication
	int	picYTbl[RACERTECHNIQUE_KIND_OF_TYPE] = {
		OFFROADRACER_IMAGE_RACER_HEIGHT,
		OFFROADRACER_IMAGE_RACER_HEIGHT * 2,
		OFFROADRACER_IMAGE_RACER_HEIGHT,
		OFFROADRACER_IMAGE_RACER_HEIGHT * 2,
		0,
	};
	//each max fram to next
	int		cntMaxTbl[RACERTECHNIQUE_KIND_OF_TYPE] = {
		OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX,
		OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX,
		OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX,
		OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX,
		OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX,
	};
	//each fram speed to next
	int		framTbl[RACERTECHNIQUE_KIND_OF_TYPE] = {
		OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM * 3,
		OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM * 3,
		OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM * 2,
		OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM * 2,
		OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM,
	};
	//kind of word for techniques
	tagRACERTECHNIQUE wordType[SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RIDER] = {
		RACERTECHNIQUE_TYPE_RIGHT_HALF_ROTATE,		//half turn right in the sky
		RACERTECHNIQUE_TYPE_LEFT_HALF_ROTATE,		//half trun left in the sky
		RACERTECHNIQUE_TYPE_RIGHT_ROTATE,			//once turn right in the sky
		RACERTECHNIQUE_TYPE_LEFT_ROTATE,			//once turn left in the sky
	};

	//draw character
	if (g_SelectChForBrief.flag == false) {
		if (g_SelectBriefCh.picx == SELECTSTAGE_BRIEF_IMAGE_WIDTH) g_SelectChForBrief.flag = true;
		g_SelectAniForBrief.type = RACERTECHNIQUE_TYPE_NORMAL;				//starting animation type is normal, that is running on the off-road
	}
	if (g_SelectChForBrief.flag) {										//when swimer is drawing
		for (int i = 0; i < RACERTECHNIQUE_KIND_OF_TYPE; i++) {			//loop to kind of animation
			if (g_SelectAniForBrief.type == presentType[i]) {
				//character's animation for the explanation of the stage
				if (UpdateReverseAnimation(&g_SelectChForBrief, &g_SelectAniForBrief) == false) {	//when present count get count max, reverse to 0 count
					g_SelectAniForBrief.sy = picYTbl[i];							//next origin coordinateY
					g_SelectAniForBrief.type = nextType[i];							//next type
					g_SelectChForBrief.w = sizeTbl[i];								//next size of width
					g_SelectAniForBrief.fram = framTbl[i];							//fram speed
					g_SelectAniForBrief.max = cntMaxTbl[i];							//animation count max
					g_SelectAniForBrief.time = 0;									//reset time
					break;
				}
			}
		}
		for (int i = 0; i < SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RIDER; i++) {
			if (g_SelectAniForBrief.type == wordType[i]) {				//describe word of action
				g_lpWordsImageCh[i].flag = true;
			}
			else {
				g_lpWordsImageCh[i].flag = false;
			}
		}
	}
}