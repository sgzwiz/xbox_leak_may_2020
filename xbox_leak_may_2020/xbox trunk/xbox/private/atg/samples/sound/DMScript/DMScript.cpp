//-----------------------------------------------------------------------------
// File: DMScript.cpp
//
// Desc: Plays a script file using DirectMusic.
//
// Hist: 01.12.00 - New for February XDK release
//       02.22.01 - Updated for March XDK release
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <XBApp.h>
#include <XBUtil.h>
#include <XBFont.h>
#include <XBHelp.h>
#include <dmusici.h>
#include "dsstdfx.h"
#include "myfactory.h"



//-----------------------------------------------------------------------------
// Callouts for labelling the gamepad on the help screen
//-----------------------------------------------------------------------------
XBHELP_CALLOUT g_HelpCallouts[] = 
{
    { XBHELP_BACK_BUTTON,  XBHELP_PLACEMENT_2, L"Display help" },
    { XBHELP_A_BUTTON,     XBHELP_PLACEMENT_2, L"+1 Home score" },
    { XBHELP_B_BUTTON,     XBHELP_PLACEMENT_2, L"+1 Visitor\nscore" },
    { XBHELP_X_BUTTON,     XBHELP_PLACEMENT_2, L"Play food\nvendor" },
    { XBHELP_Y_BUTTON,     XBHELP_PLACEMENT_1, L"Play organ" },
    { XBHELP_BLACK_BUTTON, XBHELP_PLACEMENT_2, L"Announce score" },
    { XBHELP_START_BUTTON, XBHELP_PLACEMENT_2, L"New game" },
    { XBHELP_LEFTSTICK,    XBHELP_PLACEMENT_1, L"Adjust volume" },
};

#define NUM_HELP_CALLOUTS 8




//-----------------------------------------------------------------------------
// Name: class CXBoxSample
// Desc: Application class.
//-----------------------------------------------------------------------------
class CXBoxSample : public CXBApplication
{
    CXBFont            m_Font;               // Font class
    CXBHelp            m_Help;               // Help class
    BOOL               m_bDrawHelp;          // Whether to draw help
    
	FLOAT              m_fVolume;
    WCHAR*             m_strScriptName;
    LONG               m_HomeTeamScore;
    LONG               m_AwayTeamScore;

    // DirectMusic variables
    IDirectMusicLoader8*      m_pLoader;
    IDirectMusicPerformance8* m_pPerformance;
    IDirectMusicSegment8*     m_pMusic;
    IDirectMusicAudioPath8*   m_pMusicAudioPath;
    IDirectMusicScript8*      m_pMusicScript;
    LPDIRECTSOUND8            m_pDSound;

    HRESULT AddScore( BOOL bHomeTeam, INT delta );
    HRESULT NewGame();
    HRESULT CallRoutine( const CHAR* strRoutine );
    HRESULT DrawScore( FLOAT x, FLOAT y, const WCHAR* strLabel, INT score );
    HRESULT InitDirectMusic();
    HRESULT StartScript();
    HRESULT StopScript();
    HRESULT UpdateScriptFromScore( BOOL bHomeTeam );
    HRESULT SetVolume( LONG lVolume );
    VOID    ReportScriptError( const CHAR* method, const CHAR* name,
                               DMUS_SCRIPT_ERRORINFO* pErrorInfo );
    HRESULT SetVariableNumber( const CHAR* varName, LONG value );
    HRESULT DownloadEffectsImage(PCHAR pszScratchFile);  // downloads a default DSP image to the GP

public:

    HRESULT Initialize();
    HRESULT FrameMove();
    HRESULT Render();

    CXBoxSample();
};




//-----------------------------------------------------------------------------
// Name: main()
// Desc: Entry point to the program.
//-----------------------------------------------------------------------------
VOID __cdecl main()
{
    CXBoxSample xbApp;
    if( FAILED( xbApp.Create() ) )
        return;
    xbApp.Run();
}




//-----------------------------------------------------------------------------
// Name: CXBoxSample()
// Desc: Application class constructor
//-----------------------------------------------------------------------------
CXBoxSample::CXBoxSample()
{
    m_fVolume       = 100.0f;
    m_strScriptName = NULL;
}




//-----------------------------------------------------------------------------
// Name: DownloadEffectsImage
// Desc: Downloads an effects image to the DSP
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::DownloadEffectsImage(PCHAR pszScratchFile)
{
    HANDLE hFile;
    DWORD dwSize;
    PVOID pBuffer = NULL;
    HRESULT hr = S_OK;
    LPDSEFFECTIMAGEDESC pDesc;
    DSEFFECTIMAGELOC EffectLoc;

    // open scratch image file generated by xps2 tool
    hFile = CreateFile( pszScratchFile,
                        GENERIC_READ,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL );

    if( hFile == INVALID_HANDLE_VALUE )
    {
        DWORD err;

        err = GetLastError();
        OUTPUT_DEBUG_STRING( "Failed to open the dsp image file.\n" );
        hr = HRESULT_FROM_WIN32(err);
    }

    if( SUCCEEDED(hr) )
    {
        // Determine the size of the scratch image by seeking to
        // the end of the file
        dwSize = SetFilePointer( hFile, 0, NULL, FILE_END );
        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    
        // Allocate memory to read the scratch image from disk
        pBuffer = new BYTE[dwSize];

        // Read the image in
        DWORD dwBytesRead;
        BOOL bResult = ReadFile( hFile,
                                 pBuffer,
                                 dwSize,
                                 &dwBytesRead,
                                 0 );
        
        if (!bResult)
        {
            DWORD err;

            err = GetLastError();
            OUTPUT_DEBUG_STRING( "\n Failed to open the dsp image file.\n" );
            hr = HRESULT_FROM_WIN32(err);
        }

    }

    if( SUCCEEDED(hr) )
    {
        // call dsound api to download the image..
        EffectLoc.dwI3DL2ReverbIndex = I3DL2_CHAIN_I3DL2_REVERB;
        EffectLoc.dwCrosstalkIndex = I3DL2_CHAIN_XTALK;

        hr = m_pDSound->DownloadEffectsImage( pBuffer,
                                              dwSize,
                                              &EffectLoc,
                                              &pDesc );
    }

    delete[] pBuffer;

    if( hFile != INVALID_HANDLE_VALUE ) 
    {
        CloseHandle( hFile );
    }

    return hr;
}



//-----------------------------------------------------------------------------
// Name: Initialize()
// Desc: Init DM and fonts
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::Initialize()
{
    if( FAILED( InitDirectMusic() ) )
        return E_FAIL;

    // Create a font
    if( FAILED( m_Font.Create( m_pd3dDevice, "Font.xpr" ) ) )
        return XBAPPERR_MEDIANOTFOUND;

    // Create help
    if( FAILED( m_Help.Create( m_pd3dDevice, "Gamepad.xpr" ) ) )
        return XBAPPERR_MEDIANOTFOUND;

    if( FAILED( DirectSoundCreate( NULL, &m_pDSound, NULL ) ) )
        return E_FAIL;

    //
    // download the standard DirectSound effecs image
    //
    if( FAILED( DownloadEffectsImage("d:\\media\\dsstdfx.bin") ) )
        return E_FAIL;

    m_bDrawHelp = FALSE;

    NewGame();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: NewGame
// Desc: Restart script
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::NewGame()
{
    StopScript();

    m_HomeTeamScore = 0;
    m_AwayTeamScore = 0;
    
    StartScript();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: AddScore
// Desc: Update team score
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::AddScore( BOOL bHomeTeam, INT delta )
{
    if( bHomeTeam )
        m_HomeTeamScore += delta;
    else
        m_AwayTeamScore += delta;

    UpdateScriptFromScore( bHomeTeam );

    return S_OK;
}



#ifdef _DEBUG
extern "C" ULONG __cdecl DbgPrint(PCH Format, ...);
#endif

//-----------------------------------------------------------------------------
// Name: CallRoutine()
// Desc: DM callback
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::CallRoutine( const CHAR* strRoutine )
{
    DMUS_SCRIPT_ERRORINFO errorInfo = {sizeof(errorInfo)};

    HRESULT hr;
    if( FAILED( hr = m_pMusicScript->CallRoutine( strRoutine, &errorInfo ) ) ) 
	{
        ReportScriptError("CallRoutine", strRoutine, &errorInfo);
        return hr;
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ReportScriptError()
// Desc: Debugging information
//-----------------------------------------------------------------------------
void CXBoxSample::ReportScriptError(const CHAR* method, const CHAR* name,
                                       DMUS_SCRIPT_ERRORINFO* pErrorInfo){
#ifdef _DEBUG
        DbgPrint("Script error method %s \"%s\"\n", method, name);
        DbgPrint(" hr = 0x%08x\n", pErrorInfo->hr);
        DbgPrint(" line number: %d\n", pErrorInfo->ulLineNumber);
        DbgPrint(" character position: %d\n", pErrorInfo->ichCharPosition);
        DbgPrint(" source file : %S\n", pErrorInfo->wszSourceFile);
        DbgPrint(" source component : %S\n", pErrorInfo->wszSourceComponent);
        DbgPrint(" description : %S\n", pErrorInfo->wszDescription);
        DbgPrint(" source line text : %S\n", pErrorInfo->wszSourceLineText);
#endif
}




//-----------------------------------------------------------------------------
// Name: DrawScore()
// Desc: Display score
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::DrawScore( FLOAT x, FLOAT y, const WCHAR* strLabel, 
                                INT score )
{
    WCHAR buf[100];
    wsprintfW( buf, L"%s %d", strLabel, score ); 
    m_Font.DrawText( x, y, 0xffffffff, buf );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitDirectMusic
// Desc: Start up DM
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::InitDirectMusic() 
{
    m_pLoader         = NULL;
    m_pPerformance    = NULL;
    m_pMusicAudioPath = NULL;
    m_pMusicScript    = NULL;

    // Initialize DMusic
    IDirectMusicHeap* pNormalHeap;
    DirectMusicCreateDefaultHeap( &pNormalHeap );

    IDirectMusicHeap* pPhysicalHeap;
    DirectMusicCreateDefaultPhysicalHeap( &pPhysicalHeap );

    DirectMusicInitializeEx( pNormalHeap, pPhysicalHeap, MyFactory );

    pNormalHeap->Release();
    pPhysicalHeap->Release();

    // Create loader object
    DirectMusicCreateInstance( CLSID_DirectMusicLoader, NULL, 
                               IID_IDirectMusicLoader8, (VOID**)&m_pLoader );

    // Create performance object
    DirectMusicCreateInstance( CLSID_DirectMusicPerformance, NULL,
                               IID_IDirectMusicPerformance8, (VOID**)&m_pPerformance );

    // Initialize the performance with the standard audio path.
    // The flags (final) argument allows us to specify whether or not we want
    // DirectMusic to create a thread on our behalf to process music, using 
    // DMUS_INITAUDIO_NOTHREADS.  The default is for DirectMusic to create its
    // own thread; DMUS_INITAUDIO_NOTHREADS tells DirectMusic not to do this, 
    // and the app will periodically call DirectMusicDoWork().  For software 
    // emulation on alpha hardware, it's generally better to have DirectMusic
    // create its own thread. On real hardware, periodically calling 
    // DirectMusicDoWork may provide a better option.
    m_pPerformance->InitAudioX( DMUS_APATH_SHARED_STEREOPLUSREVERB, 64, 128, 0 );

    // Tell DirectMusic where the default search path is
    m_pLoader->SetSearchDirectory( GUID_DirectMusicAllTypes, 
                                   "D:\\Media\\Sounds", FALSE );

    // Get default (music) audiopath.
    m_pPerformance->GetDefaultAudioPath( &m_pMusicAudioPath );

    // Max volume for music
    m_pMusicAudioPath->SetVolume( (100*100)-10000, 0 );

    // Now DirectMusic will play in the background, so continue on with our task
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: StopScript()
// Desc: stop the script
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::StopScript()
{
    if( m_pMusicScript )
    {
        CallRoutine("StopAll");
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: StartScript()
// Desc: Load the script
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::StartScript()
{
    HRESULT hr;

    if(! m_pMusicScript )
    {
	    // Load and init the music script
        hr = m_pLoader->LoadObjectFromFile( CLSID_DirectMusicScript, IID_IDirectMusicScript8, 
			                                "BaseBall.spt", (VOID**)&m_pMusicScript );
        if( FAILED(hr) )
        {
            m_strScriptName = L"Could not load script";
            return hr;
        }

        hr = m_pMusicScript->Init( m_pPerformance, NULL );
        if( FAILED(hr) )
        {
            m_strScriptName = L"Could not Initialize script";
            return hr;
        }
    }

    m_strScriptName = L"Baseball";

    UpdateScriptFromScore( TRUE );  // For home team
    UpdateScriptFromScore( FALSE ); // For away team

    hr = CallRoutine( "Start" );
    return hr;
}




//-----------------------------------------------------------------------------
// Name: UpdateScriptFromScore
// Desc: Update DM script
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::UpdateScriptFromScore( BOOL bHomeTeam )
{
    // Tell the script what the score is
    if( NULL == m_pMusicScript )
        return E_FAIL;

    HRESULT hr;

    if( bHomeTeam )
        hr = SetVariableNumber( "Variable1", m_HomeTeamScore);
    else
        hr = SetVariableNumber( "Variable2", m_AwayTeamScore);

    return hr;
}




//-----------------------------------------------------------------------------
// Name: SetVariableNumber
// Desc: Save variable in script
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::SetVariableNumber( const CHAR* varName, LONG value )
{
    // Tell the script what the score is
    if( NULL == m_pMusicScript )
        return E_FAIL;

    DMUS_SCRIPT_ERRORINFO errorInfo;
    HRESULT hr = m_pMusicScript->SetVariableNumber( varName, value, &errorInfo );

    if( FAILED(hr) )
    {
        ReportScriptError("SetVariableNumber", varName, &errorInfo);
    }
    return hr;
}




//-----------------------------------------------------------------------------
// Name: SetVolume()
// Desc: Set volume on the main music audipath
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::SetVolume( LONG lVolume )
{
    // Change range from [0,100] to [-3000,0]
    const INT kInLow = 0;
    const INT kInHigh = 100;
    const INT kOutLow = -3000;
    const INT kOutHigh = 0;
    lVolume = (lVolume - kInLow) * (kOutHigh - kOutLow) / (kInHigh - kInLow)
        + kOutLow;

    m_pMusicAudioPath->SetVolume( lVolume, 0 );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: IsAnyButtonActive()
// Desc: TRUE if any button depressed or any thumbstick offset on the given
//       controller.
//-----------------------------------------------------------------------------
BOOL IsAnyButtonActive( const XBGAMEPAD* pGamePad )
{
    // Check digital buttons
    if( pGamePad->wButtons )
        return TRUE;

    // Check analog buttons
    for( DWORD i = 0; i < 8; ++i )
    {
        if( pGamePad->bAnalogButtons[i] )
            return TRUE;
    }

    // Check thumbsticks
    if( pGamePad->fX1 || pGamePad->fY1 || pGamePad->fX2 || pGamePad->fX2  )
        return TRUE;

    // Nothing active
    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: GetPrimaryController()
// Desc: The primary controller is the first controller used by a player.
//       If no controller has been used or the controller has been removed,
//       the primary controller is the controller inserted at the lowest 
//       port number. Function returns NULL if no controller is inserted.
//-----------------------------------------------------------------------------
const XBGAMEPAD* GetPrimaryController()
{
    static INT nPrimaryController = -1;

    // If primary controller has been set and hasn't been removed, use it
    const XBGAMEPAD* pGamePad = NULL;
    if( nPrimaryController != -1 )
    {
        pGamePad = &g_Gamepads[ nPrimaryController ];
        if( pGamePad->hDevice != NULL )
            return pGamePad;
    }

    // Primary controller hasn't been set or has been removed...

    // Examine each inserted controller to see if any is being used
    INT nFirst = -1;
    for( DWORD i=0; i < XGetPortCount(); ++i )
    {
        pGamePad = &g_Gamepads[i];
        if( pGamePad->hDevice != NULL )
        {
            // Remember the lowest inserted controller ID
            if( nFirst == -1 )
                nFirst = i;

            // If any button is active, we found the primary controller
            if( IsAnyButtonActive( pGamePad ) )
            {
                nPrimaryController = i;
                return pGamePad;
            }
        }
    }

    // No controllers are inserted
    if( nFirst == -1 )
        return NULL;

    // The primary controller hasn't been set and no controller has been
    // used yet, so return the controller on the lowest port number
    pGamePad = &g_Gamepads[ nFirst ];
    return pGamePad;
}




//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Check controller and update script
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::FrameMove()
{
    const XBGAMEPAD* pGamepad = GetPrimaryController();
    if( pGamepad == NULL )
        return S_OK;

    // Adjust volume
    m_fVolume += pGamepad->fY1 * m_fElapsedTime * 100.0f;
    if( m_fVolume <   1.0f )
        m_fVolume =   1.0f;
    if( m_fVolume > 100.0f )
        m_fVolume = 100.0f;
    SetVolume( (INT)m_fVolume );

    if( pGamepad->bPressedAnalogButtons[XINPUT_GAMEPAD_A] )
    {
        CallRoutine( "Hit" );
        AddScore( TRUE, 1 );
    }

    if( pGamepad->bPressedAnalogButtons[XINPUT_GAMEPAD_B] )
    {
        CallRoutine( "Hit" );
        AddScore( FALSE, 1 );
    }

    if( pGamepad->bPressedAnalogButtons[XINPUT_GAMEPAD_X] )
    {
        CallRoutine( "Routine1" );
    }

    if( pGamepad->bPressedAnalogButtons[XINPUT_GAMEPAD_Y] )
    {
        CallRoutine( "Routine2" );
    }
    
    if( pGamepad->bPressedAnalogButtons[XINPUT_GAMEPAD_BLACK] )
    {
        CallRoutine( "Routine3" );
    }
    
    if( pGamepad->wPressedButtons & XINPUT_GAMEPAD_START )
    {
        NewGame();
    }

    // Toggle help
    if( m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_BACK ) 
    {
        m_bDrawHelp = !m_bDrawHelp;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Graphics display
//-----------------------------------------------------------------------------
HRESULT CXBoxSample::Render()
{
    // Clear the viewport
    m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, 0xff0000ff, 1.0f, 0L );

    // Show title, frame rate, and help
    if( m_bDrawHelp )
        m_Help.Render( &m_Font, g_HelpCallouts, NUM_HELP_CALLOUTS );
    else
    {
        // Draw title
        m_Font.DrawText( 64, 50, 0xffffffff, L"DMScript: DirectMusic AudioScript Sample" );
        m_Font.DrawText( 64, 75, 0xffffffff, m_strScriptName );

        // Draw instructions
        DrawScore( 64, 150, L"A - Add One to Home Team Score:", m_HomeTeamScore );
        DrawScore( 64, 180, L"B - Add One to Visitors Score:", m_AwayTeamScore );
        m_Font.DrawText( 64, 210, 0xffffffff, L"X - Trigger Food Vendor" );
        m_Font.DrawText( 64, 240, 0xffffffff, L"Y - Trigger Organ Music" );
        m_Font.DrawText( 64, 270, 0xffffffff, L"Black - Announce Score" );
        m_Font.DrawText( 64, 300, 0xffffffff, L"Start - Start New Game" );
        m_Font.DrawText( 64, 330, 0xffc0c0c0, L"The game is over after one team scores 10 runs." );
        m_Font.DrawText( 64, 400, 0xffffffff, L"L stick up/down - Volume:" );
    
        // Draw the volume bar
        struct BACKGROUNDVERTEX { D3DXVECTOR4 p; D3DCOLOR color; };
        BACKGROUNDVERTEX v[4];
        FLOAT x1 = 320, x2 = x1 + (280*m_fVolume)/100;
        FLOAT y1 = 400, y2 = y1 + 20;
        v[0].p = D3DXVECTOR4( x1-0.5f, y1-0.5f, 1.0f, 1.0f );  v[0].color = 0xffffffff;
        v[1].p = D3DXVECTOR4( x2-0.5f, y1-0.5f, 1.0f, 1.0f );  v[1].color = 0xffffffff;
        v[2].p = D3DXVECTOR4( x1-0.5f, y2-0.5f, 1.0f, 1.0f );  v[2].color = 0xffc0ffc0;
        v[3].p = D3DXVECTOR4( x2-0.5f, y2-0.5f, 1.0f, 1.0f );  v[3].color = 0xffc0ffc0;

        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
        m_pd3dDevice->SetVertexShader( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );
        m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(v[0]) );
    }

    // Present the scene
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

    return S_OK;
}