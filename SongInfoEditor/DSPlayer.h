// DSPlayer.h: interface for the CDSPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSPLAYER_H__E7C4CC7A_016F_48F9_80A1_1BA32303D7B5__INCLUDED_)
#define AFX_DSPLAYER_H__E7C4CC7A_016F_48F9_80A1_1BA32303D7B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/****************************************************************************************************************
		投稿说明：
****************************************************************************************************************/
#define NOTE_CDSPlayer \
_T("\r\n\
	◆◆◆ 《DirectShow多声卡播放器》◆◆◆\r\n\
	一个用DirectShow开发的音频播放程序，支持多个声卡，可以任意指定播放声卡，可以播放mp3,wav,cda,avi,asf,wmv,mpg等所有\
windows支持的多媒体视频、音频文件，还可以直接播放CD。我已经封装好一个类，还有测试程序，源代码您可以直接使用。\r\n\
	我刚接手做这个多声卡播放程序的时候也毫无头绪，我以前只用过wavOpenXXX()和mciSendXXX()系列API来播放音频\
文件，但是这些方法根本不支持多声卡，播放时的声音只能从系统默认的声卡中发出来，后来想到一个办法，在程序启动时将系统\
默认声卡改成我要播放的声卡，程序退出时再还原，可这样改来改去总会有问题。没办法，就到网上到处找资料，希望能得到指点，\
终于皇天不负有心人，我终于找到一个《多声卡SDK开发包》控件，我满心高兴地下载安装并试用，原来这是一个需要注册的控件，真是\
有违开源的思想啊，不就是一个多声卡播放程序，犯得着用来赚钱吗？你不开源也就得了，还弄个需要注册，不注册还不能用！\
	实在没辙那就自己写好了。\r\n\
\r\n\
	◆◆◆ 作者 ◆◆◆\r\n\
    谢红伟 · chrys · chrys@163.com · http://ndxz.126.com\r\n\
\r\n\
	◆◆◆ 日期 ◆◆◆\r\n\
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
