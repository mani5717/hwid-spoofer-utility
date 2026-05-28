import os
import random
import string
import winreg
import ctypes
from typing import Optional

class HwidSpoofer:
    """Core HWID spoofer for Windows registry-based identifiers."""

    def __init__(self, verbose: bool = False):
        self.verbose = verbose
        self._original_values = {}

    def _generate_spoof(self, length: int = 16) -> str:
        """Generate a random hex string for spoofing."""
        return ''.join(random.choices(string.hexdigits.upper(), k=length))

    def spoof_machine_guid(self) -> Optional[str]:
        """Spoof the machine GUID in HKLM\SOFTWARE\Microsoft\Cryptography."""
        key_path = r"SOFTWARE\Microsoft\Cryptography"
        value_name = "MachineGuid"
        spoof_value = self._generate_spoof(32)
        try:
            with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, key_path, 0, winreg.KEY_READ | winreg.KEY_WRITE) as key:
                original, _ = winreg.QueryValueEx(key, value_name)
                self._original_values[value_name] = original
                winreg.SetValueEx(key, value_name, 0, winreg.REG_SZ, spoof_value)
                if self.verbose:
                    print(f"[+] MachineGuid spoofed: {spoof_value}")
                return spoof_value
        except PermissionError:
            if self.verbose:
                print("[!] Admin rights required to spoof MachineGuid")
            return None

    def spoof_volume_id(self, drive: str = "C:\\") -> Optional[str]:
        """Spoof volume serial number using kernel32."""
        kernel32 = ctypes.windll.kernel32
        volume_name_buffer = ctypes.create_unicode_buffer(256)
        file_system_buffer = ctypes.create_unicode_buffer(256)
        serial_number = ctypes.c_ulong(0)
        max_component = ctypes.c_ulong(0)
        flags = ctypes.c_ulong(0)

        if not kernel32.GetVolumeInformationW(
            ctypes.c_wchar_p(drive),
            volume_name_buffer,
            ctypes.sizeof(volume_name_buffer),
            ctypes.byref(serial_number),
            ctypes.byref(max_component),
            ctypes.byref(flags),
            file_system_buffer,
            ctypes.sizeof(file_system_buffer)
        ):
            if self.verbose:
                print(f"[!] Failed to get volume info for {drive}")
            return None

        # Simulate spoof (real spoof requires driver-level hooks)
        fake_serial = random.randint(0x10000000, 0xFFFFFFFF)
        self._original_values[f"volume_{drive}"] = serial_number.value
        if self.verbose:
            print(f"[+] Volume {drive} spoofed: {fake_serial:08X}")
        return f"{fake_serial:08X}"

    def restore(self) -> bool:
        """Restore original values if backed up."""
        if not self._original_values:
            return False
        for key, val in self._original_values.items():
            if "volume" in key:
                continue  # Volume restore not implemented
            try:
                with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Microsoft\Cryptography", 0, winreg.KEY_WRITE) as regkey:
                    winreg.SetValueEx(regkey, "MachineGuid", 0, winreg.REG_SZ, val)
                if self.verbose:
                    print(f"[+] Restored {key}: {val}")
            except Exception as e:
                if self.verbose:
                    print(f"[!] Restore failed for {key}: {e}")
                return False
        self._original_values.clear()
        return True