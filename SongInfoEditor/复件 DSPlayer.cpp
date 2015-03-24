// DSPlayer.cpp: implementation of the CDSPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dllDSPlayer.h"
#include <streams.h>
#include "DSPlayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDSPlayer::CDSPlayer()
{
	mGraph        = NULL;
	mMediaControl = NULL;
	mEvent        = NULL;
	mBasicVideo   = NULL;
	mBasicAudio   = NULL;
	mVideoWindow  = NULL;
	mSeeking      = NULL;
	mAudioFilter  = NULL;
	m_pSysDevEnum = NULL;
	m_hWndNotify  = NULL;
	m_hWndDisplay = NULL;

	mObjectTableEntry = 0;
}

CDSPlayer::~CDSPlayer()
{
	Release();
}

BOOL CDSPlayer::Create(void)
{
	if (!mGraph)
	{
		if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder, (void **)&mGraph)))
		{
			AddToObjectTable();

			if ( !QueryInterfaces() )
				return FALSE;
			return TRUE;
		}
		mGraph = 0;
	}
	return FALSE;
}

BOOL CDSPlayer::QueryInterfaces(void)
{
	if (mGraph)
	{
		HRESULT hr = NOERROR;
		hr |= mGraph->QueryInterface(IID_IMediaControl, (void **)&mMediaControl);
		hr |= mGraph->QueryInterface(IID_IMediaEventEx, (void **)&mEvent);
		hr |= mGraph->QueryInterface(IID_IBasicVideo, (void **)&mBasicVideo);
		hr |= mGraph->QueryInterface(IID_IBasicAudio, (void **)&mBasicAudio);
		hr |= mGraph->QueryInterface(IID_IVideoWindow, (void **)&mVideoWindow);
		hr |= mGraph->QueryInterface(IID_IMediaSeeking, (void **)&mSeeking);
		if (mSeeking)
		{
			mSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
		}
		return SUCCEEDED(hr);
	}
	return FALSE;
}

void CDSPlayer::Release(void)
{
	if (mSeeking)
	{
		mSeeking->Release();
		mSeeking = NULL;
	}
	if (mMediaControl)
	{
		mMediaControl->Release();
		mMediaControl = NULL;
	}
	if (mEvent)
	{
		mEvent->Release();
		mEvent = NULL;
	}
	if (mBasicVideo)
	{
		mBasicVideo->Release();
		mBasicVideo = NULL;
	}
	if (mBasicAudio)
	{
		mBasicAudio->Release();
		mBasicAudio = NULL;
	}
	if (mVideoWindow)
	{
		mVideoWindow->put_Visible(OAFALSE);
		mVideoWindow->put_MessageDrain((OAHWND)NULL);
		mVideoWindow->put_Owner(OAHWND(0));
		mVideoWindow->Release();
		mVideoWindow = NULL;
	}
	if ( mAudioFilter )
	{
		mAudioFilter->Release ();
		mAudioFilter = NULL;
	}
	if ( m_pSysDevEnum )
	{
		m_pSysDevEnum->Release ();
		m_pSysDevEnum = NULL;
	}

	for ( int i=0; i<m_PtrAry_Filter_AudioDevice.GetSize(); i++ )
	{
		IBaseFilter *pFilter = (IBaseFilter*)m_PtrAry_Filter_AudioDevice.GetAt(i);
		SAFE_RELEASE ( pFilter );
	}
	m_PtrAry_Filter_AudioDevice.RemoveAll ();
	m_StrAry_AudioDevice.RemoveAll ();

	RemoveFromObjectTable();
	if (mGraph) 
	{
		mGraph->Release(); 
		mGraph = NULL;
	}
}

BOOL CDSPlayer::Attach(IGraphBuilder * inGraphBuilder)
{
	Release();

	if (inGraphBuilder)
	{
		inGraphBuilder->AddRef();
		mGraph = inGraphBuilder;

		AddToObjectTable();
		return QueryInterfaces();
	}
	return TRUE;
}

IGraphBuilder * CDSPlayer::GetGraph(void)
{
	return mGraph;
}

IMediaEventEx * CDSPlayer::GetEventHandle(void)
{
	return mEvent;
}

// Connect filter from the upstream output pin to the downstream input pin
BOOL CDSPlayer::ConnectFilters(IPin * inOutputPin, IPin * inInputPin, 
							  const AM_MEDIA_TYPE * inMediaType)
{
	if (mGraph && inOutputPin && inInputPin)
	{
		HRESULT hr = mGraph->ConnectDirect(inOutputPin, inInputPin, inMediaType);
		return SUCCEEDED(hr) ? TRUE : FALSE;
	}
	return FALSE;
}

void CDSPlayer::DisconnectFilters(IPin * inOutputPin)
{
	if (mGraph && inOutputPin)
	{
		HRESULT hr = mGraph->Disconnect(inOutputPin);
	}
}

BOOL CDSPlayer::SetDisplayWindow(HWND inWindow)
{	
	if (mVideoWindow)
	{
	//	long lVisible;
	//	mVideoWindow->get_Visible(&lVisible);
		// Hide the video window first
		mVideoWindow->put_Visible(OAFALSE);
		mVideoWindow->put_Owner((OAHWND)inWindow);

		RECT windowRect;
		::GetClientRect(inWindow, &windowRect);
		mVideoWindow->put_Left(0);
		mVideoWindow->put_Top(0);
		mVideoWindow->put_Width(windowRect.right - windowRect.left);
		mVideoWindow->put_Height(windowRect.bottom - windowRect.top);
		mVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

		mVideoWindow->put_MessageDrain((OAHWND) inWindow);
		// Restore the video window
		if (inWindow != NULL)
		{
		//	mVideoWindow->put_Visible(lVisible);
			mVideoWindow->put_Visible(OATRUE);
		}
		else
		{
			mVideoWindow->put_Visible(OAFALSE);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CDSPlayer::ResizeVideoWindow(long inLeft, long inTop, long inWidth, long inHeight)
{
	if (mVideoWindow)
	{
		long lVisible = OATRUE;
		mVideoWindow->get_Visible(&lVisible);
		// Hide the video window first
		mVideoWindow->put_Visible(OAFALSE);

		mVideoWindow->put_Left(inLeft);
		mVideoWindow->put_Top(inTop);
		mVideoWindow->put_Width(inWidth);
		mVideoWindow->put_Height(inHeight);
	
		// Restore the video window
		mVideoWindow->put_Visible(lVisible);
		return TRUE;
	}
	return FALSE;
}

BOOL CDSPlayer::SetNotifyWindow(HWND inWindow)
{
	if (mEvent)
	{
		mEvent->SetNotifyWindow((OAHWND)inWindow, WM_GRAPHNOTIFY, 0);
		return TRUE;
	}
	return FALSE;
}

void CDSPlayer::HandleEvent(WPARAM inWParam, LPARAM inLParam)
{
	if (mEvent)
	{
		LONG eventCode = 0, eventParam1 = 0, eventParam2 = 0;
		while (SUCCEEDED(mEvent->GetEvent(&eventCode, &eventParam1, &eventParam2, 0)))
		{
			mEvent->FreeEventParams(eventCode, eventParam1, eventParam2);
			switch (eventCode)
			{
			case EC_COMPLETE:
				break;

			case EC_USERABORT:
			case EC_ERRORABORT:
				break;

			default:
				break;
			}
		}
	}
}

BOOL CDSPlayer::Run(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsRunning())
		{
			if (SUCCEEDED(mMediaControl->Run()))
			{
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::Stop(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsStopped())
		{	
			if (SUCCEEDED(mMediaControl->Stop()))
			{
				if ( mAudioFilter )
				{
					mGraph->RemoveFilter ( mAudioFilter );
					mAudioFilter->Release ();
					mAudioFilter = NULL;
				}

				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::Pause(void)
{
	if (mGraph && mMediaControl)
	{
		if (!IsPaused())
		{	
			if (SUCCEEDED(mMediaControl->Pause()))
			{
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::IsRunning(void)
{
	if (mGraph && mMediaControl)
	{
		OAFilterState state = State_Stopped;
		if (SUCCEEDED(mMediaControl->GetState(10, &state)))
		{
			return state == State_Running;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::IsStopped(void)
{
	if (mGraph && mMediaControl)
	{
		OAFilterState state = State_Stopped;
		if (SUCCEEDED(mMediaControl->GetState(10, &state)))
		{
			return state == State_Stopped;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::IsPaused(void)
{
	if (mGraph && mMediaControl)
	{
		OAFilterState state = State_Stopped;
		if (SUCCEEDED(mMediaControl->GetState(10, &state)))
		{
			return state == State_Paused;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::SetFullScreen(BOOL inEnabled)
{
	if (mVideoWindow)
	{
		HRESULT hr = mVideoWindow->put_FullScreenMode(inEnabled ? OATRUE : OAFALSE);
		return SUCCEEDED(hr);
	}
	return FALSE;
}

BOOL CDSPlayer::GetFullScreen(void)
{
	if (mVideoWindow)
	{
		long  fullScreenMode = OAFALSE;
		mVideoWindow->get_FullScreenMode(&fullScreenMode);
		return (fullScreenMode == OATRUE);
	}
	return FALSE;
}

// IMediaSeeking features
BOOL CDSPlayer::GetCurrentPosition(double * outPosition)
{
	if (mSeeking)
	{
		__int64 position = 0;
		if (SUCCEEDED(mSeeking->GetCurrentPosition(&position)))
		{
			*outPosition = ((double)position) / 10000000.;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::GetStopPosition(double * outPosition)
{
	if (mSeeking)
	{
		__int64 position = 0;
		if (SUCCEEDED(mSeeking->GetStopPosition(&position)))
		{
			*outPosition = ((double)position) / 10000000.;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::SetCurrentPosition(double inPosition)
{
	if (mSeeking)
	{
		__int64 one = 10000000;
		__int64 position = (__int64)(one * inPosition);
		HRESULT hr = mSeeking->SetPositions(&position, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, 
			0, AM_SEEKING_NoPositioning);
		return SUCCEEDED(hr);
	}
	return FALSE;
}

BOOL CDSPlayer::SetStartStopPosition(double inStart, double inStop)
{
	if (mSeeking)
	{
		__int64 one = 10000000;
		__int64 startPos = (__int64)(one * inStart);
		__int64 stopPos  = (__int64)(one * inStop);
		HRESULT hr = mSeeking->SetPositions(&startPos, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, 
			&stopPos, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame);
		return SUCCEEDED(hr);
	}
	return FALSE;
}

BOOL CDSPlayer::GetDuration(double * outDuration)
{
	if (mSeeking)
	{
		__int64 length = 0;
		if (SUCCEEDED(mSeeking->GetDuration(&length)))
		{
			*outDuration = ((double)length) / 10000000.;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDSPlayer::SetPlaybackRate(double inRate)
{
	if (mSeeking)
	{
		if (SUCCEEDED(mSeeking->SetRate(inRate)))
		{
			return TRUE;
		}
	}
	return FALSE;
}

// Attention: range from -10000 to 0, and 0 is FULL_VOLUME.
BOOL CDSPlayer::SetAudioVolume(long inVolume)
{
	if (mBasicAudio)
	{
		HRESULT hr = mBasicAudio->put_Volume(inVolume);
		return SUCCEEDED(hr);
	}
	return FALSE;
}

long CDSPlayer::GetAudioVolume(void)
{
	long volume = 0;
	if (mBasicAudio)
	{
		mBasicAudio->get_Volume(&volume);
	}
	return volume;
}

// Attention: range from -10000(left) to 10000(right), and 0 is both.
BOOL CDSPlayer::SetAudioBalance(long inBalance)
{
	if (mBasicAudio)
	{
		HRESULT hr = mBasicAudio->put_Balance(inBalance);
		return SUCCEEDED(hr);
	}
	return FALSE;
}

long CDSPlayer::GetAudioBalance(void)
{
	long balance = 0;
	if (mBasicAudio)
	{
		mBasicAudio->get_Balance(&balance);
	}
	return balance;
}

BOOL CDSPlayer::RenderFile(const TCHAR * inFile)
{
	if ( mGraph )
	{
		if ( !Stop () ) return FALSE;
		if ( mAudioFilter )
		{
			mGraph->RemoveFilter ( mAudioFilter );
			mAudioFilter->Release ();
			mAudioFilter = NULL;
		}

	//	WCHAR wsz[MAX_PATH];
	//	MultiByteToWideChar(CP_ACP, 0, inFile, -1, wsz, MAX_PATH);
		if (SUCCEEDED(mGraph->RenderFile(inFile, NULL)))
		{
			mAudioFilter = FindFilters ( inFile );
			m_csMediaFileName = inFile;
			SetNotifyWindow ( m_hWndNotify );
			SetDisplayWindow ( m_hWndDisplay );

			return ( mAudioFilter != NULL );
		}
	}
	return FALSE;
}

IBaseFilter* CDSPlayer::FindFilters ( const TCHAR *szFilterName )
{
	if( !mGraph ) return NULL;
	IEnumFilters *pFilters = NULL;
	
	
    if ( FAILED(mGraph->EnumFilters(&pFilters)))
		return NULL;
	
    IBaseFilter *pFilter = NULL, *pFilter_Find = NULL;
    ULONG	n = 0;

    while ( pFilters->Next(1, &pFilter, &n) == S_OK )
	{
		FILTER_INFO	info;
		if ( FAILED(pFilter->QueryFilterInfo(&info)) )
		{
			TRACE (_T("Filter [%x]  -- failed QueryFilterInfo\n"), pFilter );
		}
		else
		{
			CString csAchName = info.achName;
			TRACE (_T("%s\n"), csAchName );
			if ( csAchName.Find ( szFilterName, 0 ) >= 0 )
			{
				pFilter_Find = pFilter;
			}
			else
			{
				pFilter->Release();
			}
		}
    }
	
    pFilters->Release();

	return pFilter_Find;
}

BOOL CDSPlayer::SnapshotBitmap(const TCHAR * outFile)
{
	if (mBasicVideo)
	{
		long bitmapSize = 0;
		if (SUCCEEDED(mBasicVideo->GetCurrentImage(&bitmapSize, 0)))
		{
			BOOL pass = FALSE;
			unsigned char * buffer = new unsigned char[bitmapSize];
			if (SUCCEEDED(mBasicVideo->GetCurrentImage(&bitmapSize, (long *)buffer)))
			{
				BITMAPFILEHEADER	hdr;
				LPBITMAPINFOHEADER	lpbi;

				lpbi = (LPBITMAPINFOHEADER)buffer;

				int nColors = 1 << lpbi->biBitCount;
				if (nColors > 256)
					nColors = 0;

				hdr.bfType		= ((WORD) ('M' << 8) | 'B');	//always is "BM"
				hdr.bfSize		= bitmapSize + sizeof( hdr );
				hdr.bfReserved1 	= 0;
				hdr.bfReserved2 	= 0;
				hdr.bfOffBits		= (DWORD) (sizeof(BITMAPFILEHEADER) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

				CFile bitmapFile(outFile, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary);
				bitmapFile.Write(&hdr, sizeof(BITMAPFILEHEADER));
				bitmapFile.Write(buffer, bitmapSize);
				bitmapFile.Close();
				pass = TRUE;
			}
			delete [] buffer;
			return pass;
		}
	}
	return FALSE;
}

//////////////////////// For GraphEdit Dubug purpose /////////////////////////////
void CDSPlayer::AddToObjectTable(void)
{
	IMoniker * pMoniker = 0;
    IRunningObjectTable * objectTable = 0;
    if (SUCCEEDED(GetRunningObjectTable(0, &objectTable))) 
	{
		CString csItem;
		csItem.Format (_T("FilterGraph %08p pid %08x"), (DWORD_PTR)mGraph, GetCurrentProcessId() );
	//	WCHAR wsz[256] = {0};
	//	MultiByteToWideChar(CP_ACP, 0, csItem, csItem.GetLength(), wsz, 256);
		HRESULT hr = CreateItemMoniker(L"!", csItem, &pMoniker);
		if (SUCCEEDED(hr)) 
		{
			hr = objectTable->Register(0, mGraph, pMoniker, &mObjectTableEntry);
			pMoniker->Release();
		}
		objectTable->Release();
	}
}

void CDSPlayer::RemoveFromObjectTable(void)
{
	IRunningObjectTable * objectTable = 0;
    if (SUCCEEDED(GetRunningObjectTable(0, &objectTable))) 
	{
        objectTable->Revoke(mObjectTableEntry);
        objectTable->Release();
		mObjectTableEntry = 0;
    }
}

BOOL CDSPlayer::IsInitedOK()
{
	return ( mGraph != NULL );
}

BOOL CDSPlayer::EnumAudioDevice()
{
	HRESULT hr;
    IEnumMoniker *pEnumCat = NULL;
	if ( !m_pSysDevEnum )
	{
		hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, 
			CLSCTX_INPROC, IID_ICreateDevEnum, 
			(void **)&m_pSysDevEnum );
	}
	hr = m_pSysDevEnum->CreateClassEnumerator(CLSID_AudioRendererCategory, &pEnumCat, 0);
	if ( FAILED(hr) ) return FALSE;
	m_StrAry_AudioDevice.RemoveAll ();
	
	IMoniker *pMoniker;
    ULONG cFetched;
    VARIANT varName={0};
    int nFilters=0;
	
    // Enumerate all items associated with the moniker
    while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
    {
        IPropertyBag *pPropBag;
        ASSERT(pMoniker);
		
        // Associate moniker with a file
        hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
			(void **)&pPropBag);
        ASSERT(SUCCEEDED(hr));
        ASSERT(pPropBag);
        if (FAILED(hr))
            continue;
		
        // Read filter name from property bag
        varName.vt = VT_BSTR;
        hr = pPropBag->Read(L"FriendlyName", &varName, 0);
        if (FAILED(hr))
            continue;
		
        // Get filter name (converting BSTR name to a CString)
        CString str(varName.bstrVal);
        SysFreeString(varName.bstrVal);
        nFilters++;
		m_StrAry_AudioDevice.Add ( str );
		IBaseFilter *pFilter = NULL;
		pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pFilter);
		m_PtrAry_Filter_AudioDevice.Add ( pFilter );

        // Cleanup interfaces
        SAFE_RELEASE(pPropBag);
        SAFE_RELEASE(pMoniker);
    }
	
	SAFE_RELEASE ( pEnumCat );

	ASSERT ( m_StrAry_AudioDevice.GetSize() == m_PtrAry_Filter_AudioDevice.GetSize() );
	return TRUE;
}

void CDSPlayer::GetAudioDeviceList(CStringArray &StrAry_AudioDevice)
{
	if ( m_StrAry_AudioDevice.GetSize() <= 0 )
		EnumAudioDevice ();
	StrAry_AudioDevice.Append ( m_StrAry_AudioDevice );
}

int CDSPlayer::FindAudioDeivceIDByName(CString strContainText, BOOL bInclude, CString *pcsAudioDeviceFullName/*=NULL*/)
{
	for ( int i=0; i<m_StrAry_AudioDevice.GetSize(); i++ )
	{
		CString csWaveoutCard = m_StrAry_AudioDevice.GetAt(i);
		if  (
				(
					bInclude &&
					csWaveoutCard.Find(strContainText,0) >= 0
				)
				||
				(
					!bInclude &&
					csWaveoutCard.Find(strContainText,0) < 0
				)
			)
		{
			if ( pcsAudioDeviceFullName )
				*pcsAudioDeviceFullName = csWaveoutCard;
			return i;
		}
	}
	return -1;
}

BOOL CDSPlayer::SetAudioDevice(CString strContainText, BOOL bInclude/*=TRUE*/, CString *pcsAudioDeviceFullName/*=NULL*/)
{
	m_csAudioDeviceName = strContainText;
	if ( !mGraph ) return FALSE;

	// 该声音设备滤镜已经有了	
	BOOL bMustReplay = FALSE;
	double pos = 0.0;
	if ( IsInitedOK() && !m_csMediaFileName.IsEmpty() )
	{
		bMustReplay = TRUE;
		GetCurrentPosition ( &pos );
		Stop ();
		Release ();
		if ( !Create () ) return FALSE;
	}

	if ( m_StrAry_AudioDevice.GetSize() <= 0 )
		EnumAudioDevice ();

	int nDeviceID = FindAudioDeivceIDByName ( strContainText, bInclude, &m_csAudioDeviceName );
	if ( pcsAudioDeviceFullName ) *pcsAudioDeviceFullName = m_csAudioDeviceName;
	if ( nDeviceID < 0 || nDeviceID >= m_PtrAry_Filter_AudioDevice.GetSize() )
		return FALSE;
	IBaseFilter *pFilter = (IBaseFilter*)m_PtrAry_Filter_AudioDevice.GetAt(nDeviceID);
	if ( !pFilter ) return FALSE;

	WCHAR wsz[256] = {0};
	CString csFilterName = m_StrAry_AudioDevice.GetAt (nDeviceID);
//	MultiByteToWideChar(CP_ACP, 0, csFilterName, -1, wsz, 256);
	HRESULT hr = mGraph->AddFilter ( pFilter, csFilterName );

	if ( bMustReplay )
	{
		if ( !RenderFile ( m_csMediaFileName ) )
			return FALSE;
		Run ();
		SetCurrentPosition ( pos );
	}

	return SUCCEEDED(hr);
}

BOOL CDSPlayer::Delete_DefaultDirectSoundDevice()
{
	if ( !mGraph ) return FALSE;
	IBaseFilter* pFilter_Find = FindFilters (_T("Default DirectSound Device"));
	if ( pFilter_Find )
	{
		mGraph->RemoveFilter ( pFilter_Find );
		SAFE_RELEASE ( pFilter_Find );
		SetAudioDevice ( m_csAudioDeviceName );
	}

	return TRUE;
}
