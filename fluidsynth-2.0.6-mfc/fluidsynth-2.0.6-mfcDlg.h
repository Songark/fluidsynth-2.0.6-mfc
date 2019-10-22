
// fluidsynth-2.0.6-mfcDlg.h : header file
//

#pragma once
#include "MidiFile.h"
#include "CfluidsynthWrapper.h"
#include "TimeSignature.h"

using namespace ns_midi;

// Cfluidsynth206mfcDlg dialog
class Cfluidsynth206mfcDlg : public CDialogEx
{
// Construction
public:
	Cfluidsynth206mfcDlg(CWnd* pParent = nullptr);	// standard constructor


	MidiFile m_midiFile;
	CfluidsynthWrapper fluidsynth_wrapper;
	TimeSignature* ptimesig;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLUIDSYNTH206MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSoundfont2();
	afx_msg void OnBnClickedBtnMidi();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	int m_nMaxMeasure;
	CListCtrl m_lstTrackFilter;
	afx_msg void OnClose();
	CSliderCtrl m_tempoSlider;
	CSliderCtrl m_octabSlider;
	CSliderCtrl m_midiSlider;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nCurTicks;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnFor();
	afx_msg void OnBnClickedBtnCheckall();
	afx_msg void OnBnClickedBtnUncheckall();
	double m_dbCurVolume;
};
