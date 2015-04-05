#pragma once

class CCuckooSettings
{
public:
	CCuckooSettings(void) {
		LoadDefault();
	}

	~CCuckooSettings(void) {
	}

// Attributes
	int	GetClockType() const {
		return m_nClock;
	}

	bool IsDisabledOnMonitorTurnOff() const {
		return m_bMonitorPower;
	}

	bool BellsOnHours() const {
		return m_bHourSounds;
	}

	bool BellsOnHalfHours() const {
		return m_bHalfHourSounds;
	}

	bool BellsOnQuartHours() const {
		return m_bQuartHourSounds;
	}

	bool HasTicTacSound() const {
		return m_bTicTacSounds;
	}

	void	SetClockType(int nClock) {
		m_nClock = nClock;
	}

	void	SetDisabledOnMonitorTurnOff(bool bMonitorPower) {
		m_bMonitorPower = bMonitorPower;
	}

	void	 SetBellsOnHours(bool bEnabled) {
		m_bHourSounds = bEnabled;
	}

	void	SetBellsOnHalfHours(bool bEnabled) {
		m_bHalfHourSounds = bEnabled;
	}

	void	SetBellsOnQuartHours(bool bEnabled) {
		m_bQuartHourSounds = bEnabled;
	}

	void	SetTicTacSound(bool bEnabled) {
		m_bTicTacSounds = bEnabled;
	}

// Operations
	void LoadDefault() {
		m_nClock = 0;				// Horloge à afficher
		m_bMonitorPower = true,		// Disable all sounds once monitor turn off
		m_bHourSounds = true,		// Enable Sounds: Hours
		m_bHalfHourSounds = true,	// Enable Sounds: Half-hours
		m_bQuartHourSounds = true,	// Enable Sounds: Quart-hours
		m_bTicTacSounds = true;		// Enable Sounds: Tic/Tac
	}

	bool Load() {
		BYTE	lpData[64];
		DWORD	dwSize;
		HKEY	hkRegKey;

		// Open the key
		if (RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\WBC\\Cuckoo"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkRegKey, NULL) != ERROR_SUCCESS)
			return false;

		// Load default values
		LoadDefault();

		// Check options
		if (RegQueryValueEx(hkRegKey, _T("Clock"), 0, NULL, lpData, &(dwSize=64)) == ERROR_SUCCESS)
			m_nClock = *(DWORD*)lpData;
		if (RegQueryValueEx(hkRegKey, _T("Disable on monitor turn off"), 0, NULL, lpData, &(dwSize=64)) == ERROR_SUCCESS)
			m_bMonitorPower = lpData[0] == '1';
		if (RegQueryValueEx(hkRegKey, _T("Bell hours"), 0, NULL, lpData, &(dwSize=64)) == ERROR_SUCCESS)
			m_bHourSounds = lpData[0] == '1';
		if (RegQueryValueEx(hkRegKey, _T("Bell half-hours"), 0, NULL, lpData, &(dwSize=64)) == ERROR_SUCCESS)
			m_bHalfHourSounds = lpData[0] == '1';
		if (RegQueryValueEx(hkRegKey, _T("Bell quart-hours"), 0, NULL, lpData, &(dwSize=64)) == ERROR_SUCCESS)
			m_bQuartHourSounds = lpData[0] == '1';
		if (RegQueryValueEx(hkRegKey, _T("TicTac"), 0, NULL, lpData, &(dwSize=64)) == ERROR_SUCCESS)
			m_bTicTacSounds = lpData[0] == '1';

		// Close key
		RegCloseKey(hkRegKey);
		return true;
	}

	bool Save() {
		BYTE	lpData[8];
		HKEY	hkRegKey;

		// Open the key
		if (RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\WBC\\Cuckoo"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkRegKey, NULL) != ERROR_SUCCESS)
			return false;

		// Check options
		*(DWORD*)lpData = m_nClock;
		RegSetValueEx(hkRegKey, _T("Clock"), 0, REG_DWORD, lpData, 4);

		lpData[1] = 0;
		lpData[0] = '0' + m_bMonitorPower;
		RegSetValueEx(hkRegKey, _T("Disable on monitor turn off"), 0, REG_SZ, lpData, 2);

		lpData[1] = 0;
		lpData[0] = '0' + m_bHourSounds;
		RegSetValueEx(hkRegKey, _T("Bell hours"), 0, REG_SZ, lpData, 2);

		lpData[1] = 0;
		lpData[0] = '0' + m_bHalfHourSounds;
		RegSetValueEx(hkRegKey, _T("Bell half-hours"), 0, REG_SZ, lpData, 2);

		lpData[1] = 0;
		lpData[0] = '0' + m_bQuartHourSounds;
		RegSetValueEx(hkRegKey, _T("Bell quart-hours"), 0, REG_SZ, lpData, 2);

		lpData[1] = 0;
		lpData[0] = '0' + m_bTicTacSounds;
		RegSetValueEx(hkRegKey, _T("TicTac"), 0, REG_SZ, lpData, 2);

		// Close key
		RegCloseKey(hkRegKey);
		return true;
	}

// Implementation
private:
	int	m_nClock;
	bool m_bMonitorPower;
	bool m_bHourSounds;
	bool m_bHalfHourSounds;
	bool m_bQuartHourSounds;
	bool m_bTicTacSounds;
};
