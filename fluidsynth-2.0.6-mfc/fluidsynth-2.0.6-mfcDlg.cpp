
// fluidsynth-2.0.6-mfcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fluidsynth-2.0.6-mfc.h"
#include "fluidsynth-2.0.6-mfcDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cfluidsynth206mfcDlg dialog



Cfluidsynth206mfcDlg::Cfluidsynth206mfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FLUIDSYNTH206MFC_DIALOG, pParent)
	, m_nMaxMeasure(0)
	, m_nCurTicks(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cfluidsynth206mfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_nMaxMeasure);
	DDX_Control(pDX, IDC_LIST1, m_lstTrackFilter);
	DDX_Control(pDX, IDC_SLIDER2, m_tempoSlider);
	DDX_Control(pDX, IDC_SLIDER3, m_octabSlider);
	DDX_Control(pDX, IDC_SLIDER1, m_midiSlider);
	DDX_Text(pDX, IDC_EDIT4, m_nCurTicks);
}

BEGIN_MESSAGE_MAP(Cfluidsynth206mfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SOUNDFONT2, &Cfluidsynth206mfcDlg::OnBnClickedBtnSoundfont2)
	ON_BN_CLICKED(IDC_BTN_MIDI, &Cfluidsynth206mfcDlg::OnBnClickedBtnMidi)
	ON_BN_CLICKED(IDC_BTN_PLAY, &Cfluidsynth206mfcDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &Cfluidsynth206mfcDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &Cfluidsynth206mfcDlg::OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Cfluidsynth206mfcDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &Cfluidsynth206mfcDlg::OnNMClickList1)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_BACK, &Cfluidsynth206mfcDlg::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_FOR, &Cfluidsynth206mfcDlg::OnBnClickedBtnFor)
	ON_BN_CLICKED(IDC_BTN_CHECKALL, &Cfluidsynth206mfcDlg::OnBnClickedBtnCheckall)
	ON_BN_CLICKED(IDC_BTN_UNCHECKALL, &Cfluidsynth206mfcDlg::OnBnClickedBtnUncheckall)
END_MESSAGE_MAP()


// Cfluidsynth206mfcDlg message handlers

BOOL Cfluidsynth206mfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_lstTrackFilter.SetExtendedStyle(m_lstTrackFilter.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_lstTrackFilter.InsertColumn(0, _T("Tr"), LVCFMT_LEFT, 40, -1);
	m_lstTrackFilter.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 110, -1);
	m_lstTrackFilter.InsertColumn(2, _T("Port"), LVCFMT_LEFT, 40, -1);
	m_lstTrackFilter.InsertColumn(3, _T("Ch"), LVCFMT_LEFT, 40, -1);

	m_tempoSlider.SetRange(-10, 10, TRUE);
	m_tempoSlider.SetPos(0);

	m_octabSlider.SetRange(-10, 10, TRUE);
	m_octabSlider.SetPos(0);	

	ptimesig = TimeSignature::GetInstance();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cfluidsynth206mfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cfluidsynth206mfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cfluidsynth206mfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cfluidsynth206mfcDlg::OnBnClickedBtnSoundfont2()
{
	// TODO: Add your control notification handler code here

	TCHAR szFilters[] = _T("SoundFont Files (*.sf2)|*.sf2|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("sf2"), _T("*.sf2"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);
	if (fileDlg.DoModal() == IDOK)
	{
		CString strPathname = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_SF2PATH, strPathname);
		UpdateData(FALSE);

		m_midiFile.filename_sf2 = strPathname.GetBuffer();
	}
}


void Cfluidsynth206mfcDlg::OnBnClickedBtnMidi()
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] = _T("Midi Files (*.mid)|*.mid|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("mid"), _T("*.mid"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);
	if (fileDlg.DoModal() == IDOK)
	{
		CString strPathname = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_MIDIPATH, strPathname);		
		
		if (m_midiFile.readFile(strPathname.GetBuffer())) {
			m_midiFile.sortEventArray();			

			CString strTrack, strPort, strChannel;
			m_lstTrackFilter.DeleteAllItems();
			for (int nTrack = 0; nTrack < m_midiFile.getNumTracks(); nTrack++)
			{
				MidiTrack* pTrack = m_midiFile.getTrack(nTrack);

				strTrack.Format(_T("%d"), nTrack);
				strPort.Format(_T("%d"), pTrack->port + 1);
				strChannel.Format(_T("%d"), pTrack->channel + 1);

				m_lstTrackFilter.InsertItem(nTrack, strTrack);
				m_lstTrackFilter.SetItem(nTrack, 1, LVIF_TEXT, m_midiFile.getNameTrack(nTrack).c_str(), 0, 0, 0, NULL);
				m_lstTrackFilter.SetItem(nTrack, 2, LVIF_TEXT, strPort, 0, 0, 0, NULL);
				m_lstTrackFilter.SetItem(nTrack, 3, LVIF_TEXT, strChannel, 0, 0, 0, NULL);

				m_lstTrackFilter.SetCheck(nTrack);
				m_midiFile.setEnableTrack(nTrack, true);
			}

			if (fluidsynth_wrapper.init(m_midiFile.filename_sf2, m_midiFile.filename)) {
				GetDlgItem(IDC_BTN_PLAY)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_CHECKALL)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_UNCHECKALL)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_BACK)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_FOR)->EnableWindow(TRUE);
			}
			else {
				GetDlgItem(IDC_BTN_PLAY)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_CHECKALL)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_UNCHECKALL)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_BACK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_FOR)->EnableWindow(FALSE);
			}			
		}
		UpdateData(FALSE);
	}
}


void Cfluidsynth206mfcDlg::OnBnClickedBtnPlay()
{
	// TODO: Add your control notification handler code here	
	if (fluidsynth_wrapper.get_status() != FLUID_PLAYER_PLAYING) {
		fluidsynth_wrapper.play();
		SetTimer(1, 500, NULL);
	}
}


void Cfluidsynth206mfcDlg::OnBnClickedBtnPause()
{
	// TODO: Add your control notification handler code here
	fluidsynth_wrapper.pause();
	KillTimer(1);
}


void Cfluidsynth206mfcDlg::OnBnClickedBtnStop()
{
	// TODO: Add your control notification handler code here
	fluidsynth_wrapper.stop();
	m_midiSlider.SetPos(0);
	m_nCurTicks = 0;
	KillTimer(1);
	UpdateData(FALSE);
}


void Cfluidsynth206mfcDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	fluidsynth_wrapper.stop();

	CDialogEx::OnClose();
}


void Cfluidsynth206mfcDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LVCOLUMN pColumn = { 0 };
	pColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;	
	*pResult = 0;
}


void Cfluidsynth206mfcDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}


void Cfluidsynth206mfcDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int status = fluidsynth_wrapper.get_status();

	if (status == FLUID_PLAYER_PLAYING) {
		for (int nIndex = 0; nIndex < m_lstTrackFilter.GetItemCount(); nIndex++) {
			if (m_lstTrackFilter.GetCheck(nIndex))
				fluidsynth_wrapper.setfilter(nIndex, true);
			else
				fluidsynth_wrapper.setfilter(nIndex, false);
		}		
		m_nCurTicks = ptimesig->get_tick2measure(fluidsynth_wrapper.getcurrentticks());
		m_nMaxMeasure = ptimesig->get_tick2measure(fluidsynth_wrapper.gettickscount());
		m_midiSlider.SetRange(0, m_nMaxMeasure, TRUE);
		m_midiSlider.SetPos(m_nCurTicks);
		UpdateData(FALSE);
	}
	else if (status == FLUID_PLAYER_DONE) {
		OnBnClickedBtnStop();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void Cfluidsynth206mfcDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);

	// Check which slider sent the notification  
	if (pSlider == &m_tempoSlider)
	{
		int tempoValue = fluidsynth_wrapper.gettempo();
		int tempoScroll = m_tempoSlider.GetPos();
		tempoValue = tempoValue * (2 ^ tempoScroll);
		fluidsynth_wrapper.settempo(tempoValue);
	}
	else if (pSlider == &m_octabSlider)
	{
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void Cfluidsynth206mfcDlg::OnBnClickedBtnBack()
{
	// TODO: Add your control notification handler code here
	if (m_nCurTicks >= 2) {
		m_nCurTicks -= 2;
		int ticks = ptimesig->get_measure2tick(m_nCurTicks);
		fluidsynth_wrapper.seek(ticks);
	}
}


void Cfluidsynth206mfcDlg::OnBnClickedBtnFor()
{
	// TODO: Add your control notification handler code here
	if (m_nCurTicks < m_nMaxMeasure - 2) {
		m_nCurTicks += 2;
		int ticks = ptimesig->get_measure2tick(m_nCurTicks);
		fluidsynth_wrapper.seek(ticks);
	}
}


void Cfluidsynth206mfcDlg::OnBnClickedBtnCheckall()
{
	// TODO: Add your control notification handler code here
	for (int nIndex = 0; nIndex < m_lstTrackFilter.GetItemCount(); nIndex++) {
		m_lstTrackFilter.SetCheck(nIndex, TRUE);
		fluidsynth_wrapper.setfilter(nIndex, true);
	}
}


void Cfluidsynth206mfcDlg::OnBnClickedBtnUncheckall()
{
	// TODO: Add your control notification handler code here
	for (int nIndex = 0; nIndex < m_lstTrackFilter.GetItemCount(); nIndex++) {
		m_lstTrackFilter.SetCheck(nIndex, FALSE);
		fluidsynth_wrapper.setfilter(nIndex, false);
	}
}
