//
#pragma warning ( disable : 4996 )

//���@�w�b�_�[�̃C���N���[�h
#include    "record.h"
#include    "seaenemy.h"
#include    "swimer.h"
#include    "item.h"
#include    "seaenemy.h"
#include    "swimer.h"
#include    "result.h"
#include    "play.h"
#include	<stdio.h>
#include	<string.h>


//���O���[�o���ϐ��̒�`
//�t�@�C���̍��ڂ�����
RECORD g_record[2] = {0};

//���݂̈����t�@�C���̗񋓌^
tagRENEWAL g_thisTime = RECORD_BEST;

//�L�^�ň����e���ڂ̗v�f���̃f�[�^�e�[�u��
//�@�C�X�e�[�W�F�G�����A�l���A�C�e���A���ԁA�G����CHAIN���A�W�v�|�C���g�̏���
//�A���H�X�e�[�W�F�j�󂵂���Q���A��񂾃n�[�h���A���ԁA�n�[�h������CHAIN���A�W�v�|�C���g�̏���
//�B�I�t���[�h�X�e�[�W�F���������A�N�V�����A���v�A�N�V����Pt�A���ԁA�A���A�N�V�������A�W�v�|�C���g�̏���
char g_recordScoreItemTbl[STAGE_MAX][RECORD_MAX] = {

	{ 5 , 5 , 3 , 1 , 1 } ,			//�C

	{ 2 , 3 , 3 , 1 , 2 } ,			//���H

	{ 4 , 1 , 3 , 1 , 1 } ,			//�I�t���[�h

};

//competitors' record
RECORD		g_RecordForCompetitors[STAGE_MAX][PARTICIPANT_PLAYER] = { 0 };

//**************************************************************************************************
//�L�^�̏�����
//**************************************************************************************************
void InitRecord(){

	//�e���ڂ̏�����
	//����̋L�^
	ZeroMemory( &g_record[0] , sizeof(RECORD) * 2 );

}


//**************************************************************************************************
//�t�@�C������l��ǂݍ���
//**************************************************************************************************
RECORD*	 LoadRecordFile( char* pass , tagRENEWAL thisTime ){

	//�t�@�C���|�C���^
	FILE* fp = NULL;

	//�t�@�C������̕�������󂯎��
	char word[256] = {0};

	//��؂蕶���ƃX�y�[�X�𒲂ׂ�p
	int seek = 0;

	//�������J�E���g�p
	int wordCnt = 0;

	//���ڃJ�E���g�p
	int itemCnt = 0;

	//���̍��ڂ̗v�f��
	int itemNext = 0;

	//�����p
	int digit = 0;


	//�����̃t�@�C�����J��
	fp = fopen( pass , "rb" );

	//�t�@�C�����J�������m�F
	if( fp == NULL ){
		//�G���[���e��\��
		MessageBox(NULL ,"�t�@�C���̓ǂݍ��݂Ɏ��s���܂���" , "LoadFile" , MB_OK);
		//�Ăяo�����ɕԂ�
		return 0;
	}

	//�t�@�C���̖��[�܂œǂݍ���
	while( feof(fp) == 0 ){
	
		//�������ǂݍ���
		fread( word + wordCnt , sizeof(char) , 1 , fp );

		//�󂯎�����������int�^�Ŏ󂯎��
		seek = (int)word[wordCnt];

		//���s������Ύ��̍��ڂ�
		if( word[wordCnt] == 10 && 
			word[wordCnt - 1] == 13 || 
			word[wordCnt] == '/'){

			//�������̃��Z�b�g
			wordCnt = 0;
			//������̏�����
			ZeroMemory( word , sizeof(char) * 256 );
			//���ڐ������Z�b�g
			itemCnt = 0;
			//���������Z�b�g
			digit = 0;
			//���̍��ڂ�
			itemNext++;
			continue;
		}

		//�X�y�[�X������Βl���擾����
		if( isspace( seek ) != 0 || ispunct( seek ) ){
			//�R�s�[�����������10�i���ɕϊ�����
			g_record[thisTime].recordBox[itemNext][itemCnt] = atoi( word + wordCnt - digit );
			//���������Z�b�g
			digit = 0;
			//���̍��ڂ�
			itemCnt++;
		}

		//�������J�E���g
		digit++;
		//���������J�E���g�A�b�v
		wordCnt++;
	}
	//�t�@�C�������
	fclose( fp );
	//�擾�����l��Ԃ�
	return	&g_record[thisTime];
}


//*******************************************************************************************
//�L�^��ۑ�����u��ږ��ɕ����Ċe���ڂ��������ށv
//*******************************************************************************************
void SaveRecordForStage( char* pass ,  RECORD* pre , tagRENEWAL thisTime ){
	
	//�t�@�C���|�C���^
	FILE* fp = NULL;

	//���݂̃X�e�[�W�ԍ����擾
	tagSELECTSTAGE getStage = GetStage();

	//���@����̋L�^���ō��̋L�^�֏�������
	//�t�@�C�����J��
	fp = fopen( pass , "wb" );

	//�t�@�C�����J�������m�F
	if( fp == NULL ){
		//�G���[���e��\��
		MessageBox(NULL , "�t�@�C���̓ǂݍ��݂Ɏ��s���܂���" , "SaveRecordForStage" , MB_OK);
		//�Ăяo�����ɕԂ�
		return;
	}

	//���@����L�^�����e���ڂ�ۑ�����
	for( int j = 0 ; j < RECORD_MAX ; j++ ){
		//�G�������@�ˁ@�A�C�e���l�����@�ˁ@�N���A���ԁ@�ˁ@CHAIN���@�ˁ@�W�v�|�C���g
		//�̏��Ԃŏ�������ł���
		//�e���ڐ��������[�v����
		for( int i = 0 ; i < g_recordScoreItemTbl[getStage][j] ; i++ ){
			
			//����L�^���������ރt�@�C��������L�^�����f�[�^�Ȃ�l���r�p�Ɏ擾����
			//�e���ڂ̒l��������
			if( thisTime == RECORD_PREVIEW ){
				
				g_record[RECORD_PREVIEW].recordBox[j][i] = pre->recordBox[j][i];
			}

			//�e���ڂ̐��ɒB�����甲����
			if( i == g_recordScoreItemTbl[getStage][j] - 1 ){

				//�e���ڐ����������񂾂���s���Ď��̗v�f��
				fprintf( fp , "%d /" , pre->recordBox[j][i] );

				//���̗v�f��
				break;
			}

			//�L�^����������
			fprintf( fp , "%d " , pre->recordBox[j][i] );
		}
	}
	//�t�@�C�������
	fclose(fp);

}

//*************************************************************************************************
//����̋L�^�Ɖߋ��̋L�^���r���ċL�^���X�V����
//*************************************************************************************************
void RenewalRecord( char *compairFile ){


	//���݂̃X�e�[�W�ԍ����擾
	tagSELECTSTAGE getStage = GetStage();

	//�X�V�t���O
	bool renewal = false;

	//���N���A���Ԕ�r�p�ϐ�
	//����̋L�^
	int previewTime = MINUTES_INTO_MILLISECOND( g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][0] ) +//��
					  SECONDS_INTO_MILLISECOND( g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][1] ) +//�b
					  g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][2];							 //�~���b

	//�� �t�@�C���̓ǂݍ���
	//�ō��̋L�^��ǂݍ���
	LoadRecordFile( compairFile , RECORD_BEST );

	//�ō��̋L�^
	int bestTime = MINUTES_INTO_MILLISECOND( g_record[RECORD_BEST].recordBox[RECORD_TIME][0] ) +		//��
				   SECONDS_INTO_MILLISECOND( g_record[RECORD_BEST].recordBox[RECORD_TIME][1] ) +		//�b
				   g_record[RECORD_BEST].recordBox[RECORD_TIME][2];								//�~���b


	//�� ����̋L�^�ƍō��̋L�^���r����
	for( int j = 0 ; j < RECORD_MAX ; j++ ){
		for( int i = 0 ; i < g_recordScoreItemTbl[getStage][j] ; i++ ){
			//if this record is road-stage,
			if (getStage == STAGE_ROAD && j == RECORD_TOTAL && i == 1) {
				if (g_record[RECORD_PREVIEW].recordBox[RECORD_TOTAL][1] < g_record[RECORD_BEST].recordBox[RECORD_TOTAL][1] ||
					g_record[RECORD_BEST].recordBox[RECORD_TOTAL][1] == 0) {
					g_record[RECORD_BEST].recordBox[RECORD_TOTAL][1] = g_record[RECORD_PREVIEW].recordBox[RECORD_TOTAL][1];
					g_record[RECORD_PREVIEW].renewalFlag[RECORD_TOTAL][1] = RECORD_RENEWAL_FLAG;
					renewal = true;
				}
				break;
			}
			//�N���A���Ԃ̔�r
			if( j == RECORD_TIME ){			
				//��r�p�ɕϊ������ϐ����m�Ŕ�ׂ�
				if( previewTime < bestTime || bestTime == 0 ){				
					//�l����
					//��
					g_record[RECORD_BEST].recordBox[RECORD_TIME][0] = 
						g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][0];
					//�b
					g_record[RECORD_BEST].recordBox[RECORD_TIME][1] = 
						g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][1];
					//�~���b
					g_record[RECORD_BEST].recordBox[RECORD_TIME][2] = 
						g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][2];		
					//�e���ږ��̍X�V�t���O�𗧂�����
					g_record[RECORD_PREVIEW].renewalFlag[RECORD_TIME][0] = RECORD_RENEWAL_FLAG;
					//�X�V�t���O�𗧂�����
					renewal = true;
				}
				//������I������玟�̍��ڂ�
				break;
			}
			//�l�̔�r
			//����̋L�^���ō��L�^�������Ă�����A����̋L�^��
			//�ō��̋L�^�ɏ㏑������
			if( g_record[RECORD_PREVIEW].recordBox[j][i] >
				g_record[RECORD_BEST].recordBox[j][i] ){
				//�l����
				g_record[RECORD_BEST].recordBox[j][i] = g_record[RECORD_PREVIEW].recordBox[j][i];
				//�e���ږ��̍X�V�t���O�𗧂�����
				g_record[RECORD_PREVIEW].renewalFlag[j][i] = RECORD_RENEWAL_FLAG;
				//�X�V�t���O�𗧂�����
				renewal = true;
			}
		}
	}

	//�� �ō��̋L�^���X�V����
	//�X�V�t���O��true�Ȃ�X�V����
	if( renewal ){
		//�L�^�̕ۑ�
		SaveRecordForStage( compairFile , &g_record[RECORD_BEST] , RECORD_BEST );
	}

}


/*
************************************************************************************************************
�t�@�C���֋L�^�p
************************************************************************************************************
*/
void	SaveRecord(char* lpPass, int kind, int* lpItem, RECORD* lpRecord) {

	FILE	*lpFile = NULL;

	//���@����̋L�^���ō��̋L�^�֏�������
	//�t�@�C�����J��
	lpFile = fopen(lpPass, "wb");

	//�t�@�C�����J�������m�F
	if (lpFile == NULL) {
		//�G���[���e��\��
		MessageBox(NULL, "�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "SaveRecordEndTheResult", MB_OK);
		//�Ăяo�����ɕԂ�
		return;
	}

	//�����̍��ڂ̎�ނ����������ޏ���
	for (int i = 0; i < kind; i++) {
		for (int j = 0; j < lpItem[i]; j++) {

			//�e���ڂ𖞂������玟�̋L�^��
			if (j == (lpItem[i] - 1)) {
				//�e���ڂ��������񂾂���s���Ď��̗v�f��
				fprintf(lpFile, "%d /", lpRecord->recordBox[i][j]);
				break;
			}
			//�e���ڂ𔼊p�X�y�[�X���󂯂ď�������
			fprintf(lpFile, "%d ", lpRecord->recordBox[i][j]);
		}
	}
	//�t�@�C�������
	fclose(lpFile);

}

/*
*************************************************************************************************
����̋L�^�ƍō��L�^�Ƃ��r���ō��L�^���㏑������
*************************************************************************************************
*/
void	RenewalRecordEndTheResult(char *previewpass, char *bestpass, int kind, int *lpItem) {

	//�L�^�����l�ƍX�V�t���O�����Z�b�g
	InitRecord();

	//�����̃t�@�C���p�X�̃t�@�C������l���擾����
	RECORD* previewRecord = LoadRecordFile(previewpass, RECORD_PREVIEW);
	RECORD* bestRecord = LoadRecordFile(bestpass, RECORD_BEST);

	//�X�V�t���O
	bool renewal = false;

	//���N���A���Ԕ�r�p�ϐ�
	//����̋L�^
	int previewTime = MINUTES_INTO_MILLISECOND(previewRecord->recordBox[0][0]) +//��
		SECONDS_INTO_MILLISECOND(previewRecord->recordBox[0][1]) +				//�b
		previewRecord->recordBox[0][2];											//�~���b

																				//�ō��̋L�^
	int bestTime = MINUTES_INTO_MILLISECOND(bestRecord->recordBox[0][0]) +		//��
		SECONDS_INTO_MILLISECOND(bestRecord->recordBox[0][1]) +					//�b
		bestRecord->recordBox[0][2];											//�~���b

								

	//�����̍��ڂ̎�ނ����������ޏ���
	for (int i = 0; i < kind; i++) {
		for (int j = 0; j < lpItem[i]; j++) {

			//�N���A���Ԃ̔�r
			if (i == 0) {
				//��r�p�ɕϊ������ϐ����m�Ŕ�ׂ�
				if (previewTime < bestTime || bestTime == 0) {

					//�l����
					//��
					bestRecord->recordBox[i][0] =
						previewRecord->recordBox[i][0];
					//�b
					bestRecord->recordBox[i][1] =
						previewRecord->recordBox[i][1];
					//�~���b
					bestRecord->recordBox[i][2] =
						previewRecord->recordBox[i][2];

					//�e���ږ��̍X�V�t���O�𗧂�����
					previewRecord->renewalFlag[0][0] = RECORD_RENEWAL_FLAG;
					//�X�V�t���O�𗧂�����
					renewal = true;
				}
				//������I������玟�̍��ڂ�
				break;
			}

			//�l�̔�r
			//����̋L�^���ō��L�^�������Ă�����A����̋L�^��
			//�ō��̋L�^�ɏ㏑������
			if (previewRecord->recordBox[i][j] >
				bestRecord->recordBox[i][j]) {
				//�l����
				bestRecord->recordBox[i][j] = previewRecord->recordBox[i][j];
				//�e���ږ��̍X�V�t���O�𗧂�����
				previewRecord->renewalFlag[i][j] = RECORD_RENEWAL_FLAG;
				//�X�V�t���O�𗧂�����
				renewal = true;

			}
		}
	}

	//�� �ō��̋L�^���X�V����
	//�X�V�t���O��true�Ȃ�X�V����
	if (renewal) SaveRecord(bestpass, kind, lpItem, bestRecord);

}


//***********************************************************************************************
//�L�^��Ԃ��֐�
//***********************************************************************************************
RECORD* GetRecord( tagRENEWAL record ){	return &g_record[record]; }


/*
************************************************************************************************************
�L�^�����Z�b�g����
************************************************************************************************************
*/
void	ResetRecord(char* lpPass, int kind, int* lpItem,  tagRENEWAL thisTime) {

	FILE	*lpFile = NULL;

	//���ڃJ�E���g�p
	int cnt = 0;

	//���@����̋L�^���ō��̋L�^�֏�������
	//�t�@�C�����J��
	lpFile = fopen(lpPass, "wb");

	//�t�@�C�����J�������m�F
	if (lpFile == NULL) {
		//�G���[���e��\��
		MessageBox(NULL, "�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "ResetRecord", MB_OK);
		//�Ăяo�����ɕԂ�
		return;
	}

	//�����̍��ڂ̎�ނ����������ޏ���
	for (int i = 0; i < kind; i++) {
		for (int j = 0; j < lpItem[i]; j++) {

			//�e���ڂ𖞂������玟�̋L�^��
			if (j == (lpItem[i] - 1)) {
				//�e���ڂ��������񂾂���s���Ď��̗v�f��
				fprintf(lpFile, "%d /", 0);
				//���ڂ��J�E���g
				cnt++;
				break;
			}
			//�e���ڂ𔼊p�X�y�[�X���󂯂ď�������
			fprintf(lpFile, "%d ", 0);
			//���ڂ��J�E���g
			cnt++;
		}
	}
	//�t�@�C�������
	fclose(lpFile);
}

/*
Compair time record
return is count of lpCompair1's ranking
*/
int		CompairRaceTimeRecord(TIME* lpCompair1, TIME* lpCompair2) {

	//for compairing variable
	int compairPoint1 = MINUTES_INTO_MILLISECOND(lpCompair1->minutes) +
		SECONDS_INTO_MILLISECOND(lpCompair1->second) +
		lpCompair1->millisecond;
	//competitor
	int compairPoint2 = MINUTES_INTO_MILLISECOND(lpCompair2->minutes) +
		SECONDS_INTO_MILLISECOND(lpCompair2->second) +
		lpCompair2->millisecond;
	//count ranking
	int count = 0;
	//convert minute and second into millisecond
	if (compairPoint1 > compairPoint2)  count = 1;
	return count;
}

/*
Compair record more than time record
*/
int		CompairRecord(int compair1, int compair2) {
	//count ranking
	int count = 0;
	//compair each value
	if (compair1 < compair2) count = 1;
	return count;
}

/*
Save competitors' records in this race only
competitors' temporary records
*/
void	SaveCompetitorRecordTemporary(RECORD *lpRecordPoint, tagRECORDMATRIX kind, tagSELECTSTAGE stage) {

	//loop kind of competitor, that is nine
	for (int j = 0; j < g_recordScoreItemTbl[stage][kind]; j++) {
		for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
			//aggregate point into global variable
			g_RecordForCompetitors[stage][i].recordBox[kind][j] = lpRecordPoint[i].recordBox[kind][j];
		}
	}
}

/*
get competitors' records
*/
RECORD*	GetRecordCompetitors(tagSELECTSTAGE stage) { return g_RecordForCompetitors[stage]; }

//************************************************************************************************
//���p�̐��l��S�p�ɕϊ�����
//************************************************************************************************
char* ConvertNum( int num ){

	//�����𕶎���ɕϊ��p
	char dummy[256] = {0};

	//�������P�O�i���̕�����ɕϊ�
	itoa(num , dummy , 10);

	//�󂯎���������̕�����̒������擾
	int len = strlen(dummy);

	//�S�p�����߂�l�p
	char *reply = new char[len];

	//�������ړ��p
	int p = 0;

	//�m�ۂ����T�C�Y�������[�v���đS�p�ɕϊ�
	for( int i = 0 ; i < len ; i++ ){
		switch(dummy[i]){
			case '0':
				strcpy(&reply[p] , "�O");
				p += 2;
				break;
			case '1':
				strcpy(&reply[p] , "�P");
				p += 2;
				break;
			case '2':
				strcpy(&reply[p] , "�Q");
				p += 2;
				break;
			case '3':
				strcpy(&reply[p] , "�R");
				p += 2;
				break;
			case '4':
				strcpy(&reply[p] , "�S");
				p += 2;
				break;
			case '5':
				strcpy(&reply[p] , "�T");
				p += 2;
				break;
			case '6':
				strcpy(&reply[p] , "�U");
				p += 2;
				break;
			case '7':
				strcpy(&reply[p] , "�V");
				p += 2;
				break;
			case '8':
				strcpy(&reply[p] , "�W");
				p += 2;
				break;
			case '9':
				strcpy(&reply[p] , "�X");
				p += 2;
				break;
		}
	}

	//�n���ꂽ���l��S�p�ŕԂ�
	return reply;
}


//**********************************************************************************************
//�m�ۂ������������p�̊֐�
//**********************************************************************************************
void DeleteChar( char** reply ){

	//����̏���
	delete[] *(reply);
	*(reply) = NULL;
}