#ifndef __RECORD_H__
#define __RECORD_H__

//���@�}�N���̒�`
//�����|�C���g�֕ϊ�����
#define MINUTES_INTO_MILLISECOND( a )	( ( ( a ) * ( 60 ) ) * ( 100 ) )
//�b���|�C���g�֕ϊ�����
#define SECONDS_INTO_MILLISECOND( a )	( ( a ) * ( 100 ) )
//�R�̍��ڂ̍��v�����߂�
#define SUM( a , b , c )				( ( a ) + ( b ) + ( c ) )


//�� �ÓI�ϐ��̒�`
//�L�^�X�V�t���O
const char RECORD_NOT_RENEWAL = 0;			//�X�V�Ȃ�
const char RECORD_RENEWAL_FLAG = 1;			//�X�V����

//���@�񋓌^
//�L�^�̎�ނ�����
enum tagRENEWAL{
	RECORD_PREVIEW			,	//����̋L�^
	RECORD_BEST				,	//�ō��L�^
};
//�e�X�e�[�W�̋L�^�������v�f
enum tagRECORDMATRIX{
	RECORD_ONE			,				//�v�f1
	RECORD_TWO			,				//�v�f2
	RECORD_TIME			,				//���Ԃ̗v�f
	RECORD_FOUR			,				//�v�f4
	RECORD_TOTAL		,				//�W�v�|�C���g�̗v�f
	RECORD_MAX			,				//���ڂ̐�
};
//���H�X�e�[�W�̋L�^�������v�f��
enum tagROADRECORDMATRIX{
	RECORD_DFEAT_OBSTACLES	,			//�j�󂵂���Q���̐�
	RECORD_JUMPED_HURDLE	,			//��щz�����n�[�h����
};
//kind of record of the half way
enum tagRECORDPARTICIPANTS {
	PARTICIPANT_CONPETITOR1,
	PARTICIPANT_CONPETITOR2,
	PARTICIPANT_CONPETITOR3,
	PARTICIPANT_CONPETITOR4,
	PARTICIPANT_CONPETITOR5,
	PARTICIPANT_CONPETITOR6,
	PARTICIPANT_CONPETITOR7,
	PARTICIPANT_CONPETITOR8,
	PARTICIPANT_CONPETITOR9,
	PARTICIPANT_PLAYER,
	RECORD_KIND_OF_PARTICIPANT,
};
//kind of item
enum tagRECORDTHEHALFWAYITEM {
	RECORD_HALF_WAY_RECORD_TOTAL_POINT,				//aggregate point
	RECORD_HALF_WAY_RECORD_RACE_TIME,				//time of race
	RECORD_HALF_WAY_RECORD_CHAIN,					//consecutive something
	RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT,	//kind
};
//kind of save record for option
enum tagRECORDRANKINGRESULTITEM {
	RECORD_RANKING_ITEM_TOTAL_POINT,		//total
	RECORD_RANKING_ITEM_MINUTE,				//minute
	RECORD_RANKING_ITEM_SECOND,				//second
	RECORD_RANKING_ITEM_MILLISECOND,		//millisecond
	RECORD_RANKING_ITEM_CHAIN,				//chain
	RECORD_RANKING_ITEM_COLOR_FOR_TOTAL,	//each characters' identification
	RECORD_RANKING_ITEM_COLOR_FOR_TIME,		//each characters' identification
	RECORD_RANKING_ITEM_COLOR_FOR_CHAIN,	//each characters' identification
	RECORD_KIND_OF_RANKING_ITEM,			//kind
};
//���@�w�b�_�[�̃C���N���[�h
#include	<windows.h>
#include	"ending.h"
#include	"SelectStage.h"

//�� �\���̂̒�`
//���Ԃ̍\����
typedef struct tagTIME {

	int hour;			//��
	int minutes;		//��
	int second;			//�b
	int millisecond;	//����
	bool cntFlag;		//���Ԍo�߃t���O

} TIME, *LPTIME;

//�t�@�C���̍��ڂ������uRECORD�\���́v
typedef struct tagRECORD {

	int recordBox[RECORD_KIND_OF_PARTICIPANT][RECORD_KIND_OF_RANKING_ITEM];		//�L�^�𐔒l�Ƃ��Ĉ���
	char renewalFlag[RECORD_MAX][RECORD_MAX];									//�X�R�A�F�ύX�t���O
	
}RECORD , *LPRECORD;

//���@�֐��̃v���g�^�C�v�錾

void InitRecord();

//�A�@�L�^�ۑ��E�X�V
void RenewalRecord( char *compairFile );

//�E�@�n���ꂽ���l��S�p�ɕϊ����ĕ�����Ƃ��ĕԂ�
char* ConvertNum			//�߂�l�@���@�S�p�̐���
( int num );				//�����@���@�ϊ����������l

//�t�@�C������l��ǂݍ���
RECORD* LoadRecordFile( char* pass , tagRENEWAL thisTime );

//�L�^��Ԃ��֐�
RECORD* GetRecord( tagRENEWAL record );

//�L�^����ږ��ɕ����Ċe���ڂ�ۑ�����
void SaveRecordForStage( char* pass , RECORD* pre , tagRENEWAL thisTime );

/*
************************************************************************************************************
�L�^�p
************************************************************************************************************
*/
void	SaveRecord(char* lpPass, int kind, int* lpItem , RECORD* lpRecord);
/*
*************************************************************************************************
����̋L�^�ƍō��L�^�Ƃ��r���ō��L�^���㏑������
*************************************************************************************************
*/
void	RenewalRecordEndTheResult(char *previewpass, char *bestpass, int kind, int *lpItem);
/*
************************************************************************************************************
�L�^�����Z�b�g����
************************************************************************************************************
*/
void	ResetRecord(char* lpPass, int kind, int* lpItem, tagRENEWAL thisTime);

/*
Compair tim record
return is lpCompair1's ranking
*/
int	CompairRaceTimeRecord(TIME* lpCompair1, TIME* lpCompair2);
/*
Compair record more than time record
*/
int		CompairRecord(int compair1, int compair2);
/*
Save competitors' records in this race only
competitors' temporary records
*/
void	SaveCompetitorRecordTemporary(RECORD *lpRecordPoint, tagRECORDMATRIX kind, tagSELECTSTAGE stage);
/*
get competitors' records
*/
RECORD*	GetRecordCompetitors(tagSELECTSTAGE stage);

#endif