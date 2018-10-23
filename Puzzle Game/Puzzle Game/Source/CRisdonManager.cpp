#include	"CRisdonManager.h"
#include	"CRisdonEffect.h"
#include	"CRisdon.h"
#include	"graphics.h"

//redefine the static member
HWND	CRisdonManager::m_hWnd = NULL;
const TPOINT<float>	CRisdonManager::RISDONMANAGER_IMAGE_RISDON_DEFAULT_COORDINATE_IN_PLAYING_PUZZLE = { 20.0f, 280.0f };
const int	CRisdonManager::RISDONMANAGER_KIND_OF_IMAGE = 2;
const TPOINT<int>	CRisdonManager::RISDONMANAGER_IMAGE_RISDON_DEFAULT_SIZE = { 64, 64 };
const float			CRisdonManager::RISDONMANAGER_IMAGE_DEFAULT_SCALE = { 1.0f };

//set risdon's aniamtion
//type 1 perspiration
const TPOINT<int>	CRisdonManager::RISDONEFFECT_IMAGE_PERSPIRATION_SIZE = { 32, 32 };
int		CRisdonManager::RISDONEFFECT_IMAGE_PERSPIRATION_COUNT_MAX = 3;
int		CRisdonManager::RISDONEFFECT_IMAGE_PERSPIRATION_FRAM = 20;

CRisdonManager::CRisdonManager() {}

CRisdonManager::~CRisdonManager() {}


void	CRisdonManager::InitManagerCharacter() {

	//kind of image
	char	*lpImage[CRisdonManager::RISDONMANAGER_KIND_OF_IMAGE] = {
		"Image\\Ch\\risdon.bmp",				//risdon
		"Image\\Ch\\Perspiration.bmp",			//risdon's efffect, that is perspiration
	};
	//allot each members
	this->m_lpImage = new int[CRisdonManager::RISDONMANAGER_KIND_OF_IMAGE];		//image's material
	this->m_lpCRisdon = new CRisdon;											//for risdon's setting
	this->m_lpRisdonEffect = new CRisdonEffect[RISDONMANAGER_KIND_OF_EFFECT];	//for risdon's effect

	//load image's materials
	for (int i = 0; i < CRisdonManager::RISDONMANAGER_KIND_OF_IMAGE; i++) { this->m_lpImage[i] = CreateDDImage(lpImage[i], CRisdonManager::m_hWnd); }

	//set material of image to ridon
	this->m_lpCRisdon->SetCharaImg(this->m_lpImage[0]);
	//initialize the risdon
	this->m_lpCRisdon->InitChara(
		CRisdonManager::RISDONMANAGER_IMAGE_RISDON_DEFAULT_COORDINATE_IN_PLAYING_PUZZLE,
		CRisdonManager::RISDONMANAGER_IMAGE_RISDON_DEFAULT_SIZE,
		CRisdonManager::RISDONMANAGER_IMAGE_DEFAULT_SCALE);
	this->m_lpCRisdon->m_existFlag = true;

	//get risdon's each setting
	this->m_GetPos = this->m_lpCRisdon->m_Fpos;
	this->m_GetSize = this->m_lpCRisdon->m_Isize;
	this->m_GetMove = this->m_lpCRisdon->m_Fmove;

	//initializer for risdon's animation
	//kind of effect
	tagRISDONEFFECTKIND effectKind[RISDONMANAGER_KIND_OF_EFFECT] = {
		RISDONMANAGER_EFFECT_TYPE_PERSPIRATION,					//perspiration
	};
	for (int i = 0; i < RISDONMANAGER_KIND_OF_EFFECT; i++) {
		this->m_lpRisdonEffect[i].SetAnimation(							//initializer for animation
			this->m_lpRisdonEffect[i].m_Ipic,							//origin coordinate
			CRisdonManager::RISDONEFFECT_IMAGE_PERSPIRATION_SIZE,		//size
			CRisdonManager::RISDONEFFECT_IMAGE_PERSPIRATION_COUNT_MAX,	//count max
			CRisdonManager::RISDONEFFECT_IMAGE_PERSPIRATION_FRAM,		//fram
			effectKind[i]);												//type
		//initializer for drawing of effect
		this->m_lpRisdonEffect[i].InitChara(CRisdonManager::RISDONEFFECT_IMAGE_PERSPIRATION_SIZE);
		
		//set material of imagge
		this->m_lpRisdonEffect[i].SetCharaImg(this->m_lpImage[1]);		//for drawing material
		this->m_lpRisdonEffect[i].m_existFlag = true;
	}
}

void	CRisdonManager::UpdateManagerCharacter() {

	//each related risdon update 
	this->m_lpCRisdon->UpdateChara();			//risdon own

	//kind of effect
	tagRISDONEFFECTKIND effectKind[RISDONMANAGER_KIND_OF_EFFECT] = {
		RISDONMANAGER_EFFECT_TYPE_PERSPIRATION,					//perspiration
	};
	//loop to kind of effect
	TPOINT<float> effectPos[RISDONMANAGER_KIND_OF_EFFECT] = { 0 };
	for (int i = 0; i < RISDONMANAGER_KIND_OF_EFFECT; i++) {
		if (this->m_lpRisdonEffect[i].m_existFlag) {
			//subsutitute each effect's position for present indication
			effectPos[i].x = this->m_lpCRisdon->m_Fpos.x +										//X
				((this->m_lpCRisdon->m_Isize.x - this->m_lpRisdonEffect[i].m_Isize.x) / 2);
			effectPos[i].y = this->m_lpCRisdon->m_Fpos.y - this->m_lpRisdonEffect[i].m_Isize.y;	//Y
			//subsutitute present risdon's position for position of risdon's animation
			this->m_lpRisdonEffect[i].SetPosition(effectPos[i]);
			
			this->m_lpRisdonEffect[i].UpdateReverseAnimation(		//update animation
				&this->m_lpRisdonEffect[i].m_Ipic,					//origin indication
				(int)effectKind[i],									//kind of effect
				false);												//flag of reverse aniamtion
		}
	}
}

void	CRisdonManager::DrawManagerCharacter() {

	//draw ridon own
	this->m_lpCRisdon->DrawChara();
	//loop to kind of effect
	for (int i = 0; i < RISDONMANAGER_KIND_OF_EFFECT; i++) {		//draw each effect of risdon
		this->m_lpRisdonEffect[i].DrawChara(DRAWING_TYPE_DDBLT, DDSCALE_STARTING_POINT_NOTHING);
	}
}

void	CRisdonManager::ReleaseManagerCharacter() {
	//material of image
	for (int i = 0; i < CRisdonManager::RISDONMANAGER_KIND_OF_IMAGE; i++) { ReleaseDDImage(this->m_lpImage[i]); }
	//risdon own
	this->m_lpCRisdon->ReleaseChara();
	//loop to kind of effect
	for (int i = 0; i < RISDONMANAGER_KIND_OF_EFFECT; i++) {
		this->m_lpRisdonEffect[i].ReleaseChara();		//effect  of risdon
	}
	delete[]this->m_lpImage;			//for image
	this->m_lpImage = NULL;
	delete this->m_lpCRisdon;			//for risdon
	this->m_lpCRisdon = NULL;
	delete[]this->m_lpRisdonEffect;		//effect of risdon
	this->m_lpRisdonEffect = NULL;
}