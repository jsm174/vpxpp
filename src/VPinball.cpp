#include "VPinball.h"
#include "Player.h"
#include "extern.h"

bool VPinball::m_firstRun = true;

VPinball::VPinball()
{
	m_povEdit = false;
}

VPinball::~VPinball()
{
}

void VPinball::LoadFileName(const std::string& szFileName, const bool updateEditor)
{
	if (m_vtable.size() == MAX_OPEN_TABLES)
	{
		// TODO: ShowError("Maximum amount of tables already loaded and open.");
		return;
	}

	if (m_firstRun)
	{
		// TODO: OnInitialUpdate();
	}

	// TODO: PathFromFilename(szFileName, m_currentTablePath);
	// TODO: CloseAllDialogs();

	// TODO: PinTableMDI* const mdiTable = new PinTableMDI(this);
	// TODO: CComObject<PinTable>* const pPinTable = mdiTable->GetTable();

	PinTable* pPinTable = new PinTable(this);
	const HRESULT hr = pPinTable->LoadGameFromFilename(szFileName);

	if (hr != S_OK)
	{
		// if (hr == E_ACCESSDENIED)
		// {
		// 	const LocalString ls(IDS_CORRUPTFILE);
		// 	ShowError(ls.m_szbuffer);
		// }

		// delete mdiTable;
	}
	else
	{
		m_vtable.push_back(pPinTable);

		TitleFromFilename(szFileName, pPinTable->m_szTitle);
		pPinTable->InitTablePostLoad();

		// TODO: AddMDITable(mdiTable);

		// // auto-import POV settings, if it exists...
		// string szFileNameAuto = m_currentTablePath + pPinTable->m_szTitle + ".pov";
		// if (Exists(szFileNameAuto)) // We check if there is a matching table pov settings file first
		// 	pPinTable->ImportBackdropPOV(szFileNameAuto);
		// else // Otherwise, we seek for autopov settings
		// {
		// 	szFileNameAuto = m_currentTablePath + "autopov.pov";
		// 	if (Exists(szFileNameAuto))
		// 		pPinTable->ImportBackdropPOV(szFileNameAuto);
		// }

		// // auto-import VBS table script, if it exists...
		// szFileNameAuto = m_currentTablePath + pPinTable->m_szTitle + ".vbs";
		// if (Exists(szFileNameAuto)) // We check if there is a matching table vbs first
		// 	pPinTable->m_pcv->LoadFromFile(szFileNameAuto);
		// else // Otherwise we seek in the Scripts folder
		// {
		// 	szFileNameAuto = m_szMyPath + "Scripts\\" + pPinTable->m_szTitle + ".vbs";
		// 	if (Exists(szFileNameAuto))
		// 		pPinTable->m_pcv->LoadFromFile(szFileNameAuto);
		// }

		// // auto-import VPP settings, if it exists...
		// szFileNameAuto = m_currentTablePath + pPinTable->m_szTitle + ".vpp";
		// if (Exists(szFileNameAuto)) // We check if there is a matching table vpp settings file first
		// 	pPinTable->ImportVPP(szFileNameAuto);
		// else // Otherwise, we seek for autovpp settings
		// {
		// 	szFileNameAuto = m_currentTablePath + "autovpp.vpp";
		// 	if (Exists(szFileNameAuto))
		// 		pPinTable->ImportVPP(szFileNameAuto);
		// }

		// // get the load path from the filename
		// SaveValue("RecentDir", "LoadDir", m_currentTablePath);

		// // make sure the load directory is the active directory
		// SetCurrentDirectory(m_currentTablePath.c_str());
		// UpdateRecentFileList(szFileName);

		// ProfileLog("UI Post Load Start");

		// pPinTable->AddMultiSel(pPinTable, false, true, false);
		// pPinTable->SetDirty(eSaveClean);
		// if (updateEditor)
		// {
		// 	GetLayersListDialog()->CollapseLayers();
		// 	GetLayersListDialog()->ExpandLayers();
		// 	ToggleToolbar();
		// 	if (m_dockNotes != nullptr)
		// 		m_dockNotes->Enable();

		// 	SetFocus();
		// }

		// ProfileLog("UI Post Load End");
	}
}

void VPinball::DoPlay(const bool _cameraMode)
{
	// TODO: NumVideoBytes = 0;
	// CComObject<PinTable> * const ptCur = GetActiveTable();
	// if (ptCur)
	// {
	//   ptCur->Play(_cameraMode);
	// }

	PinTable* pPinTable = m_vtable[0];
	pPinTable->Play(_cameraMode);
}

void VPinball::MainMsgLoop()
{
	if (g_pplayer) {
		g_pplayer->Run();
	}
}