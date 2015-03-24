// DSPlayer.h: interface for the CDSPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSPLAYER_H__E7C4CC7A_016F_48F9_80A1_1BA32303D7B5__INCLUDED_)
#define AFX_DSPLAYER_H__E7C4CC7A_016F_48F9_80A1_1BA32303D7B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/****************************************************************************************************************
		Ͷ��˵����
****************************************************************************************************************/
#define NOTE_CDSPlayer \
_T("\r\n\
	������ ��DirectShow��������������������\r\n\
	һ����DirectShow��������Ƶ���ų���֧�ֶ����������������ָ���������������Բ���mp3,wav,cda,avi,asf,wmv,mpg������\
windows֧�ֵĶ�ý����Ƶ����Ƶ�ļ���������ֱ�Ӳ���CD�����Ѿ���װ��һ���࣬���в��Գ���Դ����������ֱ��ʹ�á�\r\n\
	�Ҹս�����������������ų����ʱ��Ҳ����ͷ��������ǰֻ�ù�wavOpenXXX()��mciSendXXX()ϵ��API��������Ƶ\
�ļ���������Щ����������֧�ֶ�����������ʱ������ֻ�ܴ�ϵͳĬ�ϵ������з������������뵽һ���취���ڳ�������ʱ��ϵͳ\
Ĭ�������ĳ���Ҫ���ŵ������������˳�ʱ�ٻ�ԭ��������������ȥ�ܻ������⡣û�취���͵����ϵ��������ϣ�ϣ���ܵõ�ָ�㣬\
���ڻ��첻�������ˣ��������ҵ�һ����������SDK���������ؼ��������ĸ��˵����ذ�װ�����ã�ԭ������һ����Ҫע��Ŀؼ�������\
��Υ��Դ��˼�밡��������һ�����������ų��򣬷���������׬Ǯ���㲻��ԴҲ�͵��ˣ���Ū����Ҫע�ᣬ��ע�ỹ�����ã�\
	ʵ��û���Ǿ��Լ�д���ˡ�\r\n\
\r\n\
	������ ���� ������\r\n\
    л��ΰ �� chrys �� chrys@163.com �� http://ndxz.126.com\r\n\
\r\n\
	������ ���� ������\r\n\
    2006-07-05 15:49:28")

// Filter graph notification to the specified window
#define WM_GRAPHNOTIFY  (WM_USER+300)
#ifdef _DEBUG
#define _OPENTYPE _T("Media Files|*.mp3;*.wav;*.wma;*.cda;*.avi;*.asf;*.wmv;*.rmvb;*.rm;*.dat;*.mpg;*.mpeg|")
#else
#define _OPENTYPE _T("Music Files(*.mp3;*.wav;*.wma;*.cda)|*.mp3;*.wav;*.wma;*.cda|Video Files(*.avi;*.asf;*.wmv;*.rmvb;*.rm)|*.avi;*.asf;*.wmv;*.rmvb;*.rm|Mpeg Video Files(*.dat;*.mpg;*.mpeg)|*.dat;*.mpg;*.mpeg;*.mpe|All Files(*.*)|*.*|")
#endif
#define DRIVER_ROOT_LENGTH 2

#include <streams.h>

int hwprGetCDromDriver ( OUT CString &csCDROMDriver );
int GetCDTrackNum ( LPCTSTR lpszCDROM );

class CDSPlayer  
{
private:
	IGraphBuilder *     mGraph;  
	IMediaControl *		mMediaControl;
	IMediaEventEx *		mEvent;
	IBasicVideo *		mBasicVideo;
	IBasicAudio *		mBasicAudio;
	IVideoWindow  *		mVideoWindow;
	IMediaSeeking *		mSeeking;
	IBaseFilter *		mAudioFilter;
	ICreateDevEnum *	m_pSysDevEnum;

	DWORD				mObjectTableEntry;
	CStringArray		m_StrAry_AudioDevice;
	CPtrArray			m_PtrAry_Filter_AudioDevice;
	CString				m_csMediaFileName;
	CString				m_csAudioDeviceName;

public:
	CDSPlayer();
	virtual ~CDSPlayer();
	CString GetAudioDeviceName()
	{
		return m_csAudioDeviceName;
	}
	CString GetMediaFileName()
	{
		return m_csMediaFileName;
	}
	void ClearMediaFileName()
	{
		m_csMediaFileName = _T("");
	}
	BOOL Delete_DefaultDirectSoundDevice();

public:
	HWND m_hWndNotify, m_hWndDisplay;
	BOOL SetAudioDevice ( CString strContainText, BOOL bInclude=TRUE, CString *pcsAudioDeviceFullName=NULL );
	int FindAudioDeivceIDByName ( CString strContainText, BOOL bInclude, OUT CString *pcsAudioDeviceFullName=NULL);
	void GetAudioDeviceList ( OUT CStringArray &StrAry_AudioDevice );
	BOOL IsInitedOK();
	virtual BOOL Create(void);
	virtual void Release(void);
	virtual BOOL Attach(IGraphBuilder * inGraphBuilder);

	IGraphBuilder * GetGraph(void); // Not outstanding reference count
	IMediaEventEx * GetEventHandle(void);

	BOOL ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	void DisconnectFilters(IPin * inOutputPin);

	BOOL SetDisplayWindow(HWND inWindow);
	BOOL SetNotifyWindow(HWND inWindow);
	BOOL ResizeVideoWindow(long inLeft, long inTop, long inWidth, long inHeight);
	void HandleEvent(WPARAM inWParam, LPARAM inLParam);

	BOOL Run(void);        // Control filter graph
	BOOL Stop(void);
	BOOL Pause(void);
	BOOL IsRunning(void);  // Filter graph status
	BOOL IsStopped(void);
	BOOL IsPaused(void);

	BOOL SetFullScreen(BOOL inEnabled);
	BOOL GetFullScreen(void);

	// IMediaSeeking
	BOOL GetCurrentPosition(double * outPosition);
	BOOL GetStopPosition(double * outPosition);
	BOOL SetCurrentPosition(double inPosition);
	BOOL SetStartStopPosition(double inStart, double inStop);
	BOOL GetDuration(double * outDuration);
	BOOL SetPlaybackRate(double inRate);

	// Attention: range from -10000 to 0, and 0 is FULL_VOLUME.
	BOOL SetAudioVolume(long inVolume);
	long GetAudioVolume(void);
	// Attention: range from -10000(left) to 10000(right), and 0 is both.
	BOOL SetAudioBalance(long inBalance);
	long GetAudioBalance(void);

	BOOL RenderFile(const TCHAR * inFile);
	BOOL SnapshotBitmap(const TCHAR * outFile);
	void SetAudioDeviceName(CString strAudioDeviceName);

private:
	BOOL EnumAudioDevice ();
	IBaseFilter* FindFilters(const TCHAR *szFilterName);
	void AddToObjectTable(void) ;
	void RemoveFromObjectTable(void);
	
	BOOL QueryInterfaces(void);
	
	
};

#endif // !defined(AFX_DSPLAYER_H__E7C4CC7A_016F_48F9_80A1_1BA32303D7B5__INCLUDED_)
