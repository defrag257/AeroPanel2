
// AeroPanelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AeroPanel.h"
#include "AeroPanelDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAeroPanelDlg �Ի���


CAeroPanelDlg::CAeroPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAeroPanelDlg::IDD, pParent)
	, m_hotkeyid(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bDwm = FALSE;
	m_use_aero = FALSE;
	m_hotkey = 'Z';
}

void CAeroPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAeroPanelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_NCRBUTTONUP()
	ON_COMMAND(ID_APP_ABOUT, &CAeroPanelDlg::OnAppAbout)
	ON_COMMAND(ID_POP_CLOSE, &CAeroPanelDlg::OnPopClose)
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_COMMAND(ID_POP_ICONIC, &CAeroPanelDlg::OnPopIconic)
	ON_COMMAND(ID_POP_SWITCH, &CAeroPanelDlg::OnPopSwitch)
//	ON_WM_NCPAINT()
	ON_WM_THEMECHANGED()
//	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_COMMAND(ID_POP_ADV_TRANSPARENT, &CAeroPanelDlg::OnPopAdvTransparent)
	//ON_COMMAND(ID_POP_MAXIMIZE, &CAeroPanelDlg::OnPopMaximize)
	ON_WM_HOTKEY()
//	ON_UPDATE_COMMAND_UI(ID_POP_ADV_TRANSPARENT, &CAeroPanelDlg::OnUpdatePopAdvTransparent)
	ON_COMMAND_RANGE(ID_POP_ADV_HK_Z, ID_POP_ADV_HK_E, &CAeroPanelDlg::OnPopAdvHotkey)
END_MESSAGE_MAP()


// CAeroPanelDlg ��Ϣ�������

BOOL CAeroPanelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_hotkeyid = GlobalAddAtom(_T("AeroPanelKey")) - 0xC000; 

	// Ҫʵ��WS_EX_TRANSPARENT������WS_EX_LAYERED�ұ�������ALPHA������������
	SetWindowLong(m_hWnd, GWL_EXSTYLE,
		GetWindowLong(m_hWnd, GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 255, LWA_ALPHA);

	// ��ע�����ش��ڴ�С
	LoadProgSettings();
	
	// ���Լ���Aero
	TryAero();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAeroPanelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAeroPanelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		SetTimer(5, 10, NULL); // ����������ֹAboutBoxʧȥ��Ӧ
	}
}


void CAeroPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 5) {
		KillTimer(nIDEvent);
		TryAero();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAeroPanelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAeroPanelDlg::TryAero(void)
{
	DwmIsCompositionEnabled(&bDwm);
	if (m_use_aero && bDwm) {

		// ��Aero
		MARGINS mrg = {-1}; 
		DwmExtendFrameIntoClientArea(m_hWnd , &mrg);
		SetBackgroundColor(RGB(0, 0, 0));

		CString caption;
		caption.LoadString(IDS_CAPTION);
		SetWindowText(caption);

	} else {
		m_use_aero = FALSE;

		// �ر�Aero
		MARGINS mrg = {0, 0, 0, 0}; 
		DwmExtendFrameIntoClientArea(m_hWnd , &mrg);
		SetBackgroundColor(GetSysColor(COLOR_INACTIVEBORDER));

		CString caption;
		caption.LoadString(IDS_CAPTION_NOAERO);
		SetWindowText(caption);
	}
}

LRESULT CAeroPanelDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	LRESULT retval = CDialogEx::OnNcHitTest(point);

	return (retval == HTCLIENT) ? HTCAPTION : retval; // ʹ�ͻ�����Ϊ����������

}

void CAeroPanelDlg::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	// �����Ҽ��˵�

	//ClientToScreen(&point);
	this->SetForegroundWindow();
	CMenu menu, *pmenu, *padvmenu;
	menu.LoadMenu(IDR_POPMENU);
	pmenu = menu.GetSubMenu(0);
	padvmenu = pmenu->GetSubMenu(2);

	// ���¿�ݼ�ָʾ
	CString fmt; fmt.LoadString(IDS_ADVTRANS);
	fmt.Format(fmt, m_hotkey);
	padvmenu->ModifyMenu(0, MF_BYPOSITION|MF_STRING, ID_POP_ADV_TRANSPARENT, fmt);

	pmenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	CDialogEx::OnNcRButtonUp(nHitTest, point);
}


void CAeroPanelDlg::OnAppAbout()
{
	// ��ʾ���ڶԻ���
	CAboutDlg dlg;
	dlg.DoModal();
}

void CAeroPanelDlg::OnPopIconic()
{
	// ��С������
	CloseWindow();
}

void CAeroPanelDlg::OnPopClose()
{
	// �رմ���
	EndDialog(IDOK);
}

void CAeroPanelDlg::OnPopSwitch()
{
	// �л�Aero
	m_use_aero = !m_use_aero;
	TryAero();
}

void CAeroPanelDlg::LoadProgSettings(void)
{
	// ��ע�����ش��ڴ�С
	// ע�⣺ֻ��GetWindowPlacement��SetWindowPlacement�ɶ�ʹ�òſɱ�֤��С���仯
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	wp.rcNormalPosition.left = AfxGetApp()->GetProfileInt(_T("windowlocation"), _T("left"), 10);
	wp.rcNormalPosition.top = AfxGetApp()->GetProfileInt(_T("windowlocation"), _T("top"), 10);
	wp.rcNormalPosition.right = AfxGetApp()->GetProfileInt(_T("windowlocation"), _T("right"), 500);
	wp.rcNormalPosition.bottom = AfxGetApp()->GetProfileInt(_T("windowlocation"), _T("bottom"), 100);
	SetWindowPlacement(&wp);
	
	m_use_aero = AfxGetApp()->GetProfileInt(_T("windowstyle"), _T("aero"), TRUE);

	m_hotkey = AfxGetApp()->GetProfileInt(_T("advfunc"), _T("transhotkey"), 'Z');
	RegisterHotKey(m_hWnd, m_hotkeyid, MOD_CONTROL|MOD_ALT, m_hotkey);
}

void CAeroPanelDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// �����ڴ�С���浽ע���
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);

	AfxGetApp()->WriteProfileInt(_T("windowlocation"), _T("left"), wp.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt(_T("windowlocation"), _T("top"), wp.rcNormalPosition.top);
	AfxGetApp()->WriteProfileInt(_T("windowlocation"), _T("right"), wp.rcNormalPosition.right);
	AfxGetApp()->WriteProfileInt(_T("windowlocation"), _T("bottom"), wp.rcNormalPosition.bottom);
	
	AfxGetApp()->WriteProfileInt(_T("windowstyle"), _T("aero"), m_use_aero);

	AfxGetApp()->WriteProfileInt(_T("advfunc"), _T("transhotkey"), m_hotkey);
}

void CAeroPanelDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// ˢ��Aero״̬
	COLORREF bkclr;
	if (m_use_aero && bDwm) {
		bkclr = RGB(0, 0, 0);
		SetBackgroundColor(bkclr);
	}
}


LRESULT CAeroPanelDlg::OnThemeChanged()
{
	// �ù���Ҫ��ʹ�� Windows XP ����߰汾��
	// ���� _WIN32_WINNT ���� >= 0x0501��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	TryAero(); // Ȼ����û��ʲô��

	return 1;
}

void CAeroPanelDlg::OnPopAdvTransparent()
{
	// ���ô��ڶ����͸�� - ��ҪWS_EX_LAYERED��ALPHA=255
	SetWindowLong(m_hWnd, GWL_EXSTYLE,
		GetWindowLong(m_hWnd, GWL_EXSTYLE)|WS_EX_TRANSPARENT);
}

void CAeroPanelDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// �ӵ��ȼ� - ���ô��ڶ���겻͸�� - ��ҪWS_EX_LAYERED��ALPHA=255
	SetWindowLong(m_hWnd, GWL_EXSTYLE,
		GetWindowLong(m_hWnd, GWL_EXSTYLE)&~WS_EX_TRANSPARENT);

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CAeroPanelDlg::OnPopAdvHotkey(UINT nID)
{
	switch (nID)
	{
	case ID_POP_ADV_HK_Z: m_hotkey = 'Z'; break;
	case ID_POP_ADV_HK_X: m_hotkey = 'X'; break;
	case ID_POP_ADV_HK_C: m_hotkey = 'C'; break;
	case ID_POP_ADV_HK_A: m_hotkey = 'A'; break;
	case ID_POP_ADV_HK_S: m_hotkey = 'S'; break;
	case ID_POP_ADV_HK_D: m_hotkey = 'D'; break;
	case ID_POP_ADV_HK_Q: m_hotkey = 'Q'; break;
	case ID_POP_ADV_HK_W: m_hotkey = 'W'; break;
	case ID_POP_ADV_HK_E: m_hotkey = 'E'; break;
	}
	UnregisterHotKey(m_hWnd, m_hotkeyid);
	RegisterHotKey(m_hWnd, m_hotkeyid, MOD_CONTROL|MOD_ALT, m_hotkey);
}
