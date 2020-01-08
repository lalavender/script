
#pragma once
#include <Windows.h>
#include <MMSystem.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#define MAX_VOL 100

class CVolumeCtrl2
{
public:
	CVolumeCtrl2(void);
	~CVolumeCtrl2(void);

public:
	virtual	int GetVolume();
	virtual	BOOL SetVolume(int nVol);
	virtual	BOOL SetMute(BOOL bMute);
	virtual	BOOL GetMute();

protected:
	unsigned int GetVolumeXp(int   dev);
	BOOL SetVolumeXp(long   dev,long   vol);
	BOOL SetMuteXp(long   dev,long   vol);
	BOOL GetMuteXp(long   dev);

	BOOL   GetVolumeControlXp(HMIXER   hmixer   ,long   componentType,long   ctrlType,MIXERCONTROL*   mxc) ;
	unsigned int GetVolumeValueXp(HMIXER   hmixer   ,MIXERCONTROL   *mxc) ;
	BOOL   SetVolumeValueXp(HMIXER   hmixer   ,MIXERCONTROL   *mxc,   long   volume) ;
	long   GetMuteValueXp(HMIXER   hmixer   ,MIXERCONTROL   *mxc) ;
	BOOL   SetMuteValueXp(HMIXER   hmixer   ,MIXERCONTROL   *mxc,   BOOL   mute) ;

protected:
	BOOL InitEndPointVistaLater();
	BOOL SetMuteVistaLater(BOOL bMute);
	BOOL GetMuteVistaLater();
	BOOL SetVolumeVistaLater(DWORD dwVolume);
	BOOL GetVolumeVistaLater(DWORD* pdwVolume);
	BOOL UninitEndPointVistaLater();
protected: 
	bool m_bVistaLater;

protected:
	IAudioEndpointVolume *m_pEndptVolCtrl;
	IMMDeviceEnumerator *m_pEnumerator;
	IMMDevice *m_pDevice;
	GUID m_guidMyContext;
};

