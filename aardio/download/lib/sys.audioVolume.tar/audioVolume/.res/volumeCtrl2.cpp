
#include "stdafx.h"
#include "StdAfx.h"
#include "VolumeCtrl2.h"
#pragma comment(lib, "Winmm.lib")

extern "C" __declspec(dllexport) CVolumeCtrl2* __cdecl CreateVolumeCtrl() { 
    return new CVolumeCtrl2();
}
    
extern "C" __declspec(dllexport) void __cdecl DeleteVolumeCtrl( CVolumeCtrl2* pCtrl) {
    delete pCtrl;
}

extern "C" __declspec(dllexport) int __cdecl GetVolume(CVolumeCtrl2* pCtrl) {
	return pCtrl->GetVolume();
}

extern "C" __declspec(dllexport) void __cdecl SetVolume(CVolumeCtrl2* pCtrl,int nVol) {
	pCtrl->SetVolume(nVol);;
}

extern "C" __declspec(dllexport) BOOL __cdecl GetMute(CVolumeCtrl2* pCtrl) {
	return pCtrl->GetMute();
}

extern "C" __declspec(dllexport) void __cdecl SetMute(CVolumeCtrl2* pCtrl,BOOL bMute) {
	pCtrl->SetMute(bMute);;
}


CVolumeCtrl2::CVolumeCtrl2(void)
	:m_pEndptVolCtrl(NULL),
	m_pEnumerator(NULL),
	m_pDevice(NULL),
	m_guidMyContext(GUID_NULL)
{
	m_bVistaLater = true;

	OSVERSIONINFO info;
	ZeroMemory(&info, sizeof(OSVERSIONINFO));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&info))
	{
		if(info.dwMajorVersion <= 5)
		{
			m_bVistaLater = false;
		} 
	}

	if( m_bVistaLater )
	{
		InitEndPointVistaLater();
	}
}

CVolumeCtrl2::~CVolumeCtrl2(void)
{
	if(m_bVistaLater)
	{
		UninitEndPointVistaLater();
	} 
}

int CVolumeCtrl2::GetVolume()
{
	if(!m_bVistaLater){
		return GetVolumeXp(0); 
	}
	else{
		DWORD dwNewVol = 0;
		GetVolumeVistaLater(&dwNewVol);
		return dwNewVol;
	}
}

BOOL CVolumeCtrl2::SetVolume(int nVol)
{
	if(!m_bVistaLater){
		long nXPSetVol = nVol;
		return SetVolumeXp(0, nXPSetVol);
	}
	else{
		int nWin7SetVol = nVol ;
		return SetVolumeVistaLater(nWin7SetVol);
	}
}

BOOL CVolumeCtrl2::SetMute(BOOL bMute)
{
	if(!m_bVistaLater){
		return SetMuteXp(0, bMute);
	}
	else{
		return SetMuteVistaLater(bMute);
	}
}

BOOL CVolumeCtrl2::GetMute()
{
	if(!m_bVistaLater){
		return GetMuteXp(0);
	}
	else{
		return GetMuteVistaLater();
	}
}


unsigned int CVolumeCtrl2::GetVolumeXp(int   dev)
{

	long   device;
	unsigned   int rt=0;
	MIXERCONTROL   volCtrl;
	HMIXER   hmixer;
	switch   (dev)
	{
	case   1:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;
	case   2:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;
	case   3:
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;
	if(!GetVolumeControlXp(hmixer,device,MIXERCONTROL_CONTROLTYPE_VOLUME,&volCtrl))
		return   0;
	rt=GetVolumeValueXp(hmixer,&volCtrl)*MAX_VOL/volCtrl.Bounds.lMaximum;
	mixerClose(hmixer);
	return   rt;

}


BOOL CVolumeCtrl2::SetVolumeXp(long dev,long   vol)
{ 
	long   device;
	BOOL   rc = FALSE;
	MIXERCONTROL   volCtrl;
	HMIXER   hmixer;
	switch   (dev)
	{
	case   1:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;
	case   2:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;
	case   3:
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;

	if(GetVolumeControlXp(hmixer,device,MIXERCONTROL_CONTROLTYPE_VOLUME,&volCtrl))
	{
		vol=vol*volCtrl.Bounds.lMaximum/MAX_VOL;
		if(SetVolumeValueXp(hmixer,&volCtrl,vol))
			rc=true;
	}
	mixerClose(hmixer);
	return   rc;
}

BOOL CVolumeCtrl2::SetMuteXp(long   dev,long   vol)
{
	long   device;
	BOOL   rc = FALSE;
	MIXERCONTROL   volCtrl;
	HMIXER   hmixer;
	switch   (dev)
	{
	case   1:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;
	case   2:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;
	case   3:
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;

	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;
	if(GetVolumeControlXp(hmixer,device,MIXERCONTROL_CONTROLTYPE_MUTE,&volCtrl))
		if(SetMuteValueXp(hmixer,&volCtrl,(BOOL)vol))
			rc=TRUE;
	mixerClose(hmixer);
	return   rc;
}


BOOL CVolumeCtrl2::GetMuteXp(long   dev)
{
	long   device;
	BOOL   rc = FALSE;
	MIXERCONTROL   volCtrl;
	HMIXER   hmixer;
	switch   (dev)
	{
	case   1:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;
	case   2:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;
	case   3:
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;
	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}

	if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;

	if(GetVolumeControlXp(hmixer,device,MIXERCONTROL_CONTROLTYPE_MUTE,&volCtrl))
		rc = GetMuteValueXp(hmixer,&volCtrl);
	mixerClose(hmixer);
	return   rc;

}

BOOL  CVolumeCtrl2::GetVolumeControlXp(HMIXER   hmixer   ,long   componentType,long   ctrlType,MIXERCONTROL*   mxc)
{
	MIXERLINECONTROLS   mxlc;
	MIXERLINE   mxl;
	mxl.cbStruct   =   sizeof(mxl);
	mxl.dwComponentType   =   componentType;
	if(!mixerGetLineInfo((HMIXEROBJ)hmixer,   &mxl,   MIXER_GETLINEINFOF_COMPONENTTYPE))
	{
		mxlc.cbStruct   =   sizeof(mxlc);
		mxlc.dwLineID   =   mxl.dwLineID;
		mxlc.dwControlType   =   ctrlType;
		mxlc.cControls   =   1;
		mxlc.cbmxctrl   =   sizeof(MIXERCONTROL);
		mxlc.pamxctrl   =   mxc;

		if(mixerGetLineControls((HMIXEROBJ)hmixer,&mxlc,MIXER_GETLINECONTROLSF_ONEBYTYPE))
			return   FALSE;
		else
		{
			return   TRUE;
		}
	}
	return   FALSE;
}

long CVolumeCtrl2::GetMuteValueXp(HMIXER   hmixer   ,MIXERCONTROL   *mxc)
{
	MIXERCONTROLDETAILS   mxcd;
	MIXERCONTROLDETAILS_BOOLEAN   mxcdMute;
	mxcd.hwndOwner   =   0;
	mxcd.cbStruct   =   sizeof(mxcd);
	mxcd.dwControlID   =   mxc-> dwControlID;
	mxcd.cbDetails   =   sizeof(mxcdMute);
	mxcd.paDetails   =   &mxcdMute;
	mxcd.cChannels   =   1;
	mxcd.cMultipleItems   =   0;
	if   (mixerGetControlDetails((HMIXEROBJ)hmixer,   &mxcd,MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE))
		return   -1;
	return   mxcdMute.fValue;
}

unsigned int CVolumeCtrl2::GetVolumeValueXp(HMIXER   hmixer   ,MIXERCONTROL   *mxc)
{
	MIXERCONTROLDETAILS   mxcd;
	MIXERCONTROLDETAILS_UNSIGNED   vol;   vol.dwValue=0;
	mxcd.hwndOwner   =   0;
	mxcd.cbStruct   =   sizeof(mxcd);
	mxcd.dwControlID   =   mxc-> dwControlID;
	mxcd.cbDetails   =   sizeof(vol);
	mxcd.paDetails   =   &vol;
	mxcd.cChannels   =   1;
	if(mixerGetControlDetails((HMIXEROBJ)hmixer,   &mxcd,   MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE))
		return   -1;
	return   vol.dwValue;
}

BOOL CVolumeCtrl2::SetMuteValueXp(HMIXER   hmixer   ,MIXERCONTROL   *mxc,   BOOL   mute)
{
	MIXERCONTROLDETAILS   mxcd;
	MIXERCONTROLDETAILS_BOOLEAN   mxcdMute;mxcdMute.fValue=mute;
	mxcd.hwndOwner   =   0;
	mxcd.dwControlID   =   mxc-> dwControlID;
	mxcd.cbStruct   =   sizeof(mxcd);
	mxcd.cbDetails   =   sizeof(mxcdMute);
	mxcd.paDetails   =   &mxcdMute;
	mxcd.cChannels   =   1;
	mxcd.cMultipleItems   =   0;
	if   (mixerSetControlDetails((HMIXEROBJ)hmixer,   &mxcd,   MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))
		return   FALSE;
	return TRUE;
}


BOOL CVolumeCtrl2::SetVolumeValueXp(HMIXER   hmixer   ,MIXERCONTROL   *mxc,   long   volume)
{
	MIXERCONTROLDETAILS   mxcd;
	MIXERCONTROLDETAILS_UNSIGNED   vol;vol.dwValue   =   volume;
	mxcd.hwndOwner   =   0;
	mxcd.dwControlID   =   mxc-> dwControlID;
	mxcd.cbStruct   =   sizeof(mxcd);
	mxcd.cbDetails   =   sizeof(vol);
	mxcd.paDetails   =   &vol;
	mxcd.cChannels   =   1;
	if(mixerSetControlDetails((HMIXEROBJ)hmixer,   &mxcd,   MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))
		return   FALSE;
	return   TRUE;
}


BOOL CVolumeCtrl2::InitEndPointVistaLater()
{
	if(m_pEndptVolCtrl != NULL)
		return FALSE;

	//g_EPVolEvents.SetCallback(fCallback); //set callback

	HRESULT hr = S_OK;

	//CoInitialize(NULL);

	hr = CoCreateGuid(&m_guidMyContext);
	if(FAILED(hr))
		return FALSE;

	// Get enumerator for audio endpoint devices.
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&m_pEnumerator);
	if(FAILED(hr))
		return FALSE;

	// Get default audio-rendering device.
	hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
	if(FAILED(hr))
		return FALSE;

	hr = m_pDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL, NULL, (void**)&m_pEndptVolCtrl);
	if(FAILED(hr))
		return FALSE;


	if(FAILED(hr))
		return FALSE;

	return TRUE;
}


BOOL CVolumeCtrl2::SetMuteVistaLater(BOOL bMute)
{
	if(m_pEndptVolCtrl)
		m_pEndptVolCtrl->SetMute(bMute, &m_guidMyContext);
	return TRUE;
}

BOOL CVolumeCtrl2::GetMuteVistaLater()
{
	BOOL bMute = FALSE;
	if(m_pEndptVolCtrl)
		m_pEndptVolCtrl->GetMute(&bMute);
	return bMute;
}

BOOL CVolumeCtrl2::SetVolumeVistaLater(DWORD dwVolume)
{
	if((int)dwVolume < 0)
		dwVolume = 0;
	if((int)dwVolume > MAX_VOL)
		dwVolume = MAX_VOL;

	if(m_pEndptVolCtrl)
	{
		HRESULT hr = m_pEndptVolCtrl->SetMasterVolumeLevelScalar((float)dwVolume / MAX_VOL, &m_guidMyContext);
		return SUCCEEDED(hr) ? TRUE : FALSE;
	}

	return FALSE;
}

BOOL CVolumeCtrl2::GetVolumeVistaLater(DWORD* pdwVolume)
{
	float fVolume;

	if(m_pEndptVolCtrl)
	{
		m_pEndptVolCtrl->GetMasterVolumeLevelScalar(&fVolume);
		if(pdwVolume != NULL)
			*pdwVolume =(DWORD)( MAX_VOL * fVolume + 0.5);
	}

	return TRUE;
}


BOOL CVolumeCtrl2::UninitEndPointVistaLater()
{
	m_pEnumerator->Release();
	m_pDevice->Release();
	m_pEndptVolCtrl->Release();

	m_pEnumerator = NULL;
	m_pDevice  = NULL;
	m_pEndptVolCtrl  = NULL;
	//CoUninitialize();
	return TRUE;
}
