
// MToolDoc.cpp: CMToolDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MTool.h"
#endif

#include "MToolDoc.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMToolDoc

IMPLEMENT_DYNCREATE(CMToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CMToolDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CMToolDoc::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CMToolDoc::OnFileOpen)
END_MESSAGE_MAP()


// CMToolDoc 생성/소멸

CMToolDoc::CMToolDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMToolDoc::~CMToolDoc()
{
}

bool CMToolDoc::SaveHeightMap(CString path, FILE* file, float height)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(path, Drive, Dir, Name, Ext);
	M_STR filename = Drive;
	filename += Dir;
	filename += Name;
	filename += L"_height.bmp";

	HRESULT hr;
	ID3D11Texture2D* ht;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width = I_3DObjectMgr.m_InWorldFiled->ground->m_iCount;
	td.Height = I_3DObjectMgr.m_InWorldFiled->ground->m_iCount;
	td.MipLevels = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Usage = D3D11_USAGE_STAGING;
	td.BindFlags = 0;
	td.ArraySize = 1;
	hr = g_pDevice->CreateTexture2D(&td, NULL, &ht);
	if (FAILED(hr)) return false;

	if (ht)
	{
		M3DHeightMap* hm = I_3DObjectMgr.m_InWorldFiled->ground;
		int hmcount = hm->m_iCount;
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)ht, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			//MVERTEX	v;
			for (UINT row = 0; row < td.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < td.Width; col++)
				{
					UINT colStart = col * 4;
					pTexels[rowStart + colStart + 0] = hm->m_VertexList[(row* hmcount) + col].p.y * 255 / height;
				}
			}
			g_pImmediateContext->Unmap(ht, D3D11CalcSubresource(0, 0, 1));
		}
	}
	D3DX11SaveTextureToFile(g_pImmediateContext, ht, D3DX11_IFF_BMP, filename.c_str());
	SAFE_RELEASE(ht);
	_ftprintf(file, L"%s\n", filename.c_str());
	return true;
}

bool CMToolDoc::SaveAlphaMap(CString path, FILE* file)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(path, Drive, Dir, Name, Ext);
	M_STR filename = Drive;
	filename += Dir;
	filename += Name;
	filename += L"_alpha.bmp";
	D3DX11SaveTextureToFile(g_pImmediateContext, theApp.m_Tool.canvas.GetTexture(), D3DX11_IFF_BMP, filename.c_str());
	_ftprintf(file, L"%s\n", filename.c_str());
	return true;
}

bool CMToolDoc::SaveTileIndex(FILE * file)
{
	M3DHeightMap* hm = I_3DObjectMgr.m_InWorldFiled->ground;
	int icount = hm->m_List.size();
	for (auto temp : hm->m_List)
	{
		for (int i = 0; i < MAX_MAP_COUNT; i++)
		{
			_ftprintf(file, L"%d ", temp->m_pDiffuseMap[i]);
			_ftprintf(file, L"%d ", temp->m_pNormalMap[i]);
		}
		_ftprintf(file, L"\n");
	}
	return true;
}

bool CMToolDoc::SaveObject(FILE * file)
{
	int pathcount = theApp.m_Tool.instance.filepath.size();
	_ftprintf(file, L"%d \n", pathcount);
	for (int i = 0; i < pathcount; i++)
	{
		_ftprintf(file, L"%s \n", theApp.m_Tool.instance.filepath[i].c_str());
	}
	int count = I_3DObjectMgr.m_iInstanceListCount;
	_ftprintf(file, L"%d \n", count);
	for (int i = 0; i < count; i++)
	{
		M3DInstanceModel* temp = I_3DObjectMgr.GetInstanceModel(i);
		int matcount = temp->m_iSize;
		_ftprintf(file, L"%s %d \n", temp->m_name.c_str(), matcount);
		for (auto ins : temp->m_InstanceList)
		{
			D3DXVECTOR3 pos = ins.second->GetPosition();
			D3DXQUATERNION rot = ins.second->GetRotation();
			D3DXVECTOR3 scl = ins.second->GetScale();
			_ftprintf(file, L"%10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f \n",
				pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, rot.w, scl.x, scl.y, scl.z);
		}
	}
	return true;
}

BOOL CMToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMToolDoc serialization

void CMToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CMToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMToolDoc 진단

#ifdef _DEBUG
void CMToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMToolDoc 명령


CString CMToolDoc::AnotherName()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"All Files(*.*)|*.*");
	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		return path;
	}
	return NULL;
}

void CMToolDoc::OnFileSave()
{
	if (!I_3DObjectMgr.m_InWorldFiled)
	{
		AfxMessageBox(L"저장이 실패하였습니다!");
		return;
	}
	CFileDialog dlg(FALSE, L"map", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"map Files(*.map)|*.map");
	if (dlg.DoModal() == IDOK)
	{
		CString selFileName = dlg.GetPathName();

		I_3DObjectMgr.m_pTree->SetMaxY(I_3DObjectMgr.m_pTree->m_pRootNode, I_3DObjectMgr.m_InWorldFiled->ground); // Y값 리빌드

		FILE* m_pStream;
		_wfopen_s(&m_pStream, selFileName, L"w");
		int mapsize = I_3DObjectMgr.m_InWorldFiled->ground->m_iCount;
		int tilesize = I_3DObjectMgr.m_InWorldFiled->ground->m_iTileSize;
		float sellsize = I_3DObjectMgr.m_InWorldFiled->ground->m_fLeafSize;
		float height = I_3DObjectMgr.m_pTree->m_pRootNode->m_Box.vMax.y;

		_ftprintf(m_pStream, L"%d %d %10.4f %10.4f\n", mapsize, tilesize, sellsize, height);

		SaveHeightMap(selFileName, m_pStream, height);
		SaveAlphaMap(selFileName, m_pStream);
		SaveTileIndex(m_pStream);
		SaveObject(m_pStream);

		int textureCount = I_3DObjectMgr.m_InWorldFiled->ground->m_TextureList.size();
		_ftprintf(m_pStream, L"%d\n", textureCount);
		for (int i = 0; i < textureCount; i++)
		{
			M_STR path = I_TextureMgr[I_3DObjectMgr.m_InWorldFiled->ground->m_TextureList[i]]->GetPath();
			_ftprintf(m_pStream, L"%s\n", path.c_str());
		}

		int BrushCount = theApp.m_Tool.canvas.m_BrushList.size();
		_ftprintf(m_pStream, L"%d\n", BrushCount);
		for (int i = 0; i < BrushCount; i++)
		{
			M_STR path = I_TextureMgr[theApp.m_Tool.canvas.m_BrushList[i]]->GetPath();
			_ftprintf(m_pStream, L"%s\n", path.c_str());
		}
		fclose(m_pStream);
		AfxMessageBox(L"저장하였습니다!");
		// 오브젝트 세이브로드 추가 로드 실패시 다른 경로 찾아주기
	}
	else
	{
		AfxMessageBox(L"저장이 실패하였습니다!");
	}
	
}


void CMToolDoc::OnFileOpen()
{
	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
	CFileDialog dlg(TRUE, L"map", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"map Files(*.map)|*.map");
	if (dlg.DoModal() == IDOK)
	{
		TCHAR buffer[MAX_PATH] = { 0, };
		CString selFileName = dlg.GetPathName();
		FILE* m_pStream;
		_wfopen_s(&m_pStream, selFileName, L"r");
		int mapsize, tilesize;
		float sellsize, height;
		M_STR hm, am;
		fscanf_s(m_pStream, "%d %d %f %f\n", &mapsize, &tilesize, &sellsize, &height);
		fscanf_s(m_pStream, "%S\n", buffer, MAX_PATH);
		hm = buffer;
		fscanf_s(m_pStream, "%S\n", buffer, MAX_PATH);
		am = buffer;
		if (!I_Parser.Load_HM(hm.c_str(), sellsize, height, tilesize))
		{
			while (1)
			{
				M_STR oldpath = hm;
				oldpath += L" 파일은 올바른 경로가 아닙니다! 새로운 경로를 지정해주세요.";
				AfxMessageBox(oldpath.c_str());
				M_STR newstr = AnotherName();
				if (I_Parser.Load_HM(newstr.c_str(), sellsize, height, tilesize))
				{
					break;
				}
			}
		}
		I_3DObjectMgr.m_InWorldFiled->ground->CreateAlphaTexture(1, 1);
		if (!theApp.m_Tool.canvas.Load(am))
		{
			while (1)
			{
				M_STR oldpath = am;
				oldpath += L" 파일은 올바른 경로가 아닙니다! 새로운 경로를 지정해주세요.";
				AfxMessageBox(oldpath.c_str());
				M_STR newstr = AnotherName();
				if (theApp.m_Tool.canvas.Load(newstr))
				{
					break;
				}
			}
		}
		M3DHeightMap* heightmap = I_3DObjectMgr.m_InWorldFiled->ground;
		for (auto temp : heightmap->m_List)
		{
			for (int j = 0; j < MAX_MAP_COUNT; j++)
			{
				int tex, nor;
				fscanf_s(m_pStream, "%d ", &tex);
				fscanf_s(m_pStream, "%d ", &nor);
				if (tex != -1)
				{
					temp->SetTexture(tex, DIFFUSE, j);
				}
				if (nor != -1)
				{
					temp->SetTexture(nor, NORMAL, j);
				}
			}
			fscanf_s(m_pStream, "\n");
		}
		int pathcount;
		fscanf_s(m_pStream, "%d \n", &pathcount);
		for (int i = 0; i < pathcount; i++)
		{
			M_STR str;
			fscanf_s(m_pStream, "%S \n", buffer, MAX_PATH);
			str = buffer;
			if (!frame->m_ObjectPane.m_wndForm->LoadOBJ(str.c_str()))
			{
				while (1)
				{
					M_STR oldpath = str;
					oldpath += L" 파일은 올바른 경로가 아닙니다! 새로운 경로를 지정해주세요.";
					AfxMessageBox(oldpath.c_str());
					M_STR newstr = AnotherName();
					if (frame->m_ObjectPane.m_wndForm->LoadOBJ(newstr.c_str()))
					{
						break;
					}
				}
			}
		}
		int modelCount;
		fscanf_s(m_pStream, "%d \n", &modelCount);
		for (int i = 0; i < modelCount; i++)
		{
			M_STR name;
			int instanceCount;
			fscanf_s(m_pStream, "%S", buffer, MAX_PATH);
			name = buffer;
			fscanf_s(m_pStream, "%d \n", &instanceCount);
			int id = I_3DObjectMgr.GetInstanceModelID(name);
			if (id != -1)
			{
				for (int j = 0; j < instanceCount; j++)
				{
					float px, py, pz, rx, ry, rz, rw, sx, sy, sz;
					fscanf_s(m_pStream, "%f %f %f %f %f %f %f %f %f %f \n", &px, &py, &pz, &rx, &ry, &rz, &rw, &sx, &sy, &sz);
					D3DXVECTOR3 pos(px, py, pz);
					D3DXQUATERNION rot(rx, ry, rz, rw);
					D3DXVECTOR3 scl(sx, sy, sz);
					if (I_3DObjectMgr.AddInstanceObj(id, &pos, &rot, &scl) == -1)
					{
						AfxMessageBox(L"불러오기가 실패했습니다!");
						return;
					}
				}

			}
			else
			{
				AfxMessageBox(L"불러오기가 실패했습니다!");
				return;
			}
		}
		int tiletexturecount, brushcount;
		fscanf_s(m_pStream, "%d\n", &tiletexturecount);
		for (int i = 0; i < tiletexturecount; i++)
		{
			M_STR path;
			fscanf_s(m_pStream, "%S\n", &buffer, MAX_PATH);
			path = buffer;
			if(!frame->m_MapPane.m_wndForm->LoadTile(path.c_str()))
			{
				while (1)
				{
					M_STR oldpath = path;
					oldpath += L" 파일은 올바른 경로가 아닙니다! 새로운 경로를 지정해주세요.";
					AfxMessageBox(oldpath.c_str());
					M_STR newstr = AnotherName();
					if (frame->m_MapPane.m_wndForm->LoadTile(newstr.c_str()))
					{
						break;
					}
				}
			}
		}
		fscanf_s(m_pStream, "%d\n", &brushcount);
		for (int i = 0; i < brushcount; i++)
		{
			M_STR path;
			fscanf_s(m_pStream, "%S\n", &buffer, MAX_PATH);
			path = buffer;
			if (!frame->m_MapPane.m_wndForm->LoadBrush(path.c_str()))
			{
				while (1)
				{
					M_STR oldpath = path;
					oldpath += L" 파일은 올바른 경로가 아닙니다! 새로운 경로를 지정해주세요.";
					AfxMessageBox(oldpath.c_str());
					M_STR newstr = AnotherName();
					if (frame->m_MapPane.m_wndForm->LoadBrush(newstr.c_str()))
					{
						break;
					}
				}
			}
		}
		fclose(m_pStream);
		AfxMessageBox(L"불러왔습니다!");
	}
	else
	{
		AfxMessageBox(L"불러오기가 실패했습니다!");
	}
}
