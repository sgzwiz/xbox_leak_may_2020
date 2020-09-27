//
// mktyplib.cpp
//
// Implementation of CMkTypLibTool class
//

#include "stdafx.h"
#pragma hdrstop

#include "resource.h"
#include "toolmtl.h"
#include "progdlg.h"
#include "dirmgr.h"
#include "dlgbase.h"
#include "projcomp.h"

IMPLEMENT_DYNAMIC(CMkTypLibTool, CBuildTool)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern CMkTypLibTool g_CMkTypLibTool ; // forward reference

///////////////////////////////////////////////////////////////////////////////
CMkTypLibTool::CMkTypLibTool()
{
	bForceNoInferenceRule = TRUE;

	// tool name
	m_nIDName = IDS_MKTYPLIB_TOOL;
	m_nIDUIName = IDS_MKTYPLIB_TOOL_UI;

	// tool exe name and input file set
	m_strToolExeName = _TEXT("midl.exe");
	m_strToolInput = _TEXT("*.odl;*.idl");
	m_strToolPrefix = _TEXT("MTL");
	m_nOrder = 5;
}

///////////////////////////////////////////////////////////////////////////////
// Perform build action(s)
UINT CMkTypLibTool::PerformBuildActions(UINT type, UINT stage, CActionSlobList & lstActions,
									    DWORD aob, CErrorContext & EC)
{
	UINT act = ACT_Complete;

	// what stage is this?
	switch (stage)
	{
		// pre?
		case TOB_Pre:
			break;

		// post?
		case TOB_Post:
			break;

		// action?
		case TOB_Stage:
			break;

		default:
			break;
	}

	if (act != ACT_Complete)
		return act;

	return CBuildTool::PerformBuildActions(type, stage, lstActions, aob, EC);
}
///////////////////////////////////////////////////////////////////////////////
// Return the list of files generated by the mktyplib tool
BOOL CMkTypLibTool::GenerateOutput(UINT type, CActionSlobList & lstActions, CErrorContext & EC)
{
	POSITION posAction = lstActions.GetHeadPosition();
	while (posAction != (POSITION)NULL)
	{
		CActionSlob * pAction = (CActionSlob *)lstActions.GetNext(posAction);
		CProjItem * pItem = pAction->m_pItem->IsKindOf(RUNTIME_CLASS(CTargetItem)) ? pAction->m_pItem->GetProject() : pAction->m_pItem;

		FileRegHandle frh;

		CProject *pProject = pItem->GetProject();
		ASSERT (pProject);
		CDir *pDir = &pProject->GetWorkspaceDir(); // default if no P_MTLOutputDir

		CString strDirName;
		UINT idPropM = MapLogical(P_MTLOutputInc);
		CDir dirOutput;

		if ((pItem->GetStrProp (MapLogical(P_MTLOutputDir), strDirName) == valid) && !strDirName.IsEmpty())
		{
			// Try to substitute for $(INTDIR)
			if (!pItem->SubstOutDirMacro(strDirName, pItem->GetActiveConfig()))
			{
				// Try $(OUTDIR) if $(INTDIR) doesn't work
				pProject->SubstOutDirMacro(strDirName, pProject->GetActiveConfig());
			}

			int i = strDirName.GetLength();
			LPCTSTR pszTmp = _tcsdec((LPCTSTR)strDirName, LPCTSTR(strDirName)+i);
			// add trailing slash if necessary
			if (*pszTmp != _T('\\') && *pszTmp != _T('/'))
			{
				strDirName += _T('\\');
			}

			strDirName += _T('a');
			CPath tmpPath;
			if (tmpPath.CreateFromDirAndFilename(*pDir, strDirName) && dirOutput.CreateFromPath(tmpPath))
			{
				pDir = &dirOutput;
			}
		}
		if (pItem == pProject)
		{
			CObList ol;
			CProjItem * pSubItem;

		 	// assemble all the .tlb files from the project
			pItem->FlattenSubtree (ol, CProjItem::flt_ExcludeProjects | CProjItem::flt_ExcludeGroups |
									   CProjItem::flt_RespectItemExclude | CProjItem::flt_ExcludeDependencies);

			// FUTURE: worry about duplicates?
			while (!ol.IsEmpty ())
			{
				pSubItem = (CProjItem *) ol.RemoveHead();
				if (AttachToFile(pSubItem->GetFileRegHandle()))
				{
					pSubItem->ForceBaseConfigActive((ConfigurationRecord *)pItem->GetActiveConfig());
					if (GetItemOutputFile(pSubItem, MapLogical(P_MTLOutputTlb), frh, _TEXT(".tlb"), pDir))
						pAction->AddOutput(frh);
#ifdef REFCOUNT_WORK
					if (NULL!=frh)
						frh->ReleaseFRHRef();
#endif

					pSubItem->ForceConfigActive();
				}
			}
		}
		else
		{
			// .tlb file is not always generated. check if "/tlb filename" is in the
			// option table or not.
			CString strTlbName;
			UINT idPropM = MapLogical(P_MTLOutputTlb);
			if ((pItem->GetStrProp (idPropM, strTlbName) == valid) && !strTlbName.IsEmpty())
				if (!pAction->AddOutput(idPropM, _TEXT(".tlb"), pDir))
					ASSERT(FALSE);

			// now we want to check if "/h filename" is in the option table or not, if it is
			// then we need to add it to the pPtrList as well. 
			CString strHdrName;
			idPropM = MapLogical(P_MTLOutputInc);

			if ((pItem->GetStrProp (idPropM, strHdrName) == valid) && !strHdrName.IsEmpty())
				if (!pAction->AddOutput(idPropM, _TEXT(".h"), pDir))
					ASSERT(FALSE);

			// now we want to check if "/h filename" is in the option table or not, if it is
			// then we need to add it to the pPtrList as well. 
			CString strUuidName;
			idPropM = MapLogical(P_MTLOutputUuid);

			if ((pItem->GetStrProp (idPropM, strUuidName) == valid) && !strUuidName.IsEmpty())
				if (!pAction->AddOutput(idPropM, _TEXT(".c"), pDir))
					ASSERT(FALSE);
		}
	}

	return TRUE; // success
}
///////////////////////////////////////////////////////////////////////////////
// Affects our output?
BOOL CMkTypLibTool::AffectsOutput(UINT idPropL)
{
	// change our outputs?
	if (
		idPropL == P_MTLOutputUuid ||	// *.c
		idPropL == P_MTLOutputTlb ||	// *.tlb
		idPropL == P_MTLOutputDir ||	// ALL
		idPropL == P_MTLOutputInc		// *.h
	   )
	{
	   return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////
void CMkTypLibTool::WriteInferenceRuleMacro(CActionSlob * pAction)
{
	ASSERT(g_bExportMakefile);

	g_prjoptengine.SetOptionHandler(GetOptionHandler());
	g_prjoptengine.SetPropertyBag(pAction->m_pItem);

	CString strBuildLine;
	g_prjoptengine.GenerateString (strBuildLine, (OBShowMacro | OBShowDefault | OBShowFake | OBInherit));

	TRY
	{
		m_pMakWriter->WriteMacro("MTL_PROJ",strBuildLine);
	}
	CATCH (CException, e)
	{
		strBuildLine.Empty() ;
		THROW_LAST() ;
	}
	END_CATCH
}

////////////////////////////////////////////////////////////////////
void CMkTypLibTool::GetIncludePaths(CProjItem * pItem, CString & str)
{
	GetDirMgr()->GetDirListString(str, DIRLIST_INC);
	 
	CString str1 ;
	COptionHandler * pOptionHandler = GetOptionHandler();
	ASSERT (pOptionHandler != (COptionHandler *) NULL);

	if (pOptionHandler->GetListStrProp(pItem, MapLogical(P_MTLIncludes), str1, TRUE, FALSE, _T(';'))){
		ReplaceEnvVars(str1);
		str = ( str1 + _TEXT (";")) + str;
	}
}

////////////////////////////////////////////////////////////////////////
BOOL CMkTypLibTool::GetCommandLines
(
	CActionSlobList & lstActions,
	CPtrList & plCommandLines,
	DWORD attrib,
	CErrorContext & EC
)
{
	CString strDescription;
	VERIFY(strDescription.LoadString(IDS_DESC_MKTYPLIB_COMPILING));

	POSITION posAction = lstActions.GetHeadPosition();
	while (posAction != (POSITION)NULL)
	{
		CActionSlob * pAction = (CActionSlob *)lstActions.GetNext(posAction);

		CProjItem * pItem = pAction->m_pItem;
		ActionState as = pAction->m_state;

		CString str;
		if (!pAction->GetCommandOptions(str))
			return FALSE;

		ReplaceEnvVars(str);

		// str += _TEXT(" /W0 ");
		str += _T('\"');
		str += (const TCHAR *)*pItem->GetFilePath();
		str += _T('\"');

		// may create a tmp. file if cmdline too long
		if (!g_buildengine.FormCmdLine(m_strToolExeName, str, EC))
			return FALSE;	// error

		CCmdLine *pCmdLine = new CCmdLine;
		pCmdLine->nProcessor = 1;
		pCmdLine->slCommandLines.AddTail(str);
		pCmdLine->slDescriptions.AddTail(strDescription);

		if (as == _SrcOutOfDate)
		{
			plCommandLines.AddHead(pCmdLine);
		}
		else
		{
			plCommandLines.AddTail(pCmdLine);
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	Scans files for includes.
BOOL CMkTypLibTool::GenerateDependencies(CActionSlob * pAction, FileRegHandle frhFile, CErrorContext & EC)
{
	CString			strFile;
	const TCHAR *	pFileNameBegin;
	BOOL			bSkipping = FALSE;
	int				iEntryType;
	char			cDelim;

	HANDLE hMem;
	DWORD dwFileSize;

	const TCHAR * pcFileMap;
	if (!g_buildengine.OpenMemFile(frhFile, hMem, pcFileMap, dwFileSize, EC))
	{
		const CPath * pPath = g_FileRegistry.GetRegEntry(frhFile)->GetFilePath();
		g_buildengine.LogBuildError(BldError_ScanError, (void *)pPath);
		return FALSE;	// couldn't open, continue with next
	}

	const TCHAR * pMax;
	int nLine;

	if (pcFileMap == (const TCHAR *)NULL)
		goto CloseMemFile;	// no need to open

	pMax = pcFileMap + dwFileSize;
	nLine = 0;

	//	Check for afx hack.  If the file begins with "//{{NO_DEP", don't scan
	if ((dwFileSize > 10) && _tcsnicmp (pcFileMap, _TEXT("//{{NO_DEP"), 10) == 0)
	{
		pAction->SetDepState(DEP_None);
		goto CloseMemFile;
	}

	do
	{
		nLine++;	// start of a new line.

		// Skip leading blanks
		while (pcFileMap < pMax && _istspace( *pcFileMap))
		{
			if (*pcFileMap == _T('\r'))
				nLine++;
			pcFileMap++;
		}

		if (pcFileMap >= pMax)
			goto SkipToEnd;

		 // Check this first so we ignore comments.
		if (*pcFileMap == _T('/'))
		{
			if (bSkipping)
			{
				if (pMax - pcFileMap >= 17 &&
					_tcsnicmp (pcFileMap, _TEXT("//}}START_DEPSCAN"), 17) == 0
				   )
					bSkipping = FALSE;
			}
			else
			{
				if (pMax - pcFileMap >= 16 &&
					_tcsncmp (pcFileMap, _TEXT("//{{STOP_DEPSCAN"), 16) == 0
				   )
					bSkipping = TRUE;
			}
		}
		else if (*pcFileMap == _T('#'))
		{
			if (bSkipping) goto SkipToEnd;

			// Go past #
			if (++pcFileMap >= pMax)
				goto SkipToEnd;

			// Skip white space after #
			while (pcFileMap < pMax && _istspace( *pcFileMap))
				pcFileMap++;

			// Look for "include"
			if (pMax - pcFileMap <= 7 || strncmp (pcFileMap, "include", 7) != 0)
				goto SkipToEnd;

			pcFileMap += 7;

			// Skip white space after include
			while (pcFileMap < pMax && _istspace( *pcFileMap))
				pcFileMap++;

			if (pcFileMap >= pMax)
				goto SkipToEnd;

			// Look for the delimeter either " or <

			if (*pcFileMap == _T('"'))
			{
				iEntryType = IncTypeCheckParentDir |  IncTypeCheckIncludePath |
							 IncTypeCheckOriginalDir;

				cDelim = _T('"');
			}
			else if (*pcFileMap == _T('<'))
			{
				iEntryType = IncTypeCheckIncludePath;

				cDelim = _T('>');
			}
			else goto SkipToEnd;

			pcFileMap++;
			if (pcFileMap >= pMax)
				goto SkipToEnd;

			pFileNameBegin = pcFileMap;

			// Look for	other delimeter, or a return if the user screwed up
			while (pcFileMap < pMax && *pcFileMap != cDelim && *pcFileMap != _T('\r'))
				pcFileMap = _tcsinc ( (TCHAR *) pcFileMap);

			if ( pcFileMap >= pMax || *pcFileMap == _T('\r'))
				goto SkipToEnd;

			// File name now starts at pFileNameBegin and ends at pcFileMap-1.
			int nNameLength =  pcFileMap - pFileNameBegin;
			TCHAR * pbuf = strFile.GetBuffer(nNameLength + 1);

			strncpy(pbuf, pFileNameBegin, nNameLength);
			pbuf[nNameLength] = _T('\0');
			strFile.ReleaseBuffer();

			VERIFY (pAction->AddScannedDep(iEntryType, strFile, nLine));
		}

		SkipToEnd:

		while (pcFileMap < pMax && *pcFileMap != _T('\r'))
			pcFileMap = _tcsinc((TCHAR *)pcFileMap);

		if (pcFileMap < pMax && *pcFileMap == _T('\r'))
			pcFileMap++;

	} while (pcFileMap < pMax);

	CloseMemFile:

	g_buildengine.CloseMemFile(hMem, EC);

	return TRUE;	 		
}		